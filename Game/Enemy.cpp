#include "stdafx.h"
#include "Enemy.h"
#include "FaseTime.h"
#include "Player.h"
#include "EnemyBullet.h"
#include "Bullet.h"
#include "UI.h"
#include "Result.h"

Enemy::Enemy() :gen(std::random_device{}()), mode(1, 6), offset(-100, 100)
{
	testModel.Init("Assets/modelData/unityChan.tkm");

	//オブジェクトプールの初期化
	m_bulletPool.Init(100, "EnemyBullet");
}

void Enemy::Init(int hp, int attack, float CoolTime, float MoveSpeed, float ChangeMoveModeTime)
{
	EnemyHP = hp;
	EnemyMaxHP = hp;
	AttackPower = attack;
	coolTime = CoolTime;
	moveSpeed = MoveSpeed;
	changeMoveModeTime = ChangeMoveModeTime;
	firstChangeMoveModeTime = changeMoveModeTime;
	position = { 100.0f,100.0f,1000.0f };
	testModel.SetPosition(position);
	testModel.Update();
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	ui = FindGO<UI>("ui");
	player = FindGO<Player>("player");

	if (player->isWait == true)
	{
		return;
	}

	if (ui->IsChange == true)
	{
		return;
	}

	faseTime = FindGO<FaseTime>("faseTime");

	deltaTime = g_gameTime->GetFrameDeltaTime();

	testModel.Update();

	//回避フェーズ時のみ攻撃処理
	if (faseTime->nowFase == FaseTime::Fase::evadeFase && nowCoolTime <= 0.0f)
	{
		Attack(AttackPower);
	}
	else
	{
		changeMoveModeTime -= deltaTime;
		if (changeMoveModeTime <= 0.0f)
		{
			ModeChange();
		}
	}

	Move();

	//クールタイム中
	if (nowCoolTime > 0.0f)
	{
		float playertime = deltaTime;
		nowCoolTime -= playertime;
	}

	//死亡処理
	if (EnemyHP <= 0)
	{
		IsDie();
	}

	//playerの方を向く
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
}

void Enemy::OnHit(int damage)
{
	EnemyHP -= damage;
}

//攻撃処理
void Enemy::Attack(int attackPower)
{
	Vector3 diff = position - player->position;
	diff.x += (float)offset(gen);
	diff.y += (float)offset(gen);
	diff.Normalize();
	//エネミーの座標からちょっとだけ上方向(弾が出てくる場所)
	Vector3 m_shotPos = { position.x, position.y + 70.0f, position.z };
	EnemyBullet* enemyBullet = m_bulletPool.GetInactive(false);

	if (enemyBullet != nullptr)
	{
		enemyBullet->Fire(m_shotPos, diff, AttackPower);
	}
	nowCoolTime = coolTime;
}

//移動処理
void Enemy::Move()
{
	//攻撃フェーズは回避
	if (faseTime->nowFase == Player::PlayerMode::attack)
	{
		switch (moveMode)
		{
		case Enemy::left:
			position.x -= moveSpeed * deltaTime;
			break;
		case Enemy::right:
			position.x += moveSpeed * deltaTime;
			break;
		case Enemy::up:
			position.y += moveSpeed * deltaTime;
			break;
		case Enemy::down:
			position.y -= moveSpeed * deltaTime;
			break;
		case Enemy::stop:
			//停止時間が長いとタコ殴りにされるので倍の経過時間を加算する
			changeMoveModeTime -= deltaTime;
			break;
		case Enemy::bllet:
			for (auto bllets : FindGOs<Bullet>("bullet"))
			{
				//弾が自分より向こう側にいたら飛ばす
				if (bllets->bulletPos.x <= position.x)
				{
					continue;
				}

				//手前にいるならそいつから避ける
				position += bllets->bulletMoveVec * -1 * moveSpeed * deltaTime;
				break;
			}
			break;
		default:
			break;
		}
	}
	//回避フェーズはプレイヤーに追従
	else
	{
		Vector3 movePosition = player->position - position;
		movePosition.Normalize();
		position += movePosition * moveSpeed * deltaTime;
	}

	position.z = 1000.0f;
	if (position.y < 0.0f)
	{
		position.y = 0.0f;
	}
	if (position.y > 1000.0f)
	{
		position.y = 1000.0f;
	}
	if (position.x > 1000.0f)
	{
		position.x = 1000.0f;
	}
	if (position.x < -1000.0f)
	{
		position.x = 1000.0f;
	}

	testModel.SetPosition(position);
}

//移動モードの切り替え
void Enemy::ModeChange()
{
	switch (mode(gen))
	{
	case 1:
		moveMode = left;
		break;
	case 2:
		moveMode = right;
		break;
	case 3:
		moveMode = up;
		break;
	case 4:
		moveMode = down;
		break;
	case 5:
		moveMode = stop;
		break;
	case 6:
		moveMode = bllet;
		break;
	default:
		break;
	}

	changeMoveModeTime = firstChangeMoveModeTime;
}

void Enemy::Render(RenderContext& rc)
{
	testModel.Draw(rc);
	fontRender.Draw(rc);
}

//死亡処理
void Enemy::IsDie()
{
	//リザルト画面作る
	NewGO<Result>(0, "result");
	//最後に自分削除
	DeleteGO(this);
}