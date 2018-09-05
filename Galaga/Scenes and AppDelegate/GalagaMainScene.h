#ifndef __GALAGAMAIN_SCENE_H__
#define __GALAGAMAIN_SCENE_H__

#include "cocos2d.h"

USING_NS_CC;

class CatchRay;

class GalagaMain : public cocos2d::Scene
{
	int m_dFontSize;
	int m_dHeightOffset;
	int m_dWidthOffset;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(GalagaMain);
};

#endif // __GALAGAMAIN_SCENE_H__
