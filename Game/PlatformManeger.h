#pragma once

class Player;

class PlatformManeger:public IGameObject
{
public:
	PlatformManeger();
	~PlatformManeger();
	void Update();
	void Render(RenderContext& rc);

private:

	//設置予定位置に表示させる半透明モデル(memo::とりあえず一個)
	ModelRender noteGor[1];

	//実際に表示するモデルの番号(Playerから渡してもらう)
	int noteGorIndex;

	//半透明モデルの位置
	Vector3 noteGorPos;

	Player* player;
};

