#pragma once
class DebugStage : public IGameObject
{
public:
	DebugStage();
	~DebugStage();

	void Render(RenderContext& rc);

	ModelRender modelRender;
	ModelRender kabe;
	PhysicsStaticObject physicsStaticObject;
};

