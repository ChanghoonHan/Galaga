#include "BlueEnemy.h"


bool BlueEnemy::init()
{
	Enemy::init();
	m_score = 100;
	m_life = 1;
	m_sprite = Sprite::create("./GalagaRes/enemy3_0.png");
	m_scale = 2.0f;
	m_spriteSize = m_sprite->getContentSize();
	this->setScale(m_scale);
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy3_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy3_1.png");
	m_animation->setDelayPerUnit(0.5f);
	m_animate = Animate::create(m_animation);
	m_sprite->runAction(RepeatForever::create(m_animate));
	this->addChild(m_sprite);

	return true;
}

void BlueEnemy::SetAttackBezierPoints()
{
	m_attackBezierPoints[0].push_back(Vec2(0, 430));
	m_attackBezierPoints[0].push_back(Vec2(51, 393));
	m_attackBezierPoints[0].push_back(Vec2(115, 326));
	m_attackBezierPoints[0].push_back(Vec2(201, 289));
	m_attackBezierPoints[0].push_back(Vec2(278, 230));
	m_attackBezierPoints[0].push_back(Vec2(296, 158));
	m_attackBezierPoints[0].push_back(Vec2(278, 86));
	m_attackBezierPoints[0].push_back(Vec2(242, 52));
	m_attackBezierPoints[0].push_back(Vec2(186, 56));
	m_attackBezierPoints[0].push_back(Vec2(152, 105));
	m_attackBezierPoints[0].push_back(Vec2(149, 169));
	m_attackBezierPoints[0].push_back(Vec2(133, 221));
	m_attackBezierPoints[0].push_back(Vec2(70, 288));
	m_attackBezierPoints[0].push_back(Vec2(45, 340));
	m_attackBezierPoints[0].push_back(Vec2(21, 377));
}

void BlueEnemy::SetAttack()
{
	int sign = 1;
	if (m_reverse)
	{
		sign = -1;
	}

	Vec2 basePos = this->getPosition();
	std::vector<Vec2> attackBezierPointsTemp;

	attackBezierPointsTemp.push_back(basePos);
	attackBezierPointsTemp.push_back(Vec2(basePos.x + (sign * (- 50)), basePos.y + 50));
	for (auto point : m_attackBezierPoints[0])
	{
		attackBezierPointsTemp.push_back(Vec2(basePos.x + (point.x * sign), point.y));
	}
	attackBezierPointsTemp.push_back(basePos);

	MakeBezierAction(attackBezierPointsTemp, 1.0f);

	auto changeStateToIdleCallbackFunc = CallFuncN::create(CC_CALLBACK_1(BlueEnemy::ChangeStateToIdleCallback, this));
	m_bezierToVector.pushBack(changeStateToIdleCallbackFunc);
	Sequence* AttackAction = Sequence::create(m_bezierToVector);

	Sequence* fireBulletSequenceAction = NULL;
	CallFuncN* fireBulletCallbackFunc = NULL;
	float fireProb = random(0.0f, 1.0f);
	if (fireProb < m_fireProb)
	{
		fireBulletCallbackFunc = CallFuncN::create(CC_CALLBACK_1(BlueEnemy::FireBulletCallback, this));
		float fireTimeRandom = random(0.0f, 1.0f);
		fireBulletSequenceAction = Sequence::create(DelayTime::create(1.5f + fireTimeRandom), fireBulletCallbackFunc, NULL);
	}
	this->runAction(Spawn::create(AttackAction, fireBulletSequenceAction, NULL));
	ChangeState(ENEMY_STATE::ATTACK);
}

BlueEnemy::BlueEnemy()
{
}


BlueEnemy::~BlueEnemy()
{
}
