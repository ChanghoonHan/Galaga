#pragma once
#include <cocos2d.h>

USING_NS_CC;

class GalagaGame;
class BlueEnemy;
class RedEnemy;
class BossEnemy;
class Bullet;
class Enemy;
class CatchedHero;
class GroupAttackAnchor;

class EnemyGroup : public Node
{
#define BOSSCOUNT 4
#define REDCOUNT 8
#define BLUECOUNT 10
#define MAXINTERVALX 22
#define MININTERVALX 14

	enum ENEMY_GROUP_STATE
	{
		SPAWNING,
		IDLE,
	};

	std::map<int, Enemy*> m_mapEnemys;
	std::vector<Vec2> m_spawnBezierPointArray[2];
	BossEnemy* m_bossEnemys[BOSSCOUNT];
	RedEnemy* m_redEnemys[2][REDCOUNT];
	BlueEnemy* m_blueEnemys[2][BLUECOUNT];
	GroupAttackAnchor* m_groupAttackAnchor;

	int m_spawnGroup[5][8];

	Vec2 m_bossEnemeyPosArray[BOSSCOUNT];
	Vec2 m_redEnemeyPosArray[2][REDCOUNT];
	Vec2 m_blueEnemeyPosArray[2][BLUECOUNT];

	Size m_visibleSize;
	Vec2 m_pivot;
	Size m_interval;
	Size m_bossInterval;
	int m_enemyIdxNum;
	int m_intervalX;
	int m_changeIntervalXSize;
	int m_changePivotSize;
	int m_maxPivotX;
	int m_minPivotX;
	int m_enemyCount;

	float m_intervalChangeTime;
	float m_IntervalChangeDeltaTime;
	float m_pivotChangeTime;
	float m_pivotChangeDeltaTime;

	int m_spawnSequenceNum;
	int m_spawnSequenceEnemyIdx;
	bool m_enemyAttackStart;
	bool m_hadCatchedHero;
	float m_changeSequenceTime;
	float m_changeSequenceDeltaTime;
	float m_spawnEnemyTime;
	float m_spawnEnemyDeltaTime;

	bool m_isGaming;

	ENEMY_GROUP_STATE m_curEnemyGroupState;
	GalagaGame* m_gameScene;

	void SetEnemysPosition();
	void SetInterval(Size interval);
	void MakeNULLAtEnemyArray(int enemyIdxNum);
	void SetGroupAttack(BossEnemy* bossEnemy, CatchedHero* catchedHero, RedEnemy* redEnmey1 = NULL, RedEnemy* redEnmey2 = NULL);

	void SpawnEnemy(float deltaTime);
	bool CheckEnemySpawnFinish();
	void SetAttackEnemy();

	void InitAttackIndex();

	bool m_bossEnemyAttackCalled;
	float m_blueEnemyAttackDurationTime;
	float m_redEnemyAttackDurationTime;
	float m_bossEnemyAttackDurationTime;
	float m_groupEnemyAttackDurationTime;

	Vec2 m_blueAttack1Idxs[10];
	int m_blueAttack1Count;
	void SetAttackBlueEnemy1(Node* obj);
	Vec2 m_blueAttack2Idxs[10];
	int m_blueAttack2Count;
	void SetAttackBlueEnemy2(Node* obj);
	Vec2 m_redAttack1Idxs[8];
	int m_redAttack1Count;
	void SetAttackRedEnemy1(Node* obj);
	Vec2 m_redAttack2Idxs[8];
	int m_redAttack2Count;
	void SetAttackRedEnemy2(Node* obj);

	int m_bossAttackIdx;
	void SetAttackBossEnemy(Node* obj);
	int m_groupAttackIdx;
	void SetAttackGroupEnemy(Node* obj);
public:

	bool init();

	CREATE_FUNC(EnemyGroup);

	void update(float deltaTime, bool isGaming = true);
	void SetGameScene(GalagaGame* gameScene);
	void SetIsCatchedHero(bool catchedHero);
	bool CheckCollEnemys(Bullet* bullet, int* score);
	bool CheckAllEnemysStateIsIdle();
	bool isGameClear();
	std::map<int, Enemy*> GetEnmeys();

	EnemyGroup();
	~EnemyGroup();
};

