#include "stdafx.h"
#include "Platform.h"
#include "FaseTime.h"

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

	note[noteIndex].SetPosition(notePos);
	note[noteIndex].SetRotation(noteRotate);
	note[noteIndex].SetScale(1.5f, 1.5f, 1.5f);
	note[noteIndex].Update();
	noteObject.CreateFromModel(note[noteIndex].GetModel(), note[noteIndex].GetModel().GetWorldMatrix());

	faseTime = FindGO<FaseTime>("faseTime");
	crieteFase = faseTime->TurnCount;
}

void Platform::Update()
{
	faseTime = FindGO<FaseTime>("faseTime");
	//足場の寿命が来ると削除
	if (dethFase == faseTime->TurnCount - crieteFase)
	{
		DeleteGO(this);
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