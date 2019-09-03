#include "stdafx.h"
extern STRUVER s;
extern GAMELOGIC g;
extern ObjectList o;

VOID GamePaint(HWND hwnd){
	static TCHAR szFpsString[80];
	static TCHAR szMouse[80];
	static TCHAR szINFO[80];
	static int icFpsString;

	//���Ʊ���ͼ
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

	//������Ϣ���
	if (s.bShowInfo){
		//ÿ0.5�����һ��FPS
		if (s.tNow - s.tLastFpsShow >= 500){
			icFpsString = _stprintf_s<80>(szFpsString, L"FPS: %.3f DI: %d", 1000 / (double)std::abs((int)s.tNow - (int)s.tPre), g.iChD);
			s.tLastFpsShow = GetTickCount();
		}
		TextOut(s.mdc, 0, 0, szFpsString, icFpsString);
		TextOut(s.mdc, 0, 15, szMouse, wsprintf(szMouse, L"MOUSE:X %d, Y: %d, Heart: %d, Score: %d", s.iMouseX, s.iMouseY, g.heart, g.score));
		TextOut(s.mdc, 0, 30, szINFO, wsprintf(szINFO, L"BackgroundX: %d", s.iBackX));
	}


	if (g.page == PG_GAMERUNNING){
		//����Ԫ�أ��ӵ����з��ӵ��ȣ�
		DrawObjects(hwnd);
		DrawEnemyObjects(hwnd);
		//���ƽ�ɫ
		DrawLines(hwnd);
		DrawEnemy(hwnd);
		//����UI
		DrawScore(hwnd);
	}
	if (g.page == PG_GAMEOVER){
		DrawGameOverScore(hwnd);
	}

	//β����������mdc��hdc����¼������ʾʱ�䡣
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
			//����17�Ŷ���
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
		//�Ƚ���ǰ״̬����mdc
		switch (temp->texture)
		{
		case TP_MYBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 131, 0, 20, 20, RGB(255, 255, 255));
			break;
		default:
			break;
		}
		//������һ֡λ��
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

		//��ײ���
		if (o.iEnemyCount > 0){
			if (IsMybulletHint(temp->x, temp->y)){
				it = o.MyBullet->erase(it);
				delete temp;
				EnemyHinted(0);
				continue;
			}
		}

		//ɾ����ǰԪ��
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
	//��ǰ״̬����MDC
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

		//������һ֡λ��
		o.Enemy->x -= o.Enemy->speed;
		o.Enemy->bReady = TRUE;

		if (s.tNow - o.tEnemyScoreDesc >= 500){
			dCount[1] = dCount[1] - 1;
			o.tEnemyScoreDesc = GetTickCount();
		}

		//��������
		if (dCount[1] < 0){
			delete o.Enemy;
			o.iEnemyCount--;
		}
	}
}

VOID DrawEnemyObjects(HWND hwnd){
	for (std::list<GameObject *>::iterator it = o.EnemyBullet->begin(); it != o.EnemyBullet->end();){
		GameObject *temp = *it;
		//��������mdc
		switch (temp->texture){
		case TP_EBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 121, 0, 10, 10, RGB(255, 255, 255));
			break;
		case TP_GBULLET:
			TransparentBlt(s.mdc, (int)temp->x, (int)temp->y, temp->width, temp->height, s.objdc, 121, 10, 10, 10, RGB(255, 255, 255));
			break;
		}

		//������һ֡λ��
		temp->x += temp->speed;
		temp->y += temp->speedy;


		//��ײ���
		if (g.page == PG_GAMERUNNING && s.bFMode == FALSE){
			if (IsEnemyHint(temp->x, temp->y)){
				it = o.EnemyBullet->erase(it);
				delete temp;
				g.heart--;
				if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Myhint);
				continue;
			}
		}

		//ɾ����ǰԪ��
		if (temp->x + temp->width < 0 || temp->x >= WINDOW_WIDTH || temp->y + temp->height < 0 || temp->y >= WINDOW_HEIGHT){
			it = o.EnemyBullet->erase(it);
			delete temp;
		}
		else{
			it++;
		}
	}
}