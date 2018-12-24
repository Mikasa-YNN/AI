#include <stdio.h>
#include <malloc.h>
#include<string>
#include<iostream>
#include <windows.h>
#pragma warning(disable: 4996)
using namespace std;

#define BMPREAD 1024*1024*30 // ３０ＭＢまでのbmpファイルに対応
//参考<https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q14159476629>

typedef struct
{
	int size; // ファイルサイズ
	short reserve1; // ０で固定
	short reserve2; // ０で固定
	int offset; // RGBデータの場所
	int structsize; // 構造体のサイズ
	int width; // 幅
	int height; // 高さ
	short plane; // １で固定
	short bitcount; // １ピクセルあたりの色数
	int compress; // 圧縮・非圧縮
	short dsize; // 非圧縮の場合は０で良い
	int xps; // 水平方向解像度
	int yps; // 垂直方向解像度
	int colortbl; // カラーテーブルの個数
	int important; // 何か知らん
}BMPINFO;

typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGBDATA;

int main(void)
{
	int exit;//終了用
	int x;//x
	int y;//y
	int num;//座標からの番地計算結果格納用
	int pixel;//ピクセルの数
	int B, G, R;
	int pixelsize;//画質変更時の一ピクセル分の大きさを取得
	int Bave;
	int Gave;
	int Rave;//平均値格納用
	int cursorw = 0;
	int cursorh = 0;//サイズ調整用カーソル
	R = 0;
	G = 0;
	B = 0;

	unsigned char *p = (unsigned char*)malloc(BMPREAD);
	FILE *fp;

	fp = fopen("c:/Users/noriy/source/repos/AI_Chal/a.bmp", "rb"); // ディレクトリ設定要注意

	if (fp == NULL) {
		printf("ファイルオープンエラー\n");
		return -1;
	}

	fread(p, 1, BMPREAD, fp); // bmpに書き込まれているデータをメモリに展開する

	BMPINFO *bmpinf;
	RGBDATA *rgbdat;

	bmpinf = (BMPINFO*)(p + 2);
	rgbdat = (RGBDATA*)(p + (bmpinf->offset));

	printf("%d\n", bmpinf->size); // ファイルサイズ
	printf("%0d\n", bmpinf->width); // 幅
	printf("%0d\n", bmpinf->height); // 高さ

	x = 16;
	y = 16;

	//cin >> x;
	//cin >> y;

	num = (x - 1) + (y - 1)*bmpinf->width;//x,yの値から計算
	pixel = bmpinf->height*bmpinf->width;//ピクセルの数算出-1すると最後の番地になる
	//cout << pixel << endl;

	if (bmpinf->width >= bmpinf->height) {
		pixelsize = bmpinf->height / 16;
		cursorw = (bmpinf->width - pixelsize * 16) / 2;//はみ出てる分
	}
	if (bmpinf->width < bmpinf->height) {
		pixelsize = bmpinf->width / 16;
		cursorh = (bmpinf->width - pixelsize * 16) / 2;
	} 

	for (int i = 0; i < 16; i++) {
		for (int i2 = 0; i2 < 16; i2++) {
			for (int x = pixelsize * i + cursorw; x < cursorw + pixelsize * (i + 1); x++) {//平均
				for (int y = pixelsize * i2 + cursorh; y < cursorh + pixelsize * (i + 1); y++) {
					num = x + y * bmpinf->width;//x,yの値から計算
					Bave = ((rgbdat + num)->b) + B;
					Gave = ((rgbdat + num)->g) + G;
					Rave = ((rgbdat + num)->r) + R;
				}
			}
			Bave = Bave / pixelsize * pixelsize;
			Gave = Gave / pixelsize * pixelsize;
			Rave = Rave / pixelsize * pixelsize;
			cout << "B:" << Bave << endl;
			cout << "G:" << Gave << endl;
			cout << "R:" << Rave << endl;
			Bave = 0;
			Gave = 0;
			Rave = 0;
		}
	}

	fclose(fp);
	free(p); // メモリの開放
	cout << "なにか入力したら終了します" << endl;
	cin >> exit;
}
// y
//↑1
//・→x
