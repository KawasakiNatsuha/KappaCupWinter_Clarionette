#pragma once
#include "ObjectPool.h"
#include "Bullet.h"

class PlatformManeger;
class FaseTime;
class Enemy;

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
	void Hit(int damege);
	

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

	//モード
	enum PlayerMode
	{
		attack,           //攻撃モード。攻撃ボタンで攻撃できる。
		installation,     //設置モード。位置を決めて足場を設置できる。
	};
	PlayerMode playerMode = attack;
	void PlayerModeChange(PlayerMode playermode);

	int platformIndex = 0;		//足場の出席番号(直前の足場を知るために使う)

private:
	//音符の種類に応じた発射クールタイム。memo::後で増やすよ
	float noteCoolTime[2] =
	{
		0.2,0.05
	};
	//その時のクールタイム。
	float coolTime;
	//音符の種類に応じた攻撃力。
	int noteAttack[2] =
	{
		5,1
	};

	GameTime gametime;

	//弾用のオブジェクトプール
	ObjectPool<Bullet> m_bulletPool;

	//ボタン。memo::操作感を見ながら調整。
	EnButton attackButton = enButtonRB2;		//攻撃ボタン。
	EnButton platformButton = enButtonLB2;		//足場設置ボタン。
	EnButton switchButton = enButtonRB1;        //モード切替ボタン1。
	EnButton switchButton2 = enButtonLB1;		//モード切替ボタン2。

	//音符の形状
	int player_noteIndex = 0;

	FaseTime* faseTime;

	bool isJump = false;		//二段ジャンプ用

	Vector3 buckPosition;		//落下したとき戻る座標

	int fallCount = 0;			//連続落下した回数(3回以上で初期の足場に戻る)

	int HP = 100;				//playerのHP
};

