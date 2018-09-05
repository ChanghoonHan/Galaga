#include "Bullet.h"
#include "Hero.h"

bool Bullet::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_speed = 0;
	m_isHero = false;

	return true;
}

void Bullet::fire(Vec2 fireStartPos, Vec2 fireDesPos, bool isHero, Hero* pHero)
{
	if (isHero)
	{
		m_sprite = Sprite::create("./GalagaRes/bullet_player.png");
		this->setPosition(fireStartPos.x, fireStartPos.y + 20);
		m_speed = 1000;
		m_dir = Vec2(0, 1);
		m_isHero = true;
		m_pHero = pHero;
	}
	else
	{
		m_sprite = Sprite::create("./GalagaRes/bullet_enemy.png");
		this->setPosition(fireStartPos.x, fireStartPos.y + 20);
		m_speed = 300;
		m_dir = fireDesPos - fireStartPos;
		m_dir.normalize();
		m_isHero = false;
	}

	this->setScale(2.0f);
	this->addChild(m_sprite);

	return;
}

bool Bullet::BulletUpdate(float deltaTime)
{
	this->setPosition(
		this->getPositionX() + m_dir.x * m_speed * deltaTime,
		this->getPositionY() + m_dir.y * m_speed * deltaTime
	);

	if (m_isHero)
	{
		if (this->getPositionY() > (m_visibleSize.height + this->getContentSize().height / 2))
		{
			if (m_pHero != NULL)
			{
				m_pHero->DecreaseBulletCount();
			}
			return true;
		}
	}
	else
	{
		if (this->getPositionY() < -10)
		{
			return true;
		}
	}

	return false;
}

void Bullet::SetCollider()
{
	m_collider.setRect(this->getPositionX() - 6, this->getPositionY() - 13, 12, 26);
}

bool Bullet::CheckColl(Rect& collider)
{
	SetCollider();

	if (m_collider.intersectsRect(collider))
	{
		if (m_pHero != NULL && m_isHero)
		{
			m_pHero->DecreaseBulletCount();
		}

		this->removeFromParent();
		return true;
	}
	
	return false;
}

void Bullet::HeroDestroyed(Hero* pHero)
{
	if (m_pHero == pHero)
	{
		m_pHero = NULL;
	}
}

bool Bullet::GetIsHeroBullet()
{
	return m_isHero;
}

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
