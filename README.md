# UI_Blooman
Unreal Engine用プラグインです。子要素を擬似的に発光させる「Fake Bloom」ウィジェットを追加します。
Plugin for Unreal Engine. Add a "Fake Bloom" widget to make child elements pseudo-illuminate.

![optimized](https://user-images.githubusercontent.com/40533980/182017373-15ef663b-3caf-47e8-9861-0f2052ab81cb.gif)


# 特徴 / Feature
- このプラグインを導入すると「Fake Bloom」というパネルが追加されます。このパネルは、子要素が発光して見えるように描画します。  
After installing this plugin, a panel called "Fake Bloom" will be added. This panel draws child elements so that they appear to glow.  

- ブルームテクスチャをテクスチャアセットとして保存可能です。  
Bloom textures can be saved as texture assets.  

- テクスチャアセットを使う場合、単にテクスチャを描画するだけのWidgetとして動作します。つまり、パフォーマンスへの影響を小さくできます。  
When using a texture asset, it acts as a widget that simply draws a texture. This means that the performance impact can be minimized.  

- パフォーマンスに注意すれば、オンデマンドで処理を適用する事もできます。例えば、プレイヤーが変更可能な名前テキストにブルーム演出を加えられます。  
If you are careful about performance, you can also apply processing on demand. For example, a bloom effect can be added to the name text that can be changed by the player.  

- さらなるパフォーマンスへの注意を払えば、完全リアルタイムに処理を適用できます。つまり、発光対象のアニメーションが可能になります。Niagara UI Rendererプラグインなどの魅力を向上させます。  
With additional performance attention, processing can be applied in full real-time. This means that animations of luminous objects are possible, enhancing the attractiveness of the Niagara UI Renderer plug-in, for example.

- ブルームテクスチャの生成と描画はそれぞれ独立したクラスです。これらはパラメータの一部になっているので、ユーザーがより良い実装に置き換えることができます。  
Bloom texture generation and drawing are independent classes. They are part of the parameters so that the user can replace them with better implementations.

- ブルームテクスチャ発光部分を描画するMaterialも指定可能です。このMaterialのパラメータはSequencerのTrackとして制御できます。  
You can also specify the Material to draw the bloom texture emission area. The parameters of this Material can be controlled as Sequencer's Track.


# 注意事項 / Caution
このプラグインはまだ開発途上なので、多数の問題があります。  
This plugin is still under development and has numerous issues.

- C++ Projectで使う必要があります。Blueprint Projectで使用した場合、パッケージした実行ファイルでエラーが出ます。  
Must be used with C++ Project; if used with Blueprint Project, errors will occur in the packaged executable.

  - 非公式な回避策として、Marketplace提供のプラグイン(例えばNiagara UI Renderer)を併用すると、Blueprint Projectであっても正常にパッケージした実行ファイルが動作する場合があるようです。(機序はよくわかりません)  
  As an unofficial workaround, it seems that when used in conjunction with a Marketplace-provided plug-in (e.g. Niagara UI Renderer), the packaged executable may work correctly, even with Blueprint Project. (I am not sure of the mechanism.)  

- 破壊的変更が加えられる可能性があります。バージョンアップによって、以前のパラメータ設定が失われるかもしれません。  
Destructive changes may be made. Version upgrades may cause loss of previous parameter settings.

- このプラグインが実際に作品に使用され、実用に耐えたという証明はまだありません。  
There is no proof yet that this plug-in has actually been used in a work and has stood up to practical use.


# 対応バージョン / Supported Versions
- [v0.1.3 or later](https://github.com/seiko-dev/UI_Blooman/releases)
  - UE5.2.1
- [v0.1.2](https://github.com/seiko-dev/UI_Blooman/releases/tag/v0.1.2)
  - UE5.0.3

# 対応プラットフォーム / Supported Platforms
- Win64

# インストール手順 / Installation

- まず注意事項を読みます。この項目のすぐ上にあります。  
Read the caution notes first. It is located just above this section.

- [Releases](https://github.com/seiko-dev/UI_Blooman/releases)からzipファイルをダウンロードします。  
Download the zip file from [Releases](https://github.com/seiko-dev/UI_Blooman/releases).

- zipファイル内から「UI_Blooman」フォルダを展開します。  
Extract the "UI_Blooman" folder from within the zip file.

- プラグインを適用したいプロジェクトに「Plugins」フォルダを作ります。  
Create a "Plugins" folder in the project to which you want to apply the plug-in.

- 「Plugins」フォルダ内に「UI_Blooman」フォルダを移動します。  
Move the "UI_Blooman" folder into the "Plugins" folder.

- Projectを開き、Pluginsウィンドウからプラグインが適用されていることを確認します。  
Open Project and verify that the plug-in has been applied from the Plugins window.


# 使い方 / Usage
2つの方法があります。  
There are two methods.

- Hiearchyタブから発光させたい要素を右クリックし、「Warp with」コマンドでFake Bloomを選択します。  
From the Hiearchy tab, right-click the element you want to emit and select Fake Bloom with the "Wrap with" command.

- PaletteタブからFake Bloomを選び、HierachyタブやDeisngerタブにドラッグ＆ドロップします。その後、子要素を追加します。  
Select Fake Bloom from the Palette tab and drag and drop it to the Hierachy or Deisnger tab. Then add child elements.  


# パラメータ / Parameter

## Overhang
発光部分のはみだし量を設定します。広く発光させたい場合は大きくする必要があります。  
Sets the amount of overhang of the bloom area. If you want the light to emit over a wider area, you need to increase the overhang.


## Use Texture
Bloom Textureパラメータに設定したテクスチャアセットを使って描画するかどうかを選択します。  
Select whether to draw using the texture asset set in the Bloom Texture parameter.

これがオフの場合、Fake BloomはBuild Phaseパラメータの設定に従って一時的なブルームテクスチャを自動的に作ります。  
If this is off, Fake Bloom will automatically create a temporary bloom texture according to the Build Phase parameter setting.   


## Bloom Texture
Use Textureパラメータがオンの時、描画に使うテクスチャアセットを指定します。  
When the Use Texture parameter is on, it specifies the texture asset to be used for drawing.

## Builder
ブルームテクスチャ生成処理に使うクラスを指定します。  
Specifies the class used for the bloom texture generation process.

### Alpha to Luminance
子要素の全体を発光させるか、高輝度なピクセルを発光させるかを設定します。  
Sets whether to emit the entire child element or the high intensity pixels.

### Luminance Threshold
ブルームさせる輝度の閾値を設定します。  
Sets the threshold for luminance to be emitted.

### Strength
ブルームの強さを設定します。  
Sets the strength of the bloom.

### Spread
ブルームの広がりを小さく調整します。  
Adjust the bloom spread to a smaller size.

### Max Mip Level
ブルームの広がりを大きく調整します。  
Adjusts the spread of the bloom to a greater extent.

内部的には、ブルームテクスチャ生成に使うMip Mapテクスチャの最大レベル指定です。これが大きすぎると、領域全体が発光してしまいます。  
Internally, this is the maximum level specification for the Mip Map texture used to generate the bloom texture. If this is too large, the entire area will emit light.


### Compression
生成されるブルームテクスチャのサイズ圧縮度を設定します。この値が高いほど圧縮されます。  
Sets the size compression of the generated bloom texture. The higher this value, the more compressed the texture.


### Build Phase

#### At Create
Widgetの生成時に1回だけブルームテクスチャを作ります。ゲーム実行時と同じ処理です。  
Bloom texture is created only once when the widget is generated. This is the same process as when the game is executed.


#### At Design Time
Widgetの生成時に加えて、Widget Blueprintで編集中は常にブルームテクスチャを更新します。  
In addition to when the widget is generated, the Bloom texture is always updated while editing in the Widget Blueprint.

このモードはFake Bloomの子要素を調整しながらブルームの見た目を確認するために存在します。  
This mode exists to check the appearance of the bloom while adjusting the child elements of Fake Bloom.

ゲーム実行時は「At Create」と同じ挙動になります。  
When the game is executed, the behavior is the same as "At Create".  


#### Always
ゲーム実行時も常にブルームテクスチャを更新し続けます。  
The bloom texture is constantly updated even when the game is running.

パフォーマンスに重大な影響を与える可能性があります。  
This can have a significant impact on performance.  

## Painter
ブルームテクスチャ描画処理に使うクラスを指定します。  
Specifies the class used for the bloom texture drawing process.  

## Appearance
### Paint Material
ブルームテクスチャ描画処理に使うMaterialを指定します。  
Specifies the Material used for the bloom texture rendering process.

デフォルトはAdditive BlendのMaterialになっています。気に入らない場合は変更が可能です。  
The default is Additive Blend's Material. If you do not like it, you can change it.  

このMaterialのParameterはSequencer Trackとして追加可能です。  
The Parameter of this Material can be added as a Sequencer Track.


### Tint Color
Paint MaterialのParameterのうち、"Tint Color"パラメータを制御します。  
Controls the "Tint Color" parameter of the Paint Material Parameter.

Sequencer Trackを設定せずにTint Colorを設定したいケースのために、特別にパラメータ化されています。  
It is specially parameterized for cases where you want to set Tint Color without setting Sequencer Track.  


### Size Scale
ブルームテクスチャ描画時の拡大縮小を設定します。アニメーション演出用です。  
Sets the scaling when drawing bloom textures. This is for animation effect.


## Static Texture Create
### Texture Format
ブルームテクスチャをテクスチャアセット化する時のフォーマットを設定します。
Sets the format in which bloom textures are converted to texture assets.


# 今後の予定 / Road map
自分のProjectで使いながら、必要に応じてプラグインを更新していく予定です。  
I will update the plugin as needed as I use it in my Project.

[Github Project](https://github.com/users/seiko-dev/projects/1)でタスク管理しています。  
I manage tasks on [Github Project](https://github.com/users/seiko-dev/projects/1).

# 参考資料 / Reference
情報を残してくださる方々に感謝いたします。🙏  
Thanks to those who leave information.

[UMG(Widget)をテクスチャ(RenderTarget)に描画する方法について (サンプル配布) (UE4.21に対応) - ぼっちプログラマのメモ  ](https://pafuhana1213.hatenablog.com/entry/2017/05/21/171809)  


[Custom Bloom Post-Process in Unreal Engine | Froyok - Fabrice Piquet](https://www.froyok.fr/blog/2021-12-ue4-custom-bloom/)  


[【UE4】フォルダ選択ダイアログを作る - Qiita](https://qiita.com/Rinderon/items/059463ea4e44fc54f121)


[Slateのテキストに無理やり(？)アクセスする方法 - Qiita](https://qiita.com/Naotsun/items/5fa305cf91c3dec55c16)


[UnrealC++でSlateのクラスをキャストする - Qiita](https://qiita.com/Naotsun/items/47945fc303b986571063)


[UnrealC++でエディタ上の入力をプログラムから行う - Qiita](https://qiita.com/Naotsun/items/2759712e5afb8212f24b)


# 作者 / Author
[seiko_dev](https://twitter.com/seiko_dev)
