#include "stdafx.h"

extern STRUVER s;
extern GAMELOGIC g;
extern ObjectList o;

VOID DrawScore(HWND hwnd){
	if (g.page == PG_GAMERUNNING){
		//绘制分数
		int score = g.iPrintScore;
		int xSN = WINDOW_WIDTH - 10;
		do{
			xSN = DrawScoreNumber(xSN, 10, score % 10);
			score /= 10;
		} while (score);
		if (g.iPrintScore < g.score) g.iPrintScore += 3;
		else g.iPrintScore = g.score;
		//绘制剩余生命
		int xLN = 10;
		for (int i = 0; i < g.heart; i++){
			TransparentBlt(s.mdc, xLN, WINDOW_HEIGHT - 64, 51, 54, s.objdc, 121, 20, 51, 54, RGB(255, 255, 255));
			xLN += 51;
		}
	}
}

int GetScoreWidth(int score){
	int width = 0;
	int tscore = score;
	NumberFontInfo *tempNFI;
	do {
		tempNFI = GetScoreNumberInfo(tscore % 10);
		width += tempNFI->width;
		tscore /= 10;
	} while (tscore);
	return width;
}

VOID DrawGameOverScore(HWND hwnd){
	int score = g.iPrintScore;
	int xSN = 400 + (GetScoreWidth(score) / 2);
	do{
		xSN = DrawScoreNumber(xSN, 180, score % 10);
		score /= 10;
	} while (score);
	if (g.iPrintScore < g.score) g.iPrintScore += (g.score / 97);
	else g.iPrintScore = g.score;
}

int DrawScoreNumber(int xSN, int y, int number){
	NumberFontInfo *nfi = GetScoreNumberInfo(number);
	int x = xSN - nfi->width;
	TransparentBlt(s.mdc, x, y, nfi->width, nfi->height, s.objdc, nfi->x, nfi->y, nfi->width, nfi->height, RGB(255, 255, 255));
	return x;
}

NumberFontInfo *GetScoreNumberInfo(int number){
	static NumberFontInfo nfia[] = {
		0, 363, 47, 44,
		47, 363, 38, 44,
		85, 363, 39, 44,
		124, 363, 40, 44,
		164, 363, 42, 44,
		206, 363, 40, 44,
		246, 363, 44, 44,
		290, 363, 39, 44,
		329, 363, 41, 44,
		370, 363, 42, 44
	};
	return &nfia[number];
}

int IsMouseHintButton(int x, int y){
	if (x >= 262 && x <= 541){
		if (y >= 305 && y <= 347) return BT_START;
		if (y >= 366 && y <= 410) return BT_HIGH;
		if (y >= 430 && y <= 472) return BT_HOWTO;
		if (y >= 490 && y <= 534) return BT_ENDG;
	}
	if (x >= 13 && x <= 53 && y >= 552 && y <= 589){
		return BT_SOUND;
	}
	return 0;
}

VOID DrawTitlepage(HWND hwnd){
	SelectObject(s.bufdc, g.hbmpTitle);
	int xpos;
	if (s.iFocuMouseButton){
		if (s.bMouseKeydown) xpos = 1600;
		else xpos = 800;
	}
	else xpos = 0;

	//绘制标题
	if (g.page == PG_GAMEOVER) BitBlt(s.mdc, 0, 0, 800, 276, s.bufdc, 1600, 0, SRCCOPY);
	else BitBlt(s.mdc, 0, 0, 800, 276, s.bufdc, 0, 0, SRCCOPY);
	//绘制按钮1
	if (g.page == PG_GAMEOVER){
		if (s.iFocuMouseButton == BT_START){
			BitBlt(s.mdc, 0, 276, 800, 82, s.bufdc, xpos, 600, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 276, 800, 82, s.bufdc, 0, 600, SRCCOPY);
	}
	else{
		if (s.iFocuMouseButton == BT_START){
			BitBlt(s.mdc, 0, 276, 800, 82, s.bufdc, xpos, 276, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 276, 800, 82, s.bufdc, 0, 276, SRCCOPY);
	}
	//绘制按钮2
	if (s.iFocuMouseButton == BT_HIGH){
		BitBlt(s.mdc, 0, 358, 800, 55, s.bufdc, xpos, 358, SRCCOPY);
	}
	else BitBlt(s.mdc, 0, 358, 800, 55, s.bufdc, 0, 358, SRCCOPY);
	//绘制按钮3
	if (g.page == PG_GAMEOVER){
		if (s.iFocuMouseButton == BT_HOWTO){
			BitBlt(s.mdc, 0, 413, 800, 65, s.bufdc, xpos, 682, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 413, 800, 65, s.bufdc, 0, 682, SRCCOPY);
	}
	else{
		if (s.iFocuMouseButton == BT_HOWTO){
			BitBlt(s.mdc, 0, 413, 800, 65, s.bufdc, xpos, 413, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 413, 800, 65, s.bufdc, 0, 413, SRCCOPY);
	}
	//绘制按钮4
	if (s.iFocuMouseButton == BT_ENDG){
		BitBlt(s.mdc, 0, 478, 800, 71, s.bufdc, xpos, 478, SRCCOPY);
	}
	else BitBlt(s.mdc, 0, 478, 800, 71, s.bufdc, 0, 478, SRCCOPY);
	//绘制按钮5
	if (!s.bSound){
		if (s.iFocuMouseButton == BT_SOUND){
			if (s.bMouseKeydown) BitBlt(s.mdc, 0, 549, 800, 51, s.bufdc, 800, 102, SRCCOPY);
			else BitBlt(s.mdc, 0, 549, 800, 51, s.bufdc, 800, 51, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 549, 800, 51, s.bufdc, 800, 0, SRCCOPY);
	}
	else{
		if (s.iFocuMouseButton == BT_SOUND){
			BitBlt(s.mdc, 0, 549, 800, 51, s.bufdc, xpos, 549, SRCCOPY);
		}
		else BitBlt(s.mdc, 0, 549, 800, 51, s.bufdc, 0, 549, SRCCOPY);
	}
}