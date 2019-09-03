#pragma once

#include <Windows.h>
#include <cmath>
#include <tchar.h>
#include <algorithm>
#include <list>
#include "resource.h"
#pragma comment(lib, "Msimg32.lib") //TransparentBlt需要

//音乐播放
#include <dsound.h>
#include <mmsystem.h>
#include <mmreg.h>
#pragma comment(lib, "dsound.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "winmm.lib")

//初始化大小
#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

//预定义函数声明
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


//游戏音乐与音效的处理类
class GameSound{
private:
	HWND soundhwnd;

	HRESULT result;      //用来接受建立后的返回值
	LPDIRECTSOUND pDS;   //代表声卡的DirectSound对象
	LPDIRECTSOUNDBUFFER pMainBuf;  //声明主缓冲区指针
	DSBUFFERDESC desc;             //声明描述结构，用来初始化缓冲区域
	WAVEFORMATEX pwfmt;            //声明声音结构，用来设定播放格式

	WAVEFORMATEX swfmt;    //声明声音结构
	MMCKINFO     ckRiff;   //RIFF区块的信息
	MMCKINFO     ckInfo;   //子区块的信息
	MMRESULT     mmresult; //返回的结果
	DWORD        size;     //实际资料的大小
	HMMIO        hbackground;    //打开的多媒体文件
public:
	GameSound(){}
	void GameSoundInit(HWND);             //GameSound对象的建立
	void GameSoundbufferConstruct();      //缓冲区的创建
	void GameSoundfmtSet(int, int, int);  //通过主缓冲区指针来设置播放格式
	void GameSoundReadWAVfile(LPWSTR, HMMIO&);//将声音文件读入并将明细存在HMMIO结构中
	void GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER&, LPWSTR);//将声音文件读入次缓冲区中

	//以下为各个子缓冲区指针的声明
	LPDIRECTSOUNDBUFFER pMusic_Title; //标题BGM
	LPDIRECTSOUNDBUFFER pMusic_Game; //游戏BGM
	LPDIRECTSOUNDBUFFER pSound_Bullet; //发射子弹
	LPDIRECTSOUNDBUFFER pSound_Gameover; //游戏结束
	LPDIRECTSOUNDBUFFER pSound_Hint; //发生碰撞
	LPDIRECTSOUNDBUFFER pSound_Myhint; //自己被击中
	LPDIRECTSOUNDBUFFER pSound_Mouseover; //鼠标悬浮
	LPDIRECTSOUNDBUFFER pSound_Mouseclick; //鼠标点击

	void GameSoundAllstop();  //停止所有的音乐和音效播放。
	void GameMusicplay(LPDIRECTSOUNDBUFFER&);     //用来播放循环音乐
	void GameSoundplay(LPDIRECTSOUNDBUFFER&);     //用来播放一次性音效
};

//全局变量
struct STRUVER{
	TCHAR szAppName[30];
	TCHAR szTitle[30];
	HDC hdc, mdc, bufdc, objdc; //全局设备环境句柄
	DWORD tPre, tNow, tPreCtrl; //时间记录
	DWORD tLastFpsShow;
	int iMouseX, iMouseY;//当前鼠标位置
	int iScreenX, iScreenY; //当前屏幕大小
	int iBackX; //背景图片当前X值
	GameSound *Sound; //音乐与音效播放对象
	BOOL bShowInfo; //是否显示调试信息
	BOOL bFMode; //打开/关闭作弊模式
	int iFocuMouseButton; //当前鼠标激活的按钮值
	BOOL bMouseKeydown;
	BOOL bSound;
};

struct GAMELOGIC{
	int page; //0-标题画面 1-游戏进行 2-Game Over
	HBITMAP hbmpTitle, hbmpMap, hCh;
	int iChX, iChY;
	BOOL ctlUP, ctlDOWN, ctlLEFT, ctlRIGHT;
	BOOL ctlFIRE;
	int iChD; //0-RIGHT 1-UP 2-LEFT 3-DOWN
	BOOL bAnimation; //FLASE-动画未播放 TRUE-动画正在播放
	int iAniNum, iAniRow, iAniCount; //当前播放的动画号码，和动画播放张数计数。
	BOOL bAniDi; //动画方向 （FLASE-正向 TRUE-反向）
	int iAniSpeed; //动画播放速度 （只能取值为1或2）;
	DWORD tLastAni; //上次动画播放时间
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

//物体对象
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

//UI部分
//分数字体
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