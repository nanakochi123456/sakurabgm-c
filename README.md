sakurabgm-c - テキスト音楽サクラのMMLに簡易の変更を加えてアレンジするプログラム

(C) 2015 なのたん

このプログラムは自由に使っていいなの。

■使い方
コマンドプロンプトで動作するプログラムです。

・パラメータ一覧表示
sakurabgm list 入力ファイル.mml
（注：cygwinのため、文字化けします。テキストファイルに出力すると問題ありません）

・パラメータ変更
sakurabgm replace 入力ファイル.mml 出力ファイル.mml パラメータキー=値 パラメータキー=値....

Tempo=xxx と Key=xxx のみ、特別動作をします。
それ以外は、Int xxx=xxx を変更することで、動作を実現しています。

■記述方法
（例）
Int LOOP=3 		//sakurabgm:ループ数の指定,MIN=1,DEFAULT=3,MAX=50
Tempo 100 		//sakurabgm:曲の速さ,MIN=60=遅い,DEFAULT=100=標準,MAX=160,速い
Key (0) 		//sakurabgm:調性,MIN=-12,低い,DEFAULT=0=標準,MAX=12=高い
Int NODTM=1		//sakurabgm:演奏の自然さ,MIN=0=DTM,DEFAULT=1=標準（上手),MAX=3=へたくそ

※現バージョンでは、MINとMAXの値はチェックしていません。

他にも、新たなパートを加えてみたり、ドラムパートをすりかえてみたり、
IF文で同じファイルで全く違う曲にしてみるのもよいでしょう。

■詳しい動作原理
とりあえず、sample にバッチファイルがあるので、それを実行して
テキスト音楽サクラで、生成された MMLを開いて、聴いてみてください。

■その他
v1.0と名乗っていますが、評価版です。
詳しいメモリチェック、エラーチェックとか行ないきれていません。

■cSakuraについて
テキスト音楽サクラのコンソール版が評価のために入っています。
(c)クジラ飛行机     http://kujirahand.com
http://oto.chu.jp/
https://github.com/kujirahand/sakuramml
