#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "DebugStage.h"
#include "Enemy.h"
#include "PlatformManeger.h"
#include "FaseTime.h"
#include "Platform.h"
#include "UI.h"

bool Game::Start()
{	
	//memo::まずは最初のテストプレイ用
	NewGO<Player>(0, "player");
	NewGO<GameCamera>(0, "gamecamera");
	//NewGO<DebugStage>(0, "debugstage");
	Enemy* enemy = NewGO<Enemy>(0, "enemy");
	NewGO<FaseTime>(0, "faseTime");
	UI* ui = NewGO<UI>(0, "ui");

	//テスト用初期化
	enemy->Init(100, 1, 0.1f, 100.0f,1.0f);
	ui->InitUI();

	//初期足場
	platform = NewGO<Platform>(0, "platform");
	platform->noteIndex = 2;
	platform->InitPlatform();

	//スカイキューブ
	SkyCube* skyCube = NewGO<SkyCube>(0, "skycube");
	skyCube->SetType(enSkyCubeType_NightToon_2);
	skyCube->SetScale(5000.0f);
	g_renderingEngine->SetAmbientByIBLTexture(skyCube->GetTextureFilePath(), 0.5f);

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

void Game::Blinking(float interval, ModelRender& model, RenderContext& rc)
{
	deltaTime += gameTime.GetFrameDeltaTime();
	if (deltaTime >= interval)
	{
		isDrow = !isDrow;
		deltaTime -= interval;
	}

	if (isDrow == true)
	{
		model.Draw(rc);
	}
}