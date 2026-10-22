#pragma once

class Result:public IGameObject
{
public:
	Result();
	void Update();
	void Render(RenderContext& rc);
private:
};

