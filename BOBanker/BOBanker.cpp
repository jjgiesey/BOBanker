// BOBanker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BOBanker.h"
#include "Players.h"
#include "Cities.h"
#include "Companies.h"
#include "TechLevels.h"
#include "Trains.h"

using namespace std;

#define MAX_LOADSTRING 100

#define BUTTON_IDENTIFIER 1

void setupPlayers();
//Precondition: None
//Postcondition: After running all the players will be loaded into the class players
// with initial values and turn orders set

bool attemptBuy(int player, int company, int shares, int orphans);
//Precondition: player < numberPlayers
//Postcondition: returns 1 if successfull and handles all the buying transactions

bool attemptSell(int player, int company, int shares);
// This function will return a 1 when the player has sucessfully sold a stock for their turn.

bool attemptOrphan(int player);
// This function will return a 1 when a player has successfully had a company buy back orphaned stock.

int reduceForOrphans();
//Preconditions:To be run at end of market round
//Postconditions: Valuation of companies that have orphan shares
//				is reduced by one level.

int setCompanyPresident(int company);
// Preconditions: company is a vaild company. The shares ownd by players is passed through playerShares and the current president it passed
// through companyPresident
// Postcondidtion: The number of shares owned by each player is compared and if a new player has more shares their number is returned through setCompanyPresident.
// If not the old president is passed.










char readValue(ifstream& inputStream, char cArray[]);
//Precondtions: Open stream in in inputStrean and character array is returned in cArray
//Postcondition: Will return last character read.

char findEOL(ifstream& inputStream);
//Precondition: Stream to be read for end of line is in inputStream
//				It is expected that all relevant values have been read
//Postcondition: Stream cleared. Last character returned. Should be /n if successful

int getCashInfo();


// Data processing 

int fetchFileData(wchar_t pfileName[]);

int getManualData();

int pushFileData(wchar_t pfileName[]);


// Global Variables:

HWND moDlg;

HINSTANCE hInst;                                // current instance
HINSTANCE ghInstance = NULL;
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
int numberPlayers = 3;
wchar_t  playerName[6][20];
wchar_t wfileName[] = L"bodata8.csv";
char testArray[30];
int Value = 0;
Players players(0);
Trains trains(0);
TechLevels techLevels;
Cities cities;
Companies companies(0);
int gameRound = 0;
int turn = 0;
int techLevel = 1;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Players(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PlayerInfo(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Market(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    BuyShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SellShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Orphans(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InitValuation(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Scrap(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Train(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Track(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Profit(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Monitor(HWND, UINT, WPARAM, LPARAM);

int TextToMonitor(HWND hNdl, wchar_t theText[]);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,_In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR    lpCmdLine, _In_ int nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);
	
	// TODO: Place code here.


    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BOBANKER, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BOBANKER));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {


        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BOBANKER));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BOBANKER);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
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
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

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
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{

	TCHAR greeting[] = _T("Hello World!");
//	DWORD Error;
    switch (message)
    {
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);

			break;
		case IDM_START_NEWGAME:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PLAYERBOX), hWnd, Players);
			setupPlayers();
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), hWnd, Market);	

//			hmonitor = CreateDialog(hInst, MAKEINTRESOURCE(IDD_MONITORBOX), hWnd, Monitor);
//			CreateDialog(hInst, MAKEINTRESOURCE(IDD_PLAYERINFOBOX), hWnd, PlayerInfo);			

			;
		break;
		case IDM_START_SAVEDGAME:
			fetchFileData(wfileName);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), hWnd, Market);

		break;
		case IDM_MODIFY_PLAYERS:
		break;
		case IDM_MODIFY_COMPANIES:
			break;
		case IDM_MODIFY_TRAINS:
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		TCHAR szFileName[MAX_PATH];
		HINSTANCE hInstance = GetModuleHandle(NULL);
		GetModuleFileName(hInstance, szFileName, MAX_PATH);
		MessageBox(hWnd, szFileName, greeting, MB_OK | MB_ICONINFORMATION);
	}
	break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
			FillRect(hdc, &ps.rcPaint, (HBRUSH)(4));
//			TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
            // TODO: Add any drawing code that uses hdc here...
            EndPaint(hWnd, &ps);
        }
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
INT_PTR CALLBACK About(HWND abDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(abDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
// Message handler for player box.
INT_PTR CALLBACK Players(HWND pBDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDDONE)
		{
			int textLength;
			wchar_t name[20];
			// Read Player Names
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP1))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP1), name, textLength);
			wcscpy_s(players.name[0], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP2))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP2), name, textLength);
			wcscpy_s(players.name[1], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP3))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP3), name, textLength);
			wcscpy_s(players.name[2], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP4))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP4), name, textLength);
			wcscpy_s(players.name[3], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP5))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP5), name, textLength);
			wcscpy_s(players.name[4], name);
			textLength = GetWindowTextLength(GetDlgItem(pBDlg, IDC_EDITP6))+1;
			textLength = GetWindowText(GetDlgItem(pBDlg, IDC_EDITP6), name, textLength);
			wcscpy_s(players.name[5], name);

			EndDialog(pBDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case IDC_CHECKP4:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP4), TRUE);
			EnableWindow(GetDlgItem(pBDlg, IDC_CHECKP5), TRUE);
			numberPlayers = 4;
			break;
		case IDC_CHECKP5:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP5), TRUE);
			EnableWindow(GetDlgItem(pBDlg, IDC_CHECKP6), TRUE);
			numberPlayers = 5;
			break;
		case IDC_CHECKP6:
			EnableWindow(GetDlgItem(pBDlg, IDC_EDITP6), TRUE);
			break;
		}
	}
	return (INT_PTR)FALSE;
}
INT_PTR CALLBACK PlayerInfo(HWND pIDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Add items to list. 
		HWND hwndList = GetDlgItem(pIDlg, IDC_LISTPLAYER);

/*		//Place Player Info window in upper right corner

		RECT rc, rcDlg, rcOwner;
		GetWindowRect(GetDesktopWindow(), &rcOwner);
		GetWindowRect(hDlg, &rcDlg);

		CopyRect(&rc, &rcOwner);

		// Offset the owner and dialog box rectangles so that right and bottom 
		// values represent the width and height, and then offset the owner again 
		// to discard space taken up by the dialog box. 

		OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
		OffsetRect(&rc, -rc.left, -rc.top);
		OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);



		SetWindowPos(hDlg,
			HWND_TOP,
			rcOwner.left - (rcDlg.right - rcDlg.left)+ 50,
			50,
			0, 0,          // Ignores size arguments. 
			SWP_NOSIZE);*/

		for (int i = 0; i < numberPlayers; i++)
		{
			LRESULT pos = (int)SendMessage(hwndList, LB_ADDSTRING, 0,(LPARAM)playerName[i]); //Write line to list box
			SendMessage(hwndList, LB_SETITEMDATA, pos, (LPARAM)i); // Associate index with each line
		}
	
		
		// Set input focus to the list box.
		SetFocus(hwndList);
		return TRUE;
	
	
	
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(pIDlg, LOWORD(wParam));
			return TRUE;
		case IDC_LISTPLAYER:
		{
			switch (HIWORD(wParam))
			{
			case LBN_SELCHANGE:
			{
				HWND hwndList = GetDlgItem(pIDlg, IDC_LISTPLAYER);

				// Get selected index.
				int lbItem = (int)SendMessage(hwndList, LB_GETCURSEL, 0, 0);

				// Get item data.
				int i = (int)SendMessage(hwndList, LB_GETITEMDATA, lbItem, 0);
				return TRUE;
			}
			}
		}
		return TRUE;
		}
	}
	return FALSE;
}
INT_PTR CALLBACK Monitor(HWND moDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{
		// Add items to list. 
		HWND hwndList = GetDlgItem(moDlg, IDC_MONITOR);

	//Place monitor window in lower left corner

		RECT rc, rcDlg, rcOwner;
		GetWindowRect(GetDesktopWindow(), &rcOwner);
		GetWindowRect(moDlg, &rcDlg);

		CopyRect(&rc, &rcOwner);

		// Offset the owner and dialog box rectangles so that right and bottom 
		// values represent the width and height, and then offset the owner again 
		// to discard space taken up by the dialog box. 

		OffsetRect(&rcDlg, -rcDlg.left, -rcDlg.top);
		OffsetRect(&rc, -rc.left, -rc.top);
		OffsetRect(&rc, -rcDlg.right, -rcDlg.bottom);



		SetWindowPos(moDlg,HWND_TOP,rcOwner.left +50,rcOwner.bottom - (rcDlg.bottom - rcDlg.top)-50,0, 0, SWP_NOSIZE);

		GetWindowRect(GetDesktopWindow(), &rcOwner);
		GetWindowRect(moDlg, &rcDlg);

		wchar_t monitorText[40];
		wcscpy_s(monitorText, L"Here we go");
		TextToMonitor(moDlg, monitorText);


		return TRUE; 
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			EndDialog(moDlg, LOWORD(wParam));
			return TRUE;
		return TRUE;
		}
	}
	return FALSE;
}
INT_PTR CALLBACK Market(HWND maDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	int order = 1;
	int company = 0;
	HWND hwndList = GetDlgItem(maDlg, IDC_LIST_CURRENT_PLAYER);
	HINSTANCE hInstance = GetModuleHandle(NULL);
	HWND hwndLP2 = GetDlgItem(maDlg, IDC_LISTPLAYER2);
	HWND hwndCP = GetDlgItem(maDlg, IDC_CURRENT_PLAYER);

	HWND hwndPN1 = GetDlgItem(maDlg, IDC_PN1);
	HWND hwndPN2 = GetDlgItem(maDlg, IDC_PN2);
	HWND hwndPN3 = GetDlgItem(maDlg, IDC_PN3);
	HWND hwndPN4 = GetDlgItem(maDlg, IDC_PN4);
	HWND hwndPN5 = GetDlgItem(maDlg, IDC_PN5);
	HWND hwndPN6 = GetDlgItem(maDlg, IDC_PN6);

	HWND hwndPC1 = GetDlgItem(maDlg, IDC_PC1);
	HWND hwndPC2 = GetDlgItem(maDlg, IDC_PC2);
	HWND hwndPC3 = GetDlgItem(maDlg, IDC_PC3);
	HWND hwndPC4 = GetDlgItem(maDlg, IDC_PC4);
	HWND hwndPC5 = GetDlgItem(maDlg, IDC_PC5);
	HWND hwndPC6 = GetDlgItem(maDlg, IDC_PC6);

	HWND hwndPW1 = GetDlgItem(maDlg, IDC_PW1);
	HWND hwndPW2 = GetDlgItem(maDlg, IDC_PW2);
	HWND hwndPW3 = GetDlgItem(maDlg, IDC_PW3);
	HWND hwndPW4 = GetDlgItem(maDlg, IDC_PW4);
	HWND hwndPW5 = GetDlgItem(maDlg, IDC_PW5);
	HWND hwndPW6 = GetDlgItem(maDlg, IDC_PW6);

	HWND hwndCV1 = GetDlgItem(maDlg, IDC_CV1);
	HWND hwndCV2 = GetDlgItem(maDlg, IDC_CV2);
	HWND hwndCV3 = GetDlgItem(maDlg, IDC_CV3);
	HWND hwndCV4 = GetDlgItem(maDlg, IDC_CV4);
	HWND hwndCV5 = GetDlgItem(maDlg, IDC_CV5);
	HWND hwndCV6 = GetDlgItem(maDlg, IDC_CV6);
	HWND hwndCV7 = GetDlgItem(maDlg, IDC_CV7);
	HWND hwndCV8 = GetDlgItem(maDlg, IDC_CV8);
	HWND hwndCV9 = GetDlgItem(maDlg, IDC_CV9);
	HWND hwndCV0 = GetDlgItem(maDlg, IDC_CV0);

	HWND hwndSS = GetDlgItem(maDlg, IDC_SHARES_SELL);
	HWND hwndSSS = GetDlgItem(maDlg, IDC_SPIN_SHARES_SELL);
	HWND hwndSB = GetDlgItem(maDlg, IDC_SHARES_BUY);
	HWND hwndSSB = GetDlgItem(maDlg, IDC_SPIN_SHARES_BUY);
	HWND hwndOB = GetDlgItem(maDlg, IDC_ORPHANS_BUY);
	HWND hwndSOB = GetDlgItem(maDlg, IDC_SPIN_ORPHANS_BUY);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		HANDLE hImg;
		HWND hwndCR0 = GetDlgItem(maDlg, IDC_CR0);
    	hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BOM));
		SendMessageW(hwndCR0, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR1 = GetDlgItem(maDlg, IDC_CR1);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMM));
		SendMessageW(hwndCR1, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR2 = GetDlgItem(maDlg, IDC_CR2);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_COM));
		SendMessageW(hwndCR2, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR3 = GetDlgItem(maDlg, IDC_CR3);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ERM));
		SendMessageW(hwndCR3, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR4 = GetDlgItem(maDlg, IDC_CR4);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_NPM));
		SendMessageW(hwndCR4, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR5 = GetDlgItem(maDlg, IDC_CR5);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_NYM));
		SendMessageW(hwndCR5, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR6 = GetDlgItem(maDlg, IDC_CR6);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_ICM));
		SendMessageW(hwndCR6, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR7 = GetDlgItem(maDlg, IDC_CR7);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_NHM));
		SendMessageW(hwndCR7, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR8 = GetDlgItem(maDlg, IDC_CR8);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_PNM));
		SendMessageW(hwndCR8, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		HWND hwndCR9 = GetDlgItem(maDlg, IDC_CR9);
		hImg = LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_WBM));
		SendMessageW(hwndCR9, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hImg);

		//	HBITMAP hImg = (HBITMAP)LoadBitmap(hInstance, MAKEINTRESOURCE(IDB_BMS));


		HFONT bigFont;
//		const wchar_t fontType[6] = "Arial";
		wchar_t cashStr[20];
		bigFont = CreateFont(40,0,0,0,FW_NORMAL,0,0,0,ANSI_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_SWISS,'\0');
		SendMessage(hwndCP, WM_SETFONT, (WPARAM)bigFont, 0);
		SetWindowText(hwndCP, (LPCWSTR)players.name[order]);

		SetWindowText(hwndPN1, (LPCWSTR)players.name[0]);
		_itow_s(players.getCash(0), cashStr, 10);
		SetWindowText(hwndPC1, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(0), cashStr, 10);
		SetWindowText(hwndPW1, (LPCWSTR)cashStr);

		SetWindowText(hwndPN2, (LPCWSTR)players.name[1]);
		_itow_s(players.getCash(1), cashStr, 10);
		SetWindowText(hwndPC2, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(1), cashStr, 10);
		SetWindowText(hwndPW2, (LPCWSTR)cashStr);

		SetWindowText(hwndPN3, (LPCWSTR)players.name[2]);
		_itow_s(players.getCash(2), cashStr, 10);
		SetWindowText(hwndPC3, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(2), cashStr, 10);
		SetWindowText(hwndPW3, (LPCWSTR)cashStr);

		if (numberPlayers >= 4)
		{
			ShowWindow(hwndPN4, SW_SHOW);
			ShowWindow(hwndPC4, SW_SHOW);
			ShowWindow(hwndPW4, SW_SHOW);
			SetWindowText(hwndPN4, (LPCWSTR)players.name[3]);
			_itow_s(players.getCash(3), cashStr, 10);
			SetWindowText(hwndPC4, (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(3), cashStr, 10);
			SetWindowText(hwndPW4, (LPCWSTR)cashStr);
		}
		if (numberPlayers >= 5)
		{
			ShowWindow(hwndPN5, SW_SHOW);
			ShowWindow(hwndPC5, SW_SHOW);
			ShowWindow(hwndPW5, SW_SHOW);
			SetWindowText(hwndPN5, (LPCWSTR)players.name[4]);
			_itow_s(players.getCash(4), cashStr, 10);
			SetWindowText(hwndPC5, (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(4), cashStr, 10);
			SetWindowText(hwndPW5, (LPCWSTR)cashStr);
		}
		if (numberPlayers >= 6)
		{
			ShowWindow(hwndPN6, SW_SHOW);
			ShowWindow(hwndPC6, SW_SHOW);
			ShowWindow(hwndPW6, SW_SHOW);
			SetWindowText(hwndPN6, (LPCWSTR)players.name[5]);
			_itow_s(players.getCash(5), cashStr, 10);
			SetWindowText(hwndPC6, (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(5), cashStr, 10);
			SetWindowText(hwndPW6, (LPCWSTR)cashStr);
		}
		if (companies.getAvailable(j, techLevel))
		{
		}

		_itow_s(companies.getCash(0), cashStr, 10);
		SetWindowText(hwndCV0, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(1), cashStr, 10);
		SetWindowText(hwndCV1, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(2), cashStr, 10);
		SetWindowText(hwndCV2, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(3), cashStr, 10);
		SetWindowText(hwndCV3, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(4), cashStr, 10);
		SetWindowText(hwndCV4, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(5), cashStr, 10);
		SetWindowText(hwndCV5, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(6), cashStr, 10);
		SetWindowText(hwndCV6, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(7), cashStr, 10);
		SetWindowText(hwndCV7, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(8), cashStr, 10);
		SetWindowText(hwndCV8, (LPCWSTR)cashStr);

		_itow_s(companies.getCash(9), cashStr, 10);
		SetWindowText(hwndCV9, (LPCWSTR)cashStr);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_BUY:
			if (players.getSold(order, company) = 1)
			{
				break;
			}
			if (companies.getStarted(company) == 0)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVALBOX), maDlg, InitValuation);
			}
			DialogBox(hInst, MAKEINTRESOURCE(IDD_BUYBOX), maDlg, BuyShares);
			break;
		case IDC_BUTTON_SELL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SELLBOX), maDlg, SellShares);
			break;
		case IDC_BUTTON_ORPHANS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ORPHANBOX), maDlg, Orphans);
			break;
		case IDC_BUTTON_DONE:
			break;
		case IDC_CANCEL:
			EndDialog(maDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK BuyShares(HWND bSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList2 = GetDlgItem(bSDlg, IDC_STOCKS_TO_BUY1);
	HWND hwndList3 = GetDlgItem(bSDlg, IDC_SPIN_BUY1);
	HWND hwndList2 = GetDlgItem(bSDlg, IDC_STOCKS_TO_BUY2);
	HWND hwndList3 = GetDlgItem(bSDlg, IDC_SPIN_BUY2);
	int Value1, Value2;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		
		int order = 1;
		int company = 0;

		pos = SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0,companies.getShares(company));
		pos = SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)1);
		SetFocus(hwndList3);
		return TRUE;
			break;
	}
	case WM_VSCROLL:
	{
		Value1 = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value1 = SetDlgItemInt(bSDlg, IDC_STOCKS_TO_BUY, val, true);
		Value1 = Value1;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value1 = GetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY, NULL, false);
			Value1 = Value1++;
			attemptBuy(order, company, Value1, Value2)
			break;
		case IDC_CANCEL:
			EndDialog(bSDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK SellShares(HWND sSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	int order = 1;
	int company = 0;
	HWND hwndList2 = GetDlgItem(sSDlg, IDC_STOCKS_TO_SELL);
	HWND hwndList3 = GetDlgItem(sSDlg, IDC_SPIN_SELL);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0,players.getShares(order,company)));
		SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)1);
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_VSCROLL:
	{
		Value = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(sSDlg, IDC_STOCKS_TO_SELL, val, true);
		Value = Value;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SELL:
			Value = GetDlgItemInt(sSDlg, IDC_SHARES_TO_SELL, NULL, false);
			Value = Value++;
			attemptSell(order, company, Value);
			EndDialog(sSDlg, LOWORD(wParam));
			break;
		case IDC_CANCEL:
			EndDialog(sSDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Orphans(HWND orDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(orDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndList2 = GetDlgItem(orDlg, IDC_ORPHAN_STOCKS);
	HWND hwndList3 = GetDlgItem(orDlg, IDC_SPIN_ORPHANS);

	switch (message)
	{
	case WM_INITDIALOG:
	{

		if (player != companies.getPresident(company))
		{
			// cout << "Only " << companies.name[company] << " president, " << players.name[companies.getPresident(company)] << " can buy orphan shares on behalf of company \n";
			return false;
		}
		pos = SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 40));
		pos = SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)3);
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_VSCROLL:
	{
		Value = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, val, true);
		Value = Value;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(orDlg, IDC_SHARES_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(orDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK InitValuation(HWND iVDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
	{

		HWND hwndList1 = GetDlgItem(iVDlg, IDC_VAL1);
		HWND hwndList2 = GetDlgItem(iVDlg, IDC_VAL2);
		HWND hwndList3 = GetDlgItem(iVDlg, IDC_VAL3);
		CheckRadioButton(iVDlg, IDC_RADIOV1, IDC_RADIOV3, IDC_RADIOV2);
		SetWindowText(hwndList1, L"$55");
		SetWindowText(hwndList2, L"$60");
		SetWindowText(hwndList3, L"$66");
		return TRUE;
	}

	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			if (IsDlgButtonChecked(iVDlg, IDC_RADIOV1))
			{
				Value = 0;
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV2))
			{
				Value = 1;
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV3))
			{
				Value = 2;
			}
			else
				MessageBox(iVDlg, L"Must select one option.", L"ERROR", MB_OKCANCEL);
			return TRUE;
			break;
		}
		return TRUE; 
	}
	return FALSE;
}
INT_PTR CALLBACK Scrap(HWND scDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(scDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(scDlg, IDC_LIST_TRAINS_OWNED);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		LRESULT pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		wchar_t STB[20] = L"T";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)playerName[i]); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_SCRAP:
			Value = GetDlgItemInt(scDlg, IDC_LIST_TRAINS_OWNED, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(scDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Train(HWND trDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(trDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(trDlg, IDC_LIST_TRAIN_FORSALE);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		LRESULT pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		wchar_t STB[20] = L"T";
		pos = (int)SendMessage(hwndList2, LB_ADDSTRING, 0, (LPARAM)playerName[i]); // Write line to list box
		SendMessage(hwndList2, LB_SETITEMDATA, pos, (LPARAM)i); // Assocaite index with each line
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(trDlg, IDC_TRACK_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(trDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Track(HWND tkDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList1 = GetDlgItem(tkDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(tkDlg, IDC_TRACK_TO_BUY);
	HWND hwndList3 = GetDlgItem(tkDlg, IDC_SPIN_TRACK);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 

		int i = 0;
		LRESULT pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		wchar_t STB[20] = L"T";
		Value = SetDlgItemInt(tkDlg, IDC_TRACK_TO_BUY, 0, true);
		pos = SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 40));
		pos = SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);

		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_VSCROLL:
	{
		Value = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(tkDlg, IDC_TRACK_TO_BUY, val * 10, true);
	}

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(tkDlg, IDC_TRACK_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(tkDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Profit(HWND prDlg, UINT message,WPARAM wParam, LPARAM lParam)
{

	HWND hwndList1 = GetDlgItem(prDlg, IDC_LIST_CURRENT_COMPANY);
	HWND hwndList2 = GetDlgItem(prDlg, IDC_PROFIT);
	HWND hwndList3 = GetDlgItem(prDlg, IDC_SPIN_PROFIT);

	switch (message)
	{
	case WM_INITDIALOG:
	{
		// List Company Information. 
//		int val;
		int i = 2;
		LRESULT pos = (int)SendMessage(hwndList1, LB_ADDSTRING, 0, (LPARAM)playerName[i]);
		// List Trains Owned. 
		wchar_t STB[20] = L"T";
		Value = SetDlgItemInt(prDlg, IDC_PROFIT, 0, true);
		pos = SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(0, 40));
		pos = SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		SetFocus(hwndList3);
		return TRUE;
		break;
	}
/*	case WM_NOTIFY:
	{
	switch (((LPNMHDR) lParam)-> code)
		{
		case UDN_DELTAPOS:
				UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
				Value = SetDlgItemInt(prDlg, IDC_PROFIT, val*10, true);
				Value = Value;
				break;
		}
	}*/
	case WM_VSCROLL:
	{
		Value = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(prDlg, IDC_PROFIT, val * 10, true);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUY:
			Value = GetDlgItemInt(prDlg, IDC_TRACK_TO_BUY, NULL, false);
			Value = Value++;
			break;
		case IDC_CANCEL:
			EndDialog(prDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
int TextToMonitor(HWND hDlg, wchar_t theText[])
{
	HWND hwndList = GetDlgItem(hDlg, IDC_MONITOR);
	int ndx = GetWindowTextLength(hwndList);
	SetFocus(hwndList);

#ifdef WIN32
	SendMessage(hwndList, EM_SETSEL, (WPARAM)ndx, (LPARAM)ndx);
#else
	SendMessageA(hwndList, EM_SETSEL, 0, MAKELONG(ndx, ndx));
#endif
	SendMessage(hwndList, EM_REPLACESEL, 0, (LPARAM)(theText));
	return(0);

}
void setupPlayers()
{
	// Set up players names

	int tempOrder[maxPlayers] = { 0,1,2,3,4,5 };
	long int currentTime = static_cast<long int>(time(0)); //Generate random seed
	srand(currentTime);

	for (int i = 0; i < numberPlayers; i++)
	{
		players.setCash(i, rand() % 10000); // assign random number
	}
	players.getTurnorders(tempOrder, numberPlayers);

	for (int i = 0; i < numberPlayers; i++)
	{
		int player = tempOrder[i];
		players.setCash(player, totalCash / numberPlayers);
		for (int j = 0; j < maxCompanies; j++)
		{
			players.setShares(player, j, 0);
			players.clearSold(player, j);
		}
//		players.getInfo(player);
	}
}
bool attemptBuy(int player, int company, int numberShares, int numberOrphans)
{
	// Player requests to buy

	int requestValuation;
	requestValuation = companies.getValuation(company);
	if ((numberShares + numberOrphans)*valuations[requestValuation] > players.getCash(player))
	{
		// cout << "****Need " << (numberShares + numberOrphans) * valuations[requestValuation] << " for purchase.****\n";
		return false;
	}
	else
	{
		// Proceed with sale
		test = companies.setStarted(company);
		test = companies.setValuation(company, requestValuation);
		test = companies.changeShares(company, -numberShares);
		test = companies.changeCash(company, (numberShares * valuations[requestValuation]));
		test = companies.changeOrphans(company, -numberOrphans);
		players.changeShares(player, company, (numberShares + numberOrphans));
		players.changeCash(player, -((numberShares + numberOrphans) * valuations[requestValuation]));
		// cout << "++" << players.name[player] << " now has " << players.getShares(player, company) << " of " << companies.name[company] << ".\n";
		// cout << "++" << players.name[player] << " now has $" << players.getCash(player) << ".\n";
		// cout << "++" << companies.name[company] << " now has " << companies.getShares(company) << " regualr shares and " << companies.getOrphans(company) << " orphan shares. \n";
		// cout << "++" << companies.name[company] << " now has $" << companies.getCash(company) << endl;
		test = setCompanyPresident(company);
		return true;
	}
}
bool attemptSell(int player, int company, int numberShares)
// This function will return a 1 when the player has sucessfully sold a stock for their turn.
{
	
	if (players.getShares(player, company) == 0)
	{
		return (0);
	}
	if (numberShares > players.getShares(player, company))
	{
		return false;
	}
	else if (players.totalSharesOwned(company) <= numberShares)
	{
		// cout << "****Can not sell all shares of company " << companies.name[company] << ". Reduce request.**** \n";
		return false;
	}
	else  // Proceed with sale
	{
		players.changeShares(player, company, -numberShares);
		// cout << "++" << players.name[player] << " now has " << players.getShares(player, company) << " of " << companies.name[company] << ".\n";
		players.changeCash(player, (numberShares * valuations[companies.getValuation(company)]));
		// cout << "++" << players.name[player] << " now has $" << players.getCash(player) << ".\n";
		companies.changeOrphans(company, numberShares);
		// cout << "++" << companies.name[company] << " now has " << companies.getShares(company) << " regualr shares and " << companies.getOrphans(company) << " orphan shares. \n";
		// cout << "++" << companies.name[company] << " now has $" << companies.getCash(company) << endl;
		// Reduce valuation if first sale of round by player. If second or greater sale there is no additional reduction.
		if (!players.getSold(player, company))
		{
			 companies.decreaseValuation(company);
			// cout << "++" << companies.name[company] << " has been sold this turn and its valuation has been reduced to $" << valuations[companies.getValuation(company)] << " per share. \n";
		}
		setCompanyPresident(company);
		players.setSold(player, company);
		return true;
	}
	return false;
}
bool attemptOrphan(int player, int company, int numberShares)
{
	int numberShares;
	int numberOrphanCompaniesControlled = 0;
	int company;
	int test;

	// cout << players.name[player] << " wants a company to buy orphaned stock \n";
	// cout << "Select company to buy its orphaned stock:\n";

	// cout << "How many orphaned shares of " << companies.name[company] << " do you want to buy back: ";
	// cout << companies.name[company] << " has $" << companies.getCash(company) << endl;

	else if (numberShares*valuations[companies.getValuation(company)] > companies.getCash(company))
	{
		// cout << "Need " << numberShares * valuations[companies.getValuation(company)] << " for purchase\n";
		return false;
	}
	else
	{

		test = companies.changeOrphans(company, -numberShares);
		test = companies.changeShares(company, numberShares);
		test = companies.changeCash(company, -numberShares * valuations[companies.getValuation(company)]);
		// cout << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares \n";
		return true;
	}
}
int reduceForOrphans()
{
	for (int j = 0; j < maxCompanies; j++)
	{
		if (companies.getOrphans(j) != 0)
		{
			companies.decreaseValuation(j);
			// cout << "++" << companies.name[j] << " valuation decreases to $" << valuations[companies.getValuation(j)] << " per share due to orphaned share. \n";
		}

	}
	return (0);
}
void setPlayerNetworth()
{
	for (int i = 0; i < numberPlayers; i++)
	{
		int sum = players.getCash(i);
		for (int j = 0; j<maxCompanies; j++)
		{
			sum += players.getShares(i, j) * valuations[companies.getValuation(j)];
		}
		players.setNetworth(i, sum);
	}
	return;
}
char readValue(ifstream& inputStream, char cArray[])
{
	char getChar;
	inputStream.get(getChar);
	int j = 0;
	while ((getChar != ',') && (getChar != '\n'))
	{
		cArray[j] = getChar;
		j++;
		inputStream.get(getChar);
	}
	cArray[j] = '\0';
	return(getChar);
}
char findEOL(ifstream& inputStream)
{
	char getChar;
	do
	{
		inputStream.get(getChar);
	} while (getChar != '\n');
	return(getChar);
}
int fetchFileData(wchar_t pfileName[])
{

	char getChar;
	ifstream inDataStream;
	int rnumberPlayers = 0;
	inDataStream.open(pfileName);
	if (inDataStream.fail())
	{
//		// cout << "Input file stream open failed \n";
		return(1);
	}
	size_t numChars=20;
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	while (strcmp(testArray, "Done#") != 0)	//Read player names
	{
		mbstowcs_s(&numChars,players.name[rnumberPlayers],20,testArray,20);
		rnumberPlayers++;
		getChar = readValue(inDataStream, testArray);
	}
	numberPlayers = rnumberPlayers;
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int i = 0; i < numberPlayers; i++) // Read player cash
	{
		getChar = readValue(inDataStream, testArray);
		players.setCash(i, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) // Read player shares owned
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			players.setShares(i, j, atoi(testArray));
		}
		findEOL(inDataStream);
		getChar = readValue(inDataStream, testArray);
	}
	for (int j = 0; j < maxCompanies; j++) // Read player sold company
	{
		for (int i = 0; i < numberPlayers; i++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				players.setSold(i, j);
			else
				players.clearSold(i, j);
		}
		findEOL(inDataStream);
		getChar = readValue(inDataStream, testArray);
	}

	for (int j = 0; j < maxCompanies; j++) //Read company name (and discard since fixed)
	{
		getChar = readValue(inDataStream, testArray);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++)//Read company cash
	{
		getChar = readValue(inDataStream, testArray);
		companies.setCash(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company net profit
	{
		getChar = readValue(inDataStream, testArray);
		companies.setNetprofit(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company valuation
	{
		getChar = readValue(inDataStream, testArray);
		companies.setValuation(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company presidents
	{
		getChar = readValue(inDataStream, testArray);
		companies.setPresident(j, atoi(testArray));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company shares
	{
		getChar = readValue(inDataStream, testArray);
		companies.changeShares(j, atoi(testArray) - companies.getShares(j));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read comapny orphans
	{
		getChar = readValue(inDataStream, testArray);
		companies.changeOrphans(j, atoi(testArray) - companies.getOrphans(j));
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company started
	{
		getChar = readValue(inDataStream, testArray);
		if (atoi(testArray) == 1)
			companies.setStarted(j);
		else
			companies.clearStarted(j);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company recievership
	{
		getChar = readValue(inDataStream, testArray);
		if (atoi(testArray) == 1)
			companies.setRecievership(j);
		else
			companies.clearRecievership(j);
	}
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	for (int j = 0; j < maxCompanies; j++) //Read company turn order
	{
		getChar = readValue(inDataStream, testArray);
		companies.setOrder(j, atoi(testArray));
	}
	findEOL(inDataStream);
	for (int m = 0; m < maxCities; m++) // Read cities
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			if (atoi(testArray) == 1)
				companies.setCity(j, m);
			else
				companies.clearCity(j, m);
		}
		findEOL(inDataStream);
	}
	for (int k = 0; k < maxTrains; k++) // Read trains
	{
		getChar = readValue(inDataStream, testArray);
		for (int j = 0; j < maxCompanies; j++)
		{
			getChar = readValue(inDataStream, testArray);
			companies.setTrains(j, k, atoi(testArray));
		}
		findEOL(inDataStream);
	}
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	gameRound = atoi(testArray);
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	turn = atoi(testArray);
	techLevel = companies.getTechLevel(1);
	return(0);
}
int pushFileData(wchar_t pfileName[])
{

	ofstream outDataStream;
	outDataStream.open(pfileName);
	if (outDataStream.fail())
	{
		// cout << "Output file stream open failed \n";
		return(1);
	}

	outDataStream << "Name,";
	for (int i = 0; i < numberPlayers; i++) // Save player name
	{
		outDataStream << players.name[i] << ",";
	}
	outDataStream << "Done#," << "\n";
	outDataStream << "Cash,";
	for (int i = 0; i < numberPlayers; i++) // Save player cash
	{
		outDataStream << players.getCash(i) << ",";
	}
	outDataStream << "\n";
	for (int j = 0; j < maxCompanies; j++) // Save player shares owned
	{
		outDataStream << companies.name[j] << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getShares(i, j) << ",";
		}
		outDataStream << "\n";
	}
	for (int j = 0; j < maxCompanies; j++) // Save player sold company
	{
		outDataStream << companies.name[j] << ",";
		for (int i = 0; i < numberPlayers; i++)
		{
			outDataStream << players.getSold(i, j) << ",";
		}
		outDataStream << "\n";
	}	outDataStream << "Names,";
	for (int j = 0; j < maxCompanies; j++) // Save company names
	{
		outDataStream << companies.name[j] << ",";
	}
	outDataStream << "\n";
	outDataStream << "Cash, ";
	for (int j = 0; j < maxCompanies; j++) // Save company cash
	{
		outDataStream << companies.getCash(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "NetProfit,";
	for (int j = 0; j < maxCompanies; j++) // Save company netprofit
	{
		outDataStream << companies.getNetprofit(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Valuation, ";
	for (int j = 0; j < maxCompanies; j++) //Save valuation
	{
		outDataStream << companies.getValuation(j) << ",";
	}
	outDataStream << "\n";

	outDataStream << "President, ";
	for (int j = 0; j < maxCompanies; j++) //Save president
	{
		outDataStream << companies.getPresident(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Shares, ";
	for (int j = 0; j < maxCompanies; j++) // Save Company shares
	{
		outDataStream << companies.getShares(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Orphans, ";
	for (int j = 0; j < maxCompanies; j++) // Sace Company orphans
	{
		outDataStream << companies.getOrphans(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Started, ";
	for (int j = 0; j < maxCompanies; j++) // SAve company started
	{
		outDataStream << companies.getStarted(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "Reship, ";
	for (int j = 0; j < maxCompanies; j++)//Save company recievership
	{
		outDataStream << companies.getRecievership(j) << ",";
	}
	outDataStream << "\n";
	outDataStream << "TurnOrder, ";
	for (int j = 0; j < maxCompanies; j++) // Save company turnorder
	{
		outDataStream << companies.getOrder(j) << ",";
	}
	outDataStream << "\n";
	for (int m = 0; m < maxCities; m++) // Save cities
	{
		outDataStream << cities.name[m] << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getCity(j, m) << ",";
		}
		outDataStream << "\n";
	}
	for (int k = 0; k < maxTrains; k++) // Save trains
	{
		outDataStream << k + 1 << ",";
		for (int j = 0; j < maxCompanies; j++)
		{
			outDataStream << companies.getTrains(j, k) << ",";
		}
		outDataStream << "\n";
	}
	outDataStream << ("Round,");
	outDataStream << (gameRound) << ", \n";
	outDataStream << ("Turn,");
	outDataStream << (turn) << ", \n";
	for (int k = 0; k < maxTrains + 1; k++) // make sure rows are long enough
	{
		outDataStream << k << ",";
	}
	outDataStream << "\n";
	outDataStream.close();
	companies.getTechLevel(techLevel);
	return(0);
}
int setCompanyPresident(int company)
{
	int oldpresident = companies.getPresident(company);
	int president = oldpresident;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getShares(i, company) > players.getShares(president, company))
		{
			companies.setPresident(company, i);
			president = i;
		}
	}
	if (president != oldpresident)
	{
		cout << "++" << players.name[president] << " is the new president for " << companies.name[company] << endl;
	}
	return(president);
}



