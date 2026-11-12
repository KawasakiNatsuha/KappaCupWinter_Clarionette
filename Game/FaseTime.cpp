#include "stdafx.h"
#include "FaseTime.h"
#include "Player.h"
#include "UI.h"
#include "Game.h"

FaseTime::FaseTime() :gen(std::random_device{}()), notemode(0, 4)
{
	
}

FaseTime::~FaseTime()
{

}

void FaseTime::Update()
{
	//Playerを見る
	player = FindGO<Player>("player");

	game = FindGO<Game>("game");

	if (game->isInGame == false)
	{
		return;
	}

	if (player->isWait == true)
	{
		return;
	}

	//UIを見る
	ui = FindGO<UI>("ui");

	if (ui->IsChange == false)
	{
		//時間経過
		deltaTime += gameTime.GetFrameDeltaTime();
		if (deltaTime >= 1.0f)
		{
			faseTimer -= 1;
			deltaTime -= 1.0f;

			if (faseTimer <= 3)
			{
				//ここで効果音再生
				game->IsPlaySound(9);
			}
		}
	}

	//タイマー0でフェーズ切り替え
	//攻撃フェーズ→回避フェーズ
	if (nowFase == attackFase && faseTimer <= 0)
	{
		ui->PhaseIndex = 1;
		ui->ChangePhase();

		faseTimer = firstTime;
		//回避フェーズに切り替わるとき、攻撃モードでないなら初期状態(攻撃モード)に戻す
		if (player->playerMode != Player::PlayerMode::attack)
		{
			player->PlayerModeChange(Player::PlayerMode::installation);
		}
		nowFase = evadeFase;

		//ここで効果音再生
		game->IsPlaySound(7);
	}
	//回避フェーズ→攻撃フェーズ
	else if (nowFase == evadeFase && faseTimer <= 0)
	{
		ui->PhaseIndex = 0;
		ui->ChangePhase();

		faseTimer = firstTime;
		//攻撃フェーズに戻るとき、ターン+1
		TurnCount += 1;
		nowFase = attackFase;

		//ここで効果音再生
		game->IsPlaySound(8);

		while (true)
		{
			//ここで音符の抽選
			nowNoteIndex[0] = notemode(gen);
			nowNoteIndex[1] = notemode(gen);

			if (nowNoteIndex[0] != nowNoteIndex[1])
			{
				nowNoteIndex[2] = notemode(gen);

				//全部違う数値にできたらループ脱出
				if (nowNoteIndex[0] != nowNoteIndex[2] && nowNoteIndex[1] != nowNoteIndex[2])
				{
					break;
				}
			}
		}
	}
}

void FaseTime::Render(RenderContext& rc)
{
	timeFont.Draw(rc);
}

void FaseTime::Init()
{
	nowFase = attackFase;
	TurnCount = 0;
	faseTimer = 15;
}