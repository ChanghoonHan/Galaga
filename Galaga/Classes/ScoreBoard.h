#pragma once
#include "cocos2d.h"

USING_NS_CC;

class ScoreBoard : public Layer
{
	Size m_visibleSize;
	int m_highScore;
	int m_score;
	int m_life;
	Sprite* m_hero[3];
	LabelTTF* m_scoreNumLabel;
	
public:
	ScoreBoard();
	~ScoreBoard();

	void DecreaseLife();
	int GetLife();
	void IncreaseScore(int score);
	virtual bool init();

	CREATE_FUNC(ScoreBoard);
};

