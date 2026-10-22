
//プレイヤーを担当するクラス。

#include "stdafx.h"
#include "Player.h"
#include "Bullet.h"

Player::Player()
{
	//アニメーションクリップをロードする。
	animationClips[enAnimationClip_Idle].Load("Assets/animData/idle.tka");
	animationClips[enAnimationClip_Idle].SetLoopFlag(true);
	animationClips[enAnimationClip_Walk].Load("Assets/animData/walk.tka");
	animationClips[enAnimationClip_Walk].SetLoopFlag(true);
	animationClips[enAnimationClip_Jump].Load("Assets/animData/jump.tka");
	animationClips[enAnimationClip_Jump].SetLoopFlag(false);
	animationClips[enAnimationClip_Run].Load("Assets/animData/run.tka");
	animationClips[enAnimationClip_Run].SetLoopFlag(true);
	//ユニティちゃんのモデルを読み込む。
	modelRender.Init("Assets/modelData/unityChan.tkm", animationClips, enAnimationClip_Num, enModelUpAxisY);

	//キャラコンを初期化する。
	characterController.Init(25.0f, 75.0f, position);

	//オブジェクトプールの初期化
	m_bulletPool.Init(50, "bullet");
}

void Player::Update()
{
	PlayerMove();
	PlayerRotate();
	ManagePlayerState();
	PlayerAnimetion();

	if (g_pad[0]->IsPress(attackButton)&& coolTime <= 0.0f)
	{
		PlayerAttack();
	}

	if (coolTime > 0.0f)
	{
		float playertime = gametime.GetFrameDeltaTime();
		coolTime -= playertime;
	}

	modelRender.Update();
}

//プレイヤーの移動
void Player::PlayerMove()
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
	right *= stickL.x * 120.0f;
	forward *= stickL.y * 120.0f;

	//移動速度にスティックの入力量を加算する。
	moveSpeed += right + forward;

	//ダッシュ処理
	if (g_pad[0]->IsPress(enButtonB))
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
			moveSpeed.y = 480.0f;
		}
	}
	//地面に付いていなかったら。
	else
	{
		//重力を発生させる。
		moveSpeed.y -= 9.0f;
	}

	//キャラクターコントローラーを使って座標を移動させる。
	position = characterController.Execute(moveSpeed, 1.0f / 60.0f);

	//落下したとき
	if (position.y < -1500.0f)
	{
		position.x = 0.0f;
		position.y = 50.0f;
		position.z = 0.0f;

		characterController.SetPosition(position);
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

	//攻撃中はカメラ方向を向く
	if (g_pad[0]->IsPress(attackButton))
	{
		Vector3 forward = g_camera3D->GetForward();

		// 地面に沿った向きにする
		forward.y = 0.0f;
		forward.Normalize();
		
		rotation.SetRotationYFromDirectionXZ(forward);
		modelRender.SetRotation(rotation);
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
	}
}

//プレイヤーの攻撃
void Player::PlayerAttack()
{
	//カメラの前方向
	Vector3 m_forward = g_camera3D->GetForward();
	m_forward.Normalize();
	//プレイヤーの座標からちょっとだけ上方向(弾が出てくる場所)
	Vector3 m_shotPos = { position.x, position.y + 70.0f, position.z };
	Bullet* bullet = m_bulletPool.GetInactive(false);

	if (bullet != nullptr)
	{
		bullet->Fire(m_shotPos, m_forward);
	}
	coolTime = 0.2;
}

void Player::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}