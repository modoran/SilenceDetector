#pragma once

#include "resource.h"
#include "stdafx.h"

//BOOL ParseWavFile(LPCTSTR wav, HWND hwnd);
void Thread(LPVOID);
//unsigned int ProcessVawChunk (FILE* fp, char* buf, unsigned int chunk_size);
std::basic_string<TCHAR>& FormatAudioTime(double duration);

typedef struct tWAVEFORMATEX {
	WORD wFormatTag;
	WORD nChannels;
	DWORD nSamplesPerSec;
	DWORD nAvgBytesPerSec;
	WORD nBlockAlign;
	WORD wBitsPerSample;
	WORD cbSize;
	} WAVEFORMATEX, *PWAVEFORMATEX, *LPWAVEFORMATEX;

typedef struct
	{
	HWND hwndEdit ;
	std::basic_string<TCHAR> wav ;
	BOOL bKill ;
	HWND hOPenButton ;
	HWND hparent ;
	HMENU hMenu ;
	}
PARAMS, *PPARAMS ;

typedef struct {
	unsigned int volume;
	unsigned int time;
	}
CONFIG, *PCONFIG; 
