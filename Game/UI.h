#pragma once

class Player;
class Enemy;
class FaseTime;

class UI:public IGameObject
{
public:
	UI();
	~UI();
	void Update();
	void Render(RenderContext& rc);
	void GageUpdate();					//HPゲージの更新
	void FontUI(RenderContext& rc);		//数字フォントの表示
	void ChangePhase();

	//画像読み込みをコンストラクタですると絶対ごちゃつくのでここで代用。
	void InitUI();

	/// <summary>
	/// エフェクトの再生。
	/// </summary>
	/// <param name="effectNumber">再生したいエフェクトの番号。1=敵に音符が当たった際のエフェクト</param>
	/// <param name="position">エフェクトを表示する座標</param>
	void PlayEffects(int effectNumber, Vector3 position);

private:
	/// インゲーム中のUI。0は画面全体の固定UI、1は自分のHPバー、2は敵のHPバー。
	SpriteRender InGameUI[3];
	//フェーズ切り替えの際のUI。0はプレイヤーの攻撃フェーズ、1は敵の攻撃フェーズに切り替わる際の画像
	SpriteRender ChangePhaseUI[2];
	//フェーズをわかりやすく表すためのUI。0はプレイヤー、1は敵のフェーズ時にうっすら表示するフィルター的なもの
	SpriteRender PhaseUI[2];
	/// ゲーム内フォント。とりあえず数字だけ。配列の数字と実際の画像の数字は一致させること。
	SpriteRender NumberFont[11];

	//ここからエフェクト
	Effect HitEfeccts;

	FaseTime* faseTime;

	//フェーズ切り替え演出中かどうか
	bool IsChange = false;
};

