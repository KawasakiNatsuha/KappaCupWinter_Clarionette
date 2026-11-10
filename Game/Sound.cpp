#include "stdafx.h"
#include "Sound.h"
#include "sound/SoundSource.h"
#include "sound/SoundEngine.h"

Sound::Sound()
{
	//BGM
	g_soundEngine->ResistWaveFileBank(100, "Assets/waveDeta/audiostock_1576968.wav");

	//SE
	g_soundEngine->ResistWaveFileBank(1, "Assets/waveDeta/audiostock_69490.wav");		//ジャンプ
	g_soundEngine->ResistWaveFileBank(2, "Assets/waveDeta/audiostock_634448.wav");		//攻撃
	g_soundEngine->ResistWaveFileBank(3, "Assets/waveDeta/audiostock_1568516.wav");		//敵に命中
	g_soundEngine->ResistWaveFileBank(4, "Assets/waveDeta/audiostock_1173356.wav");		//足場設置
	g_soundEngine->ResistWaveFileBank(5, "Assets/waveDeta/audiostock_844598.wav");		//被弾
	g_soundEngine->ResistWaveFileBank(6, "Assets/waveDeta/audiostock_106598.wav");		//モード切替
	g_soundEngine->ResistWaveFileBank(7, "Assets/waveDeta/audiostock_1074134.wav");		//エネミーフェーズ突入時
	g_soundEngine->ResistWaveFileBank(8, "Assets/waveDeta/audiostock_1074135.wav");		//プレイヤーフェーズ突入時
	g_soundEngine->ResistWaveFileBank(9, "Assets/waveDeta/audiostock_796413.wav");		//フェーズ切り替え三秒前～の時計の音
	g_soundEngine->ResistWaveFileBank(10, "Assets/waveDeta/audiostock_1254810.wav");	//カードをめくる音
}

void Sound::PlayBGM(int number)
{
	SoundSource* bgm = NewGO<SoundSource>(0);
	bgm->Init(number);

	if (number == 100)
	{
		bgm->SetVolume(2.0f);
	}

	bgm->Play(true);
}

void Sound::PlaySE(int number)
{
	SoundSource* SE = NewGO<SoundSource>(0);
	SE->Init(number);

	if (number == 2)
	{
		SE->SetVolume(0.3f);
	}
	if (number == 9)
	{
		SE->SetVolume(2.5f);
	}

	SE->Play(false);
}