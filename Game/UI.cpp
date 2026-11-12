#include "stdafx.h"
#include "UI.h"
#include "Player.h"
#include "Enemy.h"
#include "FaseTime.h"
#include "Game.h"

UI::UI()
{
	//ここからエフェクト
	EffectEngine::GetInstance()->ResistEffect(0, u"Assets/efecct/Damege_note.efkefc");
	EffectEngine::GetInstance()->ResistEffect(1,u"Assets/efecct/StageRimit.efkefc");
	HitEfeccts.Init(0);
	HitEfeccts.SetScale(Vector3(30.0f, 30.0f, 0.0f));
	RimitEffects.Init(1);

	InGameUI[1].SetPosition({ -960.0f,0.0f,0.0f });
	InGameUI[2].SetPosition({ 960.0f,0.0f,0.0f });
	InGameUI[1].SetPivot(Vector2(0.0f, 0.5f));
	InGameUI[2].SetPivot(Vector2(1.0f, 0.5f));

	position.y = 1000.0f;
	ChangePhaseUI[0].SetPosition(position);
	ChangePhaseUI[1].SetPosition(position);

	CardPos[0] = { -600.0f,1000.0f,0.0f };
	CardPos[1] = { 0.0f,1000.0f,0.0f };
	CardPos[2] = { 600.0f,1000.0f,0.0f };
}

UI::~UI()
{

}

void UI::InitUI()
{
	InGameUI[0].Init("Assets/sprite/mainUI.DDS", 1920.0f, 1080.0f);
	InGameUI[1].Init("Assets/sprite/playerHP.DDS", 1920.0f, 1080.0f);
	InGameUI[2].Init("Assets/sprite/enemyHP.DDS", 1920.0f, 1080.0f);
	InGameUI[3].Init("Assets/sprite/gauge_back.DDS", 1920.0f, 1080.0f);
	InGameUI[4].Init("Assets/sprite/syouzyun.DDS", 1920.0f, 1080.0f);

	noteUI[0].Init("Assets/sprite/quoter.DDS", 1920.0f, 1080.0f);
	noteUI[1].Init("Assets/sprite/duplet.DDS", 1920.0f, 1080.0f);
	noteUI[2].Init("Assets/sprite/eighth.DDS", 1920.0f, 1080.0f);
	noteUI[4].Init("Assets/sprite/harf.DDS", 1920.0f, 1080.0f);
	noteUI[3].Init("Assets/sprite/quoter2.DDS", 1920.0f, 1080.0f);

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

	for (int i = 0;i < 3;i++)
	{
		CardKind[i][0].Init("Assets/sprite/Card1.DDS", 512.0f, 768.0f);
		CardKind[i][1].Init("Assets/sprite/Card3.DDS", 512.0f, 768.0f);
		CardKind[i][2].Init("Assets/sprite/Card2.DDS", 512.0f, 768.0f);
		CardKind[i][3].Init("Assets/sprite/Card4.DDS", 512.0f, 768.0f);
		CardKind[i][4].Init("Assets/sprite/Card5.DDS", 512.0f, 768.0f);
		CardKind[i][5].Init("Assets/sprite/CardEX.DDS", 512.0f, 768.0f);
	}

	ChooseTime[0].Init("Assets/sprite/chooseBack.DDS", 1920.0f, 1080.0f);
	ChooseTime[1].Init("Assets/sprite/fream.DDS", 1920.0f, 1080.0f);

	GameOberUI[0].Init("Assets/sprite/black.DDS", 1920.0f, 1080.0f);
	GameOberUI[1].Init("Assets/sprite/GameOber.DDS", 1920.0f, 1080.0f);
	GameOberUI[2].Init("Assets/sprite/GameOber_Car .DDS", 1920.0f, 1080.0f);

	PlayerModeUI[0].Init("Assets/sprite/playerMode.DDS", 1920.0f, 1080.0f);
	PlayerModeUI[1].Init("Assets/sprite/attack.DDS", 1920.0f, 1080.0f);
	PlayerModeUI[2].Init("Assets/sprite/inst.DDS", 1920.0f, 1080.0f);
}

void UI::Update()
{
	faseTime = FindGO<FaseTime>("faseTime");
	player = FindGO<Player>("player");

	if (player == nullptr)
	{
		return;
	}

	//HPバーを動かす
	GageUpdate();

	if (IsChange == true)
	{
		ChangeAction();
	}

	if (faseChanged == true)
	{
		CardLottery();
	}

	HitEfeccts.Update();
	InGameUI[1].Update();
	InGameUI[2].Update();
	ChangePhaseUI[0].Update();
	ChangePhaseUI[1].Update();
}

void UI::GageUpdate()
{
	Enemy* enemy = FindGO<Enemy>("enemy");

	if (player == nullptr or enemy == nullptr)
	{
		return;
	}

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

void UI::PlayEffects(int effectNumber, Vector3 position, int rotate)
{
	switch (effectNumber)
	{
	case 1:
		HitEfeccts.SetPosition(position);
		HitEfeccts.Play();
		break;
	case 2:
		RimitEffects.SetPosition(position);
		if (rotate == 1)
		{
			EfectRotate.AddRotationDegX(90.0f);
		}
		if (rotate == 2)
		{
			EfectRotate.AddRotationDegY(90.0f);
		}
		RimitEffects.SetRotation(EfectRotate);
		RimitEffects.Play();
	default:
		break;
	}
}

void UI::ChangePhase()
{
	IsChange = true;
}

void UI::ChangeAction()
{
	if (faseChanged == false)
	{
		if (position.y >= endPosition)
		{
			easingRadio = startPosition - position.y / 100;
			if (easingRadio > 0.0f)
			{
				position.y -= 0.05f * easingRadio;
			}
			else
			{
				easingRadio = 1.0f;
			}
		}

		ChangePhaseUI[PhaseIndex].SetPosition(position);

		if (position.y <= endPosition)
		{
			stopTime -= g_gameTime->GetFrameDeltaTime();

			if (stopTime <= 0.0f)
			{
				easingRadio = startPosition - position.y / 100;
				if (easingRadio > 0.0f)
				{
					position.y -= 0.05f * easingRadio;
				}
				else
				{
					easingRadio = 1.0f;
				}

				if (position.y <= -1000.0f)
				{
					position.y = 1000.0f;
					ChangePhaseUI[PhaseIndex].SetPosition(position);
					stopTime = 1.5f;
					faseChanged = true;
				}
			}
		}
	}
}

void UI::CardLottery()
{
	//アタックフェーズになったんならカード抽選イベント
	if (faseTime->nowFase == Player::PlayerMode::attack)
	{
		//呼び出されたらカードが順番に上から降りてくる
		switch (State)
		{
		case move:
			//一枚目が降りてくる
			CardPos[0].y -= 2500.0f * g_gameTime->GetFrameDeltaTime();
			//二枚目が降りてくる
			if (CardPos[0].y <= 100.0f)
			{
				CardPos[1].y -= 2500.0f * g_gameTime->GetFrameDeltaTime();
				//三枚目が降りてくる
				if (CardPos[1].y <= 100.0f)
				{
					CardPos[2].y -= 2500.0f * g_gameTime->GetFrameDeltaTime();
				}
			}
			for (int i = 0;i < 3;i++)
			{
				//中央で止める
				if (CardPos[i].y <= 0.0f)
				{
					CardPos[i].y = 0.0f;
					StopCard[i] = true;
				}

				CardKind[i][5].SetPosition(CardPos[i]);
				CardKind[i][5].Update();
			}

			//全部0.0f地点に到達したら
			if (StopCard[0] == true && StopCard[1] == true && StopCard[2] == true)
			{
				State = stop;
			}
			break;
		case stop:
			//回転させていって

			if (Reset == false)
			{
				//ここで効果音再生
				game->IsPlaySound(10);
				Reset = true;
			}

			rot += 100.0f * g_gameTime->GetFrameDeltaTime();
			if (rot >= 30.0f && rot <= 150.0f)
			{
				rot += 9000.0f * g_gameTime->GetFrameDeltaTime();
				if (rot >= 50.0f && rot <= 90.0f)
				{
					rot += 5.0f;
				}
			}
			CardRot.SetRotationDegY(rot);
			//90度より大きくなったら
			if (rot >= 90.0f)
			{
				//絵柄を変えてやる
				for (int i = 0;i < 3;i++)
				{
					CardKind[i][faseTime->nowNoteIndex[i]].SetPosition(CardPos[i]);
					CardKind[i][faseTime->nowNoteIndex[i]].SetRotation(CardRot);
					CardKind[i][faseTime->nowNoteIndex[i]].Update();
				}
			}
			else
			{
				for (int i = 0;i < 3;i++)
				{
					CardKind[i][5].SetRotation(CardRot);
					CardKind[i][5].Update();
				}
			}

			//そして180度回転したら次へ
			if (rot >= 180.0f)
			{
				rot = 0.0f;
				CardRot.SetRotationDegY(rot);
				for (int i = 0;i < 3;i++)
				{
					CardKind[i][faseTime->nowNoteIndex[i]].SetPosition(CardPos[i]);
					CardKind[i][faseTime->nowNoteIndex[i]].SetRotation(CardRot);
					CardKind[i][faseTime->nowNoteIndex[i]].Update();
				}
				ChooseTime[1].SetPosition({ -600.0f,0.0f,0.0f });
				Reset = false;
				State = choose;
			}
			break;
		case choose:
			//カードを選択する
			if (g_pad[0]->IsTrigger(enButtonLeft))
			{
				cardCoose -= 1;
				//ここで効果音再生
				game->IsPlaySound(11);
			}
			if (g_pad[0]->IsTrigger(enButtonRight))
			{
				cardCoose += 1;
				//ここで効果音再生
				game->IsPlaySound(11);
			}
			if (cardCoose < 0)
			{
				cardCoose = 0;
			}
			if (cardCoose > 2)
			{
				cardCoose = 2;
			}
			switch (cardCoose)
			{
			case 0:
				ChooseTime[1].SetPosition({ -600.0f,0.0f,0.0f });
				break;
			case 1:
				ChooseTime[1].SetPosition({ 0.0f,0.0f,0.0f });
				break;
			case 2:
				ChooseTime[1].SetPosition({ 600.0f,0.0f,0.0f });
				break;
			}

			for (int i = 0;i < 3;i++)
			{
				CardKind[i][faseTime->nowNoteIndex[i]].SetPosition(CardPos[i]);
				CardKind[i][faseTime->nowNoteIndex[i]].Update();
			}
			ChooseTime[1].Update();

			if (g_pad[0]->IsTrigger(enButtonA))
			{
				//ここで効果音再生
				game->IsPlaySound(12);

				Player* player = FindGO<Player>("player");
				player->player_noteIndex = faseTime->nowNoteIndex[cardCoose];

				State = fadeOut;
			}

			break;
		case fadeOut:
			faseChanged = false;
			IsChange = false;
			cardCoose = 0;
			State = move;
			break;
		}
	}
	else
	{
		faseChanged = false;
		IsChange = false;
	}
}

void UI::GameOber(RenderContext& rc)
{
	GameOberUI[0].Draw(rc);

	coloer += 0.005f;
	GameOberUI[1].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, coloer));
	GameOberUI[2].SetMulColor(Vector4(1.0f, 1.0f, 1.0f, coloer));

	if (coloer >= 1.0f)
	{
		if (g_pad[0]->IsTrigger(enButtonUp))
		{
			carPos.y += 60.0f;
		}
		if (g_pad[0]->IsTrigger(enButtonDown))
		{
			carPos.y -= 60.0f;
		}

		if (carPos.y >= 0.0f)
		{
			carPos.y = 0.0f;
		}
		if (carPos.y <= -60.0f)
		{
			carPos.y = -60.0f;
		}

		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//コンティニューの場合
			if (carPos.y == 0.0f)
			{
				game = FindGO<Game>("game");
				game->ReSet();
			}
		}
	}

	GameOberUI[2].SetPosition(carPos);

	GameOberUI[1].Update();
	GameOberUI[2].Update();

	GameOberUI[0].Draw(rc);
	GameOberUI[1].Draw(rc);
	GameOberUI[2].Draw(rc);
}

void UI::Render(RenderContext& rc)
{
	InGameUI[3].Draw(rc);
	InGameUI[2].Draw(rc);
	InGameUI[1].Draw(rc);
	InGameUI[0].Draw(rc);

	//今のフェーズがわかりやすいようなフィルターをかけてみる
	if (faseTime->nowFase == Player::PlayerMode::attack)
	{
		PhaseUI[0].Draw(rc);
		InGameUI[4].Draw(rc);
	}
	else
	{
		PhaseUI[1].Draw(rc);
	}

	//モード。
	PlayerModeUI[0].Draw(rc);
	if (player->playerMode == Player::PlayerMode::attack)
	{
		PlayerModeUI[1].Draw(rc);
	}
	else
	{
		PlayerModeUI[2].Draw(rc);
	}

	//数字の更新
	FontUI(rc);

	//現在の音符の種類
	noteUI[player->player_noteIndex].Draw(rc);

	//フェーズ切り替え演出
	if (IsChange == true)
	{
		ChangePhaseUI[PhaseIndex].Draw(rc);
	}

	if (faseChanged == true)
	{
		ChooseTime[0].Draw(rc);

		if (State == move)
		{
			CardKind[0][5].Draw(rc);
			CardKind[1][5].Draw(rc);
			CardKind[2][5].Draw(rc);
		}
		if (State == stop)
		{
			if (rot >= 90.0f)
			{
				CardKind[0][faseTime->nowNoteIndex[0]].Draw(rc);
				CardKind[1][faseTime->nowNoteIndex[1]].Draw(rc);
				CardKind[2][faseTime->nowNoteIndex[2]].Draw(rc);
			}
			else
			{
				CardKind[0][5].Draw(rc);
				CardKind[1][5].Draw(rc);
				CardKind[2][5].Draw(rc);
			}
		}
		if (State == choose)
		{
			CardKind[0][faseTime->nowNoteIndex[0]].Draw(rc);
			CardKind[1][faseTime->nowNoteIndex[1]].Draw(rc);
			CardKind[2][faseTime->nowNoteIndex[2]].Draw(rc);

			ChooseTime[1].Draw(rc);
		}
	}
}

void UI::Init()
{
	IsChange = false;
	faseChanged = false;
	coloer = 0.0f;
}