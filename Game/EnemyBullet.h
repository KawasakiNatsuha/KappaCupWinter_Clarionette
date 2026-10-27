#pragma once

class Player;

class EnemyBullet :public IGameObject
{
public:
	EnemyBullet();
	~EnemyBullet();
	void Update();
	void Render(RenderContext& rc);
	void BulletMove();
	void Fire(const Vector3& pos, const Vector3& dir, int attack);
	void Hit();

	//移動する方向
	Vector3 bulletMoveVec;
	//弾の座標
	Vector3 bulletPos;
	//弾の移動速度
	float bulletMoveSpeed = 550.0f;

private:
	//デバッグ用のモデル
	ModelRender debugBullet;

	GameTime gameTime;

	// 弾が飛んだ距離の上限
	float m_maxDistance = 3000.0f;

	// 発射地点
	Vector3 m_startPos;

	//敵との衝突判定用
	Player* player;

	//攻撃力
	int Attack = 5;
};


