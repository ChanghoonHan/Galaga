#pragma once
#include "Enemy.h"

class BossEnemy;
class CatchRay;

enum CH_STATE
{
	CH_IDLE,
	CH_GO_TO_BOSS_ENEMY,
	CH_RELEASE,
	CH_GO_TO_HERO
};

class CatchedHero : public Enemy
{
	BossEnemy* m_bossEnemy;
	CatchRay* m_ray;
	CH_STATE m_state;
	bool m_bIsCatched;
	bool m_bIsIdleActionFinish;
	std::string m_playerRedSpritePath;
	std::string m_playerSpritePath;

	void ChangeState(CH_STATE state);
	void FinishGoToBossEnemyCallback(Node* obj);
	void FinishIdleActionCallback(Node* obj);
	virtual void RemoveEnemy(Node* obj);

	virtual void SetAttackBezierPoints();
	virtual void SetCollider();
	virtual void GroupAttack();
public:
	bool init();

	virtual void SetAttack();
	void SetGoToBossEnemy(BossEnemy* bossEnemyPos, CatchRay* ray);
	void SetGoToHero();
	void update(float deltaTime);
	void ReleaseHero();
	CH_STATE GetCatchedHeroState();
	CREATE_FUNC(CatchedHero);

	CatchedHero();
	~CatchedHero();
};