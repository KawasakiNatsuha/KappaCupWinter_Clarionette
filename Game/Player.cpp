
//プレイヤーを担当するクラス。

#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"
#include "PlatformManeger.h"
#include "FaseTime.h"
#include "Enemy.h"
#include "UI.h"
#include "Game.h"
#include "Platform.h"

Player::Player()
{
	//アニメーションクリップをロードする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/Sop_idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/Sop_walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/Sop_jump2.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	animationClips[enAnimationClip_Run].Load("Assets/animData/Sop_run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	animationClips[enAnimationClip_Deth].Load("Assets/animData/Sop_deth.tka");
	animationClips[enAnimationClip_Deth].SetLoopFlag(false);
	//ユニティちゃんのモデルを読み込む。
	modelRender.Init("Assets/modelData/Sop4.tkm", animationClips, enAnimationClip_Num, enModelUpAxisZ);

	//キャラコンを初期化する。
	characterController.Init(25.0f, 75.0f, position);

	//オブジェクトプールの初期化
	m_bulletPool.Init(50, "bullet");

	//黒画像の読み込み
	fadeOUT.Init("Assets/sprite/black.DDS", 1920.0f, 1080.0f);

	player_noteIndex = 3;
}

Player::~Player()
{
	
}

void Player::Update()
{
	modelRender.Update();

	ui = FindGO<UI>("ui");
	game = FindGO<Game>("game");

	if (game->isInGame == false)
	{
		return;
	}

	if (ui->IsChange == true)
	{
		return;
	}

	//PlatformManegerがあるかどうか見る
	platformManeger = FindGO<PlatformManeger>("platformmaneger");
	//現在のフェーズを確認する
	faseTime = FindGO<FaseTime>("faseTime");

	//クールタイム明けに半透明モデルが無かったら作る
	if (coolTime <= 0.0f && platformManeger == nullptr && playerMode == installation)
	{
		platformManeger = NewGO<PlatformManeger>(0, "platformmaneger");
		//現在のモデルの形状を教える
		platformManeger->noteGorIndex = player_noteIndex;
		//引継ぎが終わったら初期化
		platformManeger->PlatformManegerInit();
	}
	
	if (isWait == false)
	{
		PlayerMove();
		PlayerRotate();
		ManagePlayerState();
	}
	PlayerAnimetion();

	//アタックフェーズのときの処理
	if (faseTime->nowFase == FaseTime::Fase::attackFase)
	{
		//攻撃ボタンで攻撃or設置
		if (g_pad[0]->IsPress(attackButton) && coolTime <= 0.0f)
		{
			if (playerMode == attack)
			{
				PlayerAttack();
			}
		}

		if (g_pad[0]->IsTrigger(platformButton) && coolTime <= 0.0f)
		{
			if (playerMode == installation)
			{
				platformManeger->PlatformInstallation();
				//ここで効果音再生
				game->IsPlaySound(4);
				coolTime = 1.0f;
			}
		}

		//クールタイム中
		if (coolTime > 0.0f)
		{
			float playertime = gametime.GetFrameDeltaTime();
			coolTime -= playertime;
		}

		//モード切替
		if (g_pad[0]->IsTrigger(switchButton))
		{
			if (playerMode == installation)
			{
				PlayerModeChange(attack);
			}
		}
		else if (g_pad[0]->IsTrigger(switchButton2))
		{
			if (playerMode == attack)
			{
				PlayerModeChange(installation);
			}
		}
	}
	else
	{
		if (platformManeger != nullptr)
		{
			DeleteGO(platformManeger);
		}
	}

	//足場の上にいるのなら
	if (characterController.IsOnGround() == true)
	{
		buckPosition = position;
		fallCount = 0;
	}

	//HPが0でゲームオーバー
	if (HP <= 0)
	{
		GameOber();
	}

	modelRender.Update();
}

//モード切替
void Player::PlayerModeChange(PlayerMode playermode)
{
	if (playermode == installation)
	{
		//モードを切り替え、半透明モデルを生成
		playerMode = installation;
		platformManeger = NewGO<PlatformManeger>(0, "platformmaneger");
		//現在のモデルの形状を教える
		platformManeger->noteGorIndex = player_noteIndex;
		//引継ぎが終わったら初期化
		platformManeger->PlatformManegerInit();
	}
	else if (playermode == attack)
	{
		//モードを切り替え、半透明モデルがあるなら削除
		playerMode = attack;
		if (platformManeger != nullptr)
		{
			DeleteGO(platformManeger);
		}
	}

	//ここで効果音再生
	game->IsPlaySound(6);

	//切り替え後クールタイムをリセット
	coolTime = 0.0f;

	g_pad[0]->SetVibration(0.3f, 0.1f);
}

//プレイヤーの移動
void Player::PlayerMove()
{
	//xzの移動速度を0.0fにする。
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;

	if (playerState != 1)
	{
		dushSpeed.x = 0.0f;
		dushSpeed.z = 0.0f;
	}
	else
	{
		dushSpeed.x = 10.0f;
		dushSpeed.z = 10.0f;
	}

	//左スティックの入力量を取得。
	Vector3 stickL;
	stickL.x = g_pad[0]->GetLStickXF();
	stickL.y = g_pad[0]->GetLStickYF();

	//カメラの前方向と右方向のベクトルを持ってくる。
	Vector3 forward = g_camera3D->GetForward();
	Vector3 right = g_camera3D->GetRight();
	//y方向には移動させない。
	forward.y = 0.0f;
	right.y = 0.0f;

	//左スティックの入力量と120.0fを乗算。
	right *= stickL.x * 150.0f;
	forward *= stickL.y * 150.0f;

	//移動速度にスティックの入力量を加算する。
	moveSpeed += right + forward;

	//ダッシュ処理(ジャンプ中は無視される)
	if (g_pad[0]->IsPress(enButtonB) && playerState != 1)
	{
		//xzの移動速度を0.0fにする。
		moveSpeed.x = 0.0f;
		moveSpeed.z = 0.0f;

		//左スティックの入力量を取得。
		Vector3 stickL;
		stickL.x = g_pad[0]->GetLStickXF();
		stickL.y = g_pad[0]->GetLStickYF();

		//カメラの前方向と右方向のベクトルを持ってくる。
		Vector3 forward = g_camera3D->GetForward();
		Vector3 right = g_camera3D->GetRight();
		//y方向には移動させない。
		forward.y = 0.0f;
		right.y = 0.0f;

		//左スティックの入力量と120.0fを乗算。
		right *= stickL.x * 260.0f;
		forward *= stickL.y * 260.0f;

		//移動速度にスティックの入力量を加算する。
		moveSpeed += right + forward;

		dushSpeed = moveSpeed;
	}

	//地面に付いていたら。
	if (characterController.IsOnGround())
	{
		//重力を無くす。
		moveSpeed.y = 0.0f;
		//Aボタンが押されたら。
		if (g_pad[0]->IsTrigger(enButtonA))
		{
			//ジャンプさせる。
			moveSpeed.y = 500.0f;
			isJump = true;

			//ここで効果音再生
			game->IsPlaySound(1);
		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		moveSpeed.y -= 15.0f;

		if (g_pad[0]->IsTrigger(enButtonA) && isJump == true)
		{
			//二段ジャンプさせる。
			moveSpeed.y = 400.0f;
			isJump = false;

			//ここで効果音再生
			game->IsPlaySound(1);
		}
	}

	moveSpeed.x *= 1.5f;
	moveSpeed.z *= 1.5f;

	//キャラクターコントローラーを使って座標を移動させる。
	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	//落下したとき
	if (position.y < -1500.0f)
	{
		Platform* platform = FindGO<Platform>("platform");
		if (platform != nullptr)
		{
			position = platform->notePos;
			position.y += 50.0f;
		}
		else
		{
			platform = NewGO<Platform>(0, "platform");
			platform->noteIndex = 1;
			platform->InitPlatform();
		}

		Hit(5);
		characterController.SetPosition(position);
	}

	//移動の限界(限界値になるとエフェクト再生)
	if (position.y > 1000.0f)
	{
		position.y = 1000.0f;
		ui->PlayEffects(2, position + Vector3{ 0.0f,30.0f,0.0f },1);
	}
	/*if (position.x > 1000.0f)
	{
		position.x = 1000.0f;
		ui->PlayEffects(2, position + Vector3{ 30.0f,0.0f,0.0f });
	}
	if (position.x < -1000.0f)
	{
		position.x = 1000.0f;
		ui->PlayEffects(2, position - Vector3{ 3.0f,30.0f,0.0f });
	}*/
	if (position.z > 1000.0f)
	{
		position.z = 1000.0f;
		ui->PlayEffects(2, position + Vector3{ 0.0f,0.0f,30.0f },2);
	}
	if (position.z < -1000.0f)
	{
		position.z = -1000.0f;
		ui->PlayEffects(2, position - Vector3{ 0.0f,0.0f,30.0f }, 2);
	}

	//絵描きさんに座標を教える。
	modelRender.SetPosition(position);
}

//プレイヤーの回転
void Player::PlayerRotate()
{
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//キャラクターの方向を変える。
		rotation.SetRotationYFromDirectionXZ(moveSpeed);
		//絵描きさんに回転を教える。
		modelRender.SetRotation(rotation);
	}

	if (faseTime->nowFase == FaseTime::Fase::attackFase)
	{
		//攻撃中はカメラ方向を向く
		if (g_pad[0]->IsPress(attackButton) && playerMode == attack)
		{
			Vector3 forward = g_camera3D->GetForward();

			// 地面に沿った向きにする
			forward.y = 0.0f;
			forward.Normalize();

			rotation.SetRotationYFromDirectionXZ(forward);
			modelRender.SetRotation(rotation);
		}
	}
}

//プレイヤーの状態変化(移動)
void Player::ManagePlayerState()
{
	//地面に付いていたら。
	//xかzの移動速度があったら(スティックの入力があったら)。
	if (fabsf(moveSpeed.x) >= 0.001f || fabsf(moveSpeed.z) >= 0.001f)
	{
		//ステートを2(歩き)にする。
		playerState = 2;

		//Bボタンダッシュしたら
		if (g_pad[0]->IsPress(enButtonB))
		{
			//ステートを3(ダッシュ)にする
			playerState = 3;
		}
	}
	//xとzの移動速度が無かったら(スティックの入力が無かったら)。
	else if (fabsf(moveSpeed.x) <= 0.0f || fabsf(moveSpeed.z) <= 0.0f)
	{
		//ステートを0(待機)にする。
		playerState = 0;
	}

	//地面に付いていなかったら。
	if (characterController.IsOnGround() == false)
	{
		//ステートを1(ジャンプ中)にする。
		playerState = 1;
		//ここでManageStateの処理を終わらせる。
		return;
	}
}

//アニメーションの再生
void Player::PlayerAnimetion()
{
	//switch文。
	switch (playerState) {
		//プレイヤーステートが0(待機)だったら。
	case 0:
		//待機アニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Idle);
		break;
		//プレイヤーステートが1(ジャンプ中)だったら。
	case 1:
		//ジャンプアニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Jump);
		break;
		//プレイヤーステートが2(歩き)だったら。
	case 2:
		//歩きアニメーションを再生する。
		modelRender.PlayAnimation(enAnimationClip_Walk);
		break;
		//プレイヤーステートが3(ダッシュ)だったら
	case 3:
		//ダッシュアニメーションを再生する
		modelRender.PlayAnimation(enAnimationClip_Run);
		break;
	case 4:
		modelRender.PlayAnimation(enAnimationClip_Deth);
		break;
	}
}

//プレイヤーの攻撃
void Player::PlayerAttack()
{
	if (player_noteIndex > 2)
	{
		return;
	}

	game = FindGO<Game>("game");

	if (game->isInGame == false)
	{
		return;
	}

	//カメラの前方向
	Vector3 m_forward = g_camera3D->GetForward();
	//m_forward.y += 0.1f;
	m_forward.Normalize();
	//プレイヤーの座標からちょっとだけ上方向(弾が出てくる場所)
	Vector3 m_shotPos = { position.x, position.y + 70.0f, position.z };
	Bullet* bullet = m_bulletPool.GetInactive(false);

	if (bullet != nullptr)
	{
		m_forward.Normalize();
		bullet->Fire(m_shotPos, m_forward,noteAttack[player_noteIndex],player_noteIndex);

		//ここで効果音再生
		game->IsPlaySound(2);
	}
	coolTime = noteCoolTime[player_noteIndex];
}

//ダメージ処理
void Player::Hit(int damege)
{
	if (inDamage == false)
	{
		HP -= damege;
		//ここで効果音再生
		game->IsPlaySound(5);
		invincibleTime = 3.0f;
		if (HP <= 0)
		{
			invincibleTime = 1.0f;
		}
		inDamage = true;
	}
}

//ゲームオーバー処理
void Player::GameOber()
{
	//他のクラスの動きを止める
	isWait = true;

	if (inDamage == false)
	{
		if (playerState != 4)
		{
			Vector3 cameraPosition = { position.x + 100.0f,position.y + 100.0f,position.z + 100.0f };

			//プレイヤーの上空にカメラを持っていく
			g_camera3D->SetPosition(cameraPosition);
			g_camera3D->SetTarget(position);
			playerState = 4;
		}

		if (coloer <= 1.0f)
		{
			coloer += 0.005f;
			fadeOUT.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, coloer));

			fadeOUT.Update();
		}
	}
}

void Player::Render(RenderContext& rc)
{
	//無敵時間中なら点滅
	if (inDamage == true)
	{
		//点滅処理
		if (invincibleTime >= 2.5f)
		{
			game->Blinking(0.1f, modelRender, rc);
		}
		else if (invincibleTime >= 0.5f)
		{
			game->Blinking(0.07f, modelRender, rc);
		}
		else
		{
			game->Blinking(0.05f, modelRender, rc);
		}
		invincibleTime -= g_gameTime->GetFrameDeltaTime();

		//無敵時間終了後
		if (invincibleTime <= 0.0f)
		{
			inDamage = false;
		}
	}
	//それ以外は常時描画
	else
	{
		if (isWait == true)
		{
			if (coloer >= 1.0f)
			{
				ui->GameOber(rc);
				return;
			}

			fadeOUT.Draw(rc);
		}

		modelRender.Draw(rc);
	}
}

//初期化処理
void Player::Init()
{
	position = { 0.0f,0.0f,0.0f };
	modelRender.SetPosition(position);
	characterController.SetPosition(position);
	modelRender.Update();

	playerState = 0;
	playerMode = attack;
	HP = 100;
	MaxHP = 100;
	player_noteIndex = 0;
	isWait = false;
	coolTime = 0;
	isJump = false;
	fallCount = 0;
	inDamage = false;
	invincibleTime = 3.0f;
	coloer = 0.0f;
	game = FindGO<Game>("game");
}