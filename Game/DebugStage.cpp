#include "stdafx.h"
#include "DebugStage.h"

DebugStage::DebugStage()
{
	modelRender.Init("Assets/modelData/kabe.tkm");
	modelRender.SetScale(1.0f, 1.0f, 5.0f);
	modelRender.Update();
	physicsStaticObject.CreateFromModel(modelRender.GetModel(), modelRender.GetModel().GetWorldMatrix());
}

DebugStage::~DebugStage()
{

}

void DebugStage::Render(RenderContext& rc)
{
	modelRender.Draw(rc);
}
