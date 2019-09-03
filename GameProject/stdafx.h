#pragma once

#include <Windows.h>
#include <cmath>
#include <tchar.h>
#include <algorithm>
#include <list>
#include "resource.h"
#pragma comment(lib, "Msimg32.lib") //TransparentBlt��Ҫ

//���ֲ���
#include <dsound.h>
#include <mmsystem.h>
#include <mmreg.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//��ʼ����С
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//Ԥ���庯������
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
BOOL GameInit(HWND hwnd);
VOID GamePaint(HWND hwnd);
BOOL GameCleanUp(HWND hwnd);
VOID DrawLines(HWND hwnd);
VOID DrawObjects(HWND hwnd);
VOID DrawEnemyObjects(HWND hwnd);
VOID DrawEnemy(HWND hwnd);
VOID DrawScore(HWND hwnd);
int DrawScoreNumber(int xSN, int y, int number);
VOID DrawTitlepage(HWND hwnd);
VOID InitPage_Title(HWND hwnd);
VOID InitPage_Game(HWND hwnd);
VOID InitPage_Over(HWND hwnd);
VOID GameControl(HWND hwnd);
VOID DiChange(HWND hwnd, int DI);
VOID EnemyBuild(HWND hwnd);
VOID EnemyHinted(int Param);
int GetRand(int n);


//��Ϸ��������Ч�Ĵ�����
class GameSound{
private:
	HWND soundhwnd;

	HRESULT result;      //�������ܽ�����ķ���ֵ
	LPDIRECTSOUND pDS;   //����������DirectSound����
	LPDIRECTSOUNDBUFFER pMainBuf;  //������������ָ��
	DSBUFFERDESC desc;             //���������ṹ��������ʼ����������
	WAVEFORMATEX pwfmt;            //���������ṹ�������趨���Ÿ�ʽ

	WAVEFORMATEX swfmt;    //���������ṹ
	MMCKINFO     ckRiff;   //RIFF�������Ϣ
	MMCKINFO     ckInfo;   //���������Ϣ
	MMRESULT     mmresult; //���صĽ��
	DWORD        size;     //ʵ�����ϵĴ�С
	HMMIO        hbackground;    //�򿪵Ķ�ý���ļ�
public:
	GameSound(){}
	void GameSoundInit(HWND);             //GameSound����Ľ���
	void GameSoundbufferConstruct();      //�������Ĵ���
	void GameSoundfmtSet(int, int, int);  //ͨ����������ָ�������ò��Ÿ�ʽ
	void GameSoundReadWAVfile(LPWSTR, HMMIO&);//�������ļ����벢����ϸ����HMMIO�ṹ��
	void GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER&, LPWSTR);//�������ļ�����λ�������

	//����Ϊ�����ӻ�����ָ�������
	LPDIRECTSOUNDBUFFER pMusic_Title; //����BGM
	LPDIRECTSOUNDBUFFER pMusic_Game; //��ϷBGM
	LPDIRECTSOUNDBUFFER pSound_Bullet; //�����ӵ�
	LPDIRECTSOUNDBUFFER pSound_Gameover; //��Ϸ����
	LPDIRECTSOUNDBUFFER pSound_Hint; //������ײ
	LPDIRECTSOUNDBUFFER pSound_Myhint; //�Լ�������
	LPDIRECTSOUNDBUFFER pSound_Mouseover; //�������
	LPDIRECTSOUNDBUFFER pSound_Mouseclick; //�����

	void GameSoundAllstop();  //ֹͣ���е����ֺ���Ч���š�
	void GameMusicplay(LPDIRECTSOUNDBUFFER&);     //��������ѭ������
	void GameSoundplay(LPDIRECTSOUNDBUFFER&);     //��������һ������Ч
};

//ȫ�ֱ���
struct STRUVER{
	TCHAR szAppName[30];
	TCHAR szTitle[30];
	HDC hdc, mdc, bufdc, objdc; //ȫ���豸�������
	DWORD tPre, tNow, tPreCtrl; //ʱ���¼
	DWORD tLastFpsShow;
	int iMouseX, iMouseY;//��ǰ���λ��
	int iScreenX, iScreenY; //��ǰ��Ļ��С
	int iBackX; //����ͼƬ��ǰXֵ
	GameSound *Sound; //��������Ч���Ŷ���
	BOOL bShowInfo; //�Ƿ���ʾ������Ϣ
	BOOL bFMode; //��/�ر�����ģʽ
	int iFocuMouseButton; //��ǰ��꼤��İ�ťֵ
	BOOL bMouseKeydown;
	BOOL bSound;
};

struct GAMELOGIC{
	int page; //0-���⻭�� 1-��Ϸ���� 2-Game Over
	HBITMAP hbmpTitle, hbmpMap, hCh;
	int iChX, iChY;
	BOOL ctlUP, ctlDOWN, ctlLEFT, ctlRIGHT;
	BOOL ctlFIRE;
	int iChD; //0-RIGHT 1-UP 2-LEFT 3-DOWN
	BOOL bAnimation; //FLASE-����δ���� TRUE-�������ڲ���
	int iAniNum, iAniRow, iAniCount; //��ǰ���ŵĶ������룬�Ͷ�����������������
	BOOL bAniDi; //�������� ��FLASE-���� TRUE-����
	int iAniSpeed; //���������ٶ� ��ֻ��ȡֵΪ1��2��;
	DWORD tLastAni; //�ϴζ�������ʱ��
	int score, maxscore, heart;
	int iPrintScore;
};
#define PG_TITLE 0
#define PG_GAMERUNNING 1
#define PG_GAMEOVER 2
#define CHD_RIGHT 0
#define CHD_UP 1
#define CHD_LEFT 2
#define CHD_DOWN 3

//�������
#define TP_MYBULLET 0
#define TP_ENEMY 1
#define TP_EBULLET 2
#define TP_GBULLET 3
struct GameObject{
	int height, width;
	int texture, type;
	int aniNum, direction;
	double x, y;
	double speed, speedy;
	BOOL bReady;
	void *ExtraInfo;
	~GameObject();
	void BuildMyBullet(double _x, double _y, int _dir, double _speed);
	void BuildEnemy(double _y, double _speed);
	void BuildEnemyBulletA(double sx, double sy, double arg, double _speed);
};
struct ObjectList{
	std::list<GameObject *> *MyBullet;
	std::list<GameObject *> *EnemyBullet;
	GameObject *Enemy;
	HBITMAP hbmpObj;
	int iEnemyCount;
	DWORD tEnemyScoreDesc;
	DWORD tLastShoot, tGreenShoot;
};
void CreateObject(int type);
BOOL IsMybulletHint(double x, double y);
BOOL IsEnemyHint(double x, double y);
BOOL IsMyChaHint();
BOOL IsHintHelp(double x1, double y1, double x2, double y2, double r1, double r2, double rd);
double CalcArg(double sx, double sy, double dx, double dy);

//UI����
//��������
struct NumberFontInfo{
	int x;
	int y;
	int width;
	int height;
};
NumberFontInfo *GetScoreNumberInfo(int number);
#define BT_START 912
#define BT_HIGH 913
#define BT_HOWTO 914
#define BT_ENDG 915
#define BT_SOUND 916
int IsMouseHintButton(int x, int y);
VOID TitleButtonDo(int BTTYPE, HWND hwnd);
int GetScoreWidth(int score);
VOID DrawGameOverScore(HWND hwnd);