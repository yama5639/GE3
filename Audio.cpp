#include "Audio.h"
#include <fstream>

#include <cassert>

#pragma comment(lib,"xaudio2.lib")

bool Audio::Initialize() {
	HRESULT result;
	result = XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if FAILED(result) {
		assert(0);
		return false;
	}
	result = xAudio2->CreateMasteringVoice(&masterVoice);
	if FAILED(result) {
		assert(0);
		return false;
	}
	return true;
}

//Žn‚ß‚é
void Audio::PlayWave(const char* filename) {
	HRESULT result;
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	if (file.fail()) {
		assert(0);
	}
	RiffHeader riff;
	file.read((char*)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}

	FormatChunk format;
	file.read((char*)&format, sizeof(format));
	Chunk data;
	file.read((char*)&data, sizeof(data));
	char* pBuffer = new char[data.size];
	file.read(pBuffer, data.size);

	//•Â‚¶‚é
	file.close();

	WAVEFORMATEX wfex{};
	memcpy(&wfex, &format.fmt, sizeof(format.fmt));
	wfex.wBitsPerSample = format.fmt.nBlockAlign * 8 / format.fmt.nChannels;

	//SourceVoice‚Ì¶¬
	result = xAudio2->CreateSourceVoice(&pSourceVoice, &wfex, 0, 2.0f, &voiceCallback);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = (BYTE*)pBuffer;
	buf.pContext = pBuffer;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	buf.AudioBytes = data.size;

	result = pSourceVoice->SubmitSourceBuffer(&buf);
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
	result = pSourceVoice->Start();
	if FAILED(result) {
		delete[] pBuffer;
		assert(0);
		return;
	}
}

//Ž~‚ß‚é
void Audio::StopWave() {
	HRESULT result;
	if (pSourceVoice == nullptr) { return; } else
	{
		result = pSourceVoice->Stop();
	}
}