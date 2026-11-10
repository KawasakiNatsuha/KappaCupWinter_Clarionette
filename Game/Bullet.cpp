#include "stdafx.h"
#include "Bullet.h"
#include "Enemy.h"
#include "UI.h"
#include "FaseTime.h"

Bullet::Bullet()
{
	bulletModel[0].Init("Assets/modelData/bullet1.tkm");
	bulletModel[1].Init("Assets/modelData/bullet3.tkm");
	bulletModel[2].Init("Assets/modelData/bullet2.tkm");
}

Bullet::~Bullet()
{

}

//初期化
void Bullet::Fire(const Vector3& pos, const Vector3& dir,int attack,int bulletNumber)
{
	bulletPos = pos;
	bulletMoveVec = dir;
	m_startPos = pos;
	Attack = attack;
	BulletNumber = bulletNumber;
	IsBuff = false;

	bulletModel[BulletNumber].SetPosition(bulletPos);

	Activate();
}

void Bullet::Update()
{
	// 使用中でなければ処理しない
	if (!IsActive())
	{
		return;
	}

	ui = FindGO<UI>("ui");
	faseTime = FindGO<FaseTime>("faseTime");

	if (ui == nullptr)
	{
		Deactivate();
	}

	if (faseTime->nowFase == FaseTime::Fase::evadeFase)
	{
		Deactivate();
	}

	BulletMove();
	Hit();
	bulletModel[BulletNumber].Update();
}

void Bullet::Render(RenderContext& rc)
{
	if (!IsActive())
	{
		return;
	}

	bulletModel[BulletNumber].Draw(rc);
}

void Bullet::BulletMove()
{
	//フレームごとの経過時間
	float deltaTime = gameTime.GetFrameDeltaTime();
	bulletPos += bulletMoveVec * bulletMoveSpeed * deltaTime;
	bulletModel[BulletNumber].SetPosition(bulletPos);

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
		//当たり判定
		Vector3 diff = bulletPos - enemy->position;
		//縦の当たり判定
		Vector3 enemyUp = enemy->position;
		enemyUp.y += 70.0f;
		Vector3 diffUp = bulletPos - enemyUp;
		//弾が当たり判定の内部に行ったら
		if (diff.Length() <= 40.0f)
		{
			enemy->OnHit(Attack);
			ui->PlayEffects(1, enemyUp);
			Deactivate();
		}
		else if (diffUp.Length() <= 40.0f)
		{
			enemy->OnHit(Attack);
			ui->PlayEffects(1, enemyUp);
			Deactivate();
		}
	}
}