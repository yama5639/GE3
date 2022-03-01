#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

/// </summary>
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// ボイス処理パスの開始時
	//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// ボイス処理パスの終了時
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// バッファストリームの再生が終了した時
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// バッファの使用開始時
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// バッファの末尾に達した時
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// バッファを解放する
		delete[] pBufferContext;
	};
	// 再生がループ位置に達した時
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// ボイスの実行エラー時
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

class Audio
{
private: // エイリアス
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct Chunk
	{
		// チャンク毎のID
		char	id[4];
		// チャンクサイズ
		int		size;
	};
	struct RiffHeader
	{
		// "RIFF"
		Chunk	chunk;
		// "RIFF"
		char	type[4];
	};
	struct FormatChunk
	{
		// "fmt "
		Chunk		chunk;
		// 波形フォーマット
		WAVEFORMAT	fmt;
	};
	// メンバ関数
public:

	bool Initialize();
	void PlayWave(const char* filename);
	void StopWave();

	// メンバ変数
private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallback voiceCallback;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};