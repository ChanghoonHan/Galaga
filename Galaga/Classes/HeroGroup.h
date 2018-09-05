#pragma once
#include <cocos2d.h>

USING_NS_CC;

class GalagaGame;
class Bullet;
class Hero;
class CatchRay;
class Enemy;
class GalagaGame;

class HeroGroup : public Node
{
	int m_heroCount;
	float m_speed;
	bool m_dir[2];
	Hero* m_heros[2];
	Size m_visibleSize;
	GalagaGame* m_gameScene;

	void CheckRemovedHero();

public:

	HeroGroup();
	~HeroGroup();

	virtual bool init();

	CREATE_FUNC(HeroGroup);

	void update(float deltaTime, bool isGaming = true);
	void CreateHero();
	int GetHeroCount();
	bool CheckCollBulletWithHero(Bullet* bullet);
	bool CheckCollWithEnemy(Enemy* enemy);
	bool CheckCollRayWithHero(CatchRay* ray);
	void SetGameScene(GalagaGame* gameScene);
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
};

