#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "DebugStage.h"
#include "Enemy.h"
#include"PlatformManeger.h"

bool Game::Start()
{	
	//memo::まずは最初のテストプレイ用
	NewGO<Player>(0, "player");
	NewGO<GameCamera>(0, "gamecamera");
	NewGO<DebugStage>(0, "debugstage");
	NewGO<Enemy>(0, "enemy");
	//NewGO<PlatformManeger>(0, "platformManeger");

	//modelRender.Init("Assets/modelData/Sop.tkm");

	return true;
}

void Game::Update()
{

}

void Game::Render(RenderContext& rc)
{
	//modelRender.Draw(rc);
}