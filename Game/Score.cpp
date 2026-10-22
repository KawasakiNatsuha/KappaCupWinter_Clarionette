
//スコアの計算と表示を担当するクラス(UI実装後表示はそちらに移植)

#include "stdafx.h"
#include "Score.h"
#include "Player.h"

Score::Score()
{
	////プレイヤーの初期位置だけ持ってくる
	//player = FindGO<Player>("player");
	//FirstPlayerPos.z = player->PlayerStartPos.z;
}

Score::~Score()
{

}

void Score::Update()
{
	//スコアの表示(UI実装後コメントアウト)
	wchar_t Score_view[256];
	swprintf_s(Score_view, L"スコア %d", score);
	View.SetText(Score_view);
	View.SetPosition({ -130.0f,500.0f,0.0f });
	View.SetColor(g_vec4White);

	////毎フレーム現在位置を持ってきて
	//player = FindGO<Player>("player");
	//NowPlayerPos.z = player->PlayerPos.z;

	//初期位置と現在位置の差が広がる度にスコア加算
	if (FirstPlayerPos.z < NowPlayerPos.z)
	{
		GetScore += 1;

		//保存している初期位置を現在位置に合わせる
		FirstPlayerPos.z = NowPlayerPos.z;
	}

	//最終的な獲得スコアを実際のスコアに反映
	score += GetScore;
	GetScore = 0;
}

void Score::Render(RenderContext& rc)
{
	//スコアの表示(UI実装後コメントアウト)
	View.Draw(rc);
}