#pragma once
class UI:public IGameObject
{
public:
	UI();
	~UI();
	void Update();
	void Render(RenderContext& rc);
	void InitUI();		//画像読み込みをコンストラクタですると絶対ごちゃつくのでここで代用。
private:
	/// <summary>
	/// インゲーム中のUI。0は画面全体の固定UI、1は自分のHPバー、2は敵のHPバー。
	/// </summary>
	SpriteRender InGameUI[3];
	/// <summary>
	/// ゲーム内フォント。とりあえず数字だけ。配列の数字と実際の画像の数字は一致させること。
	/// </summary>
	SpriteRender NumberFont[10];
};

