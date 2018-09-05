#include "Hero.h"
#include "proj.win32\GalagaGameScene.h"
#include "Bullet.h"

bool Hero::init()
{
	m_bulletCount = 0;
	m_isFire = false;
	m_bulletReloadTime = 0.3;
	m_curBulletReloadTime = m_bulletReloadTime;
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_sprite = Sprite::create("./GalagaRes/player.png");
	m_spriteSize = m_sprite->getContentSize();
	m_sprite->setPosition(Vec2::ZERO);
	this->addChild(m_sprite);
	this->setScale(2.0f);
	this->setPosition(0, 0);

	ChangeState(HERO_STATE::HS_IDLE);

	return true;
}


void Hero::update(float deltaTime, bool isGaming)
{
	if (m_curState != HERO_STATE::HS_IDLE)
	{
		return;
	}

	m_curBulletReloadTime += deltaTime;

	if (isGaming && m_isFire && m_bulletCount < 2)
	{
		if (m_curBulletReloadTime > m_bulletReloadTime)
		{
			Vec2 basePos = m_groupObj->getPosition();
			Vec2 thisPos = this->getPosition();
			Vec2 firePos = Vec2(basePos.x + thisPos.x, basePos.y);
			m_gameScene->FireBullet(firePos, Vec2(firePos.x, 1), true, this);
			m_bulletCount++;
			m_curBulletReloadTime = 0;
		}
	}
}

void Hero::SetGroupObj(Node* groupObj)
{
	m_groupObj = groupObj;
}

void Hero::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (m_curState != HERO_STATE::HS_IDLE)
	{
		return;
	}

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		m_curBulletReloadTime = m_bulletReloadTime;
		m_isFire = true;
		break;
	default:
		break;
	}
}

void Hero::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	if (m_curState != HERO_STATE::HS_IDLE)
	{
		return;
	}

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_A:
		m_isFire = false;
		break;
	default:
		break;
	}
}

void Hero::SetScene(GalagaGame* gameScene)
{
	m_gameScene = gameScene;
}

void Hero::DecreaseBulletCount()
{
	m_bulletCount--;
	if (m_bulletCount < 0)
	{
		m_bulletCount = 0;
	}
}

Rect Hero::GetCollider()
{
	if (m_curState != HERO_STATE::HS_IDLE)
	{
		return Rect(100000, 100000, 0, 0);
	}

	Vec2 basePos = m_groupObj->getPosition();
	Vec2 thisPos = this->getPosition();
	m_collider.setRect(basePos.x + thisPos.x - 20, basePos.y - 40, 40, 40);
	
	return m_collider;
}

void Hero::ChangeState(HERO_STATE state)
{
	switch (state)
	{
	case HS_IDLE:
		break;
	case HS_DEAD:
		Destroy();
		break;
	case HS_DESTROY:
		break;
	default:
		break;
	}

	m_curState = state;
}

void Hero::AttackedFromBullet()
{
	ChangeState(HERO_STATE::HS_DEAD);
}

void Hero::Destroy()
{
	this->stopAllActions();
	m_sprite->stopAllActions();

	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_player_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_player_1.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_player_2.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_player_3.png");
	m_animation->setDelayPerUnit(0.1f);
	m_animate = Animate::create(m_animation);
	auto removeFunc = CallFuncN::create(CC_CALLBACK_1(Hero::RemoveHero, this));
	Sequence* destroySequence = Sequence::create(m_animate, removeFunc, NULL);
	m_sprite->runAction(destroySequence);
}

void Hero::RemoveHero(Node* obj)
{
	ChangeState(HERO_STATE::HS_DESTROY);
}

HERO_STATE Hero::GetHeroState()
{
	return m_curState;
}

void Hero::SetHeroPositionInGroup(int pos)
{
	switch (pos)
	{
	case 0:
		this->runAction(MoveTo::create(0.5f, Vec2::ZERO));
		break;
	case 1:
		this->runAction(MoveTo::create(0.5f, Vec2(-m_spriteSize.width, 0)));
		break;
	case 2:
		this->runAction(MoveTo::create(0.5f, Vec2(m_spriteSize.width, 0)));
		break;
	default:
		break;
	}
}

Hero::Hero()
{
}


Hero::~Hero()
{
}
