
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
