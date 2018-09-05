#include "GalagaGameScene.h"
#include "SimpleAudioEngine.h"
#include "ScoreBoard.h"
#include "HeroGroup.h"
#include "BlueEnemy.h"
#include "RedEnemy.h"
#include "BossEnemy.h"
#include "EnemyGroup.h"
#include "Bullet.h"
#include "CatchRay.h"
#include "CatchedHero.h"
#include "Hero.h"

Scene* GalagaGame::createScene()
{
	return GalagaGame::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool GalagaGame::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	m_isHeroComeBack = true;
	m_dFontSize = 40;
	m_dHeightOffset = 50;
	m_dWidthOffset = 100;
	m_labelChageDeltaTime = 0;
	m_startCount = 0;
	m_isHeroReleased = false;
	m_gameState = GAMESTATE::START;
	m_catchedHero = NULL;

	m_visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	m_scoreBorad = ScoreBoard::create();
	this->addChild(m_scoreBorad, 10);

	m_stageInfoLabel = LabelTTF::create("START", "fonts/arial.ttf", 40);
	m_stageInfoLabel->setColor(Color3B::RED);
	m_stageInfoLabel->setPosition(Vec2(m_visibleSize.width/4 * 1.5, m_visibleSize.height/2));
	this->addChild(m_stageInfoLabel);

	scheduleUpdate();

	return true;
}

void GalagaGame::update(float deltaTime)
{
	switch (m_gameState)
	{
	case GAMESTATE::START:
		m_labelChageDeltaTime += deltaTime;
		if (m_labelChageDeltaTime > 1.5f)
		{
			switch (m_startCount)
			{
			case 0:
				m_stageInfoLabel->setColor(Color3B::BLUE);
				m_stageInfoLabel->setString("Stage 1");
				break;
			case 1:
				m_stageInfoLabel->setColor(Color3B::RED);
				m_stageInfoLabel->setString("READY");
				break;
			case 2:
				m_stageInfoLabel->setVisible(false);
				m_labelChageDeltaTime = 0;
				InitStage();
				break;
			default:
				break;
			}
			m_labelChageDeltaTime = 0;
			m_startCount++;
		}
		break;

	case GAMESTATE::READY:
		if (m_enemyGroup->isGameClear())
		{
			ChangeGameState(GAMESTATE::CLEAR);
			return;
		}

		m_heroGroup->update(deltaTime, false);
		m_enemyGroup->update(deltaTime, false);
		UpdateObj(deltaTime);

		if (m_enemyGroup->CheckAllEnemysStateIsIdle())
		{
			if (m_catchedHero != NULL && m_catchedHero->GetCatchedHeroState() == CH_STATE::CH_RELEASE)
			{
				m_catchedHero->SetGoToHero();
			}

			if (m_isHeroComeBack)
			{
				
				m_stageInfoLabel->setString("READY");
				m_stageInfoLabel->setVisible(true);
				m_labelChageDeltaTime += deltaTime;

				if (m_isHeroReleased)
				{
					if (m_labelChageDeltaTime > 2.0f)
					{
						m_isHeroReleased = false;
						ChangeGameState(GAMESTATE::GAME);
					}
				}
				else
				{
					if (m_labelChageDeltaTime > 2.0f)
					{
						m_heroGroup->CreateHero();
						m_scoreBorad->DecreaseLife();
						ChangeGameState(GAMESTATE::GAME);
					}
				}
			}
		}
		break;

	case GAMESTATE::GAME:
		if (m_enemyGroup->isGameClear())
		{
			ChangeGameState(GAMESTATE::CLEAR);
			return;
		}

		m_heroGroup->update(deltaTime);
		m_enemyGroup->update(deltaTime);

		UpdateObj(deltaTime);
		CheckCollision();
		break;

	case GAMESTATE::CLEAR:
		m_heroGroup->update(deltaTime);
		m_enemyGroup->update(deltaTime, false);
		UpdateObj(deltaTime);

		m_labelChageDeltaTime += deltaTime;
		if (m_labelChageDeltaTime > 3)
		{
			Director::getInstance()->popScene();
		}
		break;

	case GAMESTATE::OVER:
		m_heroGroup->update(deltaTime, false);
		m_enemyGroup->update(deltaTime, false);
		UpdateObj(deltaTime);

		if (m_enemyGroup->CheckAllEnemysStateIsIdle())
		{
			m_stageInfoLabel->setVisible(true);
			m_labelChageDeltaTime += deltaTime;
			if (m_labelChageDeltaTime > 3)
			{
				Director::getInstance()->popScene();
			}
		}
		break;


	default:
		break;
	}
}

void GalagaGame::UpdateObj(float deltaTime)
{
	m_catchRay->update(deltaTime);

	for (auto iter = m_vecBullets.begin(); iter != m_vecBullets.end();)
	{
		if ((*iter)->BulletUpdate(deltaTime))
		{
			this->removeChild((*iter));
			iter = m_vecBullets.erase(iter);
		}
		else
		{
			iter++;
		}
	}

	if (m_catchedHero != NULL)
	{
		if (m_catchedHero->GetCatchedHeroState() == CH_STATE::CH_RELEASE)
		{
			m_isHeroComeBack = false;
			ChangeGameState(GAMESTATE::READY);
			m_isHeroReleased = true;
		}

		if (m_catchedHero->GetCurEnemyState() == ENEMY_STATE::DEAD || m_catchedHero->GetCurEnemyState() == ENEMY_STATE::DESTROY)
		{
			if (m_catchedHero->GetCurEnemyState() == ENEMY_STATE::DESTROY)
			{
				if (m_catchedHero->GetCatchedHeroState() == CH_STATE::CH_GO_TO_HERO)
				{
					m_isHeroComeBack = true;
					m_heroGroup->CreateHero();
				}

				this->removeChild(m_catchedHero);
				m_catchedHero = NULL;
			}
		}
		else
		{
			m_catchedHero->update(deltaTime);
		}
	}
}

void GalagaGame::FireBullet(Vec2 fireStartPos, Vec2 fireDesPos, bool isHero, Hero* pHero)
{
	Bullet* newBullet = Bullet::create();
	newBullet->fire(fireStartPos, fireDesPos, isHero, pHero);
	this->addChild(newBullet);
	m_vecBullets.push_back(newBullet);
}

CatchRay* GalagaGame::UseCatchRay(BossEnemy* bossEnemy)
{
	m_catchRay->SetOnCatchRay(bossEnemy);

	return m_catchRay;
}

Vec2 GalagaGame::GetHeroPos()
{
	return m_heroGroup->getPosition();
}

void GalagaGame::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	m_heroGroup->onKeyPressed(keyCode, event);
}

void GalagaGame::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{	
	m_heroGroup->onKeyReleased(keyCode, event);
}

void GalagaGame::RemovepHeroFromBullet(Hero* pHero)
{
	for (auto iter = m_vecBullets.begin(); iter != m_vecBullets.end(); iter++)
	{
		if ((*iter)->GetIsHeroBullet())
		{
			(*iter)->HeroDestroyed(pHero);
		}
	}
}

void GalagaGame::InitStage()
{
	ChangeGameState(GAMESTATE::GAME);

	m_scoreBorad->DecreaseLife();

	m_heroGroup = HeroGroup::create();
	m_heroGroup->SetGameScene(this);
	m_heroGroup->setPositionX(m_visibleSize.width / 4 * 1.5f);
	this->addChild(m_heroGroup);
	m_heroGroup->CreateHero();

	m_catchRay = CatchRay::create();
	m_catchRay->setPosition(Vec2(-100, -100));
	this->addChild(m_catchRay);

	m_enemyGroup = EnemyGroup::create();
	m_enemyGroup->SetGameScene(this);
	this->addChild(m_enemyGroup);

	auto keyboardListner = EventListenerKeyboard::create();
	keyboardListner->onKeyPressed = CC_CALLBACK_2(GalagaGame::onKeyPressed, this);
	keyboardListner->onKeyReleased = CC_CALLBACK_2(GalagaGame::onKeyReleased, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListner, this);
}

void GalagaGame::CheckCollision()
{
	int score = 0;;

	for (auto iter = m_vecBullets.begin(); iter != m_vecBullets.end();)//미사일충돌체크
	{
		if (m_catchedHero != NULL && (*iter)->CheckColl(m_catchedHero->GetCollider()))
		{
			m_enemyGroup->SetIsCatchedHero(false);
			m_catchedHero->AttackedFromBullet();
			m_scoreBorad->IncreaseScore(m_catchedHero->GetScore());
		}

		if ((*iter)->GetIsHeroBullet())
		{
			if (m_enemyGroup->CheckCollEnemys(*iter, &score))
			{
				iter = m_vecBullets.erase(iter);
				m_scoreBorad->IncreaseScore(score);
			}
			else
			{
				iter++;
			}
		}
		else
		{
			if (m_heroGroup->CheckCollBulletWithHero((*iter)))
			{
				switch (m_heroGroup->GetHeroCount())
				{
				case 0:
					if (m_scoreBorad->GetLife() == 0)
					{
						ChangeGameState(GAMESTATE::OVER);
					}
					else
					{
						ChangeGameState(GAMESTATE::READY);
					}
					break;
				case 1:
					m_enemyGroup->SetIsCatchedHero(false);
					break;
				default:
					break;
				}

				iter = m_vecBullets.erase(iter);
			}
			else
			{
				iter++;
			}
		}
	}

	if (m_catchRay->GetCurRayState() == CATCH_RAY_STATE::RAY_STAY)
	{
		if (m_heroGroup->CheckCollRayWithHero(m_catchRay))
		{
			if (m_scoreBorad->GetLife() == 0)
			{
				ChangeGameState(GAMESTATE::OVER);
			}
			else
			{
				ChangeGameState(GAMESTATE::READY);
				m_stageInfoLabel->setString("FIGHTER CAPTURED");
				m_stageInfoLabel->setVisible(true);
			}

			m_isHeroReleased = false;
			m_enemyGroup->SetIsCatchedHero(true);
			m_catchedHero = CatchedHero::create();
			m_catchedHero->SetPosition(m_heroGroup->getPosition());
			m_catchedHero->SetGoToBossEnemy(m_catchRay->GetBossEnmey(), m_catchRay);
			m_catchRay->GetBossEnmey()->SetCatchedHero(m_catchedHero);
			this->addChild(m_catchedHero);
		}
	}

	std::map<int, Enemy*>& enemys = m_enemyGroup->GetEnmeys();

	for (auto enemy : enemys)
	{
		if (m_heroGroup->CheckCollWithEnemy(enemy.second))
		{
			switch (m_heroGroup->GetHeroCount())
			{
			case 0:
				if (m_scoreBorad->GetLife() == 0)
				{
					ChangeGameState(GAMESTATE::OVER);
				}
				else
				{
					ChangeGameState(GAMESTATE::READY);
				}
				break;
			case 1:
				m_enemyGroup->SetIsCatchedHero(false);
				break;
			default:
				break;
			}
		}
	}
}

void GalagaGame::ChangeGameState(GAMESTATE gameState)
{
	if (gameState == m_gameState)
	{
		return;
	}

	switch (gameState)
	{
	case GAMESTATE::START:
		break;
	case GAMESTATE::READY:
	{
		Vec2 centerPos = Vec2(m_visibleSize.width / 4 * 1.5f, 100);
		m_heroGroup->runAction(MoveTo::create(centerPos.distance(this->getPosition()) / 300, centerPos));
		m_labelChageDeltaTime = 0;
	}
		break;
	case GAMESTATE::GAME:
		m_stageInfoLabel->setVisible(false);
		break;
	case GAMESTATE::CLEAR:
		m_stageInfoLabel->setString("GAME CLEAR!");
		m_stageInfoLabel->setVisible(true);
		m_labelChageDeltaTime = 0;
		break;
	case GAMESTATE::OVER:
		m_stageInfoLabel->setString("GAME OVER!");
		m_labelChageDeltaTime = 0;
		break;
	default:
		break;
	}
	
	m_gameState = gameState;
}