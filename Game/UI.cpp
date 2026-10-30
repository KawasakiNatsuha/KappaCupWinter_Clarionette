#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
#include "FaseTime.h"

UI::UI()
{
	//ここからエフェクト
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efecct/Damege_note.efkefc");
	HitEfeccts.Init(0);
	HitEfeccts.SetScale(Vector3(30.0f, 30.0f, 0.0f));

	InGameUI[1].SetPosition({ -960.0f,0.0f,0.0f });
	InGameUI[2].SetPosition({ 960.0f,0.0f,0.0f });
	InGameUI[1].SetPivot(Vector2(0.0f, 0.5f));
	InGameUI[2].SetPivot(Vector2(1.0f, 0.5f));
}

UI::~UI()
{

}

void UI::InitUI()
{
	InGameUI[0].Init("Assets/sprite/mainUI.DDS", 1920.0f, 1080.0f);
	InGameUI[1].Init("Assets/sprite/playerHP.DDS", 1920.0f, 1080.0f);
	InGameUI[2].Init("Assets/sprite/enemyHP.DDS", 1920.0f, 1080.0f);

	ChangePhaseUI[0].Init("Assets/sprite/change_player.DDS", 1920.0f, 1080.0f);
	ChangePhaseUI[1].Init("Assets/sprite/change_enemy.DDS", 1920.0f, 1080.0f);

	PhaseUI[0].Init("Assets/sprite/playerPhase.DDS", 1920.0f, 1080.0f);
	PhaseUI[1].Init("Assets/sprite/enemyPhase.DDS", 1920.0f, 1080.0f);

	NumberFont[0].Init("Assets/sprite/0.DDS", 74.0f, 91.0f);
	NumberFont[1].Init("Assets/sprite/1.DDS", 74.0f, 91.0f);
	NumberFont[2].Init("Assets/sprite/2.DDS", 74.0f, 91.0f);
	NumberFont[3].Init("Assets/sprite/3.DDS", 74.0f, 91.0f);
	NumberFont[4].Init("Assets/sprite/4.DDS", 74.0f, 91.0f);
	NumberFont[5].Init("Assets/sprite/5.DDS", 74.0f, 91.0f);
	NumberFont[6].Init("Assets/sprite/6.DDS", 74.0f, 91.0f);
	NumberFont[7].Init("Assets/sprite/7.DDS", 74.0f, 91.0f);
	NumberFont[8].Init("Assets/sprite/8.DDS", 74.0f, 91.0f);
	NumberFont[9].Init("Assets/sprite/9.DDS", 74.0f, 91.0f);
	NumberFont[10].Init("Assets/sprite/1.DDS", 74.0f, 91.0f);
}

void UI::Update()
{
	faseTime = FindGO<FaseTime>("faseTime");

	//HPバーを動かす
	GageUpdate();

	HitEfeccts.Update();
	InGameUI[1].Update();
	InGameUI[2].Update();
}

void UI::GageUpdate()
{
	Player* player = FindGO<Player>("player");
	Enemy* enemy = FindGO<Enemy>("enemy");

	int player_maxHP = player->MaxHP;
	int player_hp = player->HP;
	int enemy_maxHP = enemy->EnemyMaxHP;
	int enemy_hp = enemy->EnemyHP;

	float playerRadio = (float)player_hp / (float)player_maxHP;
	float enemyRedio = (float)enemy_hp / (float)enemy_maxHP;
	Vector3 playerScal = { 1.0f,1.0f,1.0f };
	Vector3 enemyScal = playerScal;

	if (playerRadio > 1.0f) playerRadio = 1.0f;
	if (enemyRedio > 1.0f) enemyRedio = 1.0f;
	if (playerRadio < 0.0f) playerRadio = 0.0f;
	if (enemyRedio < 0.0f) enemyRedio = 0.0f;

	playerScal.x *= playerRadio;
	enemyScal.x *= enemyRedio;

	InGameUI[1].SetScale(playerScal);
	InGameUI[2].SetScale(enemyScal);

	//位置を補正する
	float offset_p = 600.0f * (1.0f - playerRadio) * 0.5f;
	float offset_e = 650.0f * (1.0f - enemyRedio) * 0.5f;

	InGameUI[1].SetPosition({ -960.0f + offset_p,0.0f,0.0f });
	InGameUI[2].SetPosition({ 960.0f - offset_e,0.0f,0.0f });
}

void UI::FontUI(RenderContext& rc)
{
	int ten = (faseTime->faseTimer / 10) % 10;
	int one = faseTime->faseTimer % 10;

	if (faseTime->faseTimer == 11)
	{
		ten = 1;
		one = 10;
	}

	NumberFont[ten].SetPosition({ 750.0f,390.0f,0.0f });
	NumberFont[one].SetPosition({ 800.0f,390.0f,0.0f });

	NumberFont[ten].Update();
	NumberFont[one].Update();

	NumberFont[ten].Draw(rc);
	NumberFont[one].Draw(rc);
}

void UI::PlayEffects(int effectNumber, Vector3 position)
{
	switch (effectNumber)
	{
	case 1:
		HitEfeccts.SetPosition(position);
		HitEfeccts.Play();
		break;
	default:
		break;
	}
}

void UI::ChangePhase()
{
	IsChange = true;
}

void UI::Render(RenderContext& rc)
{
	InGameUI[2].Draw(rc);
	InGameUI[1].Draw(rc);
	InGameUI[0].Draw(rc);

	//今のフェーズがわかりやすいようなフィルターをかけてみる
	if (faseTime->nowFase == Player::PlayerMode::attack)
	{
		PhaseUI[0].Draw(rc);
		if (IsChange == true)
		{
			ChangePhaseUI[1].Draw(rc);
		}
	}
	else
	{
		PhaseUI[1].Draw(rc);
		if (IsChange == true)
		{
			ChangePhaseUI[0].Draw(rc);
		}

		//備忘録::次はフェーズ切り替え時の演出を作るよ
	}

	//数字の更新
	FontUI(rc);
}