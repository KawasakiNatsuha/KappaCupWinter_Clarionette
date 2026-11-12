#pragma once

class Player;
class UI;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();
	void Reset();

	Player* m_player;	//プレイヤー。
	UI* ui;				//フェーズ切り替え演出用
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。

	//カメラ感度。memo::後で設定で変えられるようにすると◎
	float cameraSensitivity = 1.5f;

	Vector3 m_cameraResetTarget;
	bool m_isCameraReset = false;
};;

