#pragma once

//足場のオブジェクトクラス(インスタンスで増殖する方)
class Platform:public IGameObject
{
public:
	Platform();
	~Platform();
	void Update();
	void Render(RenderContext& rc);

private:
	//足場のモデル(memo::とりあえず1個)
	ModelRender note[1];

	//実際に表示するモデル番号(配列からモデルを表示させるのに使う)
	int noteIndex;

	//この足場の座標
	Vector3 notePos;
};

