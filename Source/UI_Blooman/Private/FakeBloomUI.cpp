// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomUI.h"
#include "SFakeBloomUI.h"
#include "FakeBloomUI_Builder.h"
#include "FakeBloomUI_Painter.h"
#include "AssetRegistry/AssetRegistryModule.h"

#if WITH_EDITOR
#include "Kismet/KismetRenderingLibrary.h"
#include <sstream>
#include <iomanip>
#include "AssetTools/Public/AssetToolsModule.h"
#endif

#define LOCTEXT_NAMESPACE "UI_BLOOMAN"

//------------------------------------------------------------------------------------------------

UFakeBloomUI::UFakeBloomUI(const FObjectInitializer& ObjectInitializer)
    : Super(ObjectInitializer)
{
    bIsVariable = true;
    Visibility = ESlateVisibility::SelfHitTestInvisible;

    {
        // TODO: Project Settingsでの指定
        FString Path = "/UI_Blooman/M_FakeBloomUI_PaintAdditive.M_FakeBloomUI_PaintAdditive";

        IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
        FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FName(*Path));
        
        // 同期読込
        if (UObject* PaintMaterial = AssetData.GetAsset()) {
            BaseParameter.Brush.SetResourceObject(PaintMaterial);
        }
    }

    {
        // TODO: Project Settingsでの指定
        FString Path = "/UI_Blooman/B_FakeBloomUI_Builder.B_FakeBloomUI_Builder_C";

        TSubclassOf<UFakeBloomUI_Builder> Class = TSoftClassPtr<UFakeBloomUI_Builder>(FSoftObjectPath(*Path)).LoadSynchronous();
        if (Class) {
            Builder = Cast<UFakeBloomUI_Builder>(ObjectInitializer.CreateDefaultSubobject(
                this,
                Class->GetFName(),
                Class,
                Class));
        }
    }
    {
        // TODO: Project Settingsでの指定
        FString Path = "/UI_Blooman/B_FakeBloomUI_Painter.B_FakeBloomUI_Painter_C";

        TSubclassOf<UFakeBloomUI_Painter> Class = TSoftClassPtr<UFakeBloomUI_Painter>(FSoftObjectPath(*Path)).LoadSynchronous();
        if (Class) {
            Painter = Cast<UFakeBloomUI_Painter>(ObjectInitializer.CreateDefaultSubobject(
                this,
                Class->GetFName(),
                Class,
                Class));
        }
    }
}

UWidget* UFakeBloomUI::GetChildContent() const
{
    return GetContentSlot()->Content;
}

void UFakeBloomUI::OnPaintPreProcess(const FFakeBloomUI_PreProcessArgs& args)
{
    if(Painter && Builder){
        Painter->OnPaintPreProcess(); // 先にPainterにRender Targetの受け取りを準備させる必要がある
        Builder->OnPaintPreProcess(args);
    }
}

void UFakeBloomUI::OnPaint(FPaintContext& Context)
{
    float Opacity = 1.0f;
    {
        UWidget* Widget = this;
        while (Widget) {
            Opacity *= Widget->GetRenderOpacity();
            Widget = Widget->GetParent();
        }
    }

    if (Painter) {
        Painter->SetOpacity(Opacity);
        Painter->OnPaint(Context);
    }

#if WITH_EDITOR
    UpdateStats.ExecuteIfBound(this);
#endif
}

void UFakeBloomUI::ReleaseSlateResources(bool bReleaseChildren)
{
    Super::ReleaseSlateResources(bReleaseChildren);
    MyFakeBloomUI.Reset();
}

#if WITH_EDITOR
void UFakeBloomUI::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
    Super::PostEditChangeProperty(PropertyChangedEvent);

    if (PropertyChangedEvent.ChangeType != EPropertyChangeType::Interactive)
    {
        TSharedPtr<SWidget> SafeWidget = GetCachedWidget();
        if (SafeWidget.IsValid())
        {
            if (Builder && Painter) {
                // 変更した(かもしれない)パラメータ反映
                Builder->BaseParameter = BaseParameter;
                Painter->FakeBloomUI = this;

                // 新しいパラメータ対応でRender Targetを作り直させたいので再描画
                Builder->OnRequestRedraw();                 

                // PainterもbUseTextureの状態を反映させたいので作り直し
                Painter->OnRebuild();

                // どっちかが差し替えられている可能性もあるので、
                // DelegateによるRender Target受け渡しも再構築
                Builder->OnFinishBuild.Clear();
                Builder->OnFinishBuild.AddDynamic(Painter.Get(), &UFakeBloomUI_Painter::SetRenderTexture);
            }
        }
    }
}

const FText UFakeBloomUI::GetPaletteCategory()
{
    return LOCTEXT("SpecialFX", "Special Effects");
}

void UFakeBloomUI::CreateNewTexture(UTextureRenderTarget2D* InRenderTarget)
{
    UObject* DummyTexSource(nullptr);
    {
        FString Path = "/UI_Blooman/T_Dummy.T_Dummy";
        IAssetRegistry& AssetRegistry = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry").Get();
        FAssetData AssetData = AssetRegistry.GetAssetByObjectPath(FName(*Path));
        DummyTexSource = AssetData.GetAsset();

        if (!DummyTexSource) {
            ensure(0);
            return;
        }
    }
    
    UTexture2D* NewTex(nullptr);
    {
        FString Name;
        FString PackageName;
        IAssetTools& AssetTools = FModuleManager::LoadModuleChecked<FAssetToolsModule>("AssetTools").Get();
        AssetTools.CreateUniqueAssetName(TextureSavePath, TEXT(""), PackageName, Name);

        UObject* NewObj = AssetTools.DuplicateAsset(Name, FPaths::GetPath(PackageName), DummyTexSource);
        NewTex = Cast<UTexture2D>(NewObj);

        if (!NewTex) {
            ensure(0);
            return;
        }
    }
    
    //// package needs saving
    //NewTex->MarkPackageDirty();

    //// Notify the asset registry
    //FAssetRegistryModule::AssetCreated(NewTex);
    
    BaseParameter.bUseTexture = true;
    BaseParameter.BloomTexture = NewTex;

    OverwriteTexture(InRenderTarget);
}

void UFakeBloomUI::OverwriteTexture(UTextureRenderTarget2D* InRenderTarget)
{
    Builder->OnFinishBuild.RemoveDynamic(this, &UFakeBloomUI::OverwriteTexture);

    if (BaseParameter.BloomTexture) {
        UKismetRenderingLibrary::ConvertRenderTargetToTexture2DEditorOnly(
            GetWorld(),
            InRenderTarget,
            BaseParameter.BloomTexture);

        // 形式設定も上書き
        BaseParameter.BloomTexture->CompressionSettings = TextureFormat;
        BaseParameter.BloomTexture->MipGenSettings = TextureMipGenSettings::TMGS_NoMipmaps;
        BaseParameter.BloomTexture->PostEditChange();
    }

    OnFinishWriteJob();
}

void UFakeBloomUI::OnFinishWriteJob()
{
    // DetailsCustomizationへの通知用
    FinishEditorCommand.ExecuteIfBound();
    FinishEditorCommand.Unbind();
}

FString UFakeBloomUI::GetBloomTextureStat() const
{
    int32 TexSizeX(0);
    int32 TexSizeY(0);
    int32 ResourceSize(0);

    if (BaseParameter.IsUsingValidTexture()) {
        TexSizeX = BaseParameter.BloomTexture->GetSizeX();
        TexSizeY = BaseParameter.BloomTexture->GetSizeY();
        ResourceSize = BaseParameter.BloomTexture->GetResourceSizeBytes(EResourceSizeMode::Exclusive);
        
    } else {
        if (UTextureRenderTarget2D* RenderTarget = Builder->ResultRenderTarget) {
            TexSizeX = static_cast<int32>(RenderTarget->GetSurfaceWidth());
            TexSizeY = static_cast<int32>(RenderTarget->GetSurfaceHeight());
            ResourceSize = RenderTarget->GetResourceSizeBytes(EResourceSizeMode::Exclusive);
        }
    }

    // FTextureEditorToolkit::PopulateQuickInfo に準拠
    ResourceSize = (ResourceSize + 512)/1024;

    const int32 BaseSizeX(Builder->BaseWidgetSize.X);
    const int32 BaseSizeY(Builder->BaseWidgetSize.Y);
    const int32 OverhangedSizeX(BaseSizeX + BaseParameter.Overhang.GetSizeX());
    const int32 OverhangedSizeY(BaseSizeY + BaseParameter.Overhang.GetSizeY());

    std::ostringstream oss;
    oss << "  Widget Size: " << std::setw(4) << BaseSizeX << "x" << std::setw(4) << BaseSizeY << std::endl;
    oss << "   +Overhange: " << std::setw(4) << OverhangedSizeX << "x" << std::setw(4) << OverhangedSizeY << std::endl;
    oss << " Texture Size: " << std::setw(4) << TexSizeX << "x" << std::setw(4) << TexSizeY << std::endl;
    oss << " Texture Type: " << (BaseParameter.IsUsingValidTexture() ? "Static Texture" : "Render Target") << std::endl;;
    oss << "Resource Size: " << ResourceSize << " KB";
    return oss.str().c_str();
}

#endif

TSharedRef<SWidget> UFakeBloomUI::RebuildWidget()
{
    if (Builder && Painter) {
        // OnReBuildがCommonParameterを使うので先に参照を付与
        Builder->BaseParameter = BaseParameter;
        Painter->FakeBloomUI = this;
   
        Builder->OnRebuild();
        Painter->OnRebuild();

        // 一旦Clearしておかないと、ここに来る経緯によっては多重登録になりうる
        Builder->OnFinishBuild.Clear();

        // DelegateによるRender Target受け渡しを再構築
        // PainterがTObjectPtrなので、Getで生ポインタに変換しないとコンパイルが通らない
        Builder->OnFinishBuild.AddDynamic(Painter.Get(), &UFakeBloomUI_Painter::SetRenderTexture);

#if WITH_EDITOR
        // 何かしら予約があれば実行
        CheckEditorCommand.ExecuteIfBound(this);
        CheckEditorCommand.Unbind();
#endif
    }

    MyFakeBloomUI = SNew(SFakeBloomUI);
    MyFakeBloomUI->SetWidget(this);

    // Add any existing content to the new slate box
    if (GetChildrenCount() > 0)
    {
        UPanelSlot* ContentSlot = GetContentSlot();
        if (ContentSlot->Content)
        {
            MyFakeBloomUI->SetContent(ContentSlot->Content->TakeWidget());

            if (Builder) {
                Builder->TargetWidget = ContentSlot->Content;
            }
        }
    }

    return MyFakeBloomUI.ToSharedRef();
}

void UFakeBloomUI::OnSlotAdded(UPanelSlot* InSlot)
{
    // Add the child to the live slot if it already exists
    if (MyFakeBloomUI.IsValid() && InSlot->Content)
    {
        MyFakeBloomUI->SetContent(InSlot->Content->TakeWidget());
    }
}

void UFakeBloomUI::OnSlotRemoved(UPanelSlot* InSlot)
{
    // Remove the widget from the live slot if it exists.
    if (MyFakeBloomUI.IsValid())
    {
        MyFakeBloomUI->SetContent(SNullWidget::NullWidget);
    }
}

bool UFakeBloomUI::IsDesignTime() const
{
#if WITH_EDITOR
    return UWidget::IsDesignTime();
#else
    return false;
#endif
}

//void UFakeBloomUI::SetPainter(float Value)
//{
//    //Painter->TintColor = In->TintColor;
//    UE_LOG(LogTemp, Log, TEXT("%s: set!?"), UTF8_TO_TCHAR(__func__) );
//}

#undef LOCTEXT_NAMESPACE
