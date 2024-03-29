更新履歴
- 第1版（2001/10/21）
- 第2版（2021/04/10）　Markdown化
___

## エミュレータに関する質問集 

　Marat Fayzullin氏(BERO氏翻訳)による [コンピュータエミュレータの書き方](https://fms.komkon.org/EMUL8/HOWTO.html) に影響を受けて，この文書を書くことにしました．本文書では，[InfoNES](https://github.com/jay-kumogata/InfoNES)の開発を通じて得られたノウハウを，質問形式にして，まとめてあります．エミュレータを開発したいという人には，役立つと思います．

### 一般的な質問

#### エミュレータ(Emulator)とは，なんですか？

  
　エミュレータとは，あるハードウェア用に開発されたソフトウェアを動作させることができるソフトウェアのことです．例えば，ファミコン(TM)エミュレータでは，ファミコン用に開発されたカートリッジを動作させて，ゲームを楽しむことができます．

#### コンソールエミュレータ(Console Emulator)とは，なんですか？

  
　コンソール(Console)とは，ゲーム専用のハードウェアのことです．例えば，SONY PlayStation2や任天堂GameCubeは，コンソールです．したがって，コンソールエミュレータとは，ゲーム専用機を対象としたエミュレータのことです．

#### なぜ，エミュレータを書くのですか？

  
いくつかの理由があると思います。  

1.  ゲームで遊びたいから
2.  技術的に興味があり，書いてみたかったから
3.  目立ちたかったから

　また，GameBoyエミュレータD-BOYの作者として有名な某吉氏は，emulation9のインタビュで，次のように述べています．

「エミュレータは，単に，ゲームがしたいからという理由で作るようなプログラムではない」 某吉氏は，理由1の存在を完全に否定してしまってますが，個人的にも，ほとんどないと思います．

#### エミュレータでどんなものがエミュレートできますか?
  
　Mart氏の文章にも似たような質問がありましたが，これもよく聞かれる質問です．

　理論的には，どんなものでもエミュレートできるはずです．ただ，実行速度という点で，実用的に使われているのは，下記のような例です．

1.  ゲーム機(例:ファミコン)
2.  コンピュータ(例:X68000)
3.  PDA(例:palm)

#### 仮想マシンとCPUエミュレータは、どう違うのですか?
  
　最近気付いたのですが，CPUエミュレータと仮想マシンは，非常に似ています．構造的には，まったく同じだと思います．

　違っている点としては，対象としている命令セットが，実際に存在しているチップか，存在しないチップかという点だけだと思います（ただ，例外的に，JavaChipのように，実際に存在しているが，普通，仮想マシンと呼ぶというな例もあるので，両者の境界は曖昧です）．

　最近，エミュレータという言葉を使うと，違法性が高いような風潮があるので，それを避けるために，仮想マシンという言葉を使うこともあるようです．例えば，Linux上で動作する「Windows仮想マシン」のような記述を見掛けることができます．

### 技術的な質問

#### コンソールエミュレータを書きたいのですが，どうすればいいですか？
  
　コンソールエミュレータは，通常，下記のようなモジュールから構成されています（もう少し複雑な構成をしているゲーム専用機もあります）．下記のモジュールを実装することで，コンソールエミュレータの機能が実現されます．以下では，モジュールごとに機能概要を説明していきます．

-   CPU（Central Processing Unit）機能を実現するモジュール（以下，CPUエミュレータ）
-   メモリ機能を実現するモジュール（以下，メモリエミュレータ）
-   PPU（Picture Processing Unit）機能を実現するモジュール
-   APU（Audio Processing Unit）機能を実現するモジュール

#### CPUエミュレータは，どのような構造をしているのですか？

　一般に，CPUエミュレータは，下記のような構造をしています．

	opcode = read( PC++ );
	
	switch ( opcode )
	{
	  case 命令1:
	    命令1に対する処理();
	    clock\_passed += clock[ opcode ];
	    break;
	  ...
	}

　各変数（各関数）は，下記を意味しています．

|変数名（関数名）|意味|
-----------------|----|
|PC|プログラムカウンタ|
|read()|主記憶（メモリマップされたI/O）から値を読み込む関数|
|clock_passed|経過したクロック数|
|clock[]|各命令を実行するのに必要なクロック数を格納した配列|

  
　このCPUエミュレータでは，クロック数を管理しています．これは，他のモジュール（特に，PPUエミュレータ）とタイミングを同期させるために利用し，より正確なエミュレーションを実現するためには，必須です．

　また，CPUエミュレータについては，すでに多くのオープンソースプロジェクトが存在し，ソースコードが公開されているので，それを再利用するのがいいかもしれません．

#### メモリエミュレータは，どのような構造をしているのですか？

  
　CPUエミュレータが，主記憶（例: RAM, ROM）や，他のモジュール（PPUエミュレータ, APUエミュレータ，ジョイパッド等）と通信する時に利用されるのが，メモリエミュレータです．他のモジュールと通信する時にも，メモリが利用されるということで，奇異に思うかもしれません．これは，メモリマップされたI/O（Memory-mapped I/O）という手法で，メモリ空間以外に独立したI/O空間を用意するのではなく，メモリ空間の一部にI/O空間を配置し，メモリを操作する感覚で，I/Oポートも操作できるという特徴があります．

　一般に，メモリエミュレータ（「読み込み」）は，下記のような構造をしています．

	read( addr )
	{
	  if ( addrはRAM領域? )
	  {
	    return RAM[ addr - RAM領域の先頭アドレス ];
	  } else 
	  if ( addrはROM領域? )
	  {
	    return ROM[ addr - ROM領域の先頭アドレス ];
	  } else
	  if ( addrはメモリマップされたI/O領域? )
	  {
	    return メモリマップされたI/O用の処理();
	  }
	}

　各変数（各関数）は，下記を意味しています．

|変数名（関数名）|意味|
|----------------|----|
|RAM[]|主記憶（RAM）の内容を格納した配列|
|ROM[]|主記憶（ROM）の内容を格納した配列|

#### PPUエミュレータは，どのような構造をしているのですか？

  
　PPUエミュレータは，ビデオRAM（以下，VRAM）内のデータを画面に表示する処理を行ないます．その際，CPUエミュレータとのタイミングを同期させるために，走査線毎に実行するクロックを求めておく必要があります．例えば，ファミコンの場合，1走査線，113クロック程度に対応します．

　一般に，PPUエミュレータは，下記のような構造をしています．

	for ( scanline = 0; scanline < 1画面の操作線数; scanline++ )
	{
	  exec_cpu( 1走査線当りのクロック数 );
	
	  switch( scanline )
	  {
	    case 表示領域:
	      VRAMの内容を画面に表示;
	      break;
	    case VSYNC(垂直復帰)中:
	      VSYNCの処理;
	      break;
	    ...
	  }
	}

　各変数（各関数）は，下記を意味しています．

|変数名（関数名）|意味|
|----------------|----|
|exec_cpu()|引数で指定したクロック数分だけ，CPUを動作させる関数|

  
　通常，VRAMの変更は，画面のちらつき等を避けるために，VSYNC中に行なわれます．このため，PPUは，VSYNC中か否かの情報を，現在の状態として保持しており，I/O（メモリマップされたI/O）を利用して，読み出せるようにしてあります．プログラム側では，VSYNCを待ってから，画面更新が行なわれます．

　また，VSYNC中の処理としては，下記があります．

-   APUエミュレータの呼び出し
-   コントローラ(ジョイパッド)のエミュレーション

#### APUエミュレータは，どのような構造をしているのですか？

  
　APUエミュレータの基本的な動作は，次のようなものです．前述の通り，APUエミュレータは，VSYNC毎に呼び出されます．通常，APUエミュレータは，前回のVSYNCから現在のVSYNCの間に発生したイベント（例：周波数レジスタ，ボリュームレジスタの変更）を時系列で管理しているので，この情報を基にして，波形をレンダリングし，バッファリングしつつ，音声デバイスに出力します．ここで，バッファリングしないと，音が途切れ，「プチノイズ」の原因となります．

　一般に，APUエミュレータは，下記のような構造をしています．

	for ( time = 前回のVSYNCの時刻; time < 現在の時刻; time++ )
	{
	  if ( event_queue[ ptr ].time == time )
	  {
	    周波数レジスタの更新処理;
	    ボリュームレジスタの更新処理;
	    ptr++;
	  }
	  wave_render( time, 周波数レジスタ，ボリュームレジスタ );
	  レンダリングされた波形の音声デバイスへの出力処理;
	}

　各変数（各関数）は，下記を意味しています．

|変数名（関数名）|意味|
|----------------|----|
|event_queue[]|発生したイベントが格納されたキュー（構造体の配列）|
|event_queue[].time|イベントが発生した時刻|
|ptr|イベントキューの先頭ポインタ|
|wave_render()|周波数レジスタ値，ボリュームレジスタ値から，時刻timeから時刻time+1までの波形をレンダリングする関数|

### 法的な質問

#### エミュレータは，違法ではないのですか？

  
　具体的な判例があるわけではないので，実際には，グレーだと思いますが，定説では，エミュレータを開発すること自体は，合法だといわれています．ただし，ゲームカートリッジやBIOS等のROMイメージは，その開発者が，当然，著作権を持っているので，ROMイメージを配布したり，不正に取得（インターネットからダウンロード）したりすることは，違法になります．ここらへんが，ごっちゃになって，エミュレータ＝違法というイメージが定着しているのだと思います．

　ただ，プレステのエミュレータに関しては，不正競争防止法の問題があります．不正競争防止法では，プロテクト外しに相当する行為を違法であると定めています．例えば，プレステでは，コピーしたゲームCD-ROMを実行できないようなプロテクトがかかってますが，それを無効化するMODチップなどは，明らかに、不正競争防止法違反です．

一方，プレステエミュレータでは，コピーされたゲームCD-ROMも実行可能です．そのため，この点を指摘して，プレステエミュレータ=違法という議論もあるようです(特に，SCEI社はbleem! に対して，上記のような論理で，販売の停止を求めているようです)．

いずれにしても，グレーという状態にあることは，間違いないようです．

___

Copyright (c) 1999-2001 Jay's Factory / 2021 Kumogata Jay. All Rights Reserved.

