#pragma once
#include "ObjectPool.h"
#include "EnemyBullet.h"
#include <random>

class FaseTime;
class Player;

class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Update();
	void Render(RenderContext& rc);
	void OnHit(int damage);
	void IsDie();
	void Attack(int attackPower);
	void Move();
	void ModeChange();
	void Init(int hp, int attack, float CoolTime,float MoveSpeed,float ChangeMoveModeTime);

	//座標
	Vector3 position = { 50.0f,0.0f,50.0f };
	//当たり判定の範囲
	float collisionRadius = 30.0f;

	int EnemyHP = 100;						//HP
	int AttackPower = 5;					//攻撃力
	float coolTime = 0.1;					//クールタイム
	float nowCoolTime = 0;					//現在のクールタイム
	float moveSpeed = 0.0f;					//移動速度
	float changeMoveModeTime = 0.0f;		//移動モードの切り替えペース

private:
	Quaternion quatenion;
	//テスト用のモデル
	ModelRender testModel;

	//弾用のオブジェクトプール
	ObjectPool<EnemyBullet> m_bulletPool;

	FaseTime* faseTime;
	Player* player;
	GameTime gametime;

	FontRender fontRender;
	wchar_t font[256];

	/// 敵の移動モード。
	//left=プレイヤーから見て左側に移動
	//right=プレイヤーから見て右側に移動
	//up=上方向に移動
	//down=下方向に移動
	//stop=移動しない
	//up=弾から逃げる
	enum MoveMode
	{
		left,
		right,
		up,
		down,
		stop,
		bllet
	};

	MoveMode moveMode;

	float firstChangeMoveModeTime = 0.0f;

	//乱数
	std::mt19937 gen;
	//回避モード
	std::uniform_int_distribution<int> mode;

	//1フレームの経過時間
	float deltaTime;
};

