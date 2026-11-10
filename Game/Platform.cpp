#include "stdafx.h"
#include "Platform.h"
#include "FaseTime.h"
#include "Bullet.h"
#include "EnemyBullet.h"

Platform::Platform()
{
	
}

Platform::~Platform()
{

}

void Platform::InitPlatform()
{
	//初期化
	note[0].Init("Assets/modelData/note_quarterNote.tkm");
	note[1].Init("Assets/modelData/note2.tkm");
	note[2].Init("Assets/modelData/note3.tkm");
	note[3].Init("Assets/modelData/quarterRest.tkm");
	note[4].Init("Assets/modelData/halfRest.tkm");

	note[noteIndex].SetPosition(notePos);
	note[noteIndex].SetRotation(noteRotate);
	note[noteIndex].SetScale(1.5f, 1.5f, 1.5f);
	note[noteIndex].Update();
	noteObject.CreateFromModel(note[noteIndex].GetModel(), note[noteIndex].GetModel().GetWorldMatrix());

	faseTime = FindGO<FaseTime>("faseTime");
	crieteFase = faseTime->TurnCount;

	isInit = true;
}

void Platform::Update()
{
	faseTime = FindGO<FaseTime>("faseTime");

	if (isInit == true)
	{
		//四分休符の場合
		if (noteIndex == 3)
		{
			enemyBullet = FindGO<EnemyBullet>("enemyBullet");
			if (enemyBullet != nullptr)
			{
				Vector3 diff = enemyBullet->bulletPos - notePos;
				if (diff.Length() < 50.0f)
				{
					Hit(1);
				}
			}
		}

		//二分休符の場合
		if (noteIndex == 4)
		{
			bullet = FindGO<Bullet>("bullet");
			if (enemyBullet != nullptr)
			{
				Vector3 diff = bullet->bulletPos - notePos;
				if (diff.Length() < 50.0f)
				{
					Hit(2);
				}
			}
		}
	}

	//足場の寿命が来ると削除
	if (dethFase == faseTime->TurnCount - crieteFase)
	{
		DeleteGO(this);
	}

	//HPが尽きても削除
	if (platformHP <= 0)
	{
		DeleteGO(this);
	}
}

void Platform::Hit(int Index)
{
	//敵の弾を防ぐ
	if (Index == 1)
	{
		platformHP -= enemyBullet->Attack;
		enemyBullet->Deactivate();

		if (platformHP <= 0)
		{
			DeleteGO(this);
		}
	}
	//自分の弾を強化する
	else
	{
		if (bullet->IsBuff == false)
		{
			bullet->Attack *= 2;
			bullet->IsBuff = true;
		}
	}
}

void Platform::Render(RenderContext& rc)
{
	if (dethFase - (faseTime->TurnCount - crieteFase) == 1)
	{
		game.Blinking(0.3, note[noteIndex], rc);
	}
	else
	{
		note[noteIndex].Draw(rc);
	}
}

void Platform::Init()
{
	DeleteGO(this);
}