#pragma once
#include "Game.h"

class FaseTime;

//足場のオブジェクトクラス(インスタンスで増殖する方)
class Platform:public IGameObject
{
public:
	Platform();
	~Platform();
	void Update();
	void Render(RenderContext& rc);
	void InitPlatform();

	//この足場の座標
	Vector3 notePos;

	//実際に表示するモデル番号(配列からモデルを表示させるのに使う)
	int noteIndex;

	//回転
	Quaternion noteRotate;

	//足場の出席番号(半透明モデルから渡してもらう)
	int Index;

private:
	//足場のモデル(memo::とりあえず1個)
	ModelRender note[1];

	PhysicsStaticObject noteObject;
	FaseTime* faseTime;
	Game game;

	int crieteFase;		//作られたフェーズ
	int dethFase = 5;		//足場の寿命
};

