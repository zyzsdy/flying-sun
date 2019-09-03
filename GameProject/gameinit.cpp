#include "stdafx.h"
extern STRUVER s;
extern GAMELOGIC g;
ObjectList o = { 0 };

BOOL GameInit(HWND hwnd){
	s.hdc = GetDC(hwnd);
	s.bShowInfo = s.bFMode = s.bMouseKeydown = FALSE;
	s.bSound = TRUE;
	g.maxscore = 0;

	//��ʼ�����ֺ���Ч����
	s.Sound = new GameSound();
	s.Sound->GameSoundInit(hwnd);

	//�����ڴ�������
	s.mdc = CreateCompatibleDC(s.hdc);
	s.bufdc = CreateCompatibleDC(s.hdc);
	s.objdc = CreateCompatibleDC(s.hdc);
	HBITMAP hbmp;

	s.iScreenX = GetSystemMetrics(SM_CXSCREEN);
	s.iScreenY = GetSystemMetrics(SM_CYSCREEN);

	hbmp = CreateCompatibleBitmap(s.hdc, s.iScreenX, s.iScreenY);
	SelectObject(s.mdc, hbmp);

	//��ȡ����������Ϸ��ͼ��λͼ�ļ�
	g.hbmpTitle = (HBITMAP)LoadImage(NULL, L"images//title.bmp", IMAGE_BITMAP, 2400, 747, LR_LOADFROMFILE);
	g.hbmpMap = (HBITMAP)LoadImage(NULL, L"images//background.bmp", IMAGE_BITMAP, 3200, 600, LR_LOADFROMFILE);
	s.iBackX = 0;
	//��ʼ�����⻭��
	InitPage_Title(hwnd);
	//��һ�λ���
	GamePaint(hwnd);
	return TRUE;
}
//�����ʼ��
VOID InitPage_Title(HWND hwnd){
	if(s.bSound) s.Sound->GameMusicplay(s.Sound->pMusic_Title);
	g.page = PG_TITLE;
}
VOID InitPage_Game(HWND hwnd){
	if(s.bSound) s.Sound->GameMusicplay(s.Sound->pMusic_Game);
	g.hCh = (HBITMAP)LoadImage(NULL, L"images//cha.bmp", IMAGE_BITMAP, 1800, 400, LR_LOADFROMFILE);
	o.MyBullet = new std::list<GameObject *>();
	o.EnemyBullet = new std::list<GameObject *>();
	o.hbmpObj = (HBITMAP)LoadImage(NULL, L"images//obj.bmp", IMAGE_BITMAP, 412, 407, LR_LOADFROMFILE);
	o.iEnemyCount = o.tEnemyScoreDesc = o.tGreenShoot = o.tLastShoot = 0;
	SelectObject(s.objdc, o.hbmpObj);
	//������ʼ��
	g.heart = 3;
	g.iChX = g.iChY = 100;
	g.ctlDOWN = g.ctlLEFT = g.ctlRIGHT = g.ctlUP = g.ctlFIRE = FALSE;
	g.bAnimation = g.bAniDi = FALSE;
	g.iChD = CHD_RIGHT;
	g.iAniNum = g.iAniRow = g.iAniCount = 0;
	g.iAniSpeed = 1;
	g.tLastAni = 0;
	g.score = g.iPrintScore = 0;
	//�л�����ҳ
	g.page = PG_GAMERUNNING;
}
VOID InitPage_Over(HWND hwnd){
	g.iPrintScore = 0;
	if (g.score > g.maxscore) g.maxscore = g.score;
	if(s.bSound) s.Sound->GameSoundplay(s.Sound->pSound_Gameover);
	if (o.MyBullet) delete o.MyBullet;
	if (o.EnemyBullet) delete o.EnemyBullet;
	if (o.Enemy) delete o.Enemy;
	g.page = PG_GAMEOVER;
}

//��Դ����
BOOL GameCleanUp(HWND hwnd){
	ReleaseDC(hwnd, s.hdc);
	DeleteDC(s.mdc);
	DeleteDC(s.bufdc);
	DeleteDC(s.objdc);
	s.Sound->GameSoundAllstop();
	if (s.Sound) delete s.Sound;
	s.Sound = NULL;
	return TRUE;
}