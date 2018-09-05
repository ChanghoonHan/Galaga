#include "GalagaMainScene.h"
#include "SimpleAudioEngine.h"
#include "proj.win32\GalagaGameScene.h"

Scene* GalagaMain::createScene()
{
    return GalagaMain::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GalagaMain::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }
	m_dFontSize = 40;
	m_dHeightOffset = 50;
	m_dWidthOffset = 100;

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	LabelTTF* p1UpLabel = LabelTTF::create("1UP", "fonts/arial.ttf", m_dFontSize);
	p1UpLabel->setAnchorPoint(Vec2(0, 1));
	p1UpLabel->setColor(Color3B::RED);
	this->addChild(p1UpLabel);
	p1UpLabel->setPosition(0 + m_dWidthOffset, visibleSize.height - m_dHeightOffset);
	LabelTTF* p1UpNumLabel = LabelTTF::create("00", "fonts/arial.ttf", m_dFontSize);
	p1UpNumLabel->setAnchorPoint(Vec2(0, 1));
	p1UpNumLabel->setPosition(m_dWidthOffset, visibleSize.height - m_dHeightOffset * 2);
	this->addChild(p1UpNumLabel);

	LabelTTF* p2UpLabel = LabelTTF::create("2UP", "fonts/arial.ttf", m_dFontSize);
	p2UpLabel->setColor(Color3B::RED);
	p2UpLabel->setAnchorPoint(Vec2(0, 1));
	p2UpLabel->setPosition(visibleSize.width - m_dWidthOffset * 2, visibleSize.height - m_dHeightOffset);
	this->addChild(p2UpLabel);
	LabelTTF* p2UpNumLabel = LabelTTF::create("00", "fonts/arial.ttf", m_dFontSize);
	p2UpNumLabel->setAnchorPoint(Vec2(0, 1));
	p2UpNumLabel->setPosition(visibleSize.width - m_dWidthOffset * 2, visibleSize.height - m_dHeightOffset * 2);
	this->addChild(p2UpNumLabel);
	
	LabelTTF* pHighScoreLabel = LabelTTF::create("HIGH-SCORE", "fonts/arial.ttf", m_dFontSize);
	pHighScoreLabel->setColor(Color3B::RED);
	pHighScoreLabel->setAnchorPoint(Vec2(0.5f, 1));
	pHighScoreLabel->setPosition(visibleSize.width/2, visibleSize.height - m_dHeightOffset);
	this->addChild(pHighScoreLabel);
	LabelTTF* pHighScoreNumLabel = LabelTTF::create("30000", "fonts/arial.ttf", m_dFontSize);
	pHighScoreNumLabel->setAnchorPoint(Vec2(0.5f, 1));
	pHighScoreNumLabel->setPosition(visibleSize.width/2, visibleSize.height - m_dHeightOffset * 2);
	this->addChild(pHighScoreNumLabel);

	LabelTTF* FristPlayerLabel = LabelTTF::create("1 PLAYER", "fonts/arial.ttf", m_dFontSize);
	FristPlayerLabel->setAnchorPoint(Vec2(0.5f, 1));
	FristPlayerLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - m_dHeightOffset);
	this->addChild(FristPlayerLabel);
	LabelTTF* SecondPlayerLabel = LabelTTF::create("2 PLAYER", "fonts/arial.ttf", m_dFontSize);
	SecondPlayerLabel->setAnchorPoint(Vec2(0.5f, 1));
	SecondPlayerLabel->setPosition(visibleSize.width / 2, visibleSize.height / 2 - m_dHeightOffset * 2);
	this->addChild(SecondPlayerLabel);

	Sprite* cursor = Sprite::create("./GalagaRes/cursor.png");
	cursor->setAnchorPoint(Vec2(0.5f, 1));
	cursor->setScale(2.0f);
	cursor->setPosition(visibleSize.width / 2 - 130, visibleSize.height / 2 - m_dHeightOffset -5);
	this->addChild(cursor);

	Animation* logoAnimation = Animation::create();
	logoAnimation->addSpriteFrameWithFile("./GalagaRes/title_00.png");
	logoAnimation->addSpriteFrameWithFile("./GalagaRes/title_01.png");
	logoAnimation->setDelayPerUnit(0.1f);

	Animate* logoAnimate = Animate::create(logoAnimation);

	Sprite* logo = Sprite::create("./GalagaRes/title_00.png");
	logo->setScale(2.0f);
	logo->setPosition(visibleSize.width / 2, visibleSize.height / 3 * 2 - m_dHeightOffset);
	this->addChild(logo);
	logo->runAction(RepeatForever::create(logoAnimate));

	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(GalagaMain::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void GalagaMain::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		Director::getInstance()->pushScene(GalagaGame::create());
		break;
	default:
		break;
	}
}

void GalagaMain::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
