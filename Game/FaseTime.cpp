#include "stdafx.h"
#include "FaseTime.h"
#include "Player.h"

FaseTime::FaseTime()
{

}

FaseTime::~FaseTime()
{

}

void FaseTime::Update()
{
	//Playerを見る
	player = FindGO<Player>("player");

	//時間経過
	deltaTime += gameTime.GetFrameDeltaTime();
	if (deltaTime >= 1.0f)
	{
		faseTimer -= 1;
		deltaTime -= 1.0f;
	}

	//タイマー0でフェーズ切り替え
	if (nowFase == attackFase && faseTimer <= 0)
	{
		faseTimer = firstTime;
		//回避フェーズに切り替わるとき、攻撃モードでないなら初期状態(攻撃モード)に戻す
		if (player->playerMode != Player::PlayerMode::attack)
		{
			player->PlayerModeChange(Player::PlayerMode::installation);
		}
		nowFase = evadeFase;
	}
	if (nowFase == evadeFase && faseTimer <= 0)
	{
		faseTimer = firstTime;
		//攻撃フェーズに戻るとき、ターン+1
		TurnCount += 1;
		nowFase = attackFase;
	}

	//memo::残り時間表示
	swprintf_s(font, L"残り時間: %d", faseTimer);
	timeFont.SetText(font);
	timeFont.SetPosition({ 400.0f,500.0f,0.0f });
	timeFont.SetColor(g_vec4White);
}

void FaseTime::Render(RenderContext& rc)
{
	timeFont.Draw(rc);
}