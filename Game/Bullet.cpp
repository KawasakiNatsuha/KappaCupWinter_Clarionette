#include "stdafx.h"
#include "Bullet.h"
#include "Enemy.h"

Bullet::Bullet()
{
	debugBullet.Init("Assets/modelData/Bullet.tkm");
}

Bullet::~Bullet()
{

}

void Bullet::Fire(const Vector3& pos, const Vector3& dir)
{
	bulletPos = pos;
	bulletMoveVec = dir;
	m_startPos = pos;

	debugBullet.SetPosition(bulletPos);

	Activate();
}

void Bullet::Update()
{
	// 使用中でなければ処理しない
	if (!IsActive())
	{
		return;
	}

	BulletMove();
	Hit();
	debugBullet.Update();
}

void Bullet::Render(RenderContext& rc)
{
	if (!IsActive())
	{
		return;
	}

	debugBullet.Draw(rc);
}

void Bullet::BulletMove()
{
	//フレームごとの経過時間
	float deltaTime = gameTime.GetFrameDeltaTime();
	bulletPos += bulletMoveVec * bulletMoveSpeed * deltaTime;
	debugBullet.SetPosition(bulletPos);

	// 発射地点から一定距離を飛んだら再利用可能にする
	Vector3 diff = bulletPos - m_startPos;

	if (diff.Length() >= m_maxDistance)
	{
		Deactivate();
	}
}

void Bullet::Hit()
{
	enemy = FindGO<Enemy>("enemy");
	if (enemy != nullptr)
	{
		Vector3 diff = bulletPos - enemy->position;
		//弾が当たり判定の内部に行ったら
		if (diff.Length() <= 5.0f)
		{
			//memo::仮で10ダメージ
			enemy->OnHit(10);
			Deactivate();
		}
	}
}