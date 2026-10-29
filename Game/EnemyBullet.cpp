#include "stdafx.h"
#include "EnemyBullet.h"
#include "Player.h"

EnemyBullet::EnemyBullet()
{
	debugBullet.Init("Assets/modelData/Bullet.tkm");
}

EnemyBullet::~EnemyBullet()
{

}

void EnemyBullet::Fire(const Vector3& pos, const Vector3& dir, int attack)
{
	bulletPos = pos;
	bulletMoveVec = dir;
	m_startPos = pos;
	Attack = attack;

	debugBullet.SetPosition(bulletPos);

	Activate();
}

void EnemyBullet::Update()
{
	// 使用中でなければ処理しない
	if (IsActive()==true)
	{
		BulletMove();
		Hit();
		debugBullet.Update();
	}
}

void EnemyBullet::Render(RenderContext& rc)
{
	if (IsActive() == true)
	{
		debugBullet.Draw(rc);
	}
}

void EnemyBullet::BulletMove()
{
	//フレームごとの経過時間
	float deltaTime = gameTime.GetFrameDeltaTime();
	bulletPos += bulletMoveVec * bulletMoveSpeed * deltaTime * -1.0f;
	debugBullet.SetPosition(bulletPos);

	// 発射地点から一定距離を飛んだら再利用可能にする
	Vector3 diff = bulletPos - m_startPos;

	if (diff.Length() >= m_maxDistance)
	{
		Deactivate();
	}
}

void EnemyBullet::Hit()
{
	player = FindGO<Player>("player");
	if (player != nullptr)
	{
		//当たり判定
		Vector3 diff = bulletPos - player->position;
		//縦の当たり判定
		Vector3 enemyUp = player->position;
		enemyUp.y += 70.0f;
		Vector3 diffUp = bulletPos - enemyUp;
		//弾が当たり判定の内部に行ったら
		if (diff.Length() <= 40.0f)
		{
			player->Hit(Attack);
			Deactivate();
		}
		else if (diffUp.Length() <= 40.0f)
		{
			player->Hit(Attack);
			Deactivate();
		}
	}
}