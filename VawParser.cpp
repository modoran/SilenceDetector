// VawParser.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "VawParser.h"
#include <shellapi.h>


// Global Variables:
HINSTANCE hInst;
static std::basic_string<TCHAR> sResult;

// Forward declarations of functions included in this code module:

INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DialogProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
					 HINSTANCE hPrevInstance,
					 LPTSTR    lpCmdLine,
					 int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	hInst = hInstance;
	//MessageBox (NULL, NULL, NULL, MB_OK);
	return DialogBox (hInstance, MAKEINTRESOURCE(IDD_DIALOG1), NULL, DialogProc);

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

// Message handler for main about box.
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
	{
	static HMENU hMenu;
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
		{
	case WM_INITDIALOG:
		{
		hMenu = LoadMenu (hInst, MAKEINTRESOURCE (IDC_VAWPARSER));
		HICON hIcon = (HICON)LoadImage(hInst, MAKEINTRESOURCE(IDI_VAWPARSER), IMAGE_ICON, 0, 0, LR_SHARED);
		SendMessage (hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
		SendMessage (hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
		SendMessage(GetDlgItem(hDlg, IDC_EDIT2), WM_SETTEXT, NULL, (WPARAM)_T("500"));
		SendMessage(GetDlgItem(hDlg, IDC_EDIT3), WM_SETTEXT, NULL, (WPARAM)_T("1000"));
		return (INT_PTR)TRUE;
		}
		
	case WM_CLOSE:
		{
			DestroyMenu(hMenu);
			EndDialog(hDlg, 0);
			return (INT_PTR)TRUE;
		}
	case WM_DROPFILES:
		{
		TCHAR lpszFile[MAX_PATH];
		UINT uFile = 0;
		HDROP hDrop = (HDROP)wParam;

		uFile = DragQueryFile(hDrop, 0xFFFFFFFF, NULL, NULL);
		if (uFile != 1){
			MessageBox (hDlg, _T("Dropping multiple files is not supported."), NULL, MB_ICONERROR);
			DragFinish(hDrop);
			break;
			}
		lpszFile[0] = '\0';
		if (DragQueryFile(hDrop, 0, lpszFile, MAX_PATH))
			{
				if (_tcsicmp (PathFindExtension(lpszFile), _T(".wav")) == 0){
					//MessageBox(hDlg, lpszFile, NULL, MB_OK);
					static PARAMS params;
					ZeroMemory (&params, sizeof (PARAMS));
					params.hparent = hDlg;
					params.wav = lpszFile;
					params.hwndEdit = GetDlgItem(hDlg, IDC_EDIT1);
					params.hMenu = hMenu;
					_beginthread(Thread, NULL, &params);
					}
			}
			
		DragFinish(hDrop);
			break;
		}
	case WM_COMMAND:
		{
		switch (LOWORD(wParam))
			{
		case IDM_ABOUT:
			{
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hDlg, About);
			break;
			}
		case IDM_EXIT:
			{
				SendMessage (hDlg, WM_CLOSE, NULL, NULL);
				break;
			}
		case IDM_FILE_ADD:
		case IDC_BUTTON1:
			{
			OPENFILENAME ofn;
			TCHAR szFileName[MAX_PATH] = _T("");
			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn); // SEE NOTE BELOW
			ofn.hwndOwner = hDlg;
			ofn.lpstrFilter = _T("WAV Files (*.wav)\0*.wav\0All Files (*.*)\0*.*\0");
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_HIDEREADONLY;
			ofn.lpstrDefExt = _T("wav");
			ofn.lpstrTitle  = _T("Choose WAV PCM file to open");
			if(GetOpenFileName(&ofn)){
				static PARAMS params = {0};
				params.hparent = hDlg;
				params.wav = szFileName;
				params.hwndEdit = GetDlgItem(hDlg, IDC_EDIT1);
				params.hMenu = hMenu;
				//MessageBox (hDlg, szFileName, NULL, MB_OK);
				_beginthread(Thread, NULL, &params);
				}
			break;
			}
			}
		break;
		}
	default:
		break;
		}
	return (INT_PTR)FALSE;
	}


void Thread(LPVOID lpvoid){
	PPARAMS p = (PPARAMS) lpvoid;
	std::ifstream s(p->wav, std::ios::binary);
	//SetMenuItemInfo ((HMENU)GetDlgItem(p->hparent, IDC_VAWPARSER), IDC_VAWPARSER, FALSE, &mf);
	EnableMenuItem(p->hMenu, 2, MF_GRAYED);
	std::basic_stringstream<TCHAR> ss;
	ss << _T("Opening '");
	ss << p->wav.c_str();
	ss << _T("' for analyzing ... \r\n\r\n");
	ss << _T("Parsing file header chunks ...\r\n\r\n");
	SendMessage (p->hwndEdit, WM_SETTEXT, NULL, WPARAM(ss.str().c_str()));
	
	DWORD size = 0;
	WAVEFORMATEX waveformatex = {0};
	char* buf = new char[32];
	 
	memset (buf, 0, 32);
	memset (&waveformatex, 0, sizeof (WAVEFORMATEX));

	s.read(buf, 4);
	SendMessage(p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)ss.str().c_str());
	
	unsigned int fmt_offset = 0;
	unsigned int data_offset = 0;

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
				fmt_offset = *(unsigned int*)buf;
				/*if (*(unsigned int*)buf != 16U){
					std::stringstream ssss;
					ssss << *(unsigned int*)buf;
					MessageBoxA(NULL, ssss.str().c_str(), NULL, MB_OK);
					throw  std::exception("Unable to find decimal value \"16\" or 0x10 0x00 0x00 0x00 at offset 16.");
					}*/
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
						s.seekg (fmt_offset - 16, std::ios::cur);
						s.read (buf, 4);
						if (buf[0] == 'f' && buf[1] == 'a' && buf[2] == 'c' && buf[3] == 't'){
							// added support for 'fact' chunk while mantain compatibility
							s.read (buf, 4); // get fact chunk size
							s.seekg (*(DWORD*)buf, std::ios::cur); // move stream position
							s.read (buf, 4);
							}

						if (!(buf[0] == 'd' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'a')){
							throw  std::exception("Unable to find \"data\" sub-chunk at required offset.");
							}
						s.read (buf, 4);
						data_offset = s.tellg();
						if (*(DWORD*)buf + s.tellg() != size){
							throw  std::exception("\"data\" chunk size mismatch, possible corrupted wave header or format not supported.");
							}
		} catch (std::exception& e){
			SendMessage(p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)_T(""));
			delete [] buf;
			s.close();
			MessageBoxA(NULL, e.what(), NULL, MB_OK);
			_endthread();
			
			
		}

	delete [] buf;

	double duration = (size - data_offset) / (double)((double)(waveformatex.nSamplesPerSec * waveformatex.nChannels * waveformatex.wBitsPerSample) / 8.0);

	std::basic_string<TCHAR> str(FormatAudioTime(duration));
	ss << _T("File size: ") << size << _T(" bytes\r\n");
	ss << _T("Duration: ");
	ss << str.c_str() << _T("\r\n");
	ss << _T("BitRate: ") << unsigned int((size - data_offset) / duration * 8.0 / 1000.0) << _T(" kbps\r\n");
	ss << _T("Audio channels: ") << waveformatex.nChannels << _T("\r\n");
	ss << _T("Sample Rate: ") << waveformatex.nSamplesPerSec << _T(" Hz\r\n");
	ss << _T("Bits per sample: ") << waveformatex.wBitsPerSample << _T("\r\n");
	ss << _T("Total audio samples: ") << ((size - data_offset) / waveformatex.nBlockAlign) << _T("\r\n");
	ss << _T("\r\nAnalyzing ...\r\n\r\n");
	SendMessage(p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)ss.str().c_str());

	
	CONFIG cnfg;
	TCHAR myBuf[6];
	GetWindowText(GetDlgItem(p->hparent, IDC_EDIT2), myBuf, sizeof (myBuf) / sizeof (TCHAR));
	cnfg.time = _ttoi (myBuf);
	GetWindowText(GetDlgItem(p->hparent, IDC_EDIT3), myBuf, sizeof (myBuf) / sizeof (TCHAR));
	cnfg.volume = _ttoi (myBuf);
		

	if (cnfg.volume < 1){
		cnfg.volume = 1000;
		} else if(cnfg.volume > 30000) {
			cnfg.volume = 30000;
		}
	if (cnfg.time < 1){
		cnfg.time = 1;
		} else if(cnfg.time > 30000) {
			cnfg.time = 30000;
		}
	//MessageBox(NULL, myBuf, myBuf, MB_OK);
	// calculate buffer size
	
	double frames = (waveformatex.nSamplesPerSec * cnfg.time) / 1000;
	unsigned int frame_size = static_cast<unsigned int>(frames * waveformatex.nBlockAlign);
	// allocate buffer
	char* buffer = new char[frame_size];
	memset (buffer, 0, frame_size);
	unsigned int offset = data_offset;
	int bSilent = -1;

	//std::ofstream ofs("testt.bin", std::ios::binary);

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
				//sh = abs(sh);
				}
			iCount += sh;
			j++;
			}
		if (j == 0)
			break;
		unsigned int iThreshold = iCount / j;
		TCHAR t[1024];
		_itot_s(iThreshold, t, 10);
		//ofs.write(t, 4);
		//ofs.write("\r\n", 2);
		double durr = (size  - offset) / (double)((double)(waveformatex.nSamplesPerSec * waveformatex.nChannels * waveformatex.wBitsPerSample) / 8.0);
		double dur = duration - durr;
		if(iThreshold > cnfg.volume){
			if (bSilent != 1){
				bSilent = 1;
				sResult += _T("SIGNAL\t");
				std::basic_stringstream<TCHAR> sss;
				sss << offset;
				sResult += sss.str();
				sResult += _T("\t");
				sResult += FormatAudioTime(dur).c_str();
				sResult += _T("\r\n");
				}
			} else {
				if (bSilent != 0){
					bSilent = 0;
					sResult += _T("SILENCE\t");
					std::basic_stringstream<TCHAR> sss; 
					sss << offset;
					sResult += sss.str();
					sResult += _T("\t");
					sResult += FormatAudioTime(dur).c_str();
					sResult += _T("\r\n");

					}
				
			}
		
		
		}
	offset = (unsigned int)s.tellg() - 44;
	
	if ((s.bad()) || (s.eof()))
			break;  
		}

	
	delete [] buffer;


	//ofs.close();

	
	ss << std::setw(16) << sResult.c_str();
	ss << _T("\r\n\r\nAnalyze completed.");
	SendMessage (p->hwndEdit, WM_SETTEXT, NULL, (WPARAM)ss.str().c_str());

	sResult.clear();
	MessageBox(p->hparent, _T("WAVE file has been processed"), _T("Operation completed"), MB_OK);
	_endthread();
	}


std::basic_string<TCHAR>& FormatAudioTime(double duration){
	static std::basic_string<TCHAR> s;
	s.clear();
	std::basic_stringstream<TCHAR> ss;
	unsigned int min = 0;
	unsigned int sec = 0;
	std::basic_stringstream<TCHAR> buf2;
	std::basic_stringstream<TCHAR> buf3;
	if(duration < 60.0f){
		ss << _T("00:00:");
		if(duration < 10.0f){
			ss << _T("0");
			}
		ss << (unsigned int)duration;
		} else if((duration < 60 * 60) && (duration >= 60.0f)){
			min = unsigned int((unsigned int)duration / 60);
			if (min == 0){
				buf2 << _T("00");
				} else if(min < 10){
					buf2 << _T("0") << min;
				} else {
					buf2 << min;
					}
				sec = (unsigned int)duration % 60;
				if (sec == 0){
					buf3 << _T("00");
					} else if(sec < 10){
						buf3 << _T("0");
						buf3 << (unsigned int)sec;
						//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
					} else {
						buf3 << (unsigned int)sec;
						//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
						}
					ss << _T("00:");
					ss << buf2.str().c_str();
					ss << _T(":");
					ss << buf3.str().c_str();
		} else {

			}

	s = ss.str().c_str();
	//MessageBoxA(NULL, buf3.str().c_str(), NULL, MB_OK);
	return s;
	}

void GetWaveFile (LPTSTR wav, HWND hDlg){

	}