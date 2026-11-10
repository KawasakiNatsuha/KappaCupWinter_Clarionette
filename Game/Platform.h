#pragma once
#include "Game.h"

class FaseTime;
class Bullet;
class EnemyBullet;

//足場のオブジェクトクラス(インスタンスで増殖する方)
class Platform:public IGameObject
{
public:
	Platform();
	~Platform();
	void Update();
	void Render(RenderContext& rc);
	void InitPlatform();
	void Hit(int Index);
	void Init();

	//この足場の座標
	Vector3 notePos = { 0.0f,0.0f,0.0f };

	//実際に表示するモデル番号(配列からモデルを表示させるのに使う)
	int noteIndex;

	//回転
	Quaternion noteRotate;

	//足場の出席番号(半透明モデルから渡してもらう)
	int Index;

private:
	//足場のモデル
	ModelRender note[5];

	PhysicsStaticObject noteObject;
	FaseTime* faseTime;
	Bullet* bullet;
	EnemyBullet* enemyBullet;
	Game game;

	int crieteFase;		//作られたフェーズ
	int dethFase = 5;		//足場の寿命
	int platformHP = 50;	//四分休符の耐久値
	bool isInit = false;	//初期化が終わったかどうか
};

