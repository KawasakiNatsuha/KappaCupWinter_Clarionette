#pragma once

class Enemy:public IGameObject
{
public:
	Enemy();
	~Enemy();
	void Update();
	void Render(RenderContext& rc);
	void OnHit(int damage);
	void IsDie();

	//座標
	Vector3 position = { 50.0f,0.0f,50.0f };
	//当たり判定の範囲
	float collisionRadius = 30.0f;

private:
	enum EnAnimationClip {		//テスト用アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];		//テスト用アニメーションクリップ。
	CharacterController characterController;  //キャラクターコントローラー。

	Quaternion quatenion;
	//テスト用のモデル
	ModelRender testModel;
	Vector3 moveSpeed;

	//HP
	int EnemyHP = 100;

	FontRender fontRender;
	wchar_t font[256];
};

