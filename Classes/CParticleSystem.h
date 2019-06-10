#ifndef __CPARTICLE_SYSTEM_H__
#define __CPARTICLE_SYSTEM_H__

#include "cocos2d.h"
#include "CParticle.h"
#include <list>

using namespace std;


class CParticleSystem
{
private:
	CParticle* _pParticles;
	list<CParticle*> _FreeList;
	list<CParticle*> _InUsedList;
	int _iFree;	// �i�Ϊ�  Particle �Ӽ�
	int _iInUsed;  // ���b�ϥΪ� Particle �Ӽ�
	int _iType;  // �ثe������l�B�ʼҦ��O���@�ث��A

	bool _bEmitterOn; // Emitter �O�_���
	bool _bRaining;
	bool _bFire;
	bool _bOut = 0;
	bool _bWait;
	bool _bBoom;


public:
	// Emitter �]�w�������Ѽ�
	cocos2d::Point _emitterPt;// Emitter ����m	
	cocos2d::Point _rainPt;
	cocos2d::Point _firepos;
	cocos2d::Point _boomPt;
	float    _fDir; // Emitter ���Q�g��V�A0 �� 360�סA�f�ɰw��V����
	int		_iNumParticles;	// �C�������ͪ����l�Ӽ�
	int     _iGenParticles; // ��ثe����Ҳ��ͪ����l�Ӽ�(�H�C�����)
	float	_fSpread;	 // 0 �� 180 �סA�w�]�� 180 
	float   _fVelocity;	 // ���l�����}�t��
	float   _fLifeTime;	 // ���l���s���ɶ�
	float	_fSpin;		// ���l������q degree/sec
	float   _fGravity;
	float   _fElpasedTime;	// �g�L�h�֮ɶ��A�H�����A�@�����ͤ��l���p��̾�
	float	_fOpacity;	// �ثe���l���z����
	float	_fRed;
	float	_fGreen;
	float	_fBlue;
	int		_iStyle;
	float	_fcount, _fcount2;

	cocos2d::Point _windDir;  // ������V�A���d�ҨS����@

	CParticleSystem();
	~CParticleSystem();
	void init(cocos2d::Layer &inlayer);
	void doStep(float dt);
	void setEmitter(bool bEm) { _bEmitterOn = bEm; }
	void setRaining(bool bRain) { _bRaining = bRain; }
	void setFire(bool b) { _bFire = b; }
	void setType(int type) { _iType = type; }
	

 //   void onTouchesEnded(const cocos2d::CCPoint &touchPoint);
    void onTouchesBegan(const cocos2d::CCPoint &touchPoint);
    void onTouchesMoved(const cocos2d::CCPoint &touchPoint);

	void setGravity(float fGravity);

};

#endif