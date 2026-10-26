#include "stdafx.h"
#include "Enemy.h"

Enemy::Enemy()
{
	testModel.Init("Assets/modelData/unityChan.tkm");
	//キャラコンを初期化する。
	characterController.Init(25.0f, 75.0f, position);
}

Enemy::~Enemy()
{

}

void Enemy::Update()
{
	testModel.Update();

	//死亡処理
	if (EnemyHP <= 0)
	{
		IsDie();
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