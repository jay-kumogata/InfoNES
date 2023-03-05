//
// kakipee: InfoNES Advance V0_Hazuki用 ROM書き込みツール
//
// Copyright (C) Jay's Factory 2002
//

#include <stdio.h>

int main(int argc, char* argv[])
{
  FILE *fp1, *fp2;
  char buf[ 16 ];
  int i;

  // 開始処理
  if ( ( fp1 = fopen("InfoNES.mb.gba", "r+b" ) ) == NULL )
  {
    printf( "Can't open InfoNES.mb.gba.\n" );
    return -1;
  }

  if ( argc != 2 || ( fp2 = fopen(argv[1], "rb" ) ) == NULL )
  {
    printf( "Can't open ROM file.\n" );
    return -1;
  }

  // 開始位置の設定
  fseek( fp1, 0x130f1, SEEK_SET );

  printf ("Now writing ");

  // バッファサイズ毎に読み込み
  // ROMサイズがバッファサイズで割り切れないと正常に動かない
  i = fread( buf, sizeof buf, 1, fp2 );
  while ( i > 0 )
  {
    // バッファサイズ毎に書き込み
    fwrite( buf, sizeof buf, i, fp1 );
    i = fread( buf, sizeof buf, 1, fp2 );
    printf (".");
  }
  printf(" done.\n");

  // 終了処理
  fflush( fp1 );
  fclose( fp1 );
  fclose( fp2 );

	return 0;
}
