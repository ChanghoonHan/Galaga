#include "GroupAttackAnchor.h"
#include "Enemy.h"

bool GroupAttackAnchor::init()
{
	test = Sprite::create("./GalagaRes/enemy3_0.png");
	test->setPosition(Vec2::ZERO);
	test->setVisible(false);
	this->addChild(test);

	m_sign = 1;

	m_attackBezierPoints.push_back(Vec2(150, 600));
	m_attackBezierPoints.push_back(Vec2(150, 500));
	m_attackBezierPoints.push_back(Vec2(150, 450));
	m_attackBezierPoints.push_back(Vec2(100, 400));
	m_attackBezierPoints.push_back(Vec2(50, 400));
	m_attackBezierPoints.push_back(Vec2(0, 450));
	m_attackBezierPoints.push_back(Vec2(0, 500));
	m_attackBezierPoints.push_back(Vec2(50, 550));
	m_attackBezierPoints.push_back(Vec2(100, 550));
	m_attackBezierPoints.push_back(Vec2(150, 500));
	m_attackBezierPoints.push_back(Vec2(150, 450));
	m_attackBezierPoints.push_back(Vec2(100, 400));
	m_attackBezierPoints.push_back(Vec2(75, 350));
	m_attackBezierPoints.push_back(Vec2(50, 300));
	m_attackBezierPoints.push_back(Vec2(25, 250));
	m_attackBezierPoints.push_back(Vec2(0, 200));
	m_attackBezierPoints.push_back(Vec2(0, 150));
	m_attackBezierPoints.push_back(Vec2(25, 100));
	m_attackBezierPoints.push_back(Vec2(50, 50));
	m_attackBezierPoints.push_back(Vec2(80, 0));
	m_attackBezierPoints.push_back(Vec2(80, -50));
	m_attackBezierPoints.push_back(Vec2(80, -100));

	return true;
}

void GroupAttackAnchor::SetAttack(Enemy* groupEnemys[4])
{
	for (size_t i = 0; i < 4; i++)
	{
		m_groupEnemys[i] = groupEnemys[i];
	}

	SetStartPos();

	for (size_t i = 0; i < 4; i++)
	{
		if (m_groupEnemys[i] == NULL)
		{
			continue;
		}

		m_groupEnemys[i]->SetGroupAttack(this);
	}

	Vec2 thisPos = this->getPosition();
	Size visibleSize = Director::getInstance()->getVisibleSize();

	std::vector<Vec2> bezierPoints;
	bezierPoints.push_back(Vec2(thisPos.x + (50 * m_sign), thisPos.y + 50));
	bezierPoints.push_back(Vec2(thisPos.x + (100 * m_sign), thisPos.y));

	for (auto i : m_attackBezierPoints)
	{
		bezierPoints.push_back(Vec2(Vec2(thisPos.x + (i.x * m_sign), i.y)));
	}

	MakeBezierAction(bezierPoints, 0.7);

	MoveTo* moveToAction = MoveTo::create(0, Vec2(thisPos.x, visibleSize.height + 100));
	m_bezierToVector.pushBack(moveToAction);
	moveToAction = MoveTo::create(1.5, thisPos);
	m_bezierToVector.pushBack(moveToAction);
	auto callbackFunc = CallFuncN::create(CC_CALLBACK_1(GroupAttackAnchor::FinishAttackCallback, this));
	m_bezierToVector.pushBack(callbackFunc);

	this->runAction(Sequence::create(m_bezierToVector));

	m_sign *= -1;
}

void GroupAttackAnchor::MakeBezierAction(std::vector<Vec2>& bezierPoints, float actionTime)
{
	int pointIdx = 0;

	for (auto bezierToAction : m_bezierToVector)
	{
		bezierToAction->release();
	}
	m_bezierToVector.clear();

	ccBezierConfig bezierConfig;
	BezierTo* bezierTo = NULL;

	for (auto iter = bezierPoints.begin(); iter != bezierPoints.end(); iter++)
	{
		switch (pointIdx)
		{
		case 0:
			bezierConfig.controlPoint_1 = *iter;
			bezierConfig.controlPoint_2 = *iter;
			bezierConfig.endPosition = *iter;
			break;
		case 1:
			bezierConfig.controlPoint_2 = *iter;
			bezierConfig.endPosition = *iter;
			break;
		case 2:
			bezierConfig.endPosition = *iter;
			bezierTo = BezierTo::create(actionTime, bezierConfig);
			m_bezierToVector.pushBack(bezierTo);
			break;
		default:
			break;
		}

		if (iter + 1 == bezierPoints.end() && pointIdx != 2)
		{
			bezierTo = BezierTo::create(actionTime, bezierConfig);
			m_bezierToVector.pushBack(bezierTo);
		}

		pointIdx++;
		if (pointIdx == 3)
		{
			pointIdx = 0;
		}
	}
}

void GroupAttackAnchor::FinishAttackCallback(Node* obj)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (m_groupEnemys[i] == NULL)
		{
			continue;
		}

		m_groupEnemys[i]->EndGroupAttack();
	}
}

void GroupAttackAnchor::SetStartPos()
{
	Vec2 anchorPos = Vec2::ZERO;
	Vec2 redPos1 = Vec2::ZERO;
	Vec2 redPos2 = Vec2::ZERO;

	Vec2 bossPos = m_groupEnemys[0]->getPosition();

	if (m_groupEnemys[1] != NULL)
	{
		redPos1 = m_groupEnemys[1]->getPosition();
	}

	if (m_groupEnemys[2] != NULL)
	{
		redPos2 = m_groupEnemys[2]->getPosition();
	}

	if (m_groupEnemys[2] != NULL)
	{
		if (m_groupEnemys[1] != NULL)
		{
			anchorPos.x = (redPos1.x + redPos2.x) / 2;
			anchorPos.y = (bossPos.y + redPos1.y) / 2;
		}
		else
		{
			anchorPos.x = (bossPos.x + redPos2.x) / 2;
			anchorPos.y = (bossPos.y + redPos2.y) / 2;
		}
	}
	else if (m_groupEnemys[1] != NULL)
	{
		anchorPos.x = (bossPos.x + redPos1.x) / 2;
		anchorPos.y = (bossPos.y + redPos1.y) / 2;
	}
	else
	{
		anchorPos = bossPos;
	}

	m_startPos = anchorPos;
	this->setPosition(m_startPos);
}

void GroupAttackAnchor::SetNULLEnemyArray(Enemy* enemy)
{
	for (size_t i = 0; i < 4; i++)
	{
		if (m_groupEnemys[i] != NULL && m_groupEnemys[i] == enemy)
		{
			m_groupEnemys[i] = NULL;
			return;
		}
	}
}

GroupAttackAnchor::GroupAttackAnchor()
{
}


GroupAttackAnchor::~GroupAttackAnchor()
{
}
