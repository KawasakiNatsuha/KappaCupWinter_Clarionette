#pragma once

class Player;
class Enemy;
class FaseTime;
class Game;

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
	void ChangeAction();
	void GameOber(RenderContext& rc);
	void Init();

	//画像読み込みをコンストラクタですると絶対ごちゃつくのでここで代用。
	void InitUI();

	/// <summary>
	/// エフェクトの再生。
	/// </summary>
	/// <param name="effectNumber">再生したいエフェクトの番号。1=敵に音符が当たった際のエフェクト/2=ステージの限界点に到達した際のエフェクト</param>
	/// <param name="position">エフェクトを表示する座標</param>
	/// <param name="rotate">座標限界の時のみ使う。1でX軸回転、2でY軸回転となる</param>
	void PlayEffects(int effectNumber, Vector3 position, int rotate = 0);

	/// <summary>
	/// カード抽選演出。
	/// </summary>
	void CardLottery();

	//フェーズ切り替え演出中かどうか
	bool IsChange = false;

	int PhaseIndex;

private:
	//インゲーム中のUI。0は画面全体の固定UI、1は自分のHPバー、2は敵のHPバー、3はHPバーの背景、4は照準。
	SpriteRender InGameUI[5];
	//現在の音符を表すUI。player、platform等の配列番号と一致している。
	SpriteRender noteUI[5];
	//フェーズ切り替えの際のUI。0はプレイヤーの攻撃フェーズ、1は敵の攻撃フェーズに切り替わる際の画像
	SpriteRender ChangePhaseUI[2];
	//フェーズをわかりやすく表すためのUI。0はプレイヤー、1は敵のフェーズ時にうっすら表示するフィルター的なもの
	SpriteRender PhaseUI[2];
	// ゲーム内フォント。とりあえず数字だけ。配列の数字と実際の画像の数字は一致させること。
	SpriteRender NumberFont[11];
	//抽選時のカード。EXは裏面。
	SpriteRender CardKind[3][6];
	//抽選時の背景と選択肢。1=背景、2=選択時のフレーム
	SpriteRender ChooseTime[2];
	//ゲームオーバー時のフェードインに使う画像。1=背景、2=常に表示させる画像、3=カーソル
	SpriteRender GameOberUI[3];
	//プレイヤーのモードを可視化するためのUI。
	SpriteRender PlayerModeUI[3];
	
	//カードの座標。
	Vector3 CardPos[3];
	enum CardStep
	{
		move,		//上から降りてくる
		stop,		//止まって裏返る
		choose,		//選んでもらう
		fadeOut		//選択後、消える
	};
	CardStep State;

	//ここからエフェクト
	Effect HitEfeccts;
	Effect RimitEffects;

	//エフェクトの角度
	Quaternion EfectRotate;

	Vector3 position;					//フェーズ切り替え演出の座標
	float startPosition = 1000.0f;		//フェーズ切り替え時の画像の初期座標
	float endPosition = 0.0f;			//画像の終了時の座標
	float easingRadio;					//お試し(多分うまくいってない)
	float stopTime = 1.5f;				//フェーズ切り替え演出の停止時間
	bool faseChanged = false;			//フェーズ切り替え演出中か
	Quaternion CardRot;					//カード回転演出のためのクォータニオン
	float rot;							//カード回転演出の角度
	bool StopCard[3] = { false };		//カードが止まったかどうか
	int cardCoose = 0;					//現在選んでいるカードの番号
	float deltaTime = 0.0f;				//1フレームの経過時間
	float coloer = 0.0f;				//フェードインのための不透明度
	Vector3 carPos;						//ゲームオーバー時のカーソルの座標
	bool Reset = false;

	FaseTime* faseTime;
	Game* game;
	Player* player;
};

