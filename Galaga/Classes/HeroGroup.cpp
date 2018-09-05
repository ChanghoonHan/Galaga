#include "HeroGroup.h"
#include "Hero.h"
#include "Bullet.h"
#include "CatchRay.h"
#include "Enemy.h"
#include "GalagaGameScene.h"

bool HeroGroup::init()
{
	m_speed = 300;
	m_dir[0] = false;
	m_dir[1] = false;
	m_heroCount = 0;
	m_heros[0] = NULL;
	m_heros[1] = NULL;

	m_visibleSize = Director::getInstance()->getVisibleSize();
	this->setPosition(m_visibleSize.width / 4 * 1.5f, m_visibleSize.height / 10);

	return true;
}

void HeroGroup::update(float deltaTime, bool isGaming)
{
	if (isGaming)
	{
		if (m_dir[0])
		{
			this->setPositionX(this->getPositionX() - m_speed * deltaTime);
			if (this->getPositionX() < 50 * m_heroCount)
			{
				this->setPositionX(50 * m_heroCount);
			}
		}

		if (m_dir[1])
		{
			this->setPositionX(this->getPositionX() + m_speed * deltaTime);
			if (this->getPositionX() > m_visibleSize.width / 4 * 3 - 50 * m_heroCount)
			{
				this->setPositionX(m_visibleSize.width / 4 * 3 - 50 * m_heroCount);
			}
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL && m_heros[i]->GetHeroState() == HERO_STATE::HS_DESTROY)
		{
			this->removeChild(m_heros[i]);
			m_heros[i] = NULL;
			CheckRemovedHero();
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL)
		{
			m_heros[i]->update(deltaTime, isGaming);
		}
	}
}

void HeroGroup::CheckRemovedHero()
{
	if (m_heros[0] == NULL)
	{
		if (m_heros[1] != NULL)
		{
			m_heros[0] = m_heros[1];
			m_heros[0]->SetHeroPositionInGroup(0);
			return;
		}
	}
	else
	{
		if (m_heros[1] == NULL)
		{
			m_heros[0]->SetHeroPositionInGroup(0);
		}
	}
}

void HeroGroup::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL)
		{
			m_heros[i]->onKeyPressed(keyCode, event);
		}
	}

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_dir[0] = true;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_dir[1] = true;
		break;
	default:
		break;
	}
}

void HeroGroup::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL)
		{
			m_heros[i]->onKeyReleased(keyCode, event);
		}
	}

	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		m_dir[0] = false;
		break;
	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		m_dir[1] = false;
		break;
	default:
		break;
	}
}

void HeroGroup::CreateHero()
{
	if (m_heroCount == 2)
	{
		return;
	}

	Hero* createdHero = Hero::create();
	createdHero->SetScene(m_gameScene);
	createdHero->SetGroupObj(this);
	this->addChild(createdHero);
	
	switch (m_heroCount)
	{
	case 0:
		m_heros[0] = createdHero;
		m_heros[0]->SetHeroPositionInGroup(0);
		m_heros[1] = NULL;
		break;
	case 1:
		m_heros[0]->SetHeroPositionInGroup(1);
		m_heros[1] = createdHero;
		m_heros[1]->SetHeroPositionInGroup(2);
		break;
	default:
		break;
	}

	m_heroCount++;
}

bool HeroGroup::CheckCollBulletWithHero(Bullet* bullet)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL)
		{
			if (bullet->CheckColl(m_heros[i]->GetCollider()))
			{
				m_heros[i]->AttackedFromBullet();
				m_gameScene->RemovepHeroFromBullet(m_heros[i]);
				m_heroCount--;
				return true;
			}
		}
	}

	return false;
}

bool HeroGroup::CheckCollWithEnemy(Enemy* enemy)
{
	for (size_t i = 0; i < 2; i++)
	{
		if (m_heros[i] != NULL)
		{
			if (enemy != NULL && enemy->CheckCollWithHero(m_heros[i]->GetCollider()))
			{
				m_heros[i]->AttackedFromBullet();
				m_gameScene->RemovepHeroFromBullet(m_heros[i]);
				m_heroCount--;
				return true;
			}
		}
	}

	return false;
}

bool HeroGroup::CheckCollRayWithHero(CatchRay* ray)
{
	if (m_heros[0] != NULL && ray->CheckCatchRayCollWithHero(m_heros[0]->GetCollider()))
	{
		m_gameScene->RemovepHeroFromBullet(m_heros[0]);
		this->removeChild(m_heros[0]);
		m_heros[0] = NULL;
		m_heroCount = 0;
		return true;
	}

	return false;
}

void HeroGroup::SetGameScene(GalagaGame* gameScene)
{
	m_gameScene = gameScene;
}

int HeroGroup::GetHeroCount()
{
	return m_heroCount;
}

HeroGroup::HeroGroup()
{
}


HeroGroup::~HeroGroup()
{
}

