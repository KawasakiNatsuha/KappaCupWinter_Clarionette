#pragma once

class Platform;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Blinking(float interval, ModelRender& model, RenderContext& rc);
private:

	//お試し
	ModelRender modelRender;
	enum EnAnimationClip {		//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[1];

	GameTime gameTime;
	float deltaTime;		//1フレームの経過時間(点滅処理用)
	bool isDrow;			//足場を描画するかしないか(点滅処理用)

	Platform* platform;
};

