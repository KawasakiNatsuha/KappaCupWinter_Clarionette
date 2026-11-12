#include "stdafx.h"
#include "GameCamera.h"
#include "Player.h"
#include "UI.h"

GameCamera::GameCamera()
{
	
}


GameCamera::~GameCamera()
{

}

bool GameCamera::Start()
{
	//注視点から視点までのベクトルを設定。
	m_toCameraPos.Set(0.0f, 50.0f, -250.0f);
	//プレイヤーのインスタンスを探す。
	m_player = FindGO<Player>("player");

	//カメラのニアクリップとファークリップを設定する。
	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(1000.0f);

	//画角の変更
	float angle = g_camera3D->GetViewAngle();
	angle += 0.5f;
	g_camera3D->SetViewAngle(angle);

	return true;
}
void GameCamera::Update()
{
	ui = FindGO<UI>("ui");
	m_player = FindGO<Player>("player");

	if (m_player->isWait == true)
	{
		return;
	}

	if (ui->IsChange == true)
	{
		return;
	}

	if (g_pad[0]->IsTrigger(enButtonX))
	{
		Reset();
	}

	//カメラを更新。

	g_camera3D->SetNear(1.0f);
	g_camera3D->SetFar(5000.0f);

	//注視点を計算する。
	Vector3 target = m_player->position;
	//プレイヤの足元からちょっと上を注視点とする。
	target.y += 150.0f;

	if (m_isCameraReset == true)
	{
		// 現在位置から目標位置に近づける
		m_toCameraPos.Lerp(5.0f * g_gameTime->GetFrameDeltaTime(), m_toCameraPos, m_cameraResetTarget);
		Vector3 diff = m_cameraResetTarget - m_toCameraPos;

		if (diff.Length() < 1.0f)
		{
			m_toCameraPos = m_cameraResetTarget;
			m_isCameraReset = false;
		}
	}
	else
	{
		Vector3 toCameraPosOld = m_toCameraPos;
		//パッドの入力を使ってカメラを回す。
		float x = g_pad[0]->GetRStickXF();
		float y = g_pad[0]->GetRStickYF();
		//Y軸周りの回転
		Quaternion qRot;
		qRot.SetRotationDeg(Vector3::AxisY, cameraSensitivity * x);
		qRot.Apply(m_toCameraPos);
		//X軸周りの回転。
		Vector3 axisX;
		axisX.Cross(Vector3::AxisY, m_toCameraPos);
		axisX.Normalize();
		qRot.SetRotationDeg(axisX, cameraSensitivity * y);
		qRot.Apply(m_toCameraPos);

		Vector3 toPosDir = m_toCameraPos;
		toPosDir.Normalize();
		if (toPosDir.y < -0.8f) {
			//カメラが上向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
		else if (toPosDir.y > 0.9f) {
			//カメラが下向きすぎ。
			m_toCameraPos = toCameraPosOld;
		}
	}

	//視点を計算する。
	Vector3 pos = target + m_toCameraPos;

	//メインカメラに注視点と視点を設定する。
	g_camera3D->SetTarget(target);
	g_camera3D->SetPosition(pos);

	//カメラの更新。
	g_camera3D->Update();
}

void GameCamera::Reset()
{
	//// カメラの初期オフセット
	//Vector3 cameraOffset(0.0f, 50.0f, -250.0f);

	//// プレイヤーの向きに合わせて回転
	//m_player->rotation.Apply(cameraOffset);

	//// カメラ位置を更新するための相対位置
	//m_toCameraPos = cameraOffset;

	Vector3 cameraOffset(0.0f, 50.0f, -250.0f);

	// プレイヤーの向きに合わせる
	m_player->rotation.Apply(cameraOffset);

	// 目標位置を保存
	m_cameraResetTarget = cameraOffset;

	m_isCameraReset = true;
}