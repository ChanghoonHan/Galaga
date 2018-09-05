#pragma once
#include "Enemy.h"

class CatchRay;
class CatchedHero;

class BossEnemy : public Enemy
{
	CatchRay* m_usingRay;
	CatchedHero* m_catchedHero;

	void ChageSpriteTexture();
	void UseCatchRayCallback(Node* obj);

	virtual void Destroy();
	virtual void SetAttackBezierPoints();

public:
	bool init();

	virtual void AttackedFromBullet();
	void ResumeAttackMove(bool isCatchHero);
	virtual void SetAttack();

	void SetCatchedHero(CatchedHero* catchedHero);
	void SetCatchedHeroDie();
	CatchedHero* GetCatchedHero();
	CREATE_FUNC(BossEnemy);

	BossEnemy();
	~BossEnemy();
};

