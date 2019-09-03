#include "stdafx.h"

extern STRUVER s;
extern GAMELOGIC g;
extern ObjectList o;

GameObject::~GameObject(){
	switch (this->type){
	case TP_ENEMY:
	{
					 int *temp = (int *)this->ExtraInfo;
					 delete[] temp;
	}
		break;
	default:
		break;
	}
}

void GameObject::BuildMyBullet(double _x, double _y, int _dir, double _speed){
	this->height = this->width = 20;
	this->texture = this->aniNum = 0; //未使用变量
	this->speedy = 0;
	this->type = 0;
	this->x = _x;
	this->y = _y;
	this->direction = _dir;
	this->speed = _speed;
	this->bReady = TRUE;
	this->ExtraInfo = NULL;
}

void GameObject::BuildEnemy(double _y, double _speed){
	this->height = this->width = 121;
	this->texture = this->type = TP_ENEMY;
	this->aniNum = 0;
	this->speedy = 0;
	this->x = WINDOW_WIDTH + 50;
	this->y = _y;
	this->direction = CHD_LEFT;
	this->speed = _speed;
	this->bReady = FALSE;
	int *extinfo = new int[2];
	extinfo[0] = 6;
	extinfo[1] = 300; //Score
	this->ExtraInfo = (void *)extinfo;
}

void GameObject::BuildEnemyBulletA(double sx, double sy, double arg, double _speed){
	this->height = this->width = 10;
	this->texture = this->type = TP_EBULLET;
	this->aniNum = 0;
	this->x = sx + 60;
	this->y = sy + 60;
	this->direction = CHD_RIGHT;
	this->bReady = TRUE;
	this->ExtraInfo = NULL;
	this->speedy = _speed * std::sin(arg);
	this->speed = _speed * std::cos(arg);
}

double CalcArg(double sx, double sy, double dx, double dy){
	return std::atan2(dy - sy, dx - sx);
}