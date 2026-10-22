#include "stdafx.h"
#include "Platform.h"

Platform::Platform()
{
	note[0].Init("Assets/modelData/note_quarterNote.tkm");
}

Platform::~Platform()
{

}

void Platform::Update()
{

}

void Platform::Render(RenderContext& rc)
{
	note[noteIndex].Draw(rc);
}