#include "EnemyGroup.h"
#include "BossEnemy.h"
#include "RedEnemy.h"
#include "BlueEnemy.h"
#include "Bullet.h"
#include "GroupAttackAnchor.h"
#include "CatchedHero.h"

bool EnemyGroup::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_enemyIdxNum = 0;
	m_hadCatchedHero = false;
	m_blueEnemyAttackDurationTime = 8;
	m_redEnemyAttackDurationTime = 9;
	m_bossEnemyAttackDurationTime = 17;
	m_groupEnemyAttackDurationTime = 13;
	m_pivot.setPoint(m_visibleSize.width / 4 * 1.5, m_visibleSize.height - 100);
	m_maxPivotX = m_visibleSize.width / 4 * 1.5 + 30;
	m_minPivotX = m_visibleSize.width / 4 * 1.5 - 30;
	SetInterval(Size(MININTERVALX, 60));
	m_intervalX = MININTERVALX;
	m_curEnemyGroupState = ENEMY_GROUP_STATE::SPAWNING;
	m_changeIntervalXSize = 1;
	m_changePivotSize = 5;
	m_intervalChangeTime = 1.0f;
	m_IntervalChangeDeltaTime = 0;
	m_pivotChangeTime = 0.3f;
	m_pivotChangeDeltaTime = 0;
	m_spawnSequenceNum = 0;
	m_spawnSequenceEnemyIdx = 0;
	memset(m_bossEnemys, NULL, 4);
	memset(m_redEnemys, NULL, 2*8);
	memset(m_blueEnemys, NULL, 2*10);
	InitAttackIndex();
	m_enemyCount = 0;

	bool reverse = false;
	m_enemyAttackStart = false;
	m_groupAttackAnchor = GroupAttackAnchor::create();
	this->addChild(m_groupAttackAnchor);

	for (size_t i = 0; i < BOSSCOUNT; i++)
	{
		m_bossEnemys[i] = BossEnemy::create();
		m_bossEnemys[i]->SetMapIndex(m_enemyIdxNum);
		m_mapEnemys.insert(std::make_pair(m_enemyIdxNum++, m_bossEnemys[i]));
		this->addChild(m_bossEnemys[i],3);
		m_bossEnemys[i]->retain();
		m_bossEnemys[i]->SetPosition(Vec2(-100, -100));
		reverse = false;
		if (i > BOSSCOUNT / 2)
		{
			reverse = true;
		}

		m_bossEnemys[i]->SetBasePoint(&m_bossEnemeyPosArray[i], reverse);
		m_enemyCount++;
	}

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < REDCOUNT; j++)
		{
			m_redEnemys[i][j] = RedEnemy::create();
			m_redEnemys[i][j]->SetMapIndex(m_enemyIdxNum);
			m_mapEnemys.insert(std::make_pair(m_enemyIdxNum++, m_redEnemys[i][j]));
			this->addChild(m_redEnemys[i][j], 2);
			m_redEnemys[i][j]->retain();
			m_redEnemys[i][j]->SetPosition(Vec2(-100, -100));
			reverse = false;
			if (j > REDCOUNT / 2)
			{
				reverse = true;
			}
			m_redEnemys[i][j]->SetBasePoint(&m_redEnemeyPosArray[i][j], reverse);
			m_enemyCount++;
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < BLUECOUNT; j++)
		{
			m_blueEnemys[i][j] = BlueEnemy::create();
			m_blueEnemys[i][j]->SetMapIndex(m_enemyIdxNum);
			m_mapEnemys.insert(std::make_pair(m_enemyIdxNum++, m_blueEnemys[i][j]));
			this->addChild(m_blueEnemys[i][j], 1);
			m_blueEnemys[i][j]->retain();
			m_blueEnemys[i][j]->SetPosition(Vec2(-100, -100));
			reverse = false;
			if (j > BLUECOUNT / 2)
			{
				reverse = true;
			}
			m_blueEnemys[i][j]->SetBasePoint(&m_blueEnemeyPosArray[i][j], reverse);
			m_enemyCount++;
		}
	}
	
	m_changeSequenceTime = 1;
	m_changeSequenceDeltaTime = 0;
	m_spawnEnemyTime = 0.2f;
	m_spawnEnemyDeltaTime = 0.5f;

	SetEnemysPosition();

	int offsetX = m_visibleSize.width / 4 * 1.5;

	m_spawnBezierPointArray[0].push_back(Vec2(-88, 850));
	m_spawnBezierPointArray[0].push_back(Vec2(-88, 800));
	m_spawnBezierPointArray[0].push_back(Vec2(-51, 708));
	m_spawnBezierPointArray[0].push_back(Vec2(36, 625));
	m_spawnBezierPointArray[0].push_back(Vec2(149, 561));
	m_spawnBezierPointArray[0].push_back(Vec2(275, 521));
	m_spawnBezierPointArray[0].push_back(Vec2(355, 467));
	m_spawnBezierPointArray[0].push_back(Vec2(355, 369));
	m_spawnBezierPointArray[0].push_back(Vec2(275, 300));
	m_spawnBezierPointArray[0].push_back(Vec2(188, 272));
	m_spawnBezierPointArray[0].push_back(Vec2(87, 300));
	m_spawnBezierPointArray[0].push_back(Vec2(45, 397));
	m_spawnBezierPointArray[0].push_back(Vec2(45, 496));

	m_spawnBezierPointArray[1].push_back(Vec2(-430, 156));
	m_spawnBezierPointArray[1].push_back(Vec2(-385, 156));
	m_spawnBezierPointArray[1].push_back(Vec2(-285, 176));
	m_spawnBezierPointArray[1].push_back(Vec2(-162, 228));
	m_spawnBezierPointArray[1].push_back(Vec2(-125, 328));
	m_spawnBezierPointArray[1].push_back(Vec2(-176, 495));
	m_spawnBezierPointArray[1].push_back(Vec2(-271, 495));
	m_spawnBezierPointArray[1].push_back(Vec2(-355, 415));
	m_spawnBezierPointArray[1].push_back(Vec2(-355, 328));
	m_spawnBezierPointArray[1].push_back(Vec2(-271, 228));
	m_spawnBezierPointArray[1].push_back(Vec2(-162, 228));
	m_spawnBezierPointArray[1].push_back(Vec2(-125, 328));
	m_spawnBezierPointArray[1].push_back(Vec2(-125, 415));
	m_spawnBezierPointArray[1].push_back(Vec2(-125, 495));

	m_spawnGroup[0][0] = m_redEnemys[0][3]->GetMapIndex();
	m_spawnGroup[0][1] = m_redEnemys[0][4]->GetMapIndex();
	m_spawnGroup[0][2] = m_redEnemys[1][3]->GetMapIndex();
	m_spawnGroup[0][3] = m_redEnemys[1][4]->GetMapIndex();
	m_spawnGroup[0][4] = m_blueEnemys[0][4]->GetMapIndex();
	m_spawnGroup[0][5] = m_blueEnemys[0][5]->GetMapIndex();
	m_spawnGroup[0][6] = m_blueEnemys[1][4]->GetMapIndex();
	m_spawnGroup[0][7] = m_blueEnemys[1][5]->GetMapIndex();
						 
	m_spawnGroup[1][0] = m_bossEnemys[0]->GetMapIndex();
	m_spawnGroup[1][1] = m_redEnemys[0][2]->GetMapIndex();
	m_spawnGroup[1][2] = m_bossEnemys[1]->GetMapIndex();
	m_spawnGroup[1][3] = m_redEnemys[0][5]->GetMapIndex();
	m_spawnGroup[1][4] = m_bossEnemys[2]->GetMapIndex();
	m_spawnGroup[1][5] = m_redEnemys[1][2]->GetMapIndex();
	m_spawnGroup[1][6] = m_bossEnemys[3]->GetMapIndex();
	m_spawnGroup[1][7] = m_redEnemys[1][5]->GetMapIndex();
						 
	m_spawnGroup[2][0] = m_redEnemys[0][1]->GetMapIndex();
	m_spawnGroup[2][1] = m_redEnemys[0][6]->GetMapIndex();
	m_spawnGroup[2][2] = m_redEnemys[1][1]->GetMapIndex();
	m_spawnGroup[2][3] = m_redEnemys[1][6]->GetMapIndex();
	m_spawnGroup[2][4] = m_redEnemys[0][0]->GetMapIndex();
	m_spawnGroup[2][5] = m_redEnemys[0][7]->GetMapIndex();
	m_spawnGroup[2][6] = m_redEnemys[1][0]->GetMapIndex();
	m_spawnGroup[2][7] = m_redEnemys[1][7]->GetMapIndex();
						 
	m_spawnGroup[3][0] = m_blueEnemys[0][3]->GetMapIndex();
	m_spawnGroup[3][1] = m_blueEnemys[0][6]->GetMapIndex();
	m_spawnGroup[3][2] = m_blueEnemys[1][3]->GetMapIndex();
	m_spawnGroup[3][3] = m_blueEnemys[1][6]->GetMapIndex();
	m_spawnGroup[3][4] = m_blueEnemys[0][2]->GetMapIndex();
	m_spawnGroup[3][5] = m_blueEnemys[0][7]->GetMapIndex();
	m_spawnGroup[3][6] = m_blueEnemys[1][2]->GetMapIndex();
	m_spawnGroup[3][7] = m_blueEnemys[1][7]->GetMapIndex();

	m_spawnGroup[4][0] = m_blueEnemys[0][1]->GetMapIndex();
	m_spawnGroup[4][1] = m_blueEnemys[0][8]->GetMapIndex();
	m_spawnGroup[4][2] = m_blueEnemys[1][1]->GetMapIndex();
	m_spawnGroup[4][3] = m_blueEnemys[1][8]->GetMapIndex();
	m_spawnGroup[4][4] = m_blueEnemys[0][0]->GetMapIndex();
	m_spawnGroup[4][5] = m_blueEnemys[0][9]->GetMapIndex();
	m_spawnGroup[4][6] = m_blueEnemys[1][0]->GetMapIndex();
	m_spawnGroup[4][7] = m_blueEnemys[1][9]->GetMapIndex();


	return true;
}

void EnemyGroup::SetEnemysPosition()
{
	int intervalCount = 0;

	for (size_t i = 0; i < BOSSCOUNT; i++)
	{
		if (i < BOSSCOUNT/2)
		{
			intervalCount = (BOSSCOUNT / 2) - (i + 1);
			m_bossEnemeyPosArray[i] = Vec2(m_pivot.x - m_interval.width - (m_interval.width * 2 * intervalCount), m_pivot.y);
 		}
		else
		{
			intervalCount = (i - (BOSSCOUNT / 2));
			m_bossEnemeyPosArray[i] = Vec2(m_pivot.x + m_interval.width + (m_interval.width * 2 * intervalCount), m_pivot.y);
		}

		if (m_bossEnemys[i] != NULL && m_bossEnemys[i]->GetCurEnemyState() == ENEMY_STATE::IDLE)
		{
			m_bossEnemys[i]->SetPosition(m_bossEnemeyPosArray[i]);
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < REDCOUNT; j++)
		{
			if (i < REDCOUNT / 2)
			{
				intervalCount = (REDCOUNT / 2) - (j + 1);
			}
			else
			{
				intervalCount = (j - (REDCOUNT / 2));
			}
			m_redEnemeyPosArray[i][j] = Vec2(m_pivot.x - m_interval.width - (m_interval.width * 2 * intervalCount), m_pivot.y - (m_interval.height * (i + 1)));

			if (m_redEnemys[i][j] != NULL && m_redEnemys[i][j]->GetCurEnemyState() == ENEMY_STATE::IDLE)
			{
				m_redEnemys[i][j]->SetPosition(m_redEnemeyPosArray[i][j]);
			}
		}
	}

	for (size_t i = 0; i < 2; i++)
	{
		for (size_t j = 0; j < BLUECOUNT; j++)
		{
			int intervalCount = 0;
			if (i < BLUECOUNT / 2)
			{
				intervalCount = (BLUECOUNT / 2) - (j + 1);
			}
			else
			{
				intervalCount = (j - (BLUECOUNT / 2));
			}
			m_blueEnemeyPosArray[i][j] = Vec2(m_pivot.x - m_interval.width - (m_interval.width * 2 * intervalCount), m_pivot.y - (m_interval.height * 2) - (m_interval.height * (i + 1)));
			
			if (m_blueEnemys[i][j] != NULL && m_blueEnemys[i][j]->GetCurEnemyState() == ENEMY_STATE::IDLE)
			{
				m_blueEnemys[i][j]->SetPosition(m_blueEnemeyPosArray[i][j]);
			}
		}
	}
}

void EnemyGroup::update(float deltaTime, bool isGaming)
{
	for (auto iter = m_mapEnemys.begin(); iter != m_mapEnemys.end(); iter++)
	{
		if (iter->second == NULL)
		{
			continue;
		}

		if (iter->second->GetCurEnemyState() == ENEMY_STATE::DESTROY)
		{
			removeChild(iter->second);
			MakeNULLAtEnemyArray(iter->first);
			iter->second = NULL;
			m_enemyCount--;
		}
		else
		{
			iter->second->update(deltaTime);
		}
	}

	switch (m_curEnemyGroupState)
	{
	case EnemyGroup::SPAWNING:
		m_pivotChangeDeltaTime += deltaTime;

		if (m_pivotChangeDeltaTime > m_pivotChangeTime)
		{
			m_pivotChangeDeltaTime = 0;

			if ((int)m_pivot.x == m_maxPivotX)
			{
				m_changePivotSize = -5;
			}

			if ((int)m_pivot.x == m_minPivotX)
			{
				m_changePivotSize = 5;
			}

			m_pivot.x += m_changePivotSize;
			SetEnemysPosition();
		}

		SpawnEnemy(deltaTime);
		break;

	case EnemyGroup::IDLE:
		m_IntervalChangeDeltaTime += deltaTime;

		if (m_IntervalChangeDeltaTime > m_intervalChangeTime)
		{
			m_IntervalChangeDeltaTime = 0;
			 
			if (m_intervalX == MAXINTERVALX)
			{
				m_changeIntervalXSize = -2;
			}

			if (m_intervalX == MININTERVALX)
			{
				m_changeIntervalXSize = 2;
			}

			m_intervalX += m_changeIntervalXSize;
			SetInterval(Size(m_intervalX, 60));
			SetEnemysPosition();
		}

		if (isGaming)
		{
			m_isGaming = true;
			if (m_enemyAttackStart == false)
			{
				m_enemyAttackStart = true;
				SetAttackEnemy();
			}
		}
		else
		{
			m_isGaming = false;
			m_enemyAttackStart = false;
		}
		break;
	default:
		break;
	}
}

void EnemyGroup::SpawnEnemy(float deltaTime)
{
	switch (m_spawnSequenceNum)
	{
	case 0:
		if (m_spawnSequenceEnemyIdx == 4)
		{
			if (CheckEnemySpawnFinish())
			{
				m_spawnSequenceNum++;
				m_spawnEnemyDeltaTime = m_spawnEnemyTime;
				m_changeSequenceDeltaTime = 0;
				m_spawnSequenceEnemyIdx = 0;
			}

			break;
		}

		m_spawnEnemyDeltaTime += deltaTime;
		if (m_spawnEnemyTime <= m_spawnEnemyDeltaTime)
		{
			m_spawnEnemyDeltaTime = 0;
			m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx]]->SetSpawn(m_spawnBezierPointArray[0]);
			m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx + 4]]->SetSpawn(m_spawnBezierPointArray[0], true);
			m_spawnSequenceEnemyIdx++;
		}
		break;

	case 1:
	case 2:
	case 3:
	case 4:
		if (m_changeSequenceTime < m_changeSequenceDeltaTime)
		{
			m_changeSequenceDeltaTime += deltaTime;
			break;
		}

		if (m_spawnSequenceEnemyIdx == 8)
		{
			if (CheckEnemySpawnFinish())
			{
				m_spawnSequenceNum++;
				m_spawnEnemyDeltaTime = m_spawnEnemyTime;
				m_changeSequenceDeltaTime = 0;
				m_spawnSequenceEnemyIdx = 0;
			}
			break;
		}

		m_spawnEnemyDeltaTime += deltaTime;
		if (m_spawnEnemyTime <= m_spawnEnemyDeltaTime)
		{
			m_spawnEnemyDeltaTime = 0;
			switch (m_spawnSequenceNum)
			{
			case 1:
				m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx]]->SetSpawn(m_spawnBezierPointArray[1]);
				break;
			case 2:
				m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx]]->SetSpawn(m_spawnBezierPointArray[1], true);
				break;
			case 3:
				m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx]]->SetSpawn(m_spawnBezierPointArray[0]);
				break;
			case 4:
				m_mapEnemys[m_spawnGroup[m_spawnSequenceNum][m_spawnSequenceEnemyIdx]]->SetSpawn(m_spawnBezierPointArray[0], true);
				break;
			}
			m_spawnSequenceEnemyIdx++;
		}
		break;

	case 5:
		if (m_pivot.x == m_visibleSize.width / 4 * 1.5)
		{
			m_curEnemyGroupState = ENEMY_GROUP_STATE::IDLE;
		}
		break;

	default:
		break;
	}
}

void EnemyGroup::SetGroupAttack(BossEnemy* bossEnemy, CatchedHero* catchedHero, RedEnemy* redEnmey1, RedEnemy* redEnmey2)
{
	Enemy* enemyGroups[4];
	enemyGroups[0] = bossEnemy;
	enemyGroups[1] = redEnmey1;
	enemyGroups[2] = redEnmey2;
	enemyGroups[3] = catchedHero;

	m_groupAttackAnchor->SetAttack(enemyGroups);
}

bool EnemyGroup::CheckEnemySpawnFinish()
{
	for (auto enemyIdx : m_spawnGroup[m_spawnSequenceNum])
	{
		if (m_mapEnemys[enemyIdx] != NULL && m_mapEnemys[enemyIdx]->GetCurEnemyState() == ENEMY_STATE::SPAWNING)
		{
			return false;
		}
	}

	return true;
}

bool EnemyGroup::CheckCollEnemys(Bullet* bullet, int* score)
{
	for (auto iter = m_mapEnemys.begin(); iter != m_mapEnemys.end(); iter++)
	{
		if (iter->second == NULL)
		{
			continue;
		}

		if (iter->second->GetCurEnemyState() != ENEMY_STATE::DEAD &&
			iter->second->GetCurEnemyState() != ENEMY_STATE::DESTROY &&
			bullet->CheckColl(iter->second->GetCollider()))
		{
			*score = iter->second->GetScore();
			iter->second->AttackedFromBullet();

			return true;
		}
	}

	return false;
}

void EnemyGroup::SetAttackEnemy()
{
	auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBlueEnemy1, this));
	Sequence* seqAction = Sequence::create(DelayTime::create(1), attackCallbackFunc, NULL);
	this->runAction(seqAction);
	attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy2, this));
	seqAction = Sequence::create(DelayTime::create(3), attackCallbackFunc, NULL);
	this->runAction(seqAction);
	attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBlueEnemy2, this));
	seqAction = Sequence::create(DelayTime::create(5), attackCallbackFunc, NULL);
	this->runAction(seqAction);
	attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackGroupEnemy, this));
	seqAction = Sequence::create(DelayTime::create(7), attackCallbackFunc, NULL);
	this->runAction(seqAction);
	attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy1, this));
	seqAction = Sequence::create(DelayTime::create(9), attackCallbackFunc, NULL);
	this->runAction(seqAction);
	attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBossEnemy, this));
	seqAction = Sequence::create(DelayTime::create(11), attackCallbackFunc, NULL);
	this->runAction(seqAction);
}

void EnemyGroup::SetAttackBlueEnemy1(Node* obj)
{
	if (!m_isGaming || m_blueAttack1Count == 10)
	{
		return;
	}

	Vec2 index = m_blueAttack1Idxs[m_blueAttack1Count];

	while(m_blueEnemys[(int)index.y][(int)index.x] == NULL)
	{
		m_blueAttack1Count++;
		if (m_blueAttack1Count == 10)
		{
			return;
		}
		index = m_blueAttack1Idxs[m_blueAttack1Count];
	}

	if (m_blueEnemys[(int)index.y][(int)index.x]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		m_blueEnemys[(int)index.y][(int)index.x]->SetAttack();
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBlueEnemy1, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_blueEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::SetAttackBlueEnemy2(Node* obj)
{
	if (!m_isGaming || m_blueAttack2Count == 10)
	{
		return;
	}

	Vec2 index = m_blueAttack2Idxs[m_blueAttack2Count];

	while (m_blueEnemys[(int)index.y][(int)index.x] == NULL)
	{
		m_blueAttack2Count++;
		if (m_blueAttack2Count == 10)
		{
			return;
		}
		index = m_blueAttack2Idxs[m_blueAttack2Count];
	}

	if (m_blueEnemys[(int)index.y][(int)index.x]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		m_blueEnemys[(int)index.y][(int)index.x]->SetAttack();
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBlueEnemy2, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_blueEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::SetAttackRedEnemy1(Node* obj)
{
	if (!m_isGaming || m_redAttack1Count == 8)
	{
		return;
	}

	Vec2 index = m_redAttack1Idxs[m_redAttack1Count];
	while (m_redEnemys[(int)index.y][(int)index.x] == NULL)
	{
		m_redAttack1Count++;
		if (m_redAttack1Count == 8)
		{
			return;
		}
		index = m_redAttack1Idxs[m_redAttack1Count];
	}

	if (m_redEnemys[(int)index.y][(int)index.x]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		m_redEnemys[(int)index.y][(int)index.x]->SetAttack();
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy1, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_redEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
	else
	{
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy1, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(1), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::SetAttackRedEnemy2(Node* obj)
{
	if (!m_isGaming || m_redAttack2Count == 8)
	{
		return;
	}

	Vec2 index = m_redAttack2Idxs[m_redAttack2Count];
	while (m_redEnemys[(int)index.y][(int)index.x] == NULL)
	{
		m_redAttack2Count++;
		if (m_redAttack2Count == 8)
		{
			return;
		}
		index = m_redAttack2Idxs[m_redAttack2Count];
	}

	if (m_redEnemys[(int)index.y][(int)index.x]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		m_redEnemys[(int)index.y][(int)index.x]->SetAttack();
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy2, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_redEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
	else
	{
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackRedEnemy2, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(1), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::SetAttackBossEnemy(Node* obj)
{
	if (!m_isGaming || m_bossAttackIdx == 4)
	{
		return;
	}

	int index = m_bossAttackIdx;
	if (m_hadCatchedHero)
	{
		m_bossEnemyAttackCalled = false;
		return;
	}

	while (m_bossEnemys[index] == NULL)
	{
		m_bossAttackIdx++;
		if (m_bossAttackIdx == 4)
		{
			return;
		}
		index = m_bossAttackIdx;
	}

	if (m_bossEnemys[index]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		m_bossEnemys[index]->SetAttack();
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBossEnemy, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_bossEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
	else
	{
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBossEnemy, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(3), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::SetAttackGroupEnemy(Node* obj)
{
	if (!m_isGaming || m_groupAttackIdx == -1)
	{
		return;
	}

	int index = m_groupAttackIdx;

	if (m_hadCatchedHero)
	{
		index = m_bossAttackIdx;

		while (m_bossEnemys[index] == NULL)
		{
			m_bossAttackIdx++;
			if (m_bossAttackIdx == 4)
			{
				return;
			}
			index = m_bossAttackIdx;
		}
	}
	else
	{
		if (!m_bossEnemyAttackCalled)
		{
			m_bossEnemyAttackCalled = true;
			auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackBossEnemy, this));
			Sequence* seqAction = Sequence::create(DelayTime::create(2), attackCallbackFunc, NULL);
			this->runAction(seqAction);
		}

		while (m_bossEnemys[index] == NULL)
		{
			m_groupAttackIdx--;
			if (m_groupAttackIdx == -1)
			{
				return;
			}
			index = m_groupAttackIdx;
		}
	}

	if (m_bossEnemys[index]->GetCurEnemyState() == ENEMY_STATE::IDLE)
	{
		RedEnemy* redEnemys[2] = {NULL, NULL};

		for (size_t i = 0; i < 2; i++)
		{
			if (m_redEnemys[0][index + (i + 1)] != NULL && m_redEnemys[0][index + (i + 1)]->GetCurEnemyState() == ENEMY_STATE::IDLE)
			{
				redEnemys[i] = m_redEnemys[0][index + (i + 1)];
				redEnemys[i]->SetNoFireBullet();
			}
		}

		if (m_bossEnemys[index]->GetCatchedHero() != NULL)
		{
			m_bossEnemys[index]->GetCatchedHero()->SetNoFireBullet();
		}

		SetGroupAttack(m_bossEnemys[index], m_bossEnemys[index]->GetCatchedHero(), redEnemys[0], redEnemys[1]);
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackGroupEnemy, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(m_groupEnemyAttackDurationTime), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
	else
	{
		auto attackCallbackFunc = CallFuncN::create(CC_CALLBACK_1(EnemyGroup::SetAttackGroupEnemy, this));
		Sequence* seqAction = Sequence::create(DelayTime::create(3), attackCallbackFunc, NULL);
		this->runAction(seqAction);
	}
}

void EnemyGroup::MakeNULLAtEnemyArray(int enemyIdxNum)
{
	Enemy* removeEnemyAddr;

	if (enemyIdxNum < BOSSCOUNT)
	{
		removeEnemyAddr = m_bossEnemys[enemyIdxNum];
		m_bossEnemys[enemyIdxNum] = NULL;
	}
	else if(enemyIdxNum < BOSSCOUNT + (REDCOUNT * 2))
	{
		int index = enemyIdxNum - BOSSCOUNT;
		if (index < REDCOUNT)
		{
			removeEnemyAddr = m_redEnemys[0][index];
			m_redEnemys[0][index] = NULL;
		}
		else
		{
			index = index - REDCOUNT;
			removeEnemyAddr = m_redEnemys[1][index];
			m_redEnemys[1][index] = NULL;
		}
	}
	else
	{
		int index = enemyIdxNum - BOSSCOUNT - (REDCOUNT * 2);
		if (index < BLUECOUNT)
		{
			removeEnemyAddr = m_blueEnemys[0][index];
			m_blueEnemys[0][index] = NULL;
		}
		else
		{
			index = index - BLUECOUNT;
			removeEnemyAddr = m_blueEnemys[1][index];
			m_blueEnemys[1][index] = NULL;
		}
	}

	m_groupAttackAnchor->SetNULLEnemyArray(removeEnemyAddr);
}

void EnemyGroup::SetInterval(Size interval)
{
	m_interval.width = 27 + (interval.width / 2);
	m_interval.height = interval.height;
	m_bossInterval.width = 27 + (interval.width);
	m_bossInterval.height = 0;
}

void EnemyGroup::SetGameScene(GalagaGame* gameScene)
{
	m_gameScene = gameScene;
	for (auto& enemy : m_mapEnemys)
	{
		enemy.second->SetScene(gameScene);
	}
}

void EnemyGroup::InitAttackIndex()
{
	m_blueAttack1Count = 0;
	m_blueAttack1Idxs[0] = Vec2(0, 1);
	m_blueAttack1Idxs[1] = Vec2(8, 1);
	m_blueAttack1Idxs[2] = Vec2(2, 1);
	m_blueAttack1Idxs[3] = Vec2(6, 1);
	m_blueAttack1Idxs[4] = Vec2(4, 1);
	m_blueAttack1Idxs[5] = Vec2(0, 0);
	m_blueAttack1Idxs[6] = Vec2(8, 0);
	m_blueAttack1Idxs[7] = Vec2(2, 0);
	m_blueAttack1Idxs[8] = Vec2(6, 0);
	m_blueAttack1Idxs[9] = Vec2(4, 0);
	
	m_blueAttack2Count = 0;
	m_blueAttack2Idxs[0] = Vec2(9, 1);
	m_blueAttack2Idxs[1] = Vec2(1, 1);
	m_blueAttack2Idxs[2] = Vec2(7, 1);
	m_blueAttack2Idxs[3] = Vec2(3, 1);
	m_blueAttack2Idxs[4] = Vec2(5, 1);
	m_blueAttack2Idxs[5] = Vec2(9, 0);
	m_blueAttack2Idxs[6] = Vec2(1, 0);
	m_blueAttack2Idxs[7] = Vec2(7, 0);
	m_blueAttack2Idxs[8] = Vec2(3, 0);
	m_blueAttack2Idxs[9] = Vec2(5, 0);
	
	m_redAttack1Count = 0;
	m_redAttack1Idxs[0] = Vec2(0, 1);
	m_redAttack1Idxs[1] = Vec2(6, 1);
	m_redAttack1Idxs[2] = Vec2(2, 1);
	m_redAttack1Idxs[3] = Vec2(4, 1);
	m_redAttack1Idxs[4] = Vec2(0, 1);
	m_redAttack1Idxs[5] = Vec2(6, 0);
	m_redAttack1Idxs[6] = Vec2(2, 0);
	m_redAttack1Idxs[7] = Vec2(4, 0);

	m_redAttack2Count = 0;
	m_redAttack2Idxs[0] = Vec2(7, 1);
	m_redAttack2Idxs[1] = Vec2(1, 1);
	m_redAttack2Idxs[2] = Vec2(5, 1);
	m_redAttack2Idxs[3] = Vec2(3, 1);
	m_redAttack2Idxs[4] = Vec2(7, 1);
	m_redAttack2Idxs[5] = Vec2(1, 0);
	m_redAttack2Idxs[6] = Vec2(5, 0);
	m_redAttack2Idxs[7] = Vec2(3, 0);

	m_bossAttackIdx = 0;
	m_groupAttackIdx = 3;
}

void EnemyGroup::SetIsCatchedHero(bool catchedHero)
{
	m_hadCatchedHero = catchedHero;
}

bool EnemyGroup::CheckAllEnemysStateIsIdle()
{
	for (auto enemy : m_mapEnemys)
	{
		if (enemy.second != NULL && enemy.second->GetCurEnemyState() != ENEMY_STATE::IDLE)
		{
			return false;
		}
	}

	return true;
}

bool EnemyGroup::isGameClear()
{
	if (m_enemyCount == 0)
	{
		return true;
	}

	return false;
}

std::map<int, Enemy*> EnemyGroup::GetEnmeys()
{
	return m_mapEnemys;
}

EnemyGroup::EnemyGroup()
{
}


EnemyGroup::~EnemyGroup()
{
}
