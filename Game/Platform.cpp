#include "stdafx.h"
#include "Platform.h"

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
	note[noteIndex].SetPosition(notePos);
	note[noteIndex].SetRotation(noteRotate);
	note[noteIndex].SetScale(1.5f, 1.5f, 1.5f);
	note[noteIndex].Update();
	noteObject.CreateFromModel(note[noteIndex].GetModel(), note[noteIndex].GetModel().GetWorldMatrix());
}

void Platform::Update()
{
	
}

void Platform::Render(RenderContext& rc)
{
	note[noteIndex].Draw(rc);
}