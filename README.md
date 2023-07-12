https://github.com/cipher-413/MirishitaPointAdjustTool/ に触発されて、ミリシタ周年イベのポイント調整ツールを作りました

## 使い方
- mirishita-point-adjust-tool-2.cppに全てが書かれています
- allLives に必要に応じてライブ定義を足してください
  - https://github.com/cipher-413/MirishitaPointAdjustTool/ の付録に入っていたものは書きました
- mirishita-point-adjust-tool-2.cpp をコンパイルして実行します
  - 手元でコンパイルできなければ https://ideone.com/ などのオンライン実行環境にコードを貼り付けて入力を予めセットしておいてから実行すれば動くと思います
- input target point で目標ポイントを入力します
- input current point で現在のポイントを入力します
- input number of event items で現在持っているイベントアイテム(6周年で言うところのパレット)の個数を入力します
- ライブ総数を**だいたい**最小化したときの各ライブの必要回数が出力されます
  - 厳密には、毎日もらえるイベントアイテム540個を考慮しきれていません
  - おすすめ楽曲でもらえる1000個も考慮していません
  - あとブーストも考慮していません

## その他
- このソースコードを実行したことで起こるいかなる損害も補償はいたしかねます。自己責任でご利用ください。
- 改変・再配布は自由です。
