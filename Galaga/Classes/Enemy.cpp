#include "Enemy.h"
#include "GroupAttackAnchor.h"
#include "GalagaGameScene.h"
#include <math.h>

bool Enemy::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_curEnemyState = ENEMY_STATE::SPAWNING;
	m_spwanStart = false;
	m_notUseSetRotation = false;
	m_groupAttackAnchor = NULL;
	m_attackIndex = 0;
	m_hitOffset = 0;
	m_fireProb = 0.2f;
	m_noFireBullet = false;
	SetAttackBezierPoints();

	return true;
}

void Enemy::SetPosition(Vec2 position)
{
	this->setPosition(position);
}

void Enemy::SetGroupAttack(GroupAttackAnchor* groupAnchor)
{
	if (!m_noFireBullet)
	{
		Sequence* fireBulletSequenceAction = NULL;
		CallFuncN* fireBulletCallbackFunc = NULL;
		float fireProb = random(0.0f, 1.0f);
		if (fireProb < m_fireProb)
		{
			fireBulletCallbackFunc = CallFuncN::create(CC_CALLBACK_1(Enemy::FireBulletCallback, this));
			float fireTimeRandom = random(0.0f, 1.0f);
			fireBulletSequenceAction = Sequence::create(DelayTime::create(2.5f + fireTimeRandom), fireBulletCallbackFunc, NULL);
			this->runAction(fireBulletSequenceAction);
		}
	}

	m_groupAttackAnchor = groupAnchor;
	Vec2 anchorPos = m_groupAttackAnchor->getPosition();
	Vec2 thisPos = this->getPosition();
	m_groupAttackOffset.width = -(anchorPos.x - thisPos.x);
	m_groupAttackOffset.height = -(anchorPos.y - thisPos.y);

	ChangeState(ENEMY_STATE::GROUPATTACK);
}

void Enemy::GroupAttack()
{
	Vec2 anchorPos = m_groupAttackAnchor->getPosition();
	this->setPosition(Vec2(anchorPos.x + m_groupAttackOffset.width, anchorPos.y + m_groupAttackOffset.height));
}

void Enemy::SetBasePoint(Vec2* basePoint, bool reverse)
{
	m_pBasePoint = basePoint;
	m_reverse = reverse;
}

void Enemy::SetSpawn(std::vector<Vec2> spawnBezierPoints, bool reverse)
{
	int offsetX = m_visibleSize.width / 4 * 1.5f;
	int sign = 1;

	if (reverse)
	{
		sign = -1;
	}

	for (auto& point : spawnBezierPoints)
	{
		point.x = offsetX + (point.x * sign);
	}

	m_prevPos = spawnBezierPoints[0];
	this->setPosition(spawnBezierPoints[0]);
	spawnBezierPoints.push_back(*m_pBasePoint);
	MakeBezierAction(spawnBezierPoints, 1.0f);
	auto changeStateToIdleCallbackFunc = CallFuncN::create(CC_CALLBACK_1(Enemy::ChangeStateToIdleCallback, this));
	m_bezierToVector.pushBack(changeStateToIdleCallbackFunc);
	Sequence* spawnAction = spawnAction = Sequence::create(m_bezierToVector);
	this->runAction(spawnAction);
}

void Enemy::MakeBezierAction(std::vector<Vec2>& bezierPoints, float actionTime)
{
	int pointIdx = 0;

	for (auto bezierToAction : m_bezierToVector)
	{
		bezierToAction->release();
	}
	m_bezierToVector.clear();
	ccBezierConfig bezierConfig;
	BezierTo* bezierTo = NULL;

	for (auto iter = bezierPoints.begin(); iter != bezierPoints.end(); iter++)
	{
		switch (pointIdx)
		{
		case 0:
			bezierConfig.controlPoint_1 = *iter;
			bezierConfig.controlPoint_2 = *iter;
			bezierConfig.endPosition = *iter;
			break;
		case 1:
			bezierConfig.controlPoint_2 = *iter;
			bezierConfig.endPosition = *iter;
			break;
		case 2:
			bezierConfig.endPosition = *iter;
			bezierTo = BezierTo::create(actionTime, bezierConfig);
			m_bezierToVector.pushBack(bezierTo);
			break;
		default:
			break;
		}

		if (iter + 1 == bezierPoints.end() && pointIdx != 2)
		{
			bezierTo = BezierTo::create(actionTime, bezierConfig);
			m_bezierToVector.pushBack(bezierTo);
		}

		pointIdx++;
		if (pointIdx == 3)
		{
			pointIdx = 0;
		}
	}
}

void Enemy::update(float deltaTime)
{
	switch (m_curEnemyState)
	{
	case SPAWNING:
		break;
	case MOVE:
		break;
	case ATTACK:
		break;
	case GROUPATTACK:
		GroupAttack();
		break;
	case DEAD:
		break;
	case DESTROY:
		break;
	default:
		break;
	}

	SetRotation();
}

void Enemy::SetScene(GalagaGame* gameScene)
{
	m_gameScene = gameScene;
}

void Enemy::SetNoFireBullet()
{
	m_noFireBullet = true;
}

void Enemy::SetCollider()
{
	m_collider.setRect(this->getPositionX() - m_spriteSize.width, this->getPositionY() - m_spriteSize.height, m_spriteSize.width * m_scale, m_spriteSize.height * m_scale);
}

void Enemy::SetRotation()
{
	if (m_notUseSetRotation)
	{
		return;
	}

	if (m_curEnemyState == ENEMY_STATE::IDLE || m_curEnemyState == ENEMY_STATE::DEAD || m_curEnemyState == ENEMY_STATE::DESTROY)
	{
		m_notUseSetRotation = false;
		this->setRotation(0);
		return;
	}

	Vec2 vec2ForRotation = this->getPosition() - m_prevPos;
	vec2ForRotation.normalize();
	this->setRotation(CC_RADIANS_TO_DEGREES(atan2(vec2ForRotation.x, vec2ForRotation.y)));
	m_prevPos = this->getPosition();
}

Rect& Enemy::GetCollider()
{
	if (m_curEnemyState == ENEMY_STATE::DEAD || m_curEnemyState == ENEMY_STATE::DESTROY)
	{
		return Rect(1000000, 1000000, 0, 0);
	}

	SetCollider();

	return m_collider;
}

void Enemy::Destroy()
{
	ChangeState(ENEMY_STATE::DEAD);
	this->stopAllActions();
	m_sprite->stopAllActions();
	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_enemy_0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_enemy_1.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_enemy_2.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/exp_enemy_3.png");
	m_animation->setDelayPerUnit(0.1f);
	m_animate = Animate::create(m_animation);
	auto removeFunc = CallFuncN::create(CC_CALLBACK_1(Enemy::RemoveEnemy, this));
	Sequence* destroySequence = Sequence::create(m_animate, removeFunc, NULL);
	m_sprite->runAction(destroySequence);
}

void Enemy::RemoveEnemy(Node* obj)
{
	ChangeState(ENEMY_STATE::DESTROY);
}

bool Enemy::CheckCollWithHero(Rect heroRect)
{
	Rect enemyRect = GetCollider();

	if (enemyRect.intersectsRect(heroRect))
	{
		AttackedFromBullet();
		return true;
	}

	return false;
}

void Enemy::AttackedFromBullet()
{
	m_life--;
	if (m_life == 0)
	{
		Destroy();
	}
}

int Enemy::GetScore()
{
	return m_score;
}

void Enemy::ChangeStateToIdleCallback(Node* obj)
{
	ChangeState(ENEMY_STATE::IDLE);
}

void Enemy::NotUseSetRotationCallback(Node* obj)
{
	m_notUseSetRotation = true;
}

void Enemy::ChangeState(ENEMY_STATE enmeyState)
{
	switch (enmeyState)
	{
	case IDLE:
		m_notUseSetRotation = false;
		break;
	case SPAWNING:
		break;
	case MOVE:
		break;
	case ATTACK:
		break;
	case GROUPATTACK:
		break;
	case DEAD:
		break;
	case DESTROY:
		this->stopAllActions();
		break;
	default:
		break;
	}

	m_curEnemyState = enmeyState;
}

void Enemy::EndGroupAttack()
{
	m_noFireBullet = false;
	m_notUseSetRotation = true;
	RotateBy* rotateByAction = RotateBy::create(0.7f, 180);
	auto changeStateToIdleCallbackFunc = CallFuncN::create(CC_CALLBACK_1(Enemy::ChangeStateToIdleCallback, this));
	Sequence* sequenceAction = Sequence::create(rotateByAction, changeStateToIdleCallbackFunc, NULL);
	this->runAction(sequenceAction);
}

void Enemy::FireBulletCallback(Node* obj)
{
	Vec2 heroPos = m_gameScene->GetHeroPos();
	heroPos.x -= m_hitOffset;
	heroPos.x += (m_hitOffset * 2) * random(0.0f, 1.0f);
	m_gameScene->FireBullet(this->getPosition(), heroPos);
}

ENEMY_STATE Enemy::GetCurEnemyState()
{
	return m_curEnemyState;
}

int Enemy::GetMapIndex()
{
	return m_mapIdx;
}

void Enemy::SetMapIndex(int mapIndex)
{
	m_mapIdx = mapIndex;
}

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}
