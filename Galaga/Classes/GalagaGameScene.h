#ifndef __GALAGAGAME_SCENE_H__
#define __GALAGAGAME_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class HeroGroup;
class Enemy;
class Bullet;
class EnemyGroup;
class BossEnemy;
class CatchRay;
class CatchedHero;
class Hero;
class ScoreBoard;

class GalagaGame : public cocos2d::Scene
{
	enum GAMESTATE
	{
		START,
		READY,
		GAME,
		CLEAR,
		OVER,
	};

	GAMESTATE m_gameState;
	std::vector<Bullet*> m_vecBullets;
	int m_dFontSize;
	int m_dHeightOffset;
	int m_dWidthOffset;
	int m_startCount;
	float m_labelChageDeltaTime;
	bool m_isHeroReleased;
	bool m_isHeroComeBack;
	LabelTTF* m_stageInfoLabel;
	CatchRay* m_catchRay;
	HeroGroup* m_heroGroup;
	CatchedHero* m_catchedHero;
	EnemyGroup* m_enemyGroup;
	ScoreBoard* m_scoreBorad;
	Size m_visibleSize;

	void InitStage();
	void UpdateObj(float deltaTime);
	void ChangeGameState(GAMESTATE gameState);
	void CheckCollision();

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float deltaTime);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	void RemovepHeroFromBullet(Hero* pHero);
	void FireBullet(Vec2 fireStartPos, Vec2 fireDesPos, bool isHero = false, Hero* pHero = NULL);
	Vec2 GetHeroPos();
	CatchRay* UseCatchRay(BossEnemy* bossEnemy);

	// implement the "static create()" method manually
	CREATE_FUNC(GalagaGame);
};

#endif // __GALAGAGAME_SCENE_H__
