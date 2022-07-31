# UI_Blooman
Unreal Engine用プラグインです。子要素を擬似的に発光させる「Fake Bloom」ウィジェットが追加されます。  


# 特徴 / Feature

- 発光部分の描画結果をテクスチャアセットに保存することが可能です。

- Fake Bloomのパラメータにテクスチャアセットを指定した場合、単にテクスチャを描画するだけのWidgetとして動作します。つまり、パフォーマンスへの影響を小さくできます。


- パフォーマンスに注意すれば、オンデマンドで処理を適用する事もできます。例えば、プレイヤーが変更可能な名前テキストにブルーム演出を加えられます。  
If you are careful about performance, you can also apply processing on demand. For example, a bloom effect can be added to the name text that can be changed by the player.  

- さらなるパフォーマンスへの注意を払えば、完全リアルタイムに処理を適用できます。つまり、発光対象のアニメーションが可能になります。Niagara UI Rendererプラグインなどの魅力を向上させます。  
With additional performance attention, processing can be applied in full real-time. This means that animations of luminous objects are possible, enhancing the attractiveness of the Niagara UI Renderer plug-in, for example.

- ブルームテクスチャ生成処理と、テクスチャ描画処理はそれぞれ独立したBlueprintになっています。より良い実装の処理に書き換えることができます。  
The texture generation process and the texture rendering process are independent Blueprints. They can be rewritten for better implementation.

- ブルームテクスチャ発光部分を描画するMaterialも指定可能です。さらに、このMaterialのパラメータはSequencerのTrackとして制御できます。


# 注意事項 / Caution
このプラグインはまだ開発途上なので、多数の問題があります。

- C++ Projectで使う必要があります。Blueprint Projectで使用した場合、パッケージした実行ファイルでエラーが出ます。

  - 非公式な回避策として、Marketplace提供のプラグイン(例えばNiagara UI Renderer)を使用すると、Blueprint Projectであっても正常にパッケージした実行ファイルが動作する場合があるようです。(機序はよくわかりません)

- 破壊的変更が加えられる可能性があります。バージョンアップによって、以前のパラメータ設定が失われるかもしれません。

- 実際に作品に使用され、実用に耐えたという証明を持っていません。

- 処理負荷は最適化されていません。


# 対応バージョン / Supported Versions
5.0.3 or later


# インストール手順 / Installation

- まず注意事項を読みます。この項目のすぐ上にあります。

- [Releases](https://github.com/seiko-dev/UI_Blooman/releases)からzipファイルをダウンロードします。  
Download the zip file from [Releases](https://github.com/seiko-dev/UI_Blooman/releases).

- zipファイル内からUI_Bloomanフォルダを展開します。

- 適用したいプロジェクトにPluginsフォルダを作ります。

- PluginsフォルダにUI_Bloomanフォルダを移動します。

- Projectを開き、Pluginsウィンドウからプラグインが適用されていることを確認します。


# 使い方 / Usage
2つの方法があります。

- Hiearchyタブから発光させたい要素を右クリックし、「Warp with」コマンドでFake Bloomを選択します。

- PaletteタブからFake Bloomを選び、HierachyタブやDeisngerタブにドラッグ＆ドロップします。その後、子要素を追加します。


# パラメータ / Parameter

## Overhang
発光部分のはみだし量を設定します。広く発光させたい場合は大きくする必要があります。

## Use Texture
Bloom Textureパラメータに設定したテクスチャを使って描画するかどうかを選択します。


これがオフの場合、Fake BloomはBuild Phaseパラメータの設定に従って一時的な発光テクスチャを自動的に作ります。


## Bloom Texture
Use Textureパラメータがオンの時、描画に使うテクスチャアセットを指定します。


## Builder
ブルームテクスチャ生成処理に使うクラスを指定します。


### Alpha to Luminance
子要素の全体を発光させるか、高輝度な部分を発光させるかを設定します。


### Luminance Threshold
発光させる輝度の閾値を設定します。


### Strength
発光の強さを設定します。


### Spread
発光の広がりを微調整します。


### Max Mip Level
発光の広がりを大きく調整します。


内部的には、ブルームテクスチャ生成に使うMip Mapテクスチャの最大レベル指定です。これが大きすぎると、領域全体が発光してしまいます。


### Compression
生成されるブルームテクスチャのサイズ圧縮度を設定します。この値が高いほど圧縮されます。

### Build Phase

#### At Create
Widgetの生成時に1回だけブルームテクスチャを作ります。ゲーム実行時と同じ処理です。

#### At Design Time
Widgetの生成時に加えて、Widget Blueprintで編集中は常にブルームテクスチャを更新します。

このモードはFake Bloomの子要素を調整しながらブルームの見た目を確認するために存在します。

ゲーム実行時は「At Once」と同じ挙動になります。


#### Always
ゲーム実行時も常にブルームテクスチャを更新し続けます。

パフォーマンスに重大な影響を与える可能性があります。


## Appearance
### Painter
ブルームテクスチャ描画処理に使うクラスを指定します。


### Paint Material
ブルームテクスチャ描画処理に使うMaterialを指定します。

デフォルトはAdditive BlendのMaterialになっています。気に入らない場合は変更が可能です。

このMaterialのParameterはSequencer Trackとして追加可能です。


### Tint Color
Paint MaterialのParameterのうち、"Tint Color"パラメータを制御します。

Sequencer Trackを設定せずにTint Colorを設定したいケースのために、特別にパラメータ化されています。


### Size Scale
ブルームテクスチャ描画時の拡大縮小を設定します。アニメーション演出用です。


## Static TExture Create
### Texture Format
ブルームテクスチャをテクスチャアセット化する時のフォーマットを設定します。


# 今後の予定 / Road map
自分のProjectで使いながら、必要に応じて更新していく予定です。[Github Project(Beta)](https://github.com/users/seiko-dev/projects/1)でタスク管理しています。



# 参考資料 / Reference
情報を残してくださる方々に感謝いたします。

[UMG(Widget)をテクスチャ(RenderTarget)に描画する方法について (サンプル配布) (UE4.21に対応) - ぼっちプログラマのメモ  ](https://pafuhana1213.hatenablog.com/entry/2017/05/21/171809)  


[Custom Bloom Post-Process in Unreal Engine | Froyok - Fabrice Piquet](https://www.froyok.fr/blog/2021-12-ue4-custom-bloom/)  


[【UE4】フォルダ選択ダイアログを作る - Qiita](https://qiita.com/Rinderon/items/059463ea4e44fc54f121)


[Slateのテキストに無理やり(？)アクセスする方法 - Qiita](https://qiita.com/Naotsun/items/5fa305cf91c3dec55c16)


[UnrealC++でSlateのクラスをキャストする - Qiita](https://qiita.com/Naotsun/items/47945fc303b986571063)


[UnrealC++でエディタ上の入力をプログラムから行う - Qiita](https://qiita.com/Naotsun/items/2759712e5afb8212f24b)


# 作者 / Author
[seiko_dev](https://twitter.com/seiko_dev)
