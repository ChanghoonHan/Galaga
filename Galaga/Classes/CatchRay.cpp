#include "CatchRay.h"
#include "proj.win32\BossEnemy.h"

bool CatchRay::init()
{
	m_sprite = Sprite::create("./GalagaRes/enemy_wave0.png");
	m_sprite->setAnchorPoint(Vec2(0.5f, 1.0f));
	m_sprite->setPosition(Vec2(0, 0));
	m_sprite->setScale(2.0f);
	m_isCatchHero = false;
	m_spriteHeightsize = 92;
	m_spriteWidthsize = 96;

	m_rayGrowTime = 0.2;
	m_rayGrowDeltaTime = 0;
	m_rayDrawHeightSize = 0.0f;
	m_growCount = 7;

	m_stayTime = 3;
	m_stayTimeDeltaTime = 0;

	m_animation = Animation::create();
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy_wave0.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy_wave1.png");
	m_animation->addSpriteFrameWithFile("./GalagaRes/enemy_wave2.png");
	m_animation->setDelayPerUnit(0.1f);
	m_animate = Animate::create(m_animation);
	m_sprite->runAction(RepeatForever::create(m_animate));
	this->addChild(m_sprite);
	this->setPosition(Vec2(-200, -200));
	m_bossEnemy = NULL;

	ChangeState(CATCH_RAY_STATE::RAY_IDLE);

	return true;
}

void CatchRay::update(float deltaTime)
{
	switch (m_catchRayState)
	{
	case RAY_IDLE:
		m_sprite->setTextureRect(Rect(0, 0, 0, 0));
		break;
	case RAY_ON:
		m_rayGrowDeltaTime += deltaTime;
		if (m_rayGrowTime <= m_rayGrowDeltaTime)
		{
			m_rayDrawHeightSize += (1 / m_growCount);
			if (m_rayDrawHeightSize >= 1)
			{
				m_rayDrawHeightSize = 1;
				ChangeState(CATCH_RAY_STATE::RAY_STAY);
			}
			m_rayGrowDeltaTime = 0;
		}
		m_sprite->setTextureRect(Rect(0, 0, 96, m_spriteHeightsize * m_rayDrawHeightSize));
		break;
	case RAY_OFF:
		if (m_rayDrawHeightSize == 0)
		{
			ChangeState(CATCH_RAY_STATE::RAY_IDLE);
		}

		m_rayGrowDeltaTime += deltaTime;
		if (m_rayGrowTime <= m_rayGrowDeltaTime)
		{
			m_rayDrawHeightSize -= (1 / m_growCount);
			if (m_rayDrawHeightSize <= 0)
			{
				m_rayDrawHeightSize = 0;
			}
			m_rayGrowDeltaTime = 0;
		}
		m_sprite->setTextureRect(Rect(0, 0, 96, m_spriteHeightsize * m_rayDrawHeightSize));
		break;
	case RAY_STAY:
		m_stayTimeDeltaTime += deltaTime;
		if (m_stayTime <= m_stayTimeDeltaTime)
		{
			ChangeState(CATCH_RAY_STATE::RAY_OFF);
		}
		break;
	case RAY_CATCH:
		m_sprite->setTextureRect(Rect(0, 0, 96, m_spriteHeightsize));
		break;
	default:
		break;
	}
}

void CatchRay::SetPosition()
{
	if (m_bossEnemy == NULL)
	{
		this->setPosition(Vec2(-200, -200));
		return;
	}

	Vec2 bossPos = m_bossEnemy->getPosition();
	this->setPosition(Vec2(bossPos.x, bossPos.y - 30));
}

void CatchRay::BossDestroyed()
{
	m_bossEnemy = NULL;
	ChangeState(CATCH_RAY_STATE::RAY_IDLE);
	SetPosition();
}

BossEnemy* CatchRay::GetBossEnmey()
{
	return m_bossEnemy;
}

void CatchRay::SetOnCatchRay(BossEnemy* bossEnemy)
{
	m_bossEnemy = bossEnemy;
	SetPosition();
	ChangeState(CATCH_RAY_STATE::RAY_ON);
}

void CatchRay::FinishCatch()
{
	ChangeState(CATCH_RAY_STATE::RAY_OFF);
}

void CatchRay::ChangeState(CATCH_RAY_STATE state)
{
	switch (state)
	{
	case RAY_IDLE:
		if (m_bossEnemy != NULL)
		{
			m_bossEnemy->ResumeAttackMove(m_isCatchHero);
			m_bossEnemy = NULL;
		}
		m_isCatchHero = false;
		m_rayDrawHeightSize = 0;
		m_rayGrowDeltaTime = 0;
		m_stayTimeDeltaTime = 0;
		m_collider.setRect(0, 0, 0, 0);
		break;
	case RAY_ON:
		m_rayDrawHeightSize = 0;
		m_rayGrowDeltaTime = 0;
		m_stayTimeDeltaTime = 0;
		break;
	case RAY_OFF:
		m_stayTimeDeltaTime = 0;
		m_collider.setRect(0, 0, 0, 0);
		m_rayGrowDeltaTime = m_rayGrowTime;
		break;
	case RAY_STAY:
		m_stayTimeDeltaTime = 0;
		break;
	case RAY_CATCH:
		m_isCatchHero = true;
		break;
	default:
		break;
	}
	m_catchRayState = state;
}

bool CatchRay::CheckCatchRayCollWithHero(Rect heroRect)
{
	Vec2 thisPos = this->getPosition();
	m_collider.setRect(thisPos.x - m_spriteWidthsize / 2, thisPos.y - (m_spriteHeightsize * 2) - 50, m_spriteWidthsize, 70);

	if (m_collider.intersectsRect(heroRect))
	{
		ChangeState(CATCH_RAY_STATE::RAY_CATCH);
		return true;
	}

	return false;
}

CATCH_RAY_STATE CatchRay::GetCurRayState()
{
	return m_catchRayState;
}

CatchRay::CatchRay()
{
}


CatchRay::~CatchRay()
{
}
