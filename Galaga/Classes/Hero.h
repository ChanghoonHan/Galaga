#pragma once
#include <cocos2d.h>

USING_NS_CC;

class GalagaGame;
class Bullet;

enum HERO_STATE
{
	HS_IDLE,
	HS_DEAD,
	HS_DESTROY
};

class Hero : public Node
{
	Sprite* m_sprite;
	GalagaGame* m_gameScene;
	Size m_visibleSize;
	Rect m_collider;
	HERO_STATE m_curState;
	Animation* m_animation;
	Animate* m_animate;
	Size m_spriteSize;
	Node* m_groupObj;

	int m_bulletCount;
	float m_curBulletReloadTime;
	float m_bulletReloadTime;
	bool m_isFire;

	void ChangeState(HERO_STATE state);
	void Destroy();
	void RemoveHero(Node* obj);

public:

	Hero();
	~Hero();

	void DecreaseBulletCount();

	virtual bool init();

	CREATE_FUNC(Hero);

	void update(float deltaTime, bool isGaming);
	void SetScene(GalagaGame* gameScene);
	void AttackedFromBullet();
	void SetGroupObj(Node* groupPos);
	void SetHeroPositionInGroup(int pos);


	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);

	HERO_STATE GetHeroState();
	Rect GetCollider();
};

