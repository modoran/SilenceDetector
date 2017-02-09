// VawParser.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VawParser.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;
TCHAR szFile[MAX_PATH] = {0};// current instance
HWND hedit;
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name
static std::string sResult;

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	return DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, (DIALOGPROC)DialogProc);
	
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_VAWPARSER, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_VAWPARSER));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return (int) msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_VAWPARSER));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= MAKEINTRESOURCE(IDC_VAWPARSER);
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      /*CW_USEDEFAULT*/100, 150, /*CW_USEDEFAULT*/650, 300, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	static HWND hOpenButton;
	static HWND hEdit;

	switch (message)	
	{
	case WM_CREATE:
		{

		
		HWND hStaticText = CreateWindow(_T("STATIC"),
			_T("Press the button below to start analyzing .wav file or just drag && drop here from explorer. "),
			WS_VISIBLE | WS_CHILD,
			10,
			10,
			600,
			30,
			hWnd,
			NULL,
			hInst,
			NULL);
		hEdit = CreateWindow(_T("EDIT"),
			_T(""),
			WS_VISIBLE | WS_CHILD | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL | ES_WANTRETURN,
			200,
			50,
			400,
			180,
			hWnd,
			NULL,
			hInst,
			NULL);
			hOpenButton = CreateWindow(_T("BUTTON"),
				_T("Open ..."),
				WS_VISIBLE | WS_CHILD,
				50,
				50,
				100,
				50,
				hWnd,
				NULL,
				hInst,
				NULL);

			break;
		}
	case WM_COMMAND:
		{
		wmId    = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		if ((HWND)lParam == hOpenButton) {
			OPENFILENAME ofn;
			static TCHAR szFileName[MAX_PATH] = _T("");
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = _T("WAV Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0");
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = _T("wav");
			ofn.lpstrTitle  = _T("Choose WAV PCM file to open");
			if(GetOpenFileName(&ofn))
				{
				// Do something usefull with the filename stored in szFileName
				
				//0hedit = hEdit;
				//_tcscpy(szFile, szFileName);
				//szFile = szFileName;
				static PARAMS pp ;
				memset (&pp, 0, sizeof (PARAMS));
				pp.hwndEdit = hEdit;
				pp.wav = szFileName;
				pp.hOPenButton = hOpenButton;
				pp.hparent = hWnd;
				//MessageBox(hWnd, pp.wav, _T("Main thread"), MB_OK);
				EnableWindow(hOpenButton, FALSE );
				_beginthread(Thread, 0, &pp);
				//ParseWavFile(szFileName, hEdit);
				//SendMessage(hEdit, WM_SETTEXT, NULL, (LPARAM)_T("TEST"));
				/*SetWindowText(hEdit, _T("bnhhjkhjh"));
				std::basic_stringstream<TCHAR>ss;
				ss << GetLastError();
				MessageBox(hWnd, ss.str().c_str(), NULL, MB_OK);*/
				}
		
		}

		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		
		break;
		}
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		// TODO: Add any drawing code here...
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		{
		RECT rc, rcDlg, rcOwner; 
		HWND hWnd = GetParent(hDlg);
		GetWindowRect(hWnd, &rcOwner); 
		GetWindowRect(hDlg, &rcDlg); 
		CopyRect(&rc, &rcOwner); 

		// Offset the owner and dialog box rectangles so that right and bottom 
		// values represent the width and height, and then offset the owner again 
		// to discard space taken up by the dialog box. 

		OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top); 
		OffsetRect(&rc, -rc.left, -rc.top); 
		OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom); 

		// The new position is the sum of half the remaining space and the owner's 
		// original position. 

		SetWindowPos(hDlg, 
			HWND_TOP, 
			rcOwner.left + (rc.right / 2), 
			rcOwner.top + (rc.bottom / 2), 
			0, 0,          // Ignores size arguments. 
			SWP_NOSIZE); 



		///////************ get memory status ************///////////
		MEMORYSTATUSEX statex;
		statex.dwLength = sizeof (statex);
		GlobalMemoryStatusEx (&statex);
		std::basic_stringstream<TCHAR> ss;
		ss << _T("Memory: ") << statex.ullAvailPhys / 1024 << _T(" MB/") << statex.ullTotalPhys / 1024 << _T(" MB (") << statex.dwMemoryLoad << _T("% used)");
		SendMessage(GetDlgItem(hDlg, IDC_STATIC5), WM_SETTEXT, NULL, (WPARAM)ss.str().c_str());

		BOOL b64 = 0;
		IsWow64Process(GetCurrentProcess(), &b64);
		std::basic_stringstream<TCHAR> ss2;
		ss2 << _T("Program build on ") << _T(__DATE__) << _T(", ") << _T(__TIME__) << _T(" (") << (b64 ? _T("X64") : _T("X86")) << _T(")");
		SendMessage(GetDlgItem(hDlg, IDC_STATIC2), WM_SETTEXT, NULL, (WPARAM)ss2.str().c_str());
		
		static CHyperLink myUrl;
		myUrl.ConvertStaticToHyperlink(GetDlgItem(hDlg, IDC_STATIC4), _T("mailto:modoran.george@gmail.com"));
		return (INT_PTR)TRUE;
		}
		

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

/*
BOOL ParseWavFile(LPCTSTR wav, HWND hwnd){
	

	std::basic_stringstream<TCHAR> ss;
	FILE* fp = _tfopen(wav, _T("rb"));
	std::ifstream ssss(wav, std::ios::binary);
	char *test = new char[32768];
	ssss.read(test, 32768);
	std::ofstream sssss("test.bin", std::ios::binary);
	sssss.write(test, 32768);
	sssss.close();
	ssss.close();
	delete [] test;
	if(fp == NULL){
		MessageBoxA(NULL, "File could not be opened.", NULL, MB_ICONERROR);
		ExitProcess(255);
		}
	//char* buf = (char*)GlobalAlloc(GPTR, 32);
	void* buf = malloc(32);
	memset (buf, 0, 32);
	//ZeroMemory(buf, 32);
	fseek(fp, 0, SEEK_SET);
	fseek(fp, 4, SEEK_CUR);
	fread(&buf, 1, 4, fp);
	
	ss << "Opening '" << wav << "' for analyzing ... \r\n\r\n";

	ss << "Parsing file header chunks ...\r\n\r\n";
	
	ss << "ChunkSize: " << (unsigned int)buf;


	fseek(fp, 16, SEEK_SET);
	fread(&buf, 1, 4, fp);
	ss << "\r\nChunk1Size: " << (unsigned int)buf;
	unsigned int pcm = (unsigned int)buf;


	fseek(fp, 20, SEEK_SET);
	fread(&buf, 1, 2, fp);

	ss << "\r\nAudio Format: " << (unsigned short)buf;
	fseek(fp, 22, SEEK_SET);
	fread(&buf, 1, 2, fp);
	ss << "\r\nChannels: " << (unsigned short)buf;
	unsigned short channels = (unsigned short)buf;

	fseek(fp, 24, SEEK_SET);
	fread(&buf, 1, 4, fp);
	ss << "\r\nSample Rate: " << (unsigned int)buf << " Hz";
	unsigned int sample_rate = (unsigned int)buf;

	fseek(fp, 28, SEEK_SET);
	fread(&buf, 1, 4, fp);
	ss << "\r\nByte Rate: " << (unsigned int)buf << "";
	unsigned int byte_rate = (unsigned int)buf;

	fseek(fp, 32, SEEK_SET);
	fread(&buf, 1, 2, fp);
	ss << "\r\nBlock Align: " << (unsigned short)buf;
	unsigned short block_align = (unsigned short)buf;

	fseek(fp, 34, SEEK_SET);
	fread(&buf, 1, 2, fp);
	ss << "\r\nBits Per Sample: " << (unsigned short)buf;
	unsigned short bits_per_sample = (unsigned short)buf;

	fseek(fp, 20 + 4 + pcm, SEEK_SET);
	fread(&buf, 1, 4, fp);
	ss << "\r\nSubChunk2Size: " << (unsigned int)buf;

	unsigned int wav_data = (unsigned int)buf;

	double duration = (double)(unsigned int)buf / (((double)sample_rate * (double)channels * (double)bits_per_sample) / (double)8);
	//ss << "\r\nDuration: ";
	

		ss << "\r\nDuration: " << FormatAudioTime(duration).c_str() << "";
		ss << "\r\nBitRate: " << (unsigned int)((double)(unsigned int)buf / (double)duration * 8.0f / 1000.0f) << " kbps\r\n";

		ss << "\r\n\r\nAnalyzing ... \r\n\r\n";
		SendMessage(hwnd, WM_SETTEXT, NULL, (LPARAM)ss.str().c_str());
		ScrollWindow (hwnd, 0, 200, NULL, NULL);
		// get total number of frames
		unsigned int frames = wav_data / (((bits_per_sample / 8) * channels));
		ss << "Total audio frames: " << frames << "\r\n";
		SendMessage(hwnd, WM_SETTEXT, NULL, (LPARAM)ss.str().c_str());
		//free(buf);
		fseek(fp, 20 + 4 + pcm + 4, SEEK_SET); // we are at data chunk offset now hopefully :) 
		unsigned int offset = 0;

		// read configuration data
		CSimpleIniA config;
		TCHAR path[MAX_PATH + 1] = {0};
		GetModuleFileName(NULL, path, sizeof(path));
		PathRemoveFileSpec(path);
		lstrcat(path, _T("\\vawparser.ini"));
		config.LoadFile(path);
		//const char* config_volume = config.GetValue("General", "MinimumVolumeForSilence", "20", false);
		//const char* config_time = config.GetValue("General", "TimeForDetectingAudioSignal", "0.5", false);
		//MessageBoxA(NULL, config_time, config_volume, MB_OK);
		
		
		double config_time = atof(config.GetValue("General", "TimeForDetectingAudioSignal", "0.5", false));
		int config_volume = atoi(config.GetValue("General", "MinimumVolumeForSilence", "20", false));

		int frame_size = bits_per_sample / 8;
		double chunk2_size = bits_per_sample / 8;
		chunk2_size *= channels;
		chunk2_size *= sample_rate;
		chunk2_size *= config_time;
		chunk2_size /= block_align;
		chunk2_size *= block_align;
		unsigned int chunk_size = (unsigned int)chunk2_size;

		double tmp = 0;
		FILE *fw;
		fw = fopen ("out.txt", "wb");*/
		/*fwrite("This file is written by wavParser\r\n", 1, 35, fw);
		std::stringstream sw;
		int i = 0;
		sw << chunk_size << "\r\n\r\n";
		fwrite(sw.str().c_str(), 1, 8, fw);**/
		//char *buffer = (char*)GlobalAlloc(GPTR, chunk_size);
		//GlobalFree(buf);
		//unsigned int total_read = 0;
	/*

		errno_t err = -55;
		int bSilent = -1;
		while (offset < wav_data!feof(fp)){
			memset(&buffer[0], 0, chunk_size);
			//total_read = ProcessVawChunk(fp, buffer, chunk_size);
			char* buff = (char*)GlobalAlloc(GPTR, 8192);
			//char* buffer = (char*)GlobalAlloc(GPTR, chunk_size);
			unsigned int total_read = 0;
			unsigned int bufsize = 8192;
			if (bufsize > chunk_size){
				bufsize = chunk_size;
				}

			while (true){

				int read = fread (&buff[0], 1, (size_t)bufsize, fp);

				memmove_s(&buffer[total_read], chunk_size - total_read, buff, read);
				total_read += read;
				if (bufsize > (chunk_size - total_read)){
					bufsize = chunk_size - total_read;

					}
				if ((bufsize != read) || (read <= 0)){

					break;
					}

				}

*/
			// buffer contains frames to analyze, hopefully
			
		//	if (frame_size == 2){
				// 16-bit PCM audio
				/*if (channels == 2){*/
				/*	
					short s = 0;
					__int64 iCount = 0;
					int j = 0;
					
					for (int i = 0; i < total_read; i += (channels * 2)){
						memmove_s(&s, sizeof(s), &buffer[i], sizeof(s));
						//fwrite(&s, 1, 2, fw);
						if (s < 0)
							s *= -1; // scale negative values
						iCount += s;
						j++;
						}
					
					memset(&buffer[0], 0, chunk_size);
					unsigned int threshold = iCount / j;
					double dur = wav_data - ftell(fp);// - total_read;
					//dur = duration - dur;
					double durr = (double)(unsigned int)dur / (((double)sample_rate * (double)channels * (double)bits_per_sample) / (double)8);
					durr = duration - durr;
					if(threshold > config_volume){
						if (bSilent != 1){
							bSilent = 1;
							sResult = "SIGNAL\t";
							std::stringstream sss;
							sss << threshold;
							sResult += sss.str();
							sResult += "\t";
							sResult += FormatAudioTime(durr).c_str();
							sResult += "\r\n";
							}
						} else {
							if (bSilent != 0){
								bSilent = 0;
								sResult = "SILENCE\t";
								std::stringstream sss;
								//sss << threshold;
								//sss << "\r\n";
								sss << "Offset: " << offset;
								sResult += sss.str();
								sResult += "\t";
								
								sResult += FormatAudioTime(durr).c_str();
								sResult += "\r\n";
								
								}
							
						}

					
					//}
				}
			
			char t2[1000];
			//_itoa(durr, t2, 10);
			
			offset += chunk_size;
			//i++;
			/*if(i > sample_rate)
				break;*/
			/*if(frame_size == 1){
				// 8 bit PCM data
				unsigned char c = unsigned char(buf);
				if(c < 128){
					ss << "Silence detected at \r\n\r\n";
					}
				} else if(frame_size == 2){
					// 16 bit PCM data
					unsigned short c = unsigned short(buf);
					
					
					tmp += c;
					//sw.clear();
					//sw << c << "\r\n";
					//char t[24];
					//itoa(c, t, 10);
//					fwrite(t, 1, strlen(t), fw);
					//fwrite("\r\n", 1, 2, fw);
					/*if (c < 1000){
						unsigned int s = offset / (((bits_per_sample / 8) * channels));
						ss << "Silence detected at " << c << "\r\n\r\n";
						//break;
						}*/
				//}
		/*	} 
			MessageBoxA(NULL, sResult.c_str(),"ftell", MB_OK);
		ss << tmp / sample_rate;
		fclose(fw);

		// Free all resources used
		fclose(fp);
		//GlobalFree((HGLOBAL)buf);
		//TCHAR t[4096]= {0};
		//_tcscpy(t, ss.str().c_str());
		SendMessage(hwnd, WM_SETTEXT, NULL, (LPARAM)ss.str().c_str());
		//MessageBox(HWND_TOP, ss.str().c_str(), NULL, MB_OK);
		return TRUE;
	}*/
/*
std::basic_string<TCHAR> GetWaveFormat(unsigned int format){
	std::basic_string<TCHAR> res;
	switch (format){
	case 0:
		res = _T("Unknown");
		break;

		}
	return res;
	}
*/
void Thread(LPVOID lpvoid){
	PPARAMS p = (PPARAMS) lpvoid;
	//MessageBox(p->hparent, p->wav, NULL, MB_OK);
	//ParseWavFile(p->wav, p->hwnd);
	
	std::ifstream s(p->wav, std::ios::binary);
	
	std::basic_stringstream<TCHAR> ss;
	ss << _T("Opening '") << p->wav << _T("' for analyzing ... \r\n\r\n");
	ss << _T("Parsing file header chunks ...\r\n\r\n");
	SendMessage (p->hwndEdit, WM_SETTEXT, NULL, WPARAM(ss.str().c_str()));
	
	DWORD size = 0;
	WAVEFORMATEX waveformatex = {0};
	char* buf = new char[32];
	 
	memset (buf, 0, 32);
	memset (&waveformatex, 0, sizeof (WAVEFORMATEX));

	s.read(buf, 4);
	
try {
	if (!(buf[0] == 'R' && buf[1] == 'I' && buf[2] == 'F' && buf[3] == 'F')){
		throw std::exception("Unable to find \"RIFF\" chunk at offset 0.");
		}

		s.read(buf, 4);
		unsigned int c1size = *(unsigned int *)buf;
		s.seekg(0, std::ios::end);
		size = static_cast<DWORD>(s.tellg());
		s.seekg(8, std::ios::beg);
		
		if (size - c1size != 8){
			throw  std::exception("WAVE header is corrupted while reading 4 bytes at offset 4.");
			}
		s.read(buf, 4);
		if (!(buf[0] == 'W' && buf[1] == 'A' && buf[2] == 'V' && buf[3] == 'E')){
			throw  std::exception("Unable to find \"WAVE\" string at offset 8.");
			}
			s.read(buf, 4);
			if (!(buf[0] == 'f' && buf[1] == 'm' && buf[2] == 't' && buf[3] == ' ')){
				throw  std::exception("Unable to find \"fmt \" chunk at offset 12.");
				}
				s.read (buf, 4);
				if (*(unsigned int*)buf != 16U){
					throw  std::exception("Unable to find decimal value \"16\" or 0x10 0x00 0x00 0x00 at offset 16.");
					}
					s.read (buf, 2);
					if (*(unsigned short*)buf != 1){
						throw  std::exception("Unable to find decimal value \"1\" or 0x00 0x02 at offset 20 corresponding to PCM uncrompressed audio format.");
						}
						s.read (buf, 2);
						waveformatex.wFormatTag = 1;
						waveformatex.nChannels = *(WORD*)buf;
						s.read (buf, 4);
						waveformatex.nSamplesPerSec = *(DWORD*)buf;
						s.read (buf, 4);
						waveformatex.nAvgBytesPerSec = *(DWORD*)buf;
						s.read (buf, 2);
						waveformatex.nBlockAlign = *(WORD*)buf;
						s.read (buf, 2);
						waveformatex.wBitsPerSample = *(WORD*)buf;
						s.read (buf, 4);
						if (!(buf[0] == 'd' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'a')){
							throw  std::exception("Unable to find \"data\" sub-chunk at offset 36.");
							}
						s.read (buf, 4);
						if (*(DWORD*)buf + 44 != size){
							throw  std::exception("\"data\" chunk size mismatch, possible corrupted wave header or format not supported.");
							}
		} catch (std::exception& e){
			SendMessage(p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)_T(""));
			MessageBoxA(NULL, e.what(), NULL, MB_OK);
			
		}

	delete [] buf;

	double duration = (size - 44) / (double)((double)(waveformatex.nSamplesPerSec * waveformatex.nChannels * waveformatex.wBitsPerSample) / 8.0);

	std::string str(FormatAudioTime(duration));
	ss << _T("File size: ") << size << _T(" bytes\r\n");
	ss << _T("Duration: ");
	ss << str.c_str() << _T("\r\n");
	ss << _T("BitRate: ") << unsigned int((size - 44) / duration * 8.0 / 1000.0) << _T(" kbps\r\n");
	ss << _T("Audio channels: ") << waveformatex.nChannels << _T("\r\n");
	ss << _T("Sample Rate: ") << waveformatex.nSamplesPerSec << _T(" Hz\r\n");
	ss << _T("Bits per sample: ") << waveformatex.wBitsPerSample << _T("\r\n");
	ss << _T("Total audio samples: ") << ((size - 44) / waveformatex.nBlockAlign) << _T("\r\n");
	ss << _T("\r\nAnalyzing ...\r\n\r\n");
	SendMessage(p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)ss.str().c_str());

	// read configuration data
	CSimpleIniA *config = new CSimpleIniA;
	TCHAR path[_MAX_PATH + 1] = {0};
	GetModuleFileName(NULL, path, sizeof(path));
	PathRemoveFileSpec(path);
	//lstrcat(path, _T("\\vawparser.ini"));
	_tcscat_s(path, _T("\\vawparser.ini"));
	config->LoadFile(path);
	CONFIG cnfg;
	cnfg.time = atoi(config->GetValue("General", "TimeForDetectingAudioSignal", "500", false));
	cnfg.volume = atoi(config->GetValue("General", "MinimumVolumeForSilence", "20", false));
	delete config;

	// calculate buffer size
	//duration *= 1000; //transform to milliseconds
	double frames = (waveformatex.nSamplesPerSec * cnfg.time) / 1000;
	unsigned int frame_size = static_cast<unsigned int>(frames * waveformatex.nBlockAlign);
	// allocate buffer
	char* buffer = new char[frame_size];
	memset (buffer, 0, frame_size);
	unsigned int offset = 44;
	int bSilent = -1;

	std::ofstream ofs("testt.bin", std::ios::binary);

	while (true)
	{
	memset (buffer, 0, frame_size);
	s.read (buffer, frame_size);
	int read = (unsigned int)s.tellg() - offset;
	
	// buffer contains frames to analyze, hopefully
	if (waveformatex.wBitsPerSample == 16){
		// 16 bit PCM audio
		unsigned int iCount = 0;
		short sh = 0;
		int j = 0;
		for (int i = 0; i < read; i += (waveformatex.nChannels * (waveformatex.wBitsPerSample / 8))){
			memmove_s(&sh, sizeof(sh), &buffer[i], sizeof(sh));

			if (sh < 0) {
				sh *= -1; // scale negative values
				}
			iCount += sh;
			j++;
			}
		if (j == 0)
			break;
		unsigned int iThreshold = iCount / j;
		char t[1024];
		_itoa(iThreshold, t, 10);
		ofs.write(t, 4);
		ofs.write("\r\n", 2);
		double durr = (size  - offset) / (double)((double)(waveformatex.nSamplesPerSec * waveformatex.nChannels * waveformatex.wBitsPerSample) / 8.0);
		double dur = duration - durr;
		if(iThreshold > cnfg.volume){
			if (bSilent != 1){
				bSilent = 1;
				sResult += "SIGNAL\t";
				std::stringstream sss;
				sss << offset;
				sResult += sss.str();
				sResult += "\t";
				sResult += FormatAudioTime(dur).c_str();
				sResult += "\r\n";
				}
			} else {
				if (bSilent != 0){
					bSilent = 0;
					sResult += "SILENCE\t";
					std::stringstream sss; 
					sss << offset;
					sResult += sss.str();
					sResult += "\t";
					sResult += FormatAudioTime(dur).c_str();
					sResult += "\r\n";

					}
				
			}
		
		
		}
	offset = (unsigned int)s.tellg() - 44;
	
	if ((s.bad()) || (s.eof()))
			break;

	ss << offset;
		//MessageBox(0, ss.str().c_str(), NULL, MB_OK);    
		}

	
	delete [] buffer;


	ofs.close();
	ss << _T("Framesize buffer: ") << frame_size << _T("\r\n");
	MessageBoxA(NULL, sResult.c_str(), NULL, MB_OK);
	sResult.clear();
	EnableWindow(p->hOPenButton, TRUE);
	_endthread();
	}

/*
unsigned int ProcessVawChunk (FILE* fp, char* buffer, unsigned int chunk_size) {
	char* buff = (char*)GlobalAlloc(GPTR, 8192);
	buffer = (char*)GlobalAlloc(GPTR, chunk_size);
	unsigned int total_read = 0;
	int bufsize = 8192;
	if (bufsize > chunk_size){
		bufsize = chunk_size;
		}

	while (true){

		int read = fread (&buff[0], 1, (size_t)bufsize, fp);

		memmove_s(&buffer[total_read], chunk_size - total_read, buff, read);
		total_read += read;
		if (bufsize > (chunk_size - total_read)){
			bufsize = chunk_size - total_read;

			}
		if ((bufsize != read) || (read <= 0)){

			break;
			}

		}
		return total_read;
	}

*/
std::string& FormatAudioTime(double duration){
	static std::string s;
	s.clear();
	std::stringstream ss;
	unsigned int min = 0;
	unsigned int sec = 0;
	std::stringstream buf2;
	std::stringstream buf3;
	if(duration < 60.0f){
		ss << "00:00:";
		if(duration < 10.0f){
			ss << "0";
			}
		ss << (unsigned int)duration;
		} else if((duration < 60 * 60) && (duration >= 60.0f)){
			min = unsigned int((unsigned int)duration / 60);
			if (min == 0){
				buf2 << "00";
				} else if(min < 10){
					buf2 << "0" << min;
				} else {
					buf2 << min;
					}
				sec = (unsigned int)duration % 60;
				if (sec == 0){
					buf3 << "00";
					} else if(sec < 10){
						buf3 << "0";
						buf3 << (unsigned int)sec;
						//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
					} else {
						buf3 << (unsigned int)sec;
						//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
						}
					ss << "00:";
					ss << buf2.str().c_str();
					ss << ":";
					ss << buf3.str().c_str();
		} else {

			}

	s = ss.str().c_str();
	//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
	return s;
	}