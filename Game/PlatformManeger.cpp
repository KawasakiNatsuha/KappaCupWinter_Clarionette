#include "stdafx.h"
#include "PlatformManeger.h"
#include "Player.h"
#include "Platform.h"

PlatformManeger::PlatformManeger()
{
	
}

PlatformManeger::~PlatformManeger()
{

}

void PlatformManeger::PlatformManegerInit()
{
	//モデル読み込み
	noteGor[0].IniTranslucent("Assets/modelData/note_quarterNote_Gor.tkm");
	noteGor[1].IniTranslucent("Assets/modelData/note2_Gor.tkm");
	noteGor[2].IniTranslucent("Assets/modelData/note3_Gor.tkm");

	//半透明モデル限定初期化
	rotX.SetRotationDegX(-90.0f);
	noteGor[noteGorIndex].SetScale(1.5f,1.5f, 1.5f);

	noteGor[noteGorIndex].SetRotation(noteRotate);
}

void PlatformManeger::Update()
{
	//playerを更新
	player = FindGO<Player>("player");

	//足場の操作
	Operation();

	//カメラの向きとplayerの座標、入力に応じて座標を決める
	noteGorPos = player->position + g_camera3D->GetForward() * depth;

	//座標＆角度の更新
	noteGor[noteGorIndex].SetPosition(noteGorPos);
	rotY.SetRotationDegY(rot);

	//X軸回転とY軸回転を合成
	noteRotate = rotY * rotX;

	noteGor[noteGorIndex].SetRotation(noteRotate);

	noteGor[noteGorIndex].Update();
}

//足場を操作する
void PlatformManeger::Operation()
{
	//奥行きの操作
	if (g_pad[0]->IsPress(nearButton))
	{
		depth -= 10.0f;
	}
	if (g_pad[0]->IsPress(farButton))
	{
		depth += 10.0f;
	}
	if (depth <= 300.0f)
	{
		depth = 300.0f;
	}
	if (depth >= 1000.0f)
	{
		depth = 1000.0f;
	}

	//回転操作
	if (g_pad[0]->IsPress(rotButtonR))
	{
		rot += 4.0f;
	}
	if (g_pad[0]->IsPress(rotButtonL))
	{
		rot -= 4.0f;
	}
}

//足場を設置する
void PlatformManeger::PlatformInstallation()
{
	//半透明モデルの座標、形状を受け継ぐ
	platform = NewGO<Platform>(0, "platform");
	platform->notePos = noteGorPos;
	platform->noteIndex = noteGorIndex;
	platform->noteRotate = rotY;
	platform->Index = platformIndex;
	platformIndex += 1;
	//引継ぎが終わったら足場の初期化作業
	platform->InitPlatform();

	//半透明モデルを削除
	player->platformManeger = nullptr;
	DeleteGO(this);
}

void PlatformManeger::Render(RenderContext& rc)
{
	noteGor[noteGorIndex].Draw(rc);
}