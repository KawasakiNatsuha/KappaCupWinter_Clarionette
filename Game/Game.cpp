#include "stdafx.h"
#include "Game.h"
#include "Player.h"
#include "GameCamera.h"
#include "DebugStage.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "PlatformManeger.h"
#include "FaseTime.h"
#include "Platform.h"
#include "UI.h"
#include "Bullet.h"

bool Game::Start()
{	
	//memo::まずは最初のテストプレイ用
	ui = NewGO<UI>(0, "ui");
	player = NewGO<Player>(0, "player");
	gameCamera = NewGO<GameCamera>(0, "gamecamera");
	debugStage = NewGO<DebugStage>(0, "debugstage");
	enemy = NewGO<Enemy>(0, "enemy");
	faseTime = NewGO<FaseTime>(0, "faseTime");

	//テスト用初期化
	enemy->Init(100, 10, 0.1f, 100.0f,1.0f);
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

	return true;
}

void Game::Update()
{
	/*modelRender.PlayAnimation(enAnimationClip_Idle);
	modelRender.Update();*/
}

void Game::Render(RenderContext& rc)
{
	/*modelRender.Draw(rc);*/
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

//初期化・リセット
void Game::ReSet()
{
	player->Init();
	for (auto platforms : FindGOs<Platform>("platform"))
	{
		platforms->Init();
	}
	if (player->platformManeger != nullptr)
	{
		DeleteGO(player->platformManeger);
	}
	faseTime->Init();
	for (auto enemyBullets : FindGOs<EnemyBullet>("enemyBullet"))
	{
		enemyBullets->Init();
	}
	//memo::これテスト用の初期化、ステージに応じて数値を変えること
	enemy->Init(100, 10, 0.1f, 100.0f, 1.0f);
	for (auto bullets : FindGOs<Bullet>("bullet"))
	{
		DeleteGO(bullets);
	}

	//初期足場
	platform = NewGO<Platform>(0, "platform");
	platform->noteIndex = 2;
	platform->InitPlatform();
}