#pragma once
#include "cocos2d.h"

USING_NS_CC;
class Enemy;

class GroupAttackAnchor : public Layer
{
	Sprite* test;
	int m_sign;
	Enemy* m_groupEnemys[4];
	Vec2 m_startPos;

	std::vector<Vec2> m_attackBezierPoints;
	Vector<FiniteTimeAction*> m_bezierToVector;

	void GroupAttackAnchor::MakeBezierAction(std::vector<Vec2>& bezierPoints, float actionTime);
	void FinishAttackCallback(Node* obj);
	void SetStartPos();

public:
	GroupAttackAnchor();
	~GroupAttackAnchor();

	virtual bool init();
	
	CREATE_FUNC(GroupAttackAnchor);

	void SetNULLEnemyArray(Enemy* enemy);
	void SetAttack(Enemy* groupEnemys[4]);
};

