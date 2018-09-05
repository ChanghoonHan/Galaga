#pragma once
#include <cocos2d.h>

USING_NS_CC;
class Hero;

class Bullet : public Layer
{
	Size m_visibleSize;
	Sprite* m_sprite;
	Hero* m_pHero;
	Vec2 m_dir;
	Rect m_collider;
	int m_speed;
	bool m_isHero;

	void SetCollider();

public:
	Bullet();
	~Bullet();

	virtual bool init();
	void fire(Vec2 fireStartPos, Vec2 fireDesPos, bool isHero = false, Hero* pHero = NULL);

	CREATE_FUNC(Bullet);

	bool BulletUpdate(float deltaTime);
	bool CheckColl(Rect& collider);
	bool GetIsHeroBullet();
	void HeroDestroyed(Hero* pHero);
};

