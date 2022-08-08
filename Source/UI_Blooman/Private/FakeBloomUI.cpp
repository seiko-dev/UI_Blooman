// Copyright seiko_dev. All Rights Reserved.

#include "FakeBloomUI.h"
#include "SFakeBloomUI.h"
#include "FakeBloomUI_Builder.h"
#include "FakeBloomUI_Painter.h"

#if WITH_EDITOR
#include "Kismet/KismetRenderingLibrary.h"
#endif
#include "AssetRegistry/AssetRegistryModule.h"

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

        FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>("AssetRegistry");
        FAssetData AssetData = AssetRegistryModule.Get().GetAssetByObjectPath(FName(*Path));
        
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
    Builder->OnFinishBuild.RemoveDynamic(this, &UFakeBloomUI::CreateNewTexture);

    if (!Builder) {
        // ここで生成できてないとおかしい
        ensure(0);
        return;
    }

    UTexture2D* Tex = UKismetRenderingLibrary::RenderTargetCreateStaticTexture2DEditorOnly(
        InRenderTarget,
        TextureSavePath,
        TextureFormat,
        TextureMipGenSettings::TMGS_NoMipmaps);

    if (Tex) {
        BaseParameter.bUseTexture = true;
        BaseParameter.BloomTexture = Tex;
    }

    OnFinishWriteJob();
}

void UFakeBloomUI::OverwriteTexture(UTextureRenderTarget2D* InRenderTarget)
{
    Builder->OnFinishBuild.RemoveDynamic(this, &UFakeBloomUI::OverwriteTexture);

    if (BaseParameter.BloomTexture) {
        UKismetRenderingLibrary::ConvertRenderTargetToTexture2DEditorOnly(
        GetWorld(),
        InRenderTarget,
        BaseParameter.BloomTexture);

        // 圧縮設定も上書き
        BaseParameter.BloomTexture->CompressionSettings = TextureFormat;
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
