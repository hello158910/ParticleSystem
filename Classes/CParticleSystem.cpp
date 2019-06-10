#include "CParticleSystem.h"

#define NUMBER_PARTICLES 1000 // �w�]�@�����o 1000 �� Particles


USING_NS_CC;

CParticleSystem::CParticleSystem()
{
	_fGravity = 0;
	_bEmitterOn = false;
}

void CParticleSystem::init(cocos2d::Layer &inlayer)
{
	_iFree = NUMBER_PARTICLES;
	_iInUsed = 0;
	_pParticles = new CParticle[NUMBER_PARTICLES]; // ���o�һݭn�� particle �Ŷ�
	 // Ū�J�x�s�h�i�Ϥ��� plist ��
	SpriteFrameCache::getInstance()->addSpriteFramesWithFile("particletexture.plist");
	for (int i = 0; i < NUMBER_PARTICLES; i++) {
		_pParticles[i].setParticle("flare.png", inlayer);
		_FreeList.push_front(&_pParticles[i]);
	}
}

void CParticleSystem::doStep(float dt)
{
	CParticle *get;
	list <CParticle *>::iterator it;
	if (_bEmitterOn) { // �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
		// ���p��A�֥[
		int n = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (n > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < n - _iGenParticles; i++) {
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setBehavior(EMITTER_DEFAULT);
					get->setVelocity(_fVelocity);
					get->setOpacity(_fOpacity);
					get->setLifetime(_fLifeTime);
					get->setGravity(_fGravity);
					get->setSpin(_fSpin);
					get->setPosition(_emitterPt);
					get->setSize(0.125f);
					// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
					t = (_fDir + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t));
					get->setDirection(vdir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
					log("%d", _iStyle);
				}
			}
			_iGenParticles = n; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//-----------------------------------
	if (_bRaining) { // �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
					   // ���p��A�֥[
		int m = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (m > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < m - _iGenParticles; i++) {
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0) {
					get = _FreeList.front();
					get->setBehavior(RANDOMS_FALLING);
					get->setVelocity(3.2f);
					get->setLifetime(_fLifeTime);
					get->setGravity(0);
					_fSpin += 2.0f;
					_fSpread = 0;
					get->setSpin(_fSpin);
					//	_emitterPt.setPoint(_emitterPt.x, _emitterPt + 20.0f);
					get->setPosition(_rainPt);
					get->setSize(0.125f);
					get->setPic(4);
					// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
					t = (_fDir + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t));
					get->setDirection(vdir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
				}
			}
			_iGenParticles = m; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}
	//-----------------------------------
	if (_bFire) { // �ھ� Emitter �]�w�������ѼơA���ͬ۹��������l
					 // ���p��A�֥[
		int f = (int)(_fElpasedTime * _iNumParticles); // ��ثe�������Ӳ��ͪ����l�Ӽ�
		if (f > _iGenParticles) {  // ���ͪ����l�ӼƤ����A���ͨ� n �Ӥ��l
			for (int i = 0; i < f - _iGenParticles; i++) {
				// �ھ� Emitter �������ѼơA�]�w�Ҳ��ͤ��l���Ѽ�
				if (_iFree != 0 && !_bOut) {
					get = _FreeList.front();
					get->setBehavior(EMITTER_DEFAULT);
					get->setVelocity(2.0f);
					get->setGravity(-1.5f);
					get->setLifetime(1.5f);
					get->setOpacity(200);
					_fSpread = 10.0f;
					//			_fDir = 250.f + rand() % 41;
								//	_emitterPt.setPoint(_emitterPt.x, _emitterPt + 20.0f);
					_firepos.setPoint(_firepos.x + rand() % 2, _firepos.y + rand() % 8);
					if (_firepos.y >= 400 + rand() % 150) {
						if (_iFree > 100)
						{
							for (int i = 0; i < 100; i++) {
								get = _FreeList.front();
								get->setBehavior(FIREWORK);
								get->setPosition(_firepos);
								get->setGravity(_fGravity);
								get->setLifetime(_fLifeTime);
								get->setVelocity(_fVelocity);
								get->setOpacity(_fOpacity);
								get->setSpin(_fSpin);
								//			get->setPic(3);
								get->setColor(Color3B(rand() % 128, rand() % 128, 128 + rand() % 128));
								_FreeList.pop_front();
								_InUsedList.push_front(get);
								_iFree--; _iInUsed++;
							}
						}
						_bOut = 1;
					}
					get->setPosition(_firepos);

					// �ھ� _fSpread �P _vDir ���ͤ�V
					float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
					t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
					t = (_fDir + t)* M_PI / 180.0f;
					Vec2 vdir(cosf(t), sinf(t));
					get->setDirection(vdir);
					_FreeList.pop_front();
					_InUsedList.push_front(get);
					_iFree--; _iInUsed++;
					//	}
				}
				else if (_iFree != 0 && _bOut) {
					_fcount += dt;
					if (_fcount >= 1.5f) {
						_fcount -= 1.5f;
						_firepos.setPoint(rand() % 981, -10.0f);
						log("yee");
						_bOut = 0;
					}
				}
			}
			_iGenParticles = f; // �ثe�w�g���� n �Ӥ��l

		}
		if (_fElpasedTime >= 1.0f) {
			_fElpasedTime -= 1.0f;
			if (_iGenParticles >= _iNumParticles) _iGenParticles -= _iNumParticles;
			else _iGenParticles = 0;
		}
		_fElpasedTime += dt;
	}

	//--------------------------------
	if (_bBoom) {
		_fcount2 += dt;
		if (_fcount2 <= 10.0f) {
			if (_iFree != 0) {
				get = _FreeList.front();
				get->setBehavior(EMITTER_DEFAULT);
				get->setVelocity(-2.0f);
				get->setGravity(-1.5f);
				get->setLifetime(1.5f);
				get->setOpacity(200);
				_fSpread = 10.0f;
				_fDir = 50.0f;
				//	_emitterPt.setPoint(_emitterPt.x, _emitterPt + 20.0f);
				_boomPt.setPoint(_boomPt.x -4, _boomPt.y -8);
				if (_boomPt.y < 50 + rand() % 150) {
					if (_iFree > 100)
					{
						get = _FreeList.front();
						get->setBehavior(STAY_FOR_TWOSECONDS);
						get->setPosition(_boomPt);
						get->setGravity(0);
						get->setLifetime(_fLifeTime);
						get->setVelocity(0);
						get->setOpacity(_fOpacity);
						get->setSpin(_fSpin);
						get->setPic(6);
						get->setColor(Color3B(rand() % 128, rand() % 128, 128 + rand() % 128));
						_FreeList.pop_front();
						_InUsedList.push_front(get);
						_iFree--; _iInUsed++;
						
					}
					_bBoom = 0;
				}
				get->setPosition(_boomPt);

				// �ھ� _fSpread �P _vDir ���ͤ�V
				float t = (rand() % 1001) / 1000.0f; // ���ͤ��� 0 �� 1 ������
				t = _fSpread - t * _fSpread * 2; //  ���ͪ����סA�ন����
				t = (_fDir + t)* M_PI / 180.0f;
				Vec2 vdir(cosf(t), sinf(t));
				get->setDirection(vdir);
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
				//	}
			}
		}
	}

	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); ) {
			if ((*it)->doStep(dt)) { // ���l�ͩR�g���w�g��F
									 // �N�ثe�o�@�Ӹ`�I�����e��^ _FreeList
				//_FreeList.push_front((*it));
				_FreeList.push_back((*it));
				it = _InUsedList.erase(it); // �����ثe�o�@��, 
				_iFree++; _iInUsed--;
			}
			else it++;
		}
	}

	
}

void CParticleSystem::setGravity(float fGravity)
{
	// �]�w�ثe particle �O�b inused �� gravity
	_fGravity = fGravity;
	list <CParticle *>::iterator it;
	if (_iInUsed != 0) { // �����l�ݭn��s��
		for (it = _InUsedList.begin(); it != _InUsedList.end(); it++) {
			(*it)->setGravity(_fGravity);
		}
	}
}

CParticleSystem::~CParticleSystem()
{
	if (_iInUsed != 0) _InUsedList.clear(); // �����Ҧ��� NODE
	if (_iFree != 0) _FreeList.clear();
	delete[] _pParticles; // ����Ҧ����o�귽
	SpriteFrameCache::getInstance()->removeSpriteFramesFromFile("particletexture.plist");
}

void CParticleSystem::onTouchesBegan(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case BOOM:
		_bBoom = 1;
		_fcount2 = 0;
		_boomPt = touchPoint;
		break;
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setOpacity(_fOpacity);
			get->setPic(_iStyle);
			get->setSpin(_fSpin);
			get->setColor(Color3B(_fRed, _fGreen,_fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING :
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setOpacity(_fOpacity);
			get->setPic(_iStyle);
			get->setSpin(_fSpin);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FIREWORK:
		if (_iFree > 150 ) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(FIREWORK);
				get->setPosition(touchPoint);
				get->setGravity(-3.0f);
				get->setLifetime(_fLifeTime);
				get->setVelocity(_fVelocity);
				get->setPic(_iStyle);
				get->setColor(Color3B(_fRed, _fGreen, _fBlue));
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setOpacity(_fOpacity);
			get->setSpin(_fSpin);
			get->setPic(_iStyle);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case EXPLOSION:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 100) {
			for (int i = 0; i < 100; i++) {
				get = _FreeList.front();
				get->setBehavior(EXPLOSION);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setLifetime(_fLifeTime);
				get->setVelocity(_fVelocity);
				get->setOpacity(_fOpacity);
				get->setSpin(_fSpin);
				get->setPic(_iStyle);
				get->setColor(Color3B(_fRed, _fGreen, _fBlue));
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case HEARTSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(HEARTSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setLifetime(_fLifeTime);
				get->setVelocity(_fVelocity);
				get->setSpin(_fSpin);
				get->setPic(_iStyle);
				get->setOpacity(_fOpacity);
				get->setColor(Color3B(_fRed, _fGreen, _fBlue));
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case BUTTERFLYSHAPE:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree > 150) {
			for (int i = 0; i < 150; i++) {
				get = _FreeList.front();
				get->setBehavior(BUTTERFLYSHAPE);
				get->setPosition(touchPoint);
				get->setGravity(_fGravity);
				get->setLifetime(_fLifeTime);
				get->setVelocity(_fVelocity);
				get->setOpacity(_fOpacity);
				get->setPic(_iStyle);
				get->setSpin(_fSpin);
				get->setColor(Color3B(_fRed, _fGreen, _fBlue));
				_FreeList.pop_front();
				_InUsedList.push_front(get);
				_iFree--; _iInUsed++;
			}
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case WATER:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(WATER);
			get->setPosition(touchPoint);
			get->setGravity(-3.0f + _fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setSpin(_fSpin);
			get->setPic(_iStyle);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	}
}

void CParticleSystem::onTouchesMoved(const cocos2d::CCPoint &touchPoint)
{
	CParticle *get;
	switch (_iType)
	{
	case STAY_FOR_TWOSECONDS:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(STAY_FOR_TWOSECONDS);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setOpacity(_fOpacity);
			get->setSpin(_fSpin);
			get->setPic(_iStyle);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case RANDOMS_FALLING:
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setBehavior(RANDOMS_FALLING);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setOpacity(_fOpacity);
			get->setSpin(_fSpin);
			get->setPic(_iStyle);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	case FREE_FLY:
		// �q _FreeList ���o�@�Ӥ��l����� _InUsed
		if (_iFree != 0) {
			get = _FreeList.front();
			get->setPic(_iStyle);
			get->setBehavior(FREE_FLY);
			get->setPosition(touchPoint);
			get->setGravity(_fGravity);
			get->setLifetime(_fLifeTime);
			get->setVelocity(_fVelocity);
			get->setOpacity(_fOpacity);
			get->setSpin(_fSpin);
			get->setColor(Color3B(_fRed, _fGreen, _fBlue));
			_FreeList.pop_front();
			_InUsedList.push_front(get);
			_iFree--; _iInUsed++;
		}
		else return;// �S�����l, �ҥH�N������
		break;
	}
}
