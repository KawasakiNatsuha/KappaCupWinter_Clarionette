#include "stdafx.h"
#include "Enemy.h"
#include "FaseTime.h"
#include "Player.h"
#include "EnemyBullet.h"

Enemy::Enemy()
{
	testModel.Init("Assets/modelData/unityChan.tkm");
	Vector3 position = { 100.0f,100.0f,100.0f };
	testModel.SetPosition(position);

	//オブジェクトプールの初期化
	m_bulletPool.Init(100, "EnemyBullet");
}

void Enemy::Init(int hp, int attack, float CoolTime, float MoveSpeed)
{
	EnemyHP = hp;
	AttackPower = attack;
	coolTime = CoolTime;
	moveSpeed = MoveSpeed;
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	faseTime = FindGO<FaseTime>("faseTime");

	testModel.Update();

	//回避フェーズ時のみ攻撃処理
	if (faseTime->nowFase == FaseTime::Fase::evadeFase && nowCoolTime <= 0.0f)
	{
		Attack(AttackPower);
	}

	//クールタイム中
	if (nowCoolTime > 0.0f)
	{
		float playertime = gametime.GetFrameDeltaTime();
		nowCoolTime -= playertime;
	}

	//死亡処理
	if (EnemyHP <= 0)
	{
		IsDie();
	}

	//playerの方を向く
	player = FindGO<Player>("player");
	Vector3 diff = position - player->position;
	diff.y = 0.0f;

	if (diff.LengthSq() > 0.001f)
	{
		diff.Normalize();

		Quaternion rot;
		Quaternion offsetRot;
		rot.SetRotationYFromDirectionXZ(diff);
		offsetRot.SetRotationDegY(170.0f);
		rot *= offsetRot;
		testModel.SetRotation(rot);
	}

	//memo::敵のHP表示
	swprintf_s(font, L"敵HP: %d", EnemyHP);
	fontRender.SetText(font);
	fontRender.SetPosition({ -130.0f,500.0f,0.0f });
	fontRender.SetColor(g_vec4White);
}

void Enemy::OnHit(int damage)
{
	EnemyHP -= damage;
}

//攻撃処理
void Enemy::Attack(int attackPower)
{
	player = FindGO<Player>("player"); 
	Vector3 diff = position - player->position;
	diff.Normalize();
	//プレイヤーの座標からちょっとだけ上方向(弾が出てくる場所)
	Vector3 m_shotPos = { position.x, position.y + 70.0f, position.z };
	EnemyBullet* enemyBullet = m_bulletPool.GetInactive(false);

	if (enemyBullet != nullptr)
	{
		enemyBullet->Fire(m_shotPos, diff, AttackPower);
	}
	nowCoolTime = coolTime;
}

void Enemy::Render(RenderContext& rc)
{
	testModel.Draw(rc);
	fontRender.Draw(rc);
}

//死亡処理
void Enemy::IsDie()
{
	

	//最後に自分削除
	DeleteGO(this);
}