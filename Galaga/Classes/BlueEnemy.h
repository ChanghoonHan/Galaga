#pragma once
#include "Enemy.h"
class BlueEnemy : public Enemy
{

	virtual void SetAttackBezierPoints();
public:
	bool init();

	CREATE_FUNC(BlueEnemy);
	virtual void SetAttack();

	BlueEnemy();
	~BlueEnemy();
};

