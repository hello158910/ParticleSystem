#ifndef __CPARTICLE_H__
#define __CPARTICLE_H__

#define STAY_FOR_TWOSECONDS 0	// 在手指滑過的地方產生 particle
#define RANDOMS_FALLING     1	// 在手指滑過的地方往下產生 particle
#define FREE_FLY 2				// 在手指滑過的地方任意向四方產生 particle
#define EXPLOSION 3				// 噴射出圓形的煙火
#define HEARTSHAPE 4			// 噴射出心型的煙火
#define BUTTERFLYSHAPE	5		// 噴射出蝴蝶外型的煙火
#define FIREWORK 6
#define WATER 7
#define BOOM 8

#define EMITTER_DEFAULT 99
#include "cocos2d.h"

class CParticle
{
private:
	cocos2d::Sprite *_Particle;	// 分子本體
	cocos2d::Point  _OldPos;		// 分子前一個位置
	cocos2d::Point  _Pos;			// 分子目前的位置
	cocos2d::Point  _Direction;	// 分子目前的運動方向，單位向量

	float _fVelocity;	// 分子的速度
	float _fLifeTime;	// 生命週期
	float _fIntensity;  // 分子能量的最大值，通常用於改變分子的顏色
	float _fOpacity;	// 目前分子的透明度
	float _fSpin;		// 分子的旋轉量
	float _fSize;		// 分子的大小
	float _fGravity;	// 分子的大小
	float _fWind;
	cocos2d::Color3B _color;	// 分子的顏色
	// 時間
	float _fElapsedTime; // 分子從顯示到目前為止的時間
	float _fDelayTime;	 // 分子顯示前的延遲時間
	// 行為模式
	int _iType;
	// 顯示與否
	bool _bVisible;
	
public:
	CParticle();
	void setParticle(const char *pngName, cocos2d::Layer &inlayer);
	void setOpacity(const float z) { _fOpacity = z; }
	bool doStep(float dt);
	void setPosition(const cocos2d::Point &inPos);
	void setColor(cocos2d::Color3B &color) { _color = color;}
	void setBehavior(int iType); // 設定分子產生的起始行為模式
	void setVisible();
	void setUnVis();
	void setGravity(const float fGravity);
	void setVelocity(const float v) { _fVelocity = v; }
	void setWind(const float v) { _fWind = v; }
	void setSpin(const float r) { _fSpin = r; }
	void setLifetime(const float lt);
	void setDirection(const cocos2d::Point pt) { _Direction = pt; }
	void setSize(float s) { _fSize = s; _Particle->setScale(_fSize); }
	void setPic(int p);
	float getPosX() { return _Pos.x; }
	float getPosY() { return _Pos.y; }

};

#endif