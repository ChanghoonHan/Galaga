#ifndef __BEZIERCURVE_SCENE_H__
#define __BEZIERCURVE_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class BezierCurveScene : public cocos2d::Scene
{
	enum BezierCurveSceneState
	{
		edit,
		play,
	};

	struct BezierPoint
	{
		Vec2 point;
		Sprite* sprite;
		Label* pointLabel;
	};
	
	int bezierCount = 0;
	int xOffset;
	BezierCurveSceneState m_curState;
	std::vector<BezierPoint> m_bezierPointSpriteList;
	std::vector<ccBezierConfig> m_bezierConfigList;
	Label* m_stateLabel;
	Sprite* m_enemy;

	Vector<FiniteTimeAction*> m_bezierToVec;
	Sequence* m_actionSequence;

	void InitebzierPointList();

public:
	static cocos2d::Scene* createScene();

	virtual bool init();
	virtual void update(float deltaTime);
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
	virtual void onMouseDown(Event *event);

	// implement the "static create()" method manually
	CREATE_FUNC(BezierCurveScene);
};

#endif // __BEZIERCURVE_SCENE_H__
