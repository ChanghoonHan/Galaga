#include "BossEnemy.h"
#include "GalagaGameScene.h"
#include "CatchRay.h"
#include "CatchedHero.h"

bool BossEnemy::init()
{
	Enemy::init();
	m_catchedHero = NULL;
	m_usingRay = NULL;
	m_score = 400;
	m_life = 2;
	m_sprite = Sprite::create("./GalagaRes/enemy0_0.png");
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy0_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy0_1.png");
	m_animation->setDelayPerUnit(0.5f);
	m_animate = Animate::create(m_animation);
	m_sprite->runAction(RepeatForever::create(m_animate));
	m_scale = 1.8f;
	m_spriteSize = m_sprite->getContentSize();
	this->setScale(m_scale);
	this->addChild(m_sprite);
	return true;
}

void BossEnemy::AttackedFromBullet()
{
	m_life--;
	switch (m_life)
	{
	case 0:
		Destroy();
		break;
	case 1:
		ChageSpriteTexture();
		break;
	default:
		break;
	}
}

void BossEnemy::ChageSpriteTexture()
{
	if (m_animate->getCurrentFrameIndex() == 0)
	{
		m_sprite->setTexture("./GalagaRes/enemy1_0.png");
	}
	else
	{
		m_sprite->setTexture("./GalagaRes/enemy1_1.png");
	}
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy1_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy1_1.png");
	m_animate->setAnimation(m_animation);
}

void BossEnemy::SetAttackBezierPoints()
{
	m_attackBezierPoints[0].push_back(Vec2(-150, 600));
	m_attackBezierPoints[0].push_back(Vec2(-100, 550));
	m_attackBezierPoints[0].push_back(Vec2(-50, 500));
	m_attackBezierPoints[0].push_back(Vec2(0, 400));
	m_attackBezierPoints[0].push_back(Vec2(0, 300));

	m_attackBezierPoints[1].push_back(Vec2(-150, 600));
	m_attackBezierPoints[1].push_back(Vec2(-100, 500));
	m_attackBezierPoints[1].push_back(Vec2(-50, 475));
	m_attackBezierPoints[1].push_back(Vec2(0, 425));
	m_attackBezierPoints[1].push_back(Vec2(100, 375));
	m_attackBezierPoints[1].push_back(Vec2(100, 300));

}

void BossEnemy::SetAttack()
{
	int sign = 1;
	if (m_reverse)
	{
		sign = -1;
	}

	Vec2 basePos = this->getPosition();
	std::vector<Vec2> attackBezierPointsTemp;

	attackBezierPointsTemp.push_back(basePos);
	attackBezierPointsTemp.push_back(Vec2(basePos.x + (sign * (-50)), basePos.y + 50));
	attackBezierPointsTemp.push_back(Vec2(basePos.x + (sign * (-100)), basePos.y));
	for (auto point : m_attackBezierPoints[m_attackIndex])
	{
		attackBezierPointsTemp.push_back(Vec2(basePos.x + (point.x * sign), point.y));
	}

	MakeBezierAction(attackBezierPointsTemp, 1.0f);

	auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(BossEnemy::NotUseSetRotationCallback, this));
	m_bezierToVector.pushBack(callbackFunc);
	callbackFunc = CallFuncN::create(CC_CALLBACK_1(BossEnemy::UseCatchRayCallback, this));
	m_bezierToVector.pushBack(callbackFunc);

	Sequence* AttackAction = Sequence::create(m_bezierToVector);
	this->runAction(AttackAction);
	ChangeState(ENEMY_STATE::ATTACK);
	m_attackIndex = (m_attackIndex + 1) % 2;
}

void BossEnemy::ResumeAttackMove(bool isCatchHero)
{
	m_usingRay = NULL;
	Vec2 thisPos = this->getPosition();
	auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(BossEnemy::ChangeStateToIdleCallback, this));
	Sequence* AttackAction = NULL;

	if (isCatchHero)
	{
		m_notUseSetRotation = false;
		ccBezierConfig bezierConfig;
		Vec2 thisPos = this->getPosition();
		Vec2 basePos = *m_pBasePoint;
		float distX = 0;
		bezierConfig.controlPoint_1 = thisPos;
		distX = thisPos.x - basePos.x;
		bezierConfig.controlPoint_2 = Vec2(thisPos.x - (distX / 2), thisPos.y-100);
		bezierConfig.endPosition = Vec2(basePos.x, thisPos.y);
		BezierTo* bezierToAction = BezierTo::create(0.7f, bezierConfig);
		MoveTo* moveToAction = MoveTo::create(2.0f, basePos);
		AttackAction = Sequence::create(bezierToAction, moveToAction, callbackFunc, NULL);
	}
	else
	{
		MoveTo* moveToActions[3];
		moveToActions[0] = MoveTo::create(1.5f, Vec2(thisPos.x, -50));
		moveToActions[1] = MoveTo::create(0, Vec2(m_pBasePoint->x, m_visibleSize.height + 50));
		moveToActions[2] = MoveTo::create(1.5f, *m_pBasePoint);
		AttackAction = Sequence::create(moveToActions[0], moveToActions[1], moveToActions[2], RotateBy::create(0.7f, 180), callbackFunc, NULL);
	}
	
	this->runAction(AttackAction);
}

void BossEnemy::UseCatchRayCallback(Node* obj)
{
	m_usingRay = m_gameScene->UseCatchRay(this);
}

void BossEnemy::Destroy()
{
	if (m_usingRay != NULL)
	{
		m_usingRay->BossDestroyed();
		m_usingRay = NULL;
	}

	if (m_catchedHero != NULL)
	{
		m_catchedHero->ReleaseHero();
	}

	Enemy::Destroy();
}

CatchedHero* BossEnemy::GetCatchedHero()
{
	return m_catchedHero;
}

void BossEnemy::SetCatchedHero(CatchedHero* catchedHero)
{
	m_catchedHero = catchedHero;
}

void BossEnemy::SetCatchedHeroDie()
{
	m_catchedHero = NULL;
}


BossEnemy::BossEnemy()
{
}


BossEnemy::~BossEnemy()
{
}
