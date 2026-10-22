#include "stdafx.h"
#include "DebugStage.h"

DebugStage::DebugStage()
{
	modelRender.Init("Assets/modelData/stage/stage.tkm");
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
