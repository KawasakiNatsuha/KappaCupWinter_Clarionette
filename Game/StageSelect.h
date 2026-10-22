#pragma once

class Game;

class StageSelect:public IGameObject
{
public:
	StageSelect();
	~StageSelect();
	void Update();
	void Render(RenderContext& rc);
private:
	
};

