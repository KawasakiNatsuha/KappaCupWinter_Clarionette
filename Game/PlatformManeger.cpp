#include "stdafx.h"
#include "PlatformManeger.h"
#include "Player.h"

PlatformManeger::PlatformManeger()
{
	noteGor[0].IniTranslucent("Assets/modelData/note_quarterNote_Gor.tkm");
}

PlatformManeger::~PlatformManeger()
{

}

void PlatformManeger::Update()
{
	//playerを更新
	player = FindGO<Player>("player");
	//memo::カメラの向きとplayerの座標からとりあえずplayerのちょっと前に出すよ
	noteGorPos = player->position + g_camera3D->GetForward() * 100.0f;

	noteGor[noteGorIndex].SetPosition(noteGorPos);
	noteGor[noteGorIndex].Update();
}

void PlatformManeger::Render(RenderContext& rc)
{
	noteGor[noteGorIndex].Draw(rc);
}