#pragma once
#include "ObjectPool.h"
#include "Bullet.h"

class PlatformManeger;

class Player:public IGameObject
	
{
public:
	Player();
	void Update();
	void Render(RenderContext& rc);
	void PlayerMove();
	void PlayerRotate();
	void ManagePlayerState();
	void PlayerAnimetion();
	void PlayerAttack();
	void PlayerModeChange();

	//メンバ変数。
	ModelRender modelRender;	//モデルレンダ―。
	Vector3 position;			//座標。
	enum EnAnimationClip {		//アニメーション。
		enAnimationClip_Idle,
		enAnimationClip_Walk,
		enAnimationClip_Jump,
		enAnimationClip_Run,
		enAnimationClip_Num,
	};
	AnimationClip animationClips[enAnimationClip_Num];		//アニメーションクリップ。
	CharacterController characterController;  //キャラクターコントローラー。
	Vector3 moveSpeed;		//移動速度。
	Quaternion rotation;  //クォータニオン。
	int playerState = 0;	//プレイヤーのステート(状態)を表す変数。
	Vector3 dushSpeed;		//慣性ジャンプに使うダッシュ速度

	PlatformManeger* platformManeger;

private:
	//音符の種類に応じた発射クールタイム。memo::後で増やすよ
	float noteCoolTime[2];
	//その時のクールタイム。
	float coolTime;

	GameTime gametime;

	//弾用のオブジェクトプール
	ObjectPool<Bullet> m_bulletPool;

	//ボタン。memo::操作感を見ながら調整。
	EnButton attackButton = enButtonRB2;      //攻撃/足場設置ボタン。
	EnButton switchButton = enButtonY;        //モード切替ボタン。

	//モード
	enum PlayerMode
	{
		attack,           //攻撃モード。攻撃ボタンで攻撃できる。
		installation,     //設置モード。位置を決めて足場を設置できる。
	};
	PlayerMode playerMode = attack;

	//音符の形状
	int player_noteIndex = 0;
};

