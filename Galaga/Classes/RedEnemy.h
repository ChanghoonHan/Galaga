#pragma once
#include "Enemy.h"
class RedEnemy : public Enemy
{

	virtual void SetAttackBezierPoints();

public:
	bool init();

	CREATE_FUNC(RedEnemy);
	virtual void SetAttack();

	RedEnemy();
	~RedEnemy();
};

