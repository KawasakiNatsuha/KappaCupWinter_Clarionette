#pragma once
#include <random>

class Player;

class FaseTime:public IGameObject
{
public:
	FaseTime();
	~FaseTime();
	void Update();
	void Render(RenderContext& rc);

	enum Fase
	{
		attackFase,
		evadeFase,
	};
	Fase nowFase = attackFase;		//現在のフェーズ
	int TurnCount = 0;		//経過ターン
	int faseTimer = 15;     //制限時間(変わる方)

private:
	FontRender timeFont;
	GameTime gameTime;
	wchar_t font[256];		//残り時間表示用フォント
	int firstTime = 15;     //制限時間(変わらない方)
	float deltaTime;		//1フレームの経過時間
	Player* player;			//プレイヤー(フェーズ切り替え時に攻撃モードに戻すのに使う)
	
	std::mt19937 gen;		//乱数
	std::uniform_int_distribution<int> notemode;		//音符の種類抽選
};

