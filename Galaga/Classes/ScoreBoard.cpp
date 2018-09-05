#include "ScoreBoard.h"

bool ScoreBoard::init()
{
	m_visibleSize = Director::getInstance()->getVisibleSize();
	m_highScore = 30000;
	m_score = 0;
	m_life = 3;

	m_visibleSize;

	DrawNode* scoreBox = DrawNode::create();
	scoreBox->drawSolidRect(Vec2(m_visibleSize.width / 4 * 3, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::BLACK);
	scoreBox->drawRect(Vec2(m_visibleSize.width / 4 * 3, 0), Vec2(m_visibleSize.width, m_visibleSize.height), Color4F::GREEN);
	this->addChild(scoreBox);

	LabelTTF* highScoreLabel = LabelTTF::create("HIGH\nSCORE", "fonts/arial.ttf", 40);
	highScoreLabel->setAnchorPoint(Vec2(0, 0));
	highScoreLabel->setColor(Color3B::RED);
	highScoreLabel->setPosition(m_visibleSize.width / 4 * 3 + 50, m_visibleSize.height - 100);
	this->addChild(highScoreLabel);

	LabelTTF* highScoreNumLabel = LabelTTF::create("30000", "fonts/arial.ttf", 40);
	highScoreNumLabel->setAnchorPoint(Vec2(1, 0));
	highScoreNumLabel->setPosition(m_visibleSize.width - 50, m_visibleSize.height - 170);
	this->addChild(highScoreNumLabel);

	LabelTTF* playerLabel = LabelTTF::create("1UP", "fonts/arial.ttf", 40);
	playerLabel->setAnchorPoint(Vec2(0, 0));
	playerLabel->setColor(Color3B::RED);
	playerLabel->setPosition(m_visibleSize.width / 4 * 3 + 50, m_visibleSize.height - 260);
	playerLabel->runAction(RepeatForever::create(Blink::create(1, 2)));
	this->addChild(playerLabel);

	m_scoreNumLabel = LabelTTF::create(String::createWithFormat("%d", m_score)->getCString(), "fonts/arial.ttf", 40);
	m_scoreNumLabel->setAnchorPoint(Vec2(1, 0));
	m_scoreNumLabel->setPosition(m_visibleSize.width - 50, m_visibleSize.height - 330);
	this->addChild(m_scoreNumLabel);

	for (size_t i = 0; i < m_life; i++)
	{
		m_hero[i] = Sprite::create("./GalagaRes/player.png");
		m_hero[i]->setAnchorPoint(Vec2(0, 0));
		m_hero[i]->setPosition(m_visibleSize.width / 4 * 3 + 30 + 70 * i, m_visibleSize.height / 2 - 50);
		m_hero[i]->setScale(2.0f);
		this->addChild(m_hero[i]);
	}

	Sprite* bomb = Sprite::create("./GalagaRes/grade1.png");
	bomb->setAnchorPoint(Vec2(0, 0));
	bomb->setPosition(m_visibleSize.width / 4 * 3 + 50, m_visibleSize.height / 2 - 250);
	bomb->setScale(2.0f);
	this->addChild(bomb);

	return true;
}

void ScoreBoard::DecreaseLife()
{
	m_life--;
	m_hero[m_life]->removeFromParent();
}

int ScoreBoard::GetLife()
{
	return m_life;
}

void ScoreBoard::IncreaseScore(int score)
{
	m_score += score;
	m_scoreNumLabel->setString(String::createWithFormat("%d", m_score)->getCString());
}

ScoreBoard::ScoreBoard()
{
}


ScoreBoard::~ScoreBoard()
{
}
