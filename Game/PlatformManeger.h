#pragma once

class Player;
class Platform;

class PlatformManeger:public IGameObject
{
public:
	PlatformManeger();
	~PlatformManeger();
	void Update();
	void Render(RenderContext& rc);
	void PlatformInstallation();
	void PlatformManegerInit();
	void Operation();

	//実際に表示するモデルの番号(Playerから渡してもらう)
	int noteGorIndex;

	//足場の出席番号
	int platformIndex;

private:

	//設置予定位置に表示させる半透明モデル(memo::とりあえず一個)
	ModelRender noteGor[3];

	//半透明モデルの位置
	Vector3 noteGorPos;

	//半透明モデルの向き(なんでか初期状態だと立ってるので横にする)
	Quaternion noteRotate;
	Quaternion rotX;
	Quaternion rotY;
	//回転速度
	float rot;

	Player* player;
	Platform* platform;

	//モデルを描画する奥行き
	float depth = 500.0f;

	//足場を近づける&遠ざけるボタン
	EnButton nearButton = enButtonDown;
	EnButton farButton = enButtonUp;

	//足場を回転させるボタン
	EnButton rotButtonR = enButtonRight;
	EnButton rotButtonL = enButtonLeft;
};

