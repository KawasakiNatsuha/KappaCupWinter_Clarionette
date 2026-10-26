#pragma once
class FaseTime:public IGameObject
{
public:
	FaseTime();
	~FaseTime();
	void Update();
	void Render(RenderContext& rc);
private:
	FontRender timeFont;
	GameTime gameTime;
	wchar_t font[256];
	int faseTimer = 10;
	float deltaTime;
};

