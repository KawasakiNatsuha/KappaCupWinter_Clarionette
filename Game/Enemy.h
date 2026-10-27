#pragma once
#include "ObjectPool.h"
#include "EnemyBullet.h"

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
	void Init(int hp, int attack, float CoolTime,float MoveSpeed);

	//座標
	Vector3 position = { 50.0f,0.0f,50.0f };
	//当たり判定の範囲
	float collisionRadius = 30.0f;

	int EnemyHP = 100;			//HP
	int AttackPower = 5;		//攻撃力
	float coolTime = 0.1;			//クールタイム
	float nowCoolTime = 0;		//現在のクールタイム
	float moveSpeed = 0.0f;		//移動速度

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
};

