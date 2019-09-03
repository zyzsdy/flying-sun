#include "stdafx.h"
extern STRUVER s;
extern GAMELOGIC g;
extern ObjectList o;

VOID DiChange(HWND hwnd, int DI){
	int Length;
	BOOL bfb;//前进/后退

	switch (std::abs(DI - g.iChD)){
	case 0:Length = 0; break;
	case 1:case 3:Length = 1; break;
	case 2:Length = 2; break;
	default:Length = 0; break;
	}

	if (DI - g.iChD >= 0)
		bfb = FALSE;
	else
		bfb = TRUE;
	if (DI == 0 && g.iChD == 3)bfb = FALSE;
	if (DI == 3 && g.iChD == 0)bfb = TRUE;

	switch (Length){
	case 0:break;
	case 1:case 2:
		g.iAniSpeed = Length;
		g.iAniCount = 0;
		if (bfb){
			g.iAniRow = g.iChD - 1;
			if (g.iAniRow < 0)g.iAniRow = 3;
			g.iAniNum = 17;
		}
		else{
			g.iAniRow = g.iChD;
			g.iAniNum = 1;
		}
		g.bAniDi = bfb;
		g.bAnimation = TRUE;
		break;
	}
	g.iChD = DI;
}

VOID GameControl(HWND hwnd){
	//主角方向控制
	if (g.ctlDOWN == TRUE){
		g.iChY += 3;
	}
	if (g.ctlLEFT == TRUE){
		g.iChX -= 3;
	}
	if (g.ctlRIGHT == TRUE){
		g.iChX += 3;
	}
	if (g.ctlUP == TRUE){
		g.iChY -= 3;
	}
	//主角活动区域限制
	if (g.iChX + 50 <= 0) g.iChX = -50;
	if (g.iChX + 50 >= WINDOW_WIDTH) g.iChX = (WINDOW_WIDTH)-50;
	if (g.iChY + 50 <= 0) g.iChY = -50;
	if (g.iChY + 50 >= WINDOW_HEIGHT) g.iChY = (WINDOW_HEIGHT)-50;
	//主角和敌人的碰撞检测——如果碰撞则减3颗心。
	if (g.page == PG_GAMERUNNING && o.iEnemyCount > 0 && IsMyChaHint()){
		g.heart -= 3;
	}
	//如果心数量归零，则游戏结束
	if (g.page == PG_GAMERUNNING &&g.heart <= 0){
		InitPage_Over(hwnd);
	}
	
	s.tPreCtrl = GetTickCount();
}

VOID CreateObject(int type){
	GameObject *newobj;

	switch (type)
	{
	case TP_MYBULLET:
		newobj = new GameObject;
		if (g.iChD == CHD_UP || g.iChD == CHD_DOWN)
			newobj->BuildMyBullet(g.iChX + 40, g.iChY + 50, g.iChD, 5);
		else
			newobj->BuildMyBullet(g.iChX + 50, g.iChY + 40, g.iChD, 5);
		o.MyBullet->push_back(newobj);
		break;
	default:
		break;
	}
	if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Bullet);
}

VOID EnemyBuild(HWND hwnd){
	if (g.page == PG_GAMERUNNING &&o.iEnemyCount <= 0){
		o.Enemy = new GameObject();
		o.Enemy->BuildEnemy(GetRand(500), 8);
		o.iEnemyCount++;
	}
	if (g.page == PG_GAMERUNNING&&o.Enemy->bReady == TRUE){
		if (s.tNow - o.tLastShoot >= 500){
			double arg = CalcArg(o.Enemy->x + 60, o.Enemy->y + 60, g.iChX + 10, g.iChY + 10);

			GameObject *neweba = new GameObject;
			neweba->BuildEnemyBulletA(o.Enemy->x, o.Enemy->y, arg - 0.05, 4);
			o.EnemyBullet->push_back(neweba);

			neweba = new GameObject;
			neweba->BuildEnemyBulletA(o.Enemy->x, o.Enemy->y, arg + 0.35, 4);
			o.EnemyBullet->push_back(neweba);

			neweba = new GameObject;
			neweba->BuildEnemyBulletA(o.Enemy->x, o.Enemy->y, arg - 0.35, 4);
			o.EnemyBullet->push_back(neweba);

			o.tLastShoot = GetTickCount();
		}
		if (s.tNow - o.tGreenShoot >= 200){
			double arg = CalcArg(o.Enemy->x + 60, o.Enemy->y + 60, g.iChX + 10, g.iChY + 10);
			GameObject *neweba;
			for (int i = 0; i < 12; i++){
				neweba = new GameObject;
				neweba->BuildEnemyBulletA(o.Enemy->x, o.Enemy->y, arg + (i * 0.5) - 1.8, 4);
				neweba->texture = TP_GBULLET;
				o.EnemyBullet->push_back(neweba);
			}
			o.tGreenShoot = GetTickCount();
		}
	}
}
VOID EnemyHinted(int Param){
	int *score = (int *)o.Enemy->ExtraInfo;
	if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Hint);
	switch (Param){
	case 0:
		if (score[1] > 100){
			g.score += 100;
		}
		else{
			g.score += score[1];
		}
		score[1] -= 100;
		break;
	case 1:
		g.score += score[1];
		score[1] -= 300;
	}
	if (score[1] <= 0){
		delete o.Enemy;
		o.iEnemyCount--;
	}
}
BOOL IsMybulletHint(double x, double y){
	return IsHintHelp(x, y, o.Enemy->x, o.Enemy->y, 10, 60, 10);
}
BOOL IsEnemyHint(double x, double y){
	return IsHintHelp(x, y, g.iChX, g.iChY, 5, 50, 40);
}
BOOL IsMyChaHint(){
	return IsHintHelp(g.iChX, g.iChY, o.Enemy->x, o.Enemy->y, 50, 60, 20);
}
BOOL IsHintHelp(double _x1, double _y1, double _x2, double _y2, double r1, double r2, double rd){
	double x1 = _x1 + r1;
	double y1 = _y1 + r1;
	double x2 = _x2 + r2;
	double y2 = _y2 + r2;
	double r = r1 + r2;
	double t1 = x1 - x2;
	double t2 = y1 - y2;
	double d = std::sqrt(t1*t1 + t2*t2);
	if (d < (double)(r - rd)){
		return TRUE;
	}
	return FALSE;
}
VOID TitleButtonDo(int BTTYPE, HWND hwnd){
	switch (BTTYPE){
	case BT_START:
		InitPage_Game(hwnd);
		break;
	case BT_HIGH:
		do{
			TCHAR tempMsgBuf[72];
			wsprintf(tempMsgBuf, L"当前最高分： %d", g.maxscore);
			MessageBox(hwnd, tempMsgBuf, L"最高分", MB_ICONINFORMATION);
		} while (0);
		break;
	case BT_HOWTO:
		if (g.page == PG_GAMEOVER){
			InitPage_Title(hwnd);
		}
		else{
			MessageBox(hwnd, L"操作说明：\r\n\r\n\
							  使用『↑』『←』『↓』『→』四个方向键来控制你的人物。\r\n\
							  使用『SHIFT』来发射子弹。\r\n\r\n\
							  你的目标是消灭尽可能多的敌人。\r\n\
							  不过注意躲开敌人的子弹。\r\n\
							  子弹只有击中你的中心才会伤害到你，击中你的边缘不会有问题。", L"操作说明", MB_ICONINFORMATION);
		}
		break;
	case BT_ENDG:
		GameCleanUp(hwnd);
		PostQuitMessage(0);
		break;
	case BT_SOUND:
		s.bSound = !s.bSound;
		if (s.bSound){
			if (g.page == PG_TITLE)
				s.Sound->GameMusicplay(s.Sound->pMusic_Title);
			else
				s.Sound->GameMusicplay(s.Sound->pMusic_Game);
		}
		else{
			s.Sound->GameSoundAllstop();
		}
		break;
	}
}