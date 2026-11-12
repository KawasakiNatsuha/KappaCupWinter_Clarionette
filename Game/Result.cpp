#include "stdafx.h"
#include "Result.h"
#include "UI.h"
#include "Game.h"

Result::Result()
{
	back[0].Init("Assets/sprite/black.DDS", 1920.0f, 1080.0f);
	back[1].Init("Assets/sprite/right.DDS", 1920.0f, 1080.0f);
	back[2].Init("Assets/sprite/left.DDS", 1920.0f, 1080.0f);
	back[3].Init("Assets/sprite/up.DDS", 1920.0f, 1080.0f);
	sentence.Init("Assets/sprite/sentence1.DDS", 1920.0f, 1080.0f);
	next.Init("Assets/sprite/sentence2.DDS", 1920.0f, 1080.0f);

	game = FindGO<Game>("game");
	game->IsPlaySound(101);
}

void Result::Update()
{
	if (PorM == true)
	{
		coler += 0.05f;
	}
	else
	{
		coler -= 0.05f;
	}

	if (coler <= 0.0f)
	{
		PorM = true;
	}
	if (coler >= 1.0f)
	{
		PorM = false;
	}

	next.SetMulColor(Vector4(1.0f, 1.0f, 1.0f, coler));
	next.Update();

	waitCount += 1;

	if (waitCount <= 5)
	{
		return;
	}

	if (g_pad[0]->IsPressAnyKey())
	{
		game->ReSet();
		DeleteGO(this);
	}
}

void Result::Render(RenderContext& rc)
{
	for (int i = 0;i < 3;i++)
	{
		back[i].Draw(rc);
	}
	sentence.Draw(rc);
	next.Draw(rc);
}