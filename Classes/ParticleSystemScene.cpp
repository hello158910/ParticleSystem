#include "ParticleSystemScene.h"

#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace cocostudio::timeline;
using namespace ui;
using namespace CocosDenshion;

Scene* ParticleSystemScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = ParticleSystemScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

ParticleSystemScene::~ParticleSystemScene()
{
	this->removeAllChildren();
	Director::getInstance()->getTextureCache()->removeUnusedTextures();
}

// on "init" you need to initialize your instance
bool ParticleSystemScene::init()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto rootNode = CSLoader::createNode("MainScene.csb");
    addChild(rootNode);

	// For Emitter
	_EmitterSprite = CDraggableSprite::create();
	_EmitterSprite->setSpriteInfo("emittericon.png", Point(-125.0f + visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	_EmitterSprite->setVisible(false);
	_bEmitterOn = false;
	this->addChild(_EmitterSprite, 5);

	//Raining Test

	_CloudSprite = CDraggableSprite::create();
	_CloudSprite->setSpriteInfo("emittericon.png", Point(-125.0f + visibleSize.width / 2.0f, visibleSize.height / 2.0f));
	_CloudSprite->setVisible(false);
	_bRaining = 0;
	this->addChild(_CloudSprite, 5);

	auto emiterpos = (Sprite *)(rootNode->getChildByName("emitterpos"));
	Point loc = emiterpos->getPosition();
	emiterpos->setVisible(false);
	_emitterSwitchBtn = CSwitchButton::create();
	_emitterSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_emitterSwitchBtn, 2);

	_rainingSwitchBtn = CSwitchButton::create();
	loc.setPoint(loc.x - 80.0f, loc.y);
	_rainingSwitchBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_rainingSwitchBtn, 2);

	_fireworkBtn = CSwitchButton::create();
	loc.setPoint(loc.x - 80.0f, loc.y);
	_fireworkBtn->setButtonInfo("emitteroff.png", "emitteron.png", "emittertd.png", loc);
	this->addChild(_fireworkBtn, 2);

	auto picpos = (Sprite *)(rootNode->getChildByName("spark_1"));
	loc = picpos->getPosition();
	picpos->setVisible(false);
	_SparkBtn = CSwitchButton::create();
	_SparkBtn->setButtonInfo("spark.png", "spark.png", "spark.png", loc);
	this->addChild(_SparkBtn, 2);

	loc.setPoint(loc.x + 65.0f, loc.y);
	_BubbleBtn = CSwitchButton::create();
	_BubbleBtn->setButtonInfo("bubble.png", "bubble.png", "bubble.png", loc);
	this->addChild(_BubbleBtn, 2);

	loc.setPoint(loc.x + 65.0f, loc.y);
	_CometBtn = CSwitchButton::create();
	_CometBtn->setButtonInfo("comet.png", "comet.png", "comet.png", loc);
	this->addChild(_CometBtn, 2);

	loc.setPoint(loc.x + 65.0f, loc.y);
	_RainDropBtn = CSwitchButton::create();
	_RainDropBtn->setButtonInfo("raindrop.png", "raindrop.png", "raindrop.png", loc);
	this->addChild(_RainDropBtn, 2);

	loc.setPoint(loc.x + 75.0f, loc.y);
	_CloudBtn = CSwitchButton::create();
	_CloudBtn->setButtonInfo("cloud.png", "cloud.png", "cloud.png", loc);
	this->addChild(_CloudBtn, 2);

	loc.setPoint(loc.x + 75.0f, loc.y);
	_CircleBtn = CSwitchButton::create();
	_CircleBtn->setButtonInfo("circle.png", "circle.png", "circle.png", loc);
	this->addChild(_CircleBtn, 2);

	loc.setPoint(loc.x + 75.0f, loc.y);
	_FlareBtn = CSwitchButton::create();
	_FlareBtn->setButtonInfo("flare.png", "flare.png", "flare.png", loc);
	this->addChild(_FlareBtn, 2);

	// Particle Control System
	// �̦n���覡�O�A�H�U�� Slider �ھڳo�̪��]�w�ȡA��ܥX���T���ƭȻP��m
	_ParticleControl.init(*this);
	_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	_ParticleControl._firepos.setPoint(rand() % 1281, -10.0f);
	_ParticleControl._rainPt = _EmitterSprite->getLoc();
	_ParticleControl._fDir = 90.0;
	_ParticleControl._iNumParticles = 100;
	_ParticleControl._iGenParticles = 0;
	_ParticleControl._fSpread = 180.0f;
	_ParticleControl._fOpacity = 255.0f;
	_ParticleControl._fRed = 255.0f;
	_ParticleControl._fGreen = 255.0f;
	_ParticleControl._fBlue = 255.0f;
	_ParticleControl._fVelocity = 2.0f;	 // ���l�����}�t��
	_ParticleControl._fLifeTime = 3.5f;	 // ���l���s���ɶ�
	_ParticleControl._fSpin = 0;
	_ParticleControl._fGravity = 0;
	_ParticleControl._fElpasedTime = 0;
	_ParticleControl._iStyle = 0;
	_ParticleControl.setType(STAY_FOR_TWOSECONDS); // ���l�B�ʪ����A�A�w�]�� 0
	_ParticleControl._windDir = Point(0, 0); // ���d�ҨS����@�����\��

	// ���l���i�վ�Ѽƪ����ʶb
	// Slider of Gravity
	auto *GravitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Gravity"));
	GravitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GravityEvent, this));
	GravitySlider->setMaxPercent(100); 	// �N 0 �� 100 ������ -10 �� 10 ����
	_GravityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GravityBMFont");

	// Slider of Spread
	auto *SpreadSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spread"));
	SpreadSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpreadEvent, this));
	SpreadSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 180 ����
	_SpreadBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpreadBMFont");

	// Slider of Direction
	auto *DirectionSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Direction"));
	DirectionSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::DirectionEvent, this));
	DirectionSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 360 ����
	_DirectionBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("DirectionBMFont");

	// Slider of Speed
	auto *SpeedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Speed"));
	SpeedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpeedEvent, this));
	SpeedSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 40 ����
	_SpeedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpeedBMFont");
	
	// Slider of LifeTime
	auto *LifeTimeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Lifetime"));
	LifeTimeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::LifeTimeEvent, this));
	LifeTimeSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 7 ����
	_LifeTimeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("LifetimeBMFont");

	// Slider of Spin
	auto *SpinSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Spin"));
	SpinSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::SpinEvent, this));
	SpinSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 360 ����
	_SpinBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("SpinBMFont");
	
	// Slider of Opacity
	auto *OpacitySlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Opacity"));
	OpacitySlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::OpacityEvent, this));
	OpacitySlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_OpacityBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("OpacityBMFont");

	// Slider of Particles
	auto *ParticlesSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Particles"));
	ParticlesSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::ParticlesEvent, this));
	ParticlesSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 200 ����
	_ParticlesBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("ParticlesBMFont");

	// Slider of Red
	auto *RedSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Red"));
	RedSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::RedEvent, this));
	RedSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_RedBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("RedBMFont");

	// Slider of Green
	auto *GreenSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Green"));
	GreenSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::GreenEvent, this));
	GreenSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_GreenBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("GreenBMFont");

	// Slider of Blue
	auto *BlueSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Blue"));
	BlueSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::BlueEvent, this));
	BlueSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_BlueBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("BlueBMFont");

	// Slider of Wind
	auto *WindSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Wind"));
	WindSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::WindEvent, this));
	WindSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 255 ����
	_WindBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("WindBMFont");

	// Slider of Type
	auto *TypeSlider = (cocos2d::ui::Slider *)(rootNode->getChildByName("Slider_Type"));
	TypeSlider->addEventListener(CC_CALLBACK_2(ParticleSystemScene::TypeEvent, this));
	TypeSlider->setMaxPercent(100); 	// �N 0 �� 100 ������ 0 �� 360 ����
	_TypeBMValue = (cocos2d::ui::TextBMFont *)rootNode->getChildByName("TypeBMFont");

	


	_listener1 = EventListenerTouchOneByOne::create();	//�Ыؤ@�Ӥ@��@���ƥ��ť��
	_listener1->onTouchBegan = CC_CALLBACK_2(ParticleSystemScene::onTouchBegan, this);		//�[�JĲ�I�}�l�ƥ�
	_listener1->onTouchMoved = CC_CALLBACK_2(ParticleSystemScene::onTouchMoved, this);		//�[�JĲ�I���ʨƥ�
	_listener1->onTouchEnded = CC_CALLBACK_2(ParticleSystemScene::onTouchEnded, this);		//�[�JĲ�I���}�ƥ�

	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(_listener1, this);	//�[�J��Ыت��ƥ��ť��
	this->schedule(CC_SCHEDULE_SELECTOR(ParticleSystemScene::doStep));

    return true;
}

void ParticleSystemScene::doStep(float dt)
{
	_ParticleControl.doStep(dt);
	//�ϼ�
	/*if (_CometBtn->getStatus()) {
		_ParticleControl.setPic(1, *this);
	}*/
}

bool ParticleSystemScene::onTouchBegan(cocos2d::Touch *pTouch, cocos2d::Event *pEvent)//Ĳ�I�}�l�ƥ�
{
	Point touchLoc = pTouch->getLocation();
	
	//��� Emitter �ɡA�i�즲�ӹϦ�
	if( _bEmitterOn ) _EmitterSprite->touchesBegan(touchLoc);
	// �S����� Emitter�A�ӥB�S�����b Emitter �������s�W�A�~�� touch �i�H�I����ܤ��l
	if (!_CircleBtn->touchesBegan(touchLoc) && !_CometBtn->touchesBegan(touchLoc) && 
		!_CloudBtn->touchesBegan(touchLoc) && !_BubbleBtn->touchesBegan(touchLoc) && 
		!_RainDropBtn->touchesBegan(touchLoc) && !_SparkBtn->touchesBegan(touchLoc) && 
		!_emitterSwitchBtn->touchesBegan(touchLoc) && !_rainingSwitchBtn->touchesBegan(touchLoc) && 
		!_fireworkBtn->touchesBegan(touchLoc) && !_FlareBtn->touchesBegan(touchLoc) &&
		!_bEmitterOn && !_bRaining && !_bFireOn ) _ParticleControl.onTouchesBegan(touchLoc);
	
	if (_bRaining)_CloudSprite->touchesBegan(touchLoc);
	if (_emitterSwitchBtn->touchesBegan(touchLoc)) _ParticleControl._fSpread = 180.0f;
	return true;
}

void  ParticleSystemScene::onTouchMoved(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I���ʨƥ�
{
	Point touchLoc = pTouch->getLocation();	
	if (_bEmitterOn) {
		if (_EmitterSprite->touchesMoved(touchLoc)) // ���ʨç�s Emitter ����m
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}
	if (_bRaining) {
		if (_CloudSprite->touchesMoved(touchLoc))
			_ParticleControl._rainPt = _CloudSprite->getLoc();
	}
	// �S����� Emitter�A�ӥB�S�����b Emitter �������s�W�A�~�� touch �i�H�I����ܤ��l
	if ( !_emitterSwitchBtn->touchesMoved(touchLoc) && !_EmitterSprite->touchesMoved(touchLoc) &&
		 !_rainingSwitchBtn->touchesMoved(touchLoc) && !_CloudSprite->touchesMoved(touchLoc) && 
		 !_fireworkBtn->touchesMoved(touchLoc) && !_bEmitterOn && !_bRaining && !_bFireOn ) _ParticleControl.onTouchesMoved(touchLoc);
}

void  ParticleSystemScene::onTouchEnded(cocos2d::Touch *pTouch, cocos2d::Event *pEvent) //Ĳ�I�����ƥ� 
{
	Point touchLoc = pTouch->getLocation();
	_CometBtn->touchesEnded(touchLoc);
	_CloudBtn->touchesEnded(touchLoc);
	_CircleBtn->touchesEnded(touchLoc);
	_FlareBtn->touchesEnded(touchLoc);
	_BubbleBtn->touchesEnded(touchLoc);
	_RainDropBtn->touchesEnded(touchLoc);
	_SparkBtn->touchesEnded(touchLoc);

	if (_bEmitterOn) {
		if( _EmitterSprite->touchesEnded(touchLoc) ) 
			_ParticleControl._emitterPt = _EmitterSprite->getLoc();
	}

	if (_bRaining) {
		if (_CloudSprite->touchesEnded(touchLoc))
			_ParticleControl._rainPt = _CloudSprite->getLoc();
	}
	// �I�b Emitter �������ϥܤW�A�i�楲�n�����A����
	if (_emitterSwitchBtn->touchesEnded(touchLoc))
	{
		_bEmitterOn = _emitterSwitchBtn->getStatus();
		if ( _bEmitterOn ) { // ��� Emitter �ϥ�
			_EmitterSprite->setVisible(true);			
		}
		else { // ���� Emitter �ϥ�
			_EmitterSprite->setVisible(false);
		}
		_ParticleControl.setEmitter(_bEmitterOn); // ��s����t�Τ��� Emitter ���A
	}

	if(_rainingSwitchBtn->touchesEnded(touchLoc))
	{
		_bRaining = _rainingSwitchBtn->getStatus();
		if (_bRaining) { _CloudSprite->setVisible(true); }
		else _CloudSprite->setVisible(false);
		_ParticleControl.setRaining(_bRaining);
	}

	if(_fireworkBtn->touchesEnded(touchLoc))
	{
		_bFireOn = _fireworkBtn->getStatus();
		_ParticleControl.setFire(_bFireOn);
	}

	if (_CometBtn->getStatus()) {
		_ParticleControl._iStyle = 1;
		_CometBtn->setStatus(false);
	}
	if (_CircleBtn->getStatus()) {
		_ParticleControl._iStyle = 2;
		_CircleBtn->setStatus(false);
	}
	if (_SparkBtn->getStatus()) {
		_ParticleControl._iStyle = 3;
		_SparkBtn->setStatus(false);
	}
	if (_RainDropBtn->getStatus()) {
		_ParticleControl._iStyle = 4;
		_RainDropBtn->setStatus(false);
	}
	if (_BubbleBtn->getStatus()) {
		_ParticleControl._iStyle = 5;
		_BubbleBtn->setStatus(false);
	}
	if (_CloudBtn->getStatus()) {
		_ParticleControl._iStyle = 6;
		_CloudBtn->setStatus(false);
	}
	if (_FlareBtn->getStatus()) {
		_ParticleControl._iStyle = 7;
		_FlareBtn->setStatus(false);
	}
}

void ParticleSystemScene::GravityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		float fGravity = (-50.0f + percent) / 5.0f;
		_GravityBMValue->setString(StringUtils::format("%2.1f", fGravity));
		_ParticleControl.setGravity(fGravity);
	}
}

void ParticleSystemScene::SpreadEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpread = percent*1.8f; // 0 �� 180 ����
		_SpreadBMValue->setString(StringUtils::format("%2.1f", fSpread));
		_ParticleControl._fSpread = fSpread;
	}
}

void ParticleSystemScene::SpeedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpeed = percent*0.4f; // 0 �� 40 ����
		_SpeedBMValue->setString(StringUtils::format("%2.1f", fSpeed));
		_ParticleControl._fVelocity = fSpeed;
	}
}

void ParticleSystemScene::LifeTimeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fLifeTime = percent*0.07f; // 0 �� 7 ����
		_LifeTimeBMValue->setString(StringUtils::format("%2.1f", fLifeTime));
		_ParticleControl._fLifeTime = fLifeTime;
	}
}

void ParticleSystemScene::DirectionEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fDir = percent*3.6f; // 0 �� 360 ����
		_DirectionBMValue->setString(StringUtils::format("%2.1f", fDir));
		_ParticleControl._fDir = fDir;
	}
}

void ParticleSystemScene::SpinEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fSpin = percent*3.6f; // 0 �� 360 ����
		_SpinBMValue->setString(StringUtils::format("%2.1f", fSpin));
		_ParticleControl._fSpin = fSpin;
	}
}

void ParticleSystemScene::OpacityEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fOpacity = percent*2.55f; // 0 �� 255 ����
		_OpacityBMValue->setString(StringUtils::format("%2.0f", fOpacity));
		_ParticleControl._fOpacity = fOpacity;
	}
}

void ParticleSystemScene::ParticlesEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float iNumParticles = percent*2.0f; // 0 �� 200 ����
		_ParticlesBMValue->setString(StringUtils::format("%2.0f", iNumParticles));
		_ParticleControl._iNumParticles = iNumParticles;
	}
}

void ParticleSystemScene::RedEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fRed = percent*2.55f; // 0 �� 255 ����
		_RedBMValue->setString(StringUtils::format("%2.0f", fRed));
		_ParticleControl._fRed = fRed;
	}
}

void ParticleSystemScene::GreenEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fGreen = percent*2.55f; // 0 �� 255 ����
		_GreenBMValue->setString(StringUtils::format("%2.0f", fGreen));
		_ParticleControl._fGreen = fGreen;
	}
}

void ParticleSystemScene::BlueEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fBlue = percent*2.55f; // 0 �� 255 ����
		_BlueBMValue->setString(StringUtils::format("%2.0f", fBlue));
		_ParticleControl._fBlue = fBlue;
	}
}

void ParticleSystemScene::TypeEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		int iType = percent/12.5f; // 0 �� 8 ����
		_TypeBMValue->setString(StringUtils::format("%2d", iType));
		_ParticleControl.setType(iType);
	}
}

void ParticleSystemScene::WindEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType type)
{
	if (type == Slider::EventType::ON_PERCENTAGE_CHANGED)
	{
		Slider* slider = dynamic_cast<Slider*>(sender);
		int percent = slider->getPercent();
		int maxPercent = slider->getMaxPercent();
		float fWind = (-50.0f + percent) / 5.0f; // -10 �� 10 ����
		_WindBMValue->setString(StringUtils::format("%2.0f", fWind));
//		_ParticleControl._windDir = fWind;
	}
}