#include "stdafx.h"
extern STRUVER s;
extern GAMELOGIC g;
extern ObjectList o;

VOID GamePaint(HWND hwnd){
	static TCHAR szFpsString[80];
	static TCHAR szMouse[80];
	static TCHAR szINFO[80];
	static int icFpsString;

	//绘制背景图
	switch (g.page){
	case PG_TITLE:case PG_GAMEOVER:
		DrawTitlepage(hwnd);
		break;
	case PG_GAMERUNNING:
		SelectObject(s.bufdc, g.hbmpMap);
		BitBlt(s.mdc, 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT, s.bufdc, s.iBackX, 0, SRCCOPY);
		s.iBackX = (s.iBackX + 1) % 1600;
		break;
	}

	//调试信息输出
	if (s.bShowInfo){
		//每0.5秒计算一下FPS
		if (s.tNow - s.tLastFpsShow >= 500){
			icFpsString = _stprintf_s<80>(szFpsString, L"FPS: %.3f DI: %d", 1000 / (double)std::abs((int)s.tNow - (int)s.tPre), g.iChD);
			s.tLastFpsShow = GetTickCount();
		}
		TextOut(s.mdc, 0, 0, szFpsString, icFpsString);
		TextOut(s.mdc, 0, 15, szMouse, wsprintf(szMouse, L"MOUSE:X %d, Y: %d, Heart: %d, Score: %d", s.iMouseX, s.iMouseY, g.heart, g.score));
		TextOut(s.mdc, 0, 30, szINFO, wsprintf(szINFO, L"BackgroundX: %d", s.iBackX));
	}


	if (g.page == PG_GAMERUNNING){
		//绘制元素（子弹、敌方子弹等）
		DrawObjects(hwnd);
		DrawEnemyObjects(hwnd);
		//绘制角色
		DrawLines(hwnd);
		DrawEnemy(hwnd);
		//绘制UI
		DrawScore(hwnd);
	}
	if (g.page == PG_GAMEOVER){
		DrawGameOverScore(hwnd);
	}

	//尾部处理：复制mdc到hdc。记录本次显示时间。
	BitBlt(s.hdc, 0, 0, s.iScreenX, s.iScreenY, s.mdc, 0, 0, SRCCOPY);
	s.tPre = GetTickCount();
}

VOID DrawLines(HWND hwnd){
	int iX, iY;

	SelectObject(s.bufdc, g.hCh);
	if (g.bAnimation == TRUE){
		iX = 100 * g.iAniNum;
		iY = 100 * g.iAniRow;
		if (s.tNow - g.tLastAni >= 5){
			//播放17张动画
			if (g.iAniCount <= 17){
				if (g.bAniDi){
					g.iAniNum -= g.iAniSpeed;
					if (g.iAniNum <= 0) {
						g.iAniNum = 17;
						g.iAniRow--;
						if (g.iAniRow < 0) g.iAniRow = 3;
					}
				}
				else{
					g.iAniNum += g.iAniSpeed;
					if (g.iAniNum > 17) {
						g.iAniNum = 1;
						g.iAniRow++;
						if (g.iAniRow > 3) g.iAniRow = 0;
					}
				}
				g.iAniCount++;
			}
			else{
				g.iAniCount = 0;
				g.iAniNum = 0;
				g.iAniRow = g.iChD;
				g.bAnimation = FALSE;
			}
			g.tLastAni = GetTickCount();
		}
	}
	else{
		iX = 0;
		iY = g.iChD * 100;
	}
	TransparentBlt(s.mdc, g.iChX, g.iChY, 100, 100, s.bufdc, iX, iY, 100, 100, RGB(255, 255, 255));
}
VOID DrawObjects(HWND hwnd){
	for (std::list<GameObject *>::iterator it = o.MyBullet->begin(); it != o.MyBullet->end(); ){
		GameObject *temp = *it;
		//先将当前状态贴入mdc
		switch (temp->texture)
		{
		case TP_MYBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 131, 0, 20, 20, RGB(255, 255, 255));
			break;
		default:
			break;
		}
		//计算下一帧位置
		switch (temp->direction)
		{
		case CHD_DOWN:
			temp->y += temp->speed;
			break;
		case CHD_LEFT:
			temp->x -= temp->speed;
			break;
		case CHD_RIGHT:
			temp->x += temp->speed;
			break;
		case CHD_UP:
			temp->y -= temp->speed;
			break;
		default:
			break;
		}

		//碰撞检测
		if (o.iEnemyCount > 0){
			if (IsMybulletHint(temp->x, temp->y)){
				it = o.MyBullet->erase(it);
				delete temp;
				EnemyHinted(0);
				continue;
			}
		}

		//删除当前元件
		if (temp->x + temp->width < 0 || temp->x >= WINDOW_WIDTH || temp->y + temp->height < 0 || temp->y >= WINDOW_HEIGHT){
			it = o.MyBullet->erase(it);
			delete temp;
		}
		else{
			it++;
		}
	}
}

VOID DrawEnemy(HWND hwnd){
	//当前状态贴入MDC
	if (o.iEnemyCount > 0){
		int *dCount = (int *)o.Enemy->ExtraInfo;
		if (dCount[1] > 200)
			TransparentBlt(s.mdc, (int)o.Enemy->x, (int)o.Enemy->y, o.Enemy->width, o.Enemy->height, s.objdc, 0, 0, 121, 121, RGB(255, 255, 255));
		else if (dCount[1] > 100 && dCount[1] <= 200)
			TransparentBlt(s.mdc, (int)o.Enemy->x, (int)o.Enemy->y, o.Enemy->width, o.Enemy->height, s.objdc, 0, 121, 121, 121, RGB(255, 255, 255));
		else
			TransparentBlt(s.mdc, (int)o.Enemy->x, (int)o.Enemy->y, o.Enemy->width, o.Enemy->height, s.objdc, 0, 242, 121, 121, RGB(255, 255, 255));
		
		if (dCount[0] >= 0){
			dCount[0] = dCount[0] - 1;
		}
		else{
			o.Enemy->speed -= 1;
			if (o.Enemy->speed < 0){
				o.Enemy->speed = 0;
			}
			dCount[0] = 5;
		}

		//计算下一帧位置
		o.Enemy->x -= o.Enemy->speed;
		o.Enemy->bReady = TRUE;

		if (s.tNow - o.tEnemyScoreDesc >= 500){
			dCount[1] = dCount[1] - 1;
			o.tEnemyScoreDesc = GetTickCount();
		}

		//自身消除
		if (dCount[1] < 0){
			delete o.Enemy;
			o.iEnemyCount--;
		}
	}
}

VOID DrawEnemyObjects(HWND hwnd){
	for (std::list<GameObject *>::iterator it = o.EnemyBullet->begin(); it != o.EnemyBullet->end();){
		GameObject *temp = *it;
		//首先贴入mdc
		switch (temp->texture){
		case TP_EBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 121, 0, 10, 10, RGB(255, 255, 255));
			break;
		case TP_GBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 121, 10, 10, 10, RGB(255, 255, 255));
			break;
		}

		//计算下一帧位置
		temp->x += temp->speed;
		temp->y += temp->speedy;


		//碰撞检测
		if (g.page == PG_GAMERUNNING && s.bFMode == FALSE){
			if (IsEnemyHint(temp->x, temp->y)){
				it = o.EnemyBullet->erase(it);
				delete temp;
				g.heart--;
				if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Myhint);
				continue;
			}
		}

		//删除当前元件
		if (temp->x + temp->width < 0 || temp->x >= WINDOW_WIDTH || temp->y + temp->height < 0 || temp->y >= WINDOW_HEIGHT){
			it = o.EnemyBullet->erase(it);
			delete temp;
		}
		else{
			it++;
		}
	}
}