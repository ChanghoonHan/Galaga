#include "proj.win32\BezierCurveScene.h"
#include "proj.win32\ScoreBoard.h"
#include "SimpleAudioEngine.h"

Scene* BezierCurveScene::createScene()
{
	return BezierCurveScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool BezierCurveScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	xOffset = (int)(visibleSize.width / 4 * 1.5f);
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	ScoreBoard* scoreBorad = ScoreBoard::create();
	this->addChild(scoreBorad);

	m_enemy = Sprite::create("./GalagaRes/enemy0_0.png");
	this->addChild(m_enemy);
	m_enemy->setPosition(Vec2(visibleSize.width/4 * 1.5f -25, 20));
	m_enemy->setScale(2.0f);

	bezierCount = 0;
	m_actionSequence = NULL;
	m_curState = BezierCurveSceneState::edit;

	m_stateLabel = Label::create("Edit", "fonts/arial.ttf", 20);
	m_stateLabel->setAnchorPoint(Vec2(-1, 1));
	m_stateLabel->setAlignment(TextHAlignment::LEFT);
	m_stateLabel->setColor(Color3B::RED);
	m_stateLabel->setPosition(0, visibleSize.height - 20);
	this->addChild(m_stateLabel);

	auto keyboardListner = EventListenerKeyboard::create();
	keyboardListner->onKeyPressed = CC_CALLBACK_2(BezierCurveScene::onKeyPressed, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListner, this);

	auto mouseListener = EventListenerMouse::create();
	mouseListener->onMouseDown = CC_CALLBACK_1(BezierCurveScene::onMouseDown, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

	scheduleUpdate();

	return true;
}

void BezierCurveScene::update(float deltaTime)
{
	switch (m_curState)
	{
	case BezierCurveScene::edit:
		
		break;
	case BezierCurveScene::play:
		
		break;
	default:
		break;
	}
}


void BezierCurveScene::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
	case cocos2d::EventKeyboard::KeyCode::KEY_Z:
		InitebzierPointList();
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_A:
		m_stateLabel->setString("Edit");
		m_curState = BezierCurveSceneState::edit;
		break;

	case cocos2d::EventKeyboard::KeyCode::KEY_S:
	{
		if (m_bezierPointSpriteList.size() == 0)
		{
			InitebzierPointList();
			m_stateLabel->setString("Edit");
			m_curState = BezierCurveSceneState::edit;
			return;
		}

		if (m_actionSequence != NULL)
		{
			m_enemy->stopAction(m_actionSequence);
			m_actionSequence->release();
			m_actionSequence = NULL;
			m_bezierToVec.clear();
		}

		m_stateLabel->setString("Play");
		m_curState = BezierCurveSceneState::play;
		m_enemy->setPosition(m_bezierPointSpriteList[0].point);


		for (auto i : m_bezierConfigList)
		{
			BezierTo* bezierToAction = BezierTo::create(1, i);
			m_bezierToVec.pushBack(bezierToAction);
		}

		
		m_actionSequence = Sequence::create(m_bezierToVec);
		m_enemy->runAction(m_actionSequence);
	}
		break;
	default:
		break;
	}
}

void BezierCurveScene::onMouseDown(Event *event)
{
	if (m_curState != BezierCurveSceneState::edit)
	{
		return;
	}
	
	EventMouse* eventMouse = (EventMouse*)event;
	if (eventMouse->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		if (m_bezierPointSpriteList.size() == 0)
		{
			return;
		}

		this->removeChild(m_bezierPointSpriteList[m_bezierPointSpriteList.size() - 1].pointLabel);
		this->removeChild(m_bezierPointSpriteList[m_bezierPointSpriteList.size() - 1].sprite);
		m_bezierPointSpriteList.pop_back();

		switch (bezierCount)
		{
		case 0:
		{
			m_bezierConfigList[m_bezierConfigList.size() - 1].endPosition = m_bezierConfigList[m_bezierConfigList.size() - 1].controlPoint_2;
		}
		break;

		case 1:
		{
			m_bezierConfigList.pop_back();
		}
		break;

		case 2:
		{
			m_bezierConfigList[m_bezierConfigList.size() - 1].controlPoint_2 = m_bezierConfigList[m_bezierConfigList.size() - 1].controlPoint_1;
			m_bezierConfigList[m_bezierConfigList.size() - 1].endPosition = m_bezierConfigList[m_bezierConfigList.size() - 1].controlPoint_1;
		}
		break;

		default:
			break;
		}

		bezierCount--;
		if (bezierCount < 0)
		{
			bezierCount = 2;
		}
	}
	else
	{
		BezierPoint bezierPoint;
		bezierPoint.point = Vec2(eventMouse->getCursorX(), eventMouse->getCursorY());
		bezierPoint.sprite = Sprite::create("./GalagaRes/BezierPoint.jpg");
		bezierPoint.sprite->setPosition(bezierPoint.point);
		std::string pointStr = String::createWithFormat("X : %.0f\nY : %.0f", bezierPoint.point.x - xOffset, bezierPoint.point.y)->getCString();
		bezierPoint.pointLabel = Label::create(pointStr, "fonts/arial.ttf", 20);
		bezierPoint.pointLabel->setAnchorPoint(Vec2(0, 1));
		bezierPoint.pointLabel->setPosition(bezierPoint.point.x, bezierPoint.point.y);
		this->addChild(bezierPoint.sprite);
		this->addChild(bezierPoint.pointLabel);


		switch (bezierCount)
		{
		case 0:
		{
			ccBezierConfig bezierConfig;
			bezierConfig.controlPoint_1 = bezierPoint.point;
			bezierConfig.controlPoint_2 = bezierPoint.point;
			bezierConfig.endPosition = bezierPoint.point;
			m_bezierConfigList.push_back(bezierConfig);
		}
		break;

		case 1:
		{
			m_bezierConfigList[m_bezierConfigList.size() - 1].controlPoint_2 = bezierPoint.point;
			m_bezierConfigList[m_bezierConfigList.size() - 1].endPosition = bezierPoint.point;
		}
		break;

		case 2:
		{
			m_bezierConfigList[m_bezierConfigList.size() - 1].endPosition = bezierPoint.point;
		}
		break;

		default:
			break;
		}

		bezierCount++;
		if (bezierCount > 2)
		{
			bezierCount = 0;
		}

		m_bezierPointSpriteList.push_back(bezierPoint);
	}
}

void BezierCurveScene::InitebzierPointList()
{
	for (auto i : m_bezierPointSpriteList)
	{
		this->removeChild(i.pointLabel);
		this->removeChild(i.sprite);
	}
	bezierCount = 0;

	m_bezierPointSpriteList.clear();
	m_bezierConfigList.clear();
	for (auto i : m_bezierToVec)
	{
		i -> release();
	}

	m_bezierToVec.clear();
	if (m_actionSequence != NULL)
	{
		this->stopAction(m_actionSequence);
		m_actionSequence->release();
		m_actionSequence = NULL;
	}
}