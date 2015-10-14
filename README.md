IDAはオンラインRPG エターナルゾーンのアイテムデータ解析ツールです。

使用方法
・コマンドラインで使用します
・実行ファイルは付属していませんので、ソースコードからコンパイルしてください
・解析対象となるitem.datは、実行モードによって参照する場所が異なります
・<実行ファイル名> <オプション> の形式で実行します
・オプション一覧
　・r : 全データ出力モード(実行ファイルのディレクトリのitem.datを解析)
　・s : 差分出力モード(実行ファイルのディレクトリにbefore、afterのディレクトリを作成し、それぞれに差のあるitem.datを入れて解析)
　・p : データベース登録用出力モード(全データと同様で出力形式が違うだけ)
・オプション入力なしの場合は全データ出力モードになります