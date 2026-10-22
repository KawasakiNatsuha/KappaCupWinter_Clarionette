#pragma once

class Player;

class GameCamera : public IGameObject
{
public:
	GameCamera();
	~GameCamera();
	bool Start();
	void Update();

	Player* m_player;	//プレイヤー。
	Vector3 m_toCameraPos;	//注視点から視点に向かうベクトル。

	//カメラ感度。memo::後で設定で変えられるようにすると◎
	float cameraSensitivity = 1.5f;
};;

