#pragma once

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
private:

	//お試し
	ModelRender modelRender;
};

