#pragma once

class UI;

class Result:public IGameObject
{
public:
	Result();
	void Update();
	void Render(RenderContext& rc);
private:
	SpriteRender back[4];	//背景の黒画像、垂れ幕右、垂れ幕左、垂れ幕上
	SpriteRender sentence;  //表示する白文字
	SpriteRender next;		//不透明度で点滅させる文字
	int turn;				//累計経過ターン数
	float coler;			//不透明度
	bool PorM;				//不透明度が上がるか下がるか
};

