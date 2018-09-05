#pragma once
#include <cocos2d.h>

USING_NS_CC;

class GalagaGame;
class GroupAttackAnchor;

enum ENEMY_TYPE
{
	BLUE,
	RED,
	BOSS
};

enum ENEMY_STATE
{
	IDLE,
	SPAWNING,
	MOVE,
	ATTACK,
	GROUPATTACK,
	DEAD,
	DESTROY,
};

class Enemy : public Node
{
protected:
	Sprite* m_sprite;
	GalagaGame* m_gameScene;
	Size m_visibleSize;
	Size m_spriteSize;
	Size m_groupAttackOffset;
	float m_scale;
	float m_hitOffset;
	float m_fireProb;
	Animation* m_animation;
	Animate* m_animate;
	Rect m_collider;
	ENEMY_STATE m_curEnemyState;
	Vector<FiniteTimeAction*> m_bezierToVector;

	GroupAttackAnchor* m_groupAttackAnchor;

	int m_score;
	int m_life;
	int m_mapIdx;
	int m_attackIndex;
	bool m_noFireBullet;
	bool m_notUseSetRotation;
	bool m_spwanStart;
	bool m_reverse;
	Vec2* m_pBasePoint;
	Vec2 m_prevPos;
	std::vector<Vec2> m_attackBezierPoints[2];

	void SetRotation();
	virtual void RemoveEnemy(Node* obj);
	void ChangeState(ENEMY_STATE enmeyState);
	void ChangeStateToIdleCallback(Node* obj);
	void NotUseSetRotationCallback(Node* obj);
	void MakeBezierAction(std::vector<Vec2>& bezierPoints, float actionTime);

	virtual void GroupAttack();
	void FireBulletCallback(Node* obj);
	virtual void Destroy();
	virtual void SetCollider();
	virtual void SetAttackBezierPoints()=0;

public:
	Enemy();
	~Enemy();

	virtual bool init();
	virtual void AttackedFromBullet();
	virtual void SetAttack()= 0;

	void SetPosition(Vec2 position);
	void SetBasePoint(Vec2* basePoint, bool reverse);
	void SetSpawn(std::vector<Vec2> spawnBezierPoints, bool reverse = false);
	void SetGroupAttack(GroupAttackAnchor* groupAnchor);
	bool CheckCollWithHero(Rect heroRect);

	int GetMapIndex();
	void SetMapIndex(int mapIndex);
	void EndGroupAttack();
	void SetNoFireBullet();
	int GetScore();

	Rect& GetCollider();
	void update(float deltaTime);
	void SetScene(GalagaGame* gameScene);
	ENEMY_STATE GetCurEnemyState();
};

