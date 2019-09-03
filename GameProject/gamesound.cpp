//GameSound���Ա�����Ķ���
//DirectSound
#include "stdafx.h"

void GameSound::GameSoundInit(HWND hwnd)
{
	this->pDS;
	this->soundhwnd = hwnd;
	this->result = DirectSoundCreate(NULL, &pDS, NULL);
	if (this->result != DS_OK)
		MessageBox(hwnd, L"���� DirectSound ����ʧ�ܣ�", NULL, MB_OK);

	this->result = this->pDS->SetCooperativeLevel(hwnd, DSSCL_PRIORITY);
	if (this->result != DS_OK)
		MessageBox(hwnd, L"�趨����Э���㼶ʧ�ܣ�", NULL, MB_OK);

	this->GameSoundbufferConstruct();
}

void GameSound::GameSoundbufferConstruct()
{

	memset(&this->desc, 0, sizeof(desc));   //��սṹ����
	desc.dwSize = sizeof(desc);             //���������ṹ��С
	desc.dwFlags = DSBCAPS_PRIMARYBUFFER;
	desc.dwBufferBytes = 0;
	desc.lpwfxFormat = NULL;
	result = pDS->CreateSoundBuffer(&desc, &this->pMainBuf, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"��������������ʧ�ܣ�", NULL, MB_OK);

	//�ӻ�������Դ����
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
		MessageBox(this->soundhwnd, L"�趨���Ÿ�ʽʧ�ܣ�", NULL, MB_OK);
}

void GameSound::GameSoundReadWAVfile(LPWSTR filename, HMMIO &hmmbackground)
{
	hmmbackground = mmioOpen(filename, NULL, MMIO_ALLOCBUF | MMIO_READ);  //���ļ�
	if (hmmbackground == NULL)
		MessageBox(this->soundhwnd, L"�ļ������ڣ�", NULL, MB_OK);

	//��������
	ckRiff.fccType = mmioFOURCC('W', 'A', 'V', 'E');//�趨�ļ�����
	mmresult = mmioDescend(hmmbackground, &ckRiff, NULL, MMIO_FINDRIFF);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"�ļ���ʽ����", NULL, MB_OK);

	//��������
	ckInfo.ckid = mmioFOURCC('f', 'm', 't', ' ');//�趨��������
	mmresult = mmioDescend(hmmbackground, &ckInfo, &ckRiff, MMIO_FINDCHUNK);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"�ļ���ʽ����", NULL, MB_OK);
	if (mmioRead(hmmbackground, (HPSTR)&swfmt, sizeof(swfmt)) == -1)
		MessageBox(this->soundhwnd, L"��ȡ��ʽʧ�ܣ�", NULL, MB_OK);

	mmresult = mmioAscend(hmmbackground, &ckInfo, 0);   //����������

	//��������
	ckInfo.ckid = mmioFOURCC('d', 'a', 't', 'a');
	mmresult = mmioDescend(hmmbackground, &ckInfo, &ckRiff, MMIO_FINDCHUNK);
	if (mmresult != MMSYSERR_NOERROR)
		MessageBox(this->soundhwnd, L"�ļ���ʽ����!", NULL, MB_OK);

	size = ckInfo.cksize;

}

void GameSound::GameSoundReadinbuffer(LPDIRECTSOUNDBUFFER& buffer, LPWSTR filename)
{
	LPVOID pAudio;
	DWORD bytesAudio;


	this->GameSoundReadWAVfile(filename, this->hbackground);

	memset(&this->desc, 0, sizeof(desc));   //��սṹ����
	desc.dwSize = sizeof(desc);             //���������ṹ��С
	desc.dwFlags = DSBCAPS_STATIC | DSBCAPS_CTRLPAN |
		DSBCAPS_CTRLVOLUME | DSBCAPS_GLOBALFOCUS;   //???
	desc.dwBufferBytes = this->size;
	desc.lpwfxFormat = &this->swfmt;
	result = pDS->CreateSoundBuffer(&desc, &buffer, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"�����λ�������ʧ�ܣ�", NULL, MB_OK);



	result = buffer->Lock(0, this->size, &pAudio, &bytesAudio, NULL, NULL, NULL);
	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"����������ʧ��!", NULL, MB_OK);

	this->mmresult = mmioRead(this->hbackground, (HPSTR)pAudio, bytesAudio);

	if (mmresult == -1)
		MessageBox(this->soundhwnd, L"��ȡ�����ļ�����ʧ��", NULL, MB_OK);

	this->result = buffer->Unlock(pAudio, bytesAudio, NULL, NULL);

	if (this->result != DS_OK)
		MessageBox(this->soundhwnd, L"�������������ʧ��!", NULL, MB_OK);

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