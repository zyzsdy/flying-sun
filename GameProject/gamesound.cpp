//GameSound类成员函数的定义
//DirectSound
#include "stdafx.h"

void GameSound::GameSoundInit(HWND hwnd)
{
	this->pDS;
	this->soundhwnd = hwnd;
	this->result = DirectSoundCreate(NULL, &pDS, NULL);
	if (this->result != DS_OK)
		MessageBox(hwnd, L"建立 DirectSound 对象失败！", NULL, MB_OK);

	this->result = this->pDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (this->result != DS_OK)
		MessageBox(hwnd, L"设定程序协调层级失败！", NULL, MB_OK);

	this->GameSoundbufferConstruct();
}

void GameSound::GameSoundbufferConstruct()
{

	memset(&this->desc, 0, sizeof(desc));   //清空结构内容
	desc.dwSize = sizeof(desc);             //配制描述结构大小
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	desc.dwBufferBytes = 0;
	desc.lpwfxFormat = NULL;
	result = pDS->CreateSoundBuffer(&desc, &this->pMainBuf, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"建立主缓冲区域失败！", NULL, MB_OK);

	//子缓冲区资源载入
	this->GameSoundReadinbuffer(this->pMusic_Title, L"sounds//m_title.wav");
	this->GameSoundReadinbuffer(this->pMusic_Game, L"sounds//m_gmrun.wav");
	this->GameSoundReadinbuffer(this->pSound_Bullet, L"sounds//s_bullet.wav");
	this->GameSoundReadinbuffer(this->pSound_Gameover, L"sounds//s_gameover.wav");
	this->GameSoundReadinbuffer(this->pSound_Hint, L"sounds//s_hint.wav");
	this->GameSoundReadinbuffer(this->pSound_Myhint, L"sounds//s_myhint.wav");
	this->GameSoundReadinbuffer(this->pSound_Mouseclick, L"sounds//us_mouseclick.wav");
	this->GameSoundReadinbuffer(this->pSound_Mouseover, L"sounds//us_mouseover.wav");
}

void GameSound::GameSoundfmtSet(int channels, int SamplesPerSec, int wBitPerSample)
{
	memset(&this->pwfmt, 0, sizeof(pwfmt));
	this->pwfmt.wFormatTag = WAVE_FORMAT_PCM;
	this->pwfmt.nChannels = channels;
	this->pwfmt.nSamplesPerSec = SamplesPerSec;
	this->pwfmt.wBitsPerSample = wBitPerSample;
	this->pwfmt.nBlockAlign = this->pwfmt.wBitsPerSample / 8 * this->pwfmt.nChannels;
	this->pwfmt.nAvgBytesPerSec = this->pwfmt.nSamplesPerSec * this->pwfmt.nBlockAlign;
	this->result = this->pMainBuf->SetFormat(&this->pwfmt);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"设定播放格式失败！", NULL, MB_OK);
}

void GameSound::GameSoundReadWAVfile(LPWSTR filename, HMMIO &hmmbackground)
{
	hmmbackground = mmioOpen(filename, NULL, MMIO_ALLOCBUF | MMIO_READ);  //打开文件
	if (hmmbackground == NULL)
		MessageBox(this->soundhwnd, L"文件不存在！", NULL, MB_OK);

	//搜索类型
	ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');//设定文件类型
	mmresult = mmioDescend(hmmbackground, &ckRiff, NULL, MMIO_FINDRIFF);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"文件格式错误！", NULL, MB_OK);

	//搜索区块
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');//设定区块类型
	mmresult = mmioDescend(hmmbackground, &ckInfo, &ckRiff, MMIO_FINDCHUNK);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"文件格式错误！", NULL, MB_OK);
	if (mmioRead(hmmbackground, (HPSTR)&swfmt, sizeof(swfmt)) == -1)
		MessageBox(this->soundhwnd, L"读取格式失败！", NULL, MB_OK);

	mmresult = mmioAscend(hmmbackground, &ckInfo, 0);   //跳出子区块

	//搜索区块
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmresult = mmioDescend(hmmbackground, &ckInfo, &ckRiff, MMIO_FINDCHUNK);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"文件格式错误!", NULL, MB_OK);

	size = ckInfo.cksize;

}

void GameSound::GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER& buffer, LPWSTR filename)
{
	LPVOID pAudio;
	DWORD bytesAudio;


	this->GameSoundReadWAVfile(filename, this->hbackground);

	memset(&this->desc, 0, sizeof(desc));   //清空结构内容
	desc.dwSize = sizeof(desc);             //配制描述结构大小
	desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;   //???
	desc.dwBufferBytes = this->size;
	desc.lpwfxFormat = &this->swfmt;
	result = pDS->CreateSoundBuffer(&desc, &buffer, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"建立次缓冲区域失败！", NULL, MB_OK);



	result = buffer->Lock(0, this->size, &pAudio, &bytesAudio, NULL, NULL, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"锁定缓冲区失败!", NULL, MB_OK);

	this->mmresult = mmioRead(this->hbackground, (HPSTR)pAudio, bytesAudio);

	if (mmresult == -1)
		MessageBox(this->soundhwnd, L"读取声音文件资料失败", NULL, MB_OK);

	this->result = buffer->Unlock(pAudio, bytesAudio, NULL, NULL);

	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"解除锁定缓冲区失败!", NULL, MB_OK);

	mmioClose(this->hbackground, 0);
}

void GameSound::GameSoundAllstop()
{
	this->pMusic_Title->Stop();
	this->pMusic_Game->Stop();
	this->pSound_Bullet->Stop();
	this->pSound_Gameover->Stop();
	this->pSound_Hint->Stop();
	this->pSound_Mouseclick->Stop();
	this->pSound_Mouseover->Stop();
	this->pSound_Myhint->Stop();
	this->pMusic_Title->SetCurrentPosition(0);
	this->pMusic_Game->SetCurrentPosition(0);
	this->pSound_Bullet->SetCurrentPosition(0);
	this->pSound_Gameover->SetCurrentPosition(0);
	this->pSound_Hint->SetCurrentPosition(0);
	this->pSound_Mouseclick->SetCurrentPosition(0);
	this->pSound_Mouseover->SetCurrentPosition(0);
	this->pSound_Myhint->SetCurrentPosition(0);
}

void GameSound::GameMusicplay(LPDIRECTSOUNDBUFFER& buffer)
{
	this->GameSoundAllstop();
	buffer->Play(0, 0, 1);
}

void GameSound::GameSoundplay(LPDIRECTSOUNDBUFFER& buffer)
{
	buffer->Play(0, 0, 0);
}