## ビルド

`-s`と`-j`の引数は適宜変更してください。

### windowsの場合

`cocos run -s ルートディレクトリパス -p win32`

### Ansdroidの場合

`cocos run -s ルートディレクトリパス -p android --ap android-19 -j 並列実行数`

## 開発環境

Cocos2d-xのREADMEから引用

	Build Requirements
	
	    Mac OS X 10.7+, Xcode 5.1+
	    or Ubuntu 12.10+, CMake 2.6+
	    or Windows 7+, VS 2013+
	    Python 2.7.5
	    NDK r10c+ is required to build Android games
	    Windows Phone/Store 8.1 VS 2013 Update 4+ or VS 2015
	    Windows Phone/Store 10.0 VS 2015
	    JRE or JDK 1.6+ is required for web publishing

	Runtime Requirements
	
	    iOS 5.0+ for iPhone / iPad games
	    Android 2.3+ for Android games
	    Windows 8.1 or Windows 10.0 for Windows Phone/Store 8.1 games
	    Windows 10.0 for Windows Phone/Store 10.0 games
	    OS X v10.6+ for Mac games
	    Windows 7+ for Win games
	    Modern browsers and IE 9+ for web games


## AndroidのAPIバージョンについて

現在19まで対応。
16については後日。


## git使用方針

とりあえず暫定で決めちゃいます。

- rebaseは使い方がわからない人は禁止
- mergeはno-ffのみ
- masterは安定版のみに使用する
- 統合ブランチはdevelop
- developからトピックブランチを切る
	- 機能追加等→feature 内容のわかる名前をつける
	- バグ修正→bugfix 内容のわかる名前をつける
- 中間ファイル等は除外しコミットしない(`git add --all`は禁止)
- 生成されたバイナリもコミットしない(gitにはソースコードのみを構築する)
- gitignoreはなるべく狭い範囲で作る(VisualStudioのgitignoreはproj.win32内のみに適用するなど)
- よくわかんなければSlackに投げる

GitHubのwikiやissueも上手く活用していきたいところ。
