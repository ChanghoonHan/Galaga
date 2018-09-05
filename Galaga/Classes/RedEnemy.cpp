#include "RedEnemy.h"

bool RedEnemy::init()
{
	Enemy::init();
	m_score = 200;
	m_life = 1;
	m_sprite = Sprite::create("./GalagaRes/enemy2_0.png");
	m_scale = 2.0f;
	m_spriteSize = m_sprite->getContentSize();
	this->setScale(m_scale);
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy2_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy2_1.png");
	m_animation->setDelayPerUnit(0.5f);
	m_animate = Animate::create(m_animation);
	m_sprite->runAction(RepeatForever::create(m_animate));
	this->addChild(m_sprite);
	return true;
}

void RedEnemy::SetAttackBezierPoints()
{
	m_attackBezierPoints[0].push_back(Vec2(-100, 500));
	m_attackBezierPoints[0].push_back(Vec2(80, 500));
	m_attackBezierPoints[0].push_back(Vec2(70, 400));
	m_attackBezierPoints[0].push_back(Vec2(0, 300));
	m_attackBezierPoints[0].push_back(Vec2(180, 300));
	m_attackBezierPoints[0].push_back(Vec2(170, 200));
	m_attackBezierPoints[0].push_back(Vec2(100, 100));
	m_attackBezierPoints[0].push_back(Vec2(280, 100));
	m_attackBezierPoints[0].push_back(Vec2(270, 25));
	m_attackBezierPoints[0].push_back(Vec2(200, -50));
}

void RedEnemy::SetAttack()
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
	for (auto point : m_attackBezierPoints[0])
	{
		attackBezierPointsTemp.push_back(Vec2(basePos.x + (point.x * sign), point.y));
	}

	MakeBezierAction(attackBezierPointsTemp, 1.0f);

	auto moveToAction = MoveTo::create(0.0f, Vec2(basePos.x, m_visibleSize.height - 50));
	m_bezierToVector.pushBack(moveToAction);
	moveToAction = MoveTo::create(1.5f, basePos);
	m_bezierToVector.pushBack(moveToAction);

	auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(RedEnemy::NotUseSetRotationCallback, this));
	m_bezierToVector.pushBack(callbackFunc);
	m_bezierToVector.pushBack(RotateBy::create(0.7f, 180));
	callbackFunc = CallFuncN::create(CC_CALLBACK_1(RedEnemy::ChangeStateToIdleCallback, this));
	m_bezierToVector.pushBack(callbackFunc);

	Sequence* AttackAction = Sequence::create(m_bezierToVector);

	Sequence* fireBulletSequenceAction = NULL;
	CallFuncN* fireBulletCallbackFunc = NULL;
	float fireProb = random(0.0f, 1.0f);
	if (fireProb < m_fireProb)
	{
		fireBulletCallbackFunc = CallFuncN::create(CC_CALLBACK_1(RedEnemy::FireBulletCallback, this));
		float fireTimeRandom = random(0.0f, 1.5f);
		fireBulletSequenceAction = Sequence::create(DelayTime::create(1.5f + fireTimeRandom), fireBulletCallbackFunc, NULL);
	}

	this->runAction(Spawn::create(AttackAction, fireBulletSequenceAction, NULL));

	ChangeState(ENEMY_STATE::ATTACK);
}

RedEnemy::RedEnemy()
{
}


RedEnemy::~RedEnemy()
{
}
