#pragma once
#include <cocos2d.h>

USING_NS_CC;

class BossEnemy;

enum CATCH_RAY_STATE
{
	RAY_IDLE,
	RAY_ON,
	RAY_OFF,
	RAY_STAY,
	RAY_CATCH,
};

class CatchRay : public Node
{
	Sprite* m_sprite;
	Animation* m_animation;
	Animate* m_animate;
	Rect m_collider;
	BossEnemy* m_bossEnemy;
	CATCH_RAY_STATE m_catchRayState;

	float m_rayGrowTime;
	float m_rayGrowDeltaTime;
	float m_stayTime;
	float m_stayTimeDeltaTime;
	float m_rayDrawHeightSize;
	float m_spriteHeightsize;
	float m_spriteWidthsize;
	float m_growCount;
	bool m_isCatchHero;

	void SetPosition();
	void ChangeState(CATCH_RAY_STATE state);

public:
	CatchRay();
	~CatchRay();
	CREATE_FUNC(CatchRay);

	virtual bool init();
	bool CheckCatchRayCollWithHero(Rect heroRect);
	void SetOnCatchRay(BossEnemy* bossEnemy);
	void BossDestroyed();
	void FinishCatch();
	void update(float deltaTime);
	BossEnemy* GetBossEnmey();
	CATCH_RAY_STATE GetCurRayState();
};