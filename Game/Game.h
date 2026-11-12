#pragma once

class Platform;
class Player;
class GameCamera;
class DebugStage;
class UI;
class Enemy;
class FaseTime;
class Bullet;
class EnemyBullet;
class Sound;

class Game : public IGameObject
{
public:
	Game() {}
	~Game() {}
	bool Start();
	void Update();
	void Render(RenderContext& rc);
	void Blinking(float interval, ModelRender& model, RenderContext& rc);
	void ReSet();
	void IsPlaySound(int numbers);

	Sound* sound;
	bool isInGame = false;	//ゲーム中かそうでないか

private:
	GameTime gameTime;
	float deltaTime;		//1フレームの経過時間(点滅処理用)
	bool isDrow;			//足場を描画するかしないか(点滅処理用)

	Platform* platform;
	Player* player;
	GameCamera* gameCamera;
	DebugStage* debugStage;
	UI* ui;
	Enemy* enemy;
	FaseTime* faseTime;
	Bullet* bullet;
};

