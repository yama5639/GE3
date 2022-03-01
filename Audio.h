#pragma once

#include <Windows.h>
#include <xaudio2.h>
#include <wrl.h>

/// </summary>
class XAudio2VoiceCallback : public IXAudio2VoiceCallback
{
public:
	// �{�C�X�����p�X�̊J�n��
	//STDMETHOD_(void, OnVoiceProcessingPassStart) (THIS_ UINT32 BytesRequired) {};
	void OnVoiceProcessingPassStart(UINT32 BytesRequired) {};
	// �{�C�X�����p�X�̏I����
	STDMETHOD_(void, OnVoiceProcessingPassEnd) (THIS) {};
	// �o�b�t�@�X�g���[���̍Đ����I��������
	STDMETHOD_(void, OnStreamEnd) (THIS) {};
	// �o�b�t�@�̎g�p�J�n��
	STDMETHOD_(void, OnBufferStart) (THIS_ void* pBufferContext) {};
	// �o�b�t�@�̖����ɒB������
	STDMETHOD_(void, OnBufferEnd) (THIS_ void* pBufferContext) {
		// �o�b�t�@���������
		delete[] pBufferContext;
	};
	// �Đ������[�v�ʒu�ɒB������
	STDMETHOD_(void, OnLoopEnd) (THIS_ void* pBufferContext) {};
	// �{�C�X�̎��s�G���[��
	STDMETHOD_(void, OnVoiceError) (THIS_ void* pBufferContext, HRESULT Error) {};
};

class Audio
{
private: // �G�C���A�X
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	struct Chunk
	{
		// �`�����N����ID
		char	id[4];
		// �`�����N�T�C�Y
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
		// �g�`�t�H�[�}�b�g
		WAVEFORMAT	fmt;
	};
	// �����o�֐�
public:

	bool Initialize();
	void PlayWave(const char* filename);
	void StopWave();

	// �����o�ϐ�
private:
	ComPtr<IXAudio2> xAudio2;
	IXAudio2MasteringVoice* masterVoice;
	XAudio2VoiceCallback voiceCallback;

	IXAudio2SourceVoice* pSourceVoice = nullptr;

};