#!/usr/local/bin/perl

;# 'turn' トランジション用の変形用テーブルを作成する

;# 64x64 の正方形を菱形に変形する変換を考える
;#                        １１１１１１１１１１２２２２２２２２２２３３
;#    ０１２３４５６７８９０１２３４５６７８９０１２３４５６７８９０１
;#  0 ●●■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
;#  1 ●＼●●・・・・・・・・・・・・・・・・・・・・・・・・・・／■
;#  2 ●・＼・●●●・・・・・・・・・・・・・・・・・・・・・・／・■
;#  3 ■●・＼・・・●●・・・・・・・・・・・・・・・・・・・／・・■
;#  4 ■●・・＼・・・・●●●・・・・・・・・・・・・・・・／・・・■
;#  5 ■●・・・＼・・・・・・●●・・・・・・・・・・・・／・・・・■
;#  6 ■●・・・・＼・・・・・・・●●・・・・・・・・・／・・・・・■
;#  7 ■●・・・・・＼・・・・・・・・●●●・・・・・／・・・・・・■
;#  8 ■・●・・・・・＼・・・・・・・・・・●●・・／・・・・・・・■
;#  9 ■・●・・・・・・＼・・・・・・・・・・・●●●・・・β・・・■
;# 10 ■・●・・・・・・・＼・・・・・・・・・・／・・●●・↓・・・■
;# 11 ■・●・・・・・・・・＼・・・・・・・・／・・・・・●●・・・■
;# 12 ■・●・・・・・・・・・＼・・・・・・／・・・・・・・●・・・■
;# 13 ■・・●・・・・・・・・・＼・・・・／・・・・・・・・●・・・■
;# 14 ■・・●・・・・・・・・・・＼・・／・・・・・・・・・・●・・■
;# 15 ■・・●・・・・・・・・・・・＼／・・・・・・・・・・・●・・■
;# 16 ■・・●・・・・・・・・・・・／＼・・・・・・・・・・・●・・■
;# 17 ■・・●・・・・・・・・・・／・・＼・・・・・・・・・・●・・■
;# 18 ■・・・●・・・・・・・・／・・・・＼・・・・・・・・・●・・■
;# 19 ■・・・●・・・・・・・／・・・・・・＼・・・・・・・・・●・■
;# 20 ■・・・●●・・・・・／・・・・・・・・＼・・・・・・・・●・■
;# 21 ■・・・↑・●●・・／・・・・・・・・・・＼・・・・・・・●・■
;# 22 ■・・・α・・・●●●・・・・・・・・・・・＼・・・・・・●・■
;# 23 ■・・・・・・・／・・●●・・・・・・・・・・＼・・・・・●・■
;# 24 ■・・・・・・／・・・・・●●●・・・・・・・・＼・・・・・●■
;# 25 ■・・・・・／・・・・・・・・・●●・・・・・・・＼・・・・●■
;# 26 ■・・・・／・・・・・・・・・・・・●●・・・・・・＼・・・●■
;# 27 ■・・・／・・・・・・・・・・・・・・・●●●・・・・＼・・●■
;# 28 ■・・／・・・・・・・・・・・・・・・・・・・●●・・・＼・●■
;# 29 ■・／・・・・・・・・・・・・・・・・・・・・・・●●●・＼・●
;# 30 ■／・・・・・・・・・・・・・・・・・・・・・・・・・・●●＼●
;# 31 ■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■●●

;# ( この図は 32x32 )

;# ここで示した■が、●で示した菱形に変形される。
;# 変換はスキャンライン ( y 座標 ) ごとに求める。

;# cpp 先頭部分出力
open FH, ">../turntrans_table.cpp";

print FH <<EOF;
/* 'turn' トランジション用変形変換テーブル */
/* このファイルは mkturntranstable.pl により作成されました */

#include "turntrans_table.h"

const tTurnTransParams TurnTransParams[64][64] =
{
	/* { start, len, sx, sy, ex, ey, stepx, stepy } */

EOF


;# n = 0 のダミーデータ出力

print FH "\t/* n = 0 (not used) */\n";
print FH "\t{\n";

for($y = 0; $y < 64; $y++)
{
	print FH ",\n" if $y != 0;
	printf FH "\t\t/* y = %2d */ {0, 0, 0, 0, 0, 0, 0, 0}", $y;
}
print FH "\n";
print FH "\t},\n";

for($n = 1; $n <= 62; $n++)
{
	;# $y (スキャンライン)
	@starts = ();
	@ends = ();
	@len = ();
	@sx = ();
	@sy = ();
	@ex = ();
	@ey = ();
	@stepx = ();
	@stepy = ();

	;# α点、β点の計算に関してはあまり厳密な式を用いない
	;# ( 厳密に計算すると平気ではみ出ることになる )
	;# まあちょっとぐらいゆがんでいてもちっちゃいから気にならんはず

	;# α点を計算
	if($n < 32)
	{
		$p = int($n * $n / 31);
		$d = int(sin($p * 3.14159265358979 / 64)*4);
		$a_x = $p - $d;
		$a_y = 63 - $p - $d;
	}
	else
	{
		$p = 63 - int((63 - $n) * (63 - $n) / 31);
		$d = int(sin((63-$p) * 3.14159265358979 / 64)*4);
		$a_x = 63 - $p + $d;
		$a_y = $p + $d;
	}

	;# β点を計算
	$b_x = 63 - $a_x;
	$b_y = 63 - $a_y;

	print FH "\t/* n = $n */\n";
	printf FH "\t/* α = (%d, %d)  β = (%d, %d) */\n", $a_x, $a_y, $b_x, $b_y;


	for($y = 0; $y < 64; $y ++)
	{
		if($y <= $a_y)
		{
			;# (0,0) - (α点) 線分上の x 座標 $l を求める
			if($a_y == 0)
			{
				$l = 0;
			}
			else
			{
				$l = int($a_x * $y / $a_y);
			}
		}
		else
		{
			;# (α点) - (63,63) 線分上の x 座標 $l を求める
			if($a_y != 63)
			{
				$l = $a_x + int((63 - $a_x) * ($y - $a_y) / (63 - $a_y));
			}
			else
			{
				$l = 63;
			}
		}

		if($y <= $b_y)
		{
			;# (0,0) - (β点) 線分上の x 座標 $r を求める
			if($b_y == 0)
			{
				$r = 63;
			}
			else
			{
				$r = int($b_x * $y / $b_y);
			}
		}
		else
		{
			;# (β点) - (63,63) 線分上の x 座標 $r を求める
			if($b_y != 63)
			{
				$r = $b_x + int((63 - $b_x) * ($y - $b_y) / (63 - $b_y));
			}
			else
			{
				$r = 63;
			}
		}

		if($l > $r) { $r = $l; } ;# 誤差補正

		push(@starts, $l);
		push(@ends, $r);

		;# 長さ $len を求める
		$len = $r - $l + 1;

		push(@len, $len);

		;# 変換元正方形の開始位置 $sx, $sy を求める
		if($y <= $a_y)
		{
			$sx = 0;
			$sy = int(63 * $y / $a_y) if $a_y;
			$sy = 0 if !$a_y;
		}
		else
		{
			$sx = int(63 * ($y - $a_y) / (63 - $a_y));
			$sy = 63;
		}

		push(@sx, $sx << 16);
		push(@sy, $sy << 16);

		;# 変換元正方形の終了位置 $ex, $ey を求める
		if($y <= $b_y)
		{
			$ex = int(63 * $y / $b_y) if $b_y;
			$ex = 63 if !$b_y;
			$ey = 0;
		}
		else
		{
			$ex = 63;
			$ey = int(63 * ($y - $b_y) / (63 - $b_y));
		}

		push(@ex, $ex << 16);
		push(@ey, $ey << 16);

		;# 変換元正方形を走査するための x, y のステップ を
		;# 16bit目に小数点を持つ固定小数点数で求める
		if($len >= 2)
		{
			$stepx = int(($ex - $sx) / ($len-1) * 65536);
			$stepy = int(($ey - $sy) / ($len-1) * 65536);
		}
		else
		{
			$stepx = 0;
			$stepy = 0;
		}

		push(@stepx, $stepx);
		push(@stepy, $stepy);
	}

	;# データの書き出し
	print FH "\t{\n";

	for($y = 0; $y < 64; $y++)
	{
		print FH ",\n" if $y != 0;
		printf FH "\t\t/* y = %2d */ /* l = %3d, r = %2d */ {%2d, %2d, %7d, %7d, %7d, %7d, %8d, %8d}",
			$y, $starts[$y], $ends[$y],
			$starts[$y], $len[$y],
			$sx[$y], $sy[$y], $ex[$y], $ey[$y],
			$stepx[$y], $stepy[$y];
	}

	print FH "\n";
	print FH "\t},\n";
}


;# n = 63 のダミーデータ出力

print FH "\t/* n = 63 (not used) */\n";
print FH "\t{\n";

for($y = 0; $y < 64; $y++)
{
	print FH ",\n" if $y != 0;
	printf FH "\t\t/* y = %2d */ {0, 0, 0, 0, 0, 0, 0, 0}", $y;
}
print FH "\n";
print FH "\t}\n";


print FH "};\n";

;# ヘッダ出力

open FH, ">../turntrans_table.h";

print FH <<EOF;
/* 'turn' トランジション用変形変換テーブル */
/* このファイルは mkturntranstable.pl により作成されました */

#ifndef turntrans_table_H
#define turntrans_table_H

#pragma pack(push, 4)
struct tTurnTransParams
{
	int start;
	int len;
	int sx; // source start x ( * 65536 )
	int sy; // source start y ( * 65536 )
	int ex; // source end x ( * 65536 )
	int ey; // source end y ( * 65536 )
	int stepx; // source step x ( * 65536 )
	int stepy; // source step y ( * 65536 )
};
#pragma pack(pop)

extern const tTurnTransParams TurnTransParams[64][64];


#endif

EOF



