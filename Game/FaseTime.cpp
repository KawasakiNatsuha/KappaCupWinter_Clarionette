#include "stdafx.h"
#include "FaseTime.h"

FaseTime::FaseTime()
{

}

FaseTime::~FaseTime()
{

}

void FaseTime::Update()
{
	//時間経過
	deltaTime += gameTime.GetFrameDeltaTime();
	if (deltaTime >= 1.0f)
	{
		faseTimer -= 1;
		deltaTime -= 1.0f;
	}

	//memo::残り時間表示
	swprintf_s(font, L"残り時間: %d", faseTimer);
	timeFont.SetText(font);
	timeFont.SetPosition({ 400.0f,500.0f,0.0f });
	timeFont.SetColor(g_vec4White);
}

void FaseTime::Render(RenderContext& rc)
{
	timeFont.Draw(rc);
}