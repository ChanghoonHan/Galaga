#include "CatchedHero.h"
#include "BossEnemy.h"
#include "CatchRay.h"

bool CatchedHero::init()
{
	Enemy::init();
	m_score = 1000;
	m_life = 1;
	m_bIsCatched = false;
	m_bossEnemy = NULL;
	m_ray = NULL;
	m_state = CH_STATE::CH_GO_TO_BOSS_ENEMY;
	m_playerRedSpritePath = "./GalagaRes/player_red.png";
	m_playerSpritePath = "./GalagaRes/player.png";
	m_sprite = Sprite::create("./GalagaRes/player.png");
	m_spriteSize = m_sprite->getContentSize();
	m_scale = 1.9f;
	this->setScale(m_scale);

	this->addChild(m_sprite);
	return true;
}

void CatchedHero::GroupAttack()
{

	return;
}

void CatchedHero::update(float deltaTime)
{
	Vec2 bossEnemyPos = Vec2::ZERO;

	switch (m_state)
	{
	case CH_STATE::CH_IDLE:
		if (m_bIsIdleActionFinish)
		{
			Enemy::update(deltaTime);
			bossEnemyPos = m_bossEnemy->getPosition();
			this->SetPosition(Vec2(bossEnemyPos.x, bossEnemyPos.y + 30 + 32));
		}
		break;
	case CH_STATE::CH_GO_TO_BOSS_ENEMY:
		if (m_bossEnemy->GetCurEnemyState() == ENEMY_STATE::IDLE)
		{
			ChangeState(CH_STATE::CH_IDLE);
		}

		if (m_bIsCatched)
		{
			bossEnemyPos = m_bossEnemy->getPosition();
			this->SetPosition(Vec2(bossEnemyPos.x, bossEnemyPos.y - 30 - 32));
			Enemy::ChangeState(ENEMY_STATE::IDLE);
		}
		break;
	default:
		break;
	}
}

void CatchedHero::SetGoToBossEnemy(BossEnemy* bossEnemy, CatchRay* ray)
{
	ChangeState(CH_STATE::CH_GO_TO_BOSS_ENEMY);
	m_notUseSetRotation = true;
	m_bossEnemy = bossEnemy;
	m_ray = ray;
	Vec2 bossEnemyPos = bossEnemy->getPosition();
	MoveTo* moveToAction = MoveTo::create(1.0f, Vec2(bossEnemyPos.x, bossEnemyPos.y - 30 - 32));
	ScaleTo* scaleToAction = ScaleTo::create(1.0f, 1.0f);
	RotateBy* rotateByAction = RotateBy::create(1.0f, 1800);
	Spawn* spawnAction = Spawn::create(moveToAction, scaleToAction, rotateByAction, NULL);
	auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(CatchedHero::FinishGoToBossEnemyCallback, this));

	Sequence* seauenceAction = Sequence::create(spawnAction, callbackFunc, NULL);

	this->runAction(seauenceAction);
}

void CatchedHero::FinishGoToBossEnemyCallback(Node* obj)
{
	m_bIsCatched = true;
	m_sprite->setTexture(m_playerRedSpritePath);
	m_ray->FinishCatch();
	this->setScale(2.0f);
}

void CatchedHero::ChangeState(CH_STATE state)
{
	switch (state)
	{
	case CH_STATE::CH_IDLE:
	{
		Vec2 bossEnemyPos = m_bossEnemy->getPosition();
		MoveTo* moveToAction = MoveTo::create(1.0f, Vec2(bossEnemyPos.x, bossEnemyPos.y + 30 + 32));
		auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(CatchedHero::FinishIdleActionCallback, this));
		m_bIsIdleActionFinish = false;
		Sequence* seauenceAction = Sequence::create(moveToAction, callbackFunc, NULL);
		this->runAction(seauenceAction);
	}
		break;
	case CH_STATE::CH_GO_TO_BOSS_ENEMY:
		break;

	case CH_STATE::CH_RELEASE:
		this->stopAllActions();
		m_sprite->setTexture(m_playerSpritePath);
		this->setScale(1.4f);
		this->runAction(RepeatForever::create(RotateBy::create(0.2f, 360)));
		break;

	case CH_STATE::CH_GO_TO_HERO:
		break;

	default:
		break;
	}

	m_state = state;
}

void CatchedHero::SetCollider()
{
	if (m_state == CH_STATE::CH_RELEASE || m_state == CH_STATE::CH_GO_TO_HERO)
	{
		m_collider = Rect::ZERO;
		return;
	}

	Enemy::SetCollider();
	return;
}

void CatchedHero::ReleaseHero()
{
	ChangeState(CH_STATE::CH_RELEASE);
}

void CatchedHero::FinishIdleActionCallback(Node* obj)
{
	m_bIsIdleActionFinish = true;
}

CH_STATE CatchedHero::GetCatchedHeroState()
{
	return m_state;
}

void CatchedHero::RemoveEnemy(Node* obj)
{
	m_bossEnemy->SetCatchedHeroDie();
	Enemy::RemoveEnemy(obj);
}

void CatchedHero::SetGoToHero()
{
	ChangeState(CH_STATE::CH_GO_TO_HERO);

	this->stopAllActions();
	this->setScale(2.0f);
	RotateTo* rotateToAction = RotateTo::create(0.3f, -0);
	Vec2 thisPos = this->getPosition();
	Vec2 DestPos = Vec2(m_visibleSize.width / 4 * 1.5f, m_visibleSize.height / 10);

	Vec2 tempPos1 = Vec2(thisPos.x, (thisPos.y + DestPos.y) / 2);
	Vec2 tempPos2 = thisPos;
	MoveTo* moveToAction1 = MoveTo::create(tempPos1.distance(tempPos2) * (1.0f / 200), tempPos1);

	tempPos2 = tempPos1;
	tempPos1 = Vec2(DestPos.x, (thisPos.y + DestPos.y) / 2);
	MoveTo* moveToAction2 = MoveTo::create(tempPos1.distance(tempPos2) * (1.0f / 200), tempPos1);

	tempPos2 = tempPos1;
	tempPos1 = Vec2(DestPos.x, DestPos.y);
	MoveTo* moveToAction3 = MoveTo::create(tempPos1.distance(tempPos2) * (1.0f / 200), tempPos1);

	CallFuncN* removeFunc = CallFuncN::create(CC_CALLBACK_1(CatchedHero::RemoveEnemy, this));
	Sequence* sequenceAction = Sequence::create(Repeat::create(RotateBy::create(0.2f, 360), 5), rotateToAction, moveToAction1, moveToAction2, moveToAction3, removeFunc, NULL);
	this->runAction(sequenceAction);
}

void CatchedHero::SetAttackBezierPoints()
{

}

void CatchedHero::SetAttack()
{

}

CatchedHero::CatchedHero()
{
}


CatchedHero::~CatchedHero()
{
}