// BOBanker.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "BOBanker.h"
#include "Players.h"
#include "Cities.h"
#include "Companies.h"
#include "TechLevels.h"
#include "Trains.h"
#include "Game.h"



#define MAX_LOADSTRING 100

#define BUTTON_IDENTIFIER 1

void setupPlayers();
//Precondition: None
//Postcondition: After running all the players will be loaded into the class players
// with initial values and turn orders set

bool attemptBuy(int player, int company, int shares, int orphans, HWND hwnd);
//Precondition: player < numberPlayers
//Postcondition: returns 1 if successfull and handles all the buying transactions

bool attemptSell(int player, int company, int shares);
// This function will return a 1 when the player has sucessfully sold a stock for their turn.

bool attemptOrphan(int player, int company, int value, HWND hwnd);
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

int updateInfo();
//Precondition: None
//Postcondition: Current values for player cash and networth and company cash are displayed in
//				edit control boxes 

int updateBuild(int company);
//Precondition: The index of the current company is passed in company
//Postcondition: The build window is refreshed for the current company.

int incrementTurn(bool success);
//Precondition: The routine is passed whether or not the operation was successful
// in success.
//Postcondition: The function will change turn to the next index to use
// in the order array to select the curren company/player.


void BuildTrack(int company, int amount);


void setCompanyDividens(int currentCompany, int option);
//Preconditions: For the current company given in currentCompany to pay dividens to shareholders and itself. Uses companyNetprofit, companySharesowned, and playerShares.
//Postcondition: Profits distributed to companyCash and playerCash

void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan);
//Preconditions: For the company given by CurrentCompany that had a net profit change given by netProfitChange (decrease = -1, increase = 1
// and same = 0) and the decision to withhold (w) or distribute (d) dividens given in companyDividenPlan. Function also uses companyValuations
//Postcondition: Valuation of company is decrease if it had a decrease in net profit. It increase if net profit increased and dividens were paied. 
//Otherwise valuation stays the same.






char readValue(ifstream& inputStream, char cArray[]);
//Precondtions: Open stream in in inputStrean and character array is returned in cArray
//Postcondition: Will return last character read.

char findEOL(ifstream& inputStream);
//Precondition: Stream to be read for end of line is in inputStream
//				It is expected that all relevant values have been read
//Postcondition: Stream cleared. Last character returned. Should be /n if successful

// Data processing 

int fetchFileData(wchar_t pfileName[]);

int getManualData();

int pushFileData(wchar_t pfileName[]);


// Global Variables:

HWND marketDlg;
HWND buildDlg;
HWND infoDlg;
HWND mainDlg;

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
typedef HWND * HPointer;
HPointer PlayerValues;
Game game(0,1);
int playerTurnorder[maxPlayers];
int companyTurnorder[maxCompanies];
int netprofitChange;
int requestValuation;
int numberPasses = 0;
int netCash;



// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Players(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    PlayerInfo(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Market(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Info(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    BuyShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    SellShares(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Orphans(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InitValuation(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Scrap(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Train(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Track(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    Build(HWND, UINT, WPARAM, LPARAM);
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

   ShowWindow(hWnd, SW_MAXIMIZE);
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
			game.setRound(1);
			game.setTurn(0);
			DialogBox(hInst, MAKEINTRESOURCE(IDD_PLAYERBOX), hWnd, Players);
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_INFOBOX), hWnd, Info);	
			DialogBox(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), hWnd, Market);
		break;
		case IDM_START_SAVEDGAME:
			fetchFileData(wfileName);
			players.getTurnorders(playerTurnorder, numberPlayers);
			companies.getTurnorders(companyTurnorder);
			CreateDialog(hInst, MAKEINTRESOURCE(IDD_INFOBOX), hWnd, Info);
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
			setupPlayers();
			players.getTurnorders(playerTurnorder, numberPlayers);
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
			numberPlayers = 6;
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
INT_PTR CALLBACK Info(HWND iNDlg, UINT message, WPARAM wParam, LPARAM lParam)
{

	//	wchar_t cashStr[10];
	infoDlg = iNDlg;


	HWND hwndList = GetDlgItem(iNDlg, IDC_LIST_CURRENT_PLAYER);
	HWND hwndLP2 = GetDlgItem(iNDlg, IDC_LISTPLAYER2);
	HWND hwndCP = GetDlgItem(iNDlg, IDC_CURRENT_PLAYER);
	switch (message)
	{

	case WM_INITDIALOG:
	{

		wchar_t cashStr[20];
		for (int i = 0; i < numberPlayers; i++)
		{
			ShowWindow(GetDlgItem(iNDlg, IDC_PN1 + i), SW_SHOW);
			SetWindowText(GetDlgItem(iNDlg, IDC_PN1 + i), (LPCWSTR)players.name[i]);
			_itow_s(players.getCash(i), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, IDC_PC1 + i), SW_SHOW);
			SetWindowText(GetDlgItem(iNDlg, IDC_PC1 + i), (LPCWSTR)cashStr);
			_itow_s(players.getNetworth(i), cashStr, 10);
			ShowWindow(GetDlgItem(iNDlg, IDC_PW1 + i), SW_SHOW);
			SetWindowText(GetDlgItem(iNDlg, IDC_PW1 + i), (LPCWSTR)cashStr);
		}
		for (int j = 0; j < maxCompanies; j++)
		{
			SendMessageW(GetDlgItem(iNDlg, IDC_BBO + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BITMAP1 + j)));
			_itow_s(companies.getCash(j), cashStr, 10);
			SetWindowText(GetDlgItem(iNDlg, IDC_CV0 + j), (LPCWSTR)cashStr);
		}
		wchar_t monitorText[40];
		wcscpy_s(monitorText, L"Here we go");
		TextToMonitor(iNDlg, monitorText);
	return TRUE;
	}
	case WM_PAINT:
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_NEXT:
		{
			if (game.getRound() == 1)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_MARKETBOX), iNDlg, Market);
			}
			else
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_BUILDBOX), iNDlg, Build);
			}
			return TRUE;
		}
		case IDC_CANCEL:
			EndDialog(iNDlg, LOWORD(wParam));
			return TRUE;
			break;
		}
		return TRUE;
	}
	return FALSE;
}
INT_PTR CALLBACK Market(HWND maDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	marketDlg = maDlg;
	HWND hwndCP = GetDlgItem(maDlg, IDC_CURRENT_PLAYER);
	switch (message)
	{

	case WM_INITDIALOG:
	{
		HFONT bigFont;
		bigFont = CreateFont(40, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, '\0');
		SendMessage(hwndCP, WM_SETFONT, (WPARAM)bigFont, 0);
		SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
		return TRUE;
	}
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(maDlg, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(4));
		//			TextOut(hdc, 5, 5, greeting, _tcslen(greeting));
		// TODO: Add any drawing code that uses hdc here...h
		EndPaint(maDlg, &ps);
	}
	break;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BUTTON_BUY:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_BUYBOX), maDlg, BuyShares);
			SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
			break;
		case IDC_BUTTON_SELL:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_SELLBOX), maDlg, SellShares);
			break;
		case IDC_BUTTON_ORPHANS:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ORPHANBOX), maDlg, Orphans);
			SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);
			break;
		case IDC_BUTTON_DONE:
			if (incrementTurn(false) == 0)
			{
				SetWindowText(hwndCP, (LPCWSTR)players.name[playerTurnorder[game.getTurn()]]);	
			}
			else
			{
				EndDialog(maDlg, LOWORD(wParam));
				return TRUE;
			}		
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
INT_PTR CALLBACK Build(HWND bDDlg, UINT message,WPARAM wParam, LPARAM lParam)
{

	HWND hwndCC = GetDlgItem(bDDlg, IDC_CURRENT_COMPANY);
	HWND hwndList1 = GetDlgItem(bDDlg, IDC_SPIN_TRACK);
	HWND hwndList2 = GetDlgItem(bDDlg, IDC_SPIN_PROFIT);
//	HWND hwndList3 = GetDlgItem(bDDlg, IDC_SPIN_SELL);
	HWND hwndNC = GetDlgItem(bDDlg, IDC_NETCASH);
	int company, profit, track, newNetCash;
	buildDlg = bDDlg;
	wchar_t buildList[500];
	wchar_t intStr[100];
	wchar_t ncStr[30];




	switch (message)
	{
	case WM_INITDIALOG:
	{
		HFONT bigFont;
//		wchar_t cashStr[20];
		company = companyTurnorder[game.getTurn()];
		netCash = companies.getCash(company);
		_itow_s(netCash, intStr, 10);
		wcscpy_s(ncStr, intStr);
		SetWindowText(hwndNC, ncStr);
		bigFont = CreateFont(20, 0, 0, 0, FW_NORMAL, 0, 0, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, '\0');
		SendMessage(hwndCC, WM_SETFONT, (WPARAM)bigFont, 0);
		SetWindowText(hwndCC, (LPCWSTR)companies.name[company]);
	// List trains to scrap
		for (int j = 0; j < companies.getTrainCount(company); j++)
		{
			ShowWindow(GetDlgItem(bDDlg, IDC_TS0 + j), SW_SHOW);
			ShowWindow(GetDlgItem(bDDlg, IDC_TSN0 + j), SW_SHOW);
			SetDlgItemInt(bDDlg, IDC_TSN0 + j, companies.getTrainOwned(company, j), true);
		}
		int train = companies.getTrainForSale();
		ShowWindow(GetDlgItem(bDDlg, IDC_TB0), SW_SHOW);
		ShowWindow(GetDlgItem(bDDlg, IDC_TBN0), SW_SHOW);
		SetDlgItemInt(bDDlg, IDC_TBN0, train, true);

//		SendMessageW(GetDlgItem(bDDlg, IDC_TB0), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + train)));
	// Intitialize Spin Boxes
		SendMessage(hwndList1, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(20, 0));
		SendMessage(hwndList1, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		SendMessage(hwndList2, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(60, 0));
		SendMessage(hwndList2, UDM_SETPOS, (WPARAM)0, (LPARAM)companies.getNetprofit(company)/10);
		UINT val1 = LOWORD(SendMessage(hwndList1, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(bDDlg, IDC_TRACK_TO_BUY, val1 * 10, true);
		UINT val2 = LOWORD(SendMessage(hwndList2, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(bDDlg, IDC_PROFIT, val2 * 10, true);
		SendMessage(GetDlgItem(bDDlg, IDC_DIST), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
		SendMessage(GetDlgItem(buildDlg, IDC_WITH), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		SetFocus(hwndList1);
		return TRUE;
		break;
	}
	case WM_VSCROLL:
	{
		UINT val1 = LOWORD(SendMessage(hwndList1, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(bDDlg, IDC_TRACK_TO_BUY, val1 * 10, true);
		newNetCash = netCash;
		newNetCash -= val1 * 10;
		_itow_s(newNetCash, intStr, 10);
		wcscpy_s(ncStr, intStr);
		SetWindowText(hwndNC, ncStr);
		UINT val2 = LOWORD(SendMessage(hwndList2, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(bDDlg, IDC_PROFIT, val2 * 10, true);
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_TS0:
			if (IsDlgButtonChecked(bDDlg, IDC_TS0))
			{
				netCash += trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 0)];
			}
			else
			{
				netCash -= trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 0)];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_TS1:
			if (IsDlgButtonChecked(bDDlg, IDC_TS1))
			{
				netCash += trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 1)];
			}
			else
			{
				netCash -= trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 1)];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_TS2:
			if (IsDlgButtonChecked(bDDlg, IDC_TS2))
			{
				netCash += trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 2)];
			}
			else
			{
				netCash -= trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 2)];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_TS3:
			if (IsDlgButtonChecked(bDDlg, IDC_TS3))
			{
				netCash += trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 3)];
			}
			else
			{
				netCash -= trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 3)];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_TS4:
			if (IsDlgButtonChecked(bDDlg, IDC_TS4))
			{
				netCash += trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 4)];
			}
			else
			{
				netCash -= trains.scrap[companies.getTrainOwned(companyTurnorder[game.getTurn()], 4)];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_TB0:
			if (IsDlgButtonChecked(bDDlg, IDC_TB0))
			{
				netCash -= trains.cost[companies.getTrainForSale()];
			}
			else
			{
				netCash += trains.cost[companies.getTrainForSale()];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			ShowWindow(GetDlgItem(bDDlg, IDC_TB1), SW_SHOW);
			ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + 1), SW_SHOW);
			SetDlgItemInt(bDDlg, IDC_TBN0 + 1, companies.getTrainForSale()+1, true);
			break;
		case IDC_TB1:
			if (IsDlgButtonChecked(bDDlg, IDC_TB1))
			{
				netCash -= trains.cost[companies.getTrainForSale()+1];
			}
			else
			{
				netCash += trains.cost[companies.getTrainForSale()+1];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			ShowWindow(GetDlgItem(bDDlg, IDC_TB2), SW_SHOW);
			ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + 2), SW_SHOW);
			SetDlgItemInt(bDDlg, IDC_TBN0 + 2, companies.getTrainForSale() + 2, true);
			break;
		case IDC_TB2:
			if (IsDlgButtonChecked(bDDlg, IDC_TB2))
			{
				netCash -= trains.cost[companies.getTrainForSale() + 2];
			}
			else
			{
				netCash += trains.cost[companies.getTrainForSale() + 2];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			ShowWindow(GetDlgItem(bDDlg, IDC_TB3), SW_SHOW);
			ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + 3), SW_SHOW);
			SetDlgItemInt(bDDlg, IDC_TBN0 + 3, companies.getTrainForSale() + 3, true);
			break;
		case IDC_TB3:
			if (IsDlgButtonChecked(bDDlg, IDC_TB3))
			{
				netCash -= trains.cost[companies.getTrainForSale() + 3];
			}
			else
			{
				netCash += trains.cost[companies.getTrainForSale() + 3];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			ShowWindow(GetDlgItem(bDDlg, IDC_TB4), SW_SHOW);
			ShowWindow(GetDlgItem(bDDlg, IDC_TBN0 + 4), SW_SHOW);
			SetDlgItemInt(bDDlg, IDC_TBN0 + 4, companies.getTrainForSale() + 4, true);
			break;
		case IDC_TB4:
			if (IsDlgButtonChecked(bDDlg, IDC_TB4))
			{
				netCash -= trains.cost[companies.getTrainForSale() + 4];
			}
			else
			{
				netCash += trains.cost[companies.getTrainForSale() + 4];
			}
			_itow_s(netCash, intStr, 10);
			wcscpy_s(ncStr, intStr);
			SetWindowText(hwndNC, ncStr);
			break;
		case IDC_OK:
//			UINT val1 = LOWORD(SendMessage(hwndList1, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
			netCash -= LOWORD(SendMessage(hwndList1, UDM_GETPOS, (WPARAM)0, (LPARAM)0)) * 10;

			if (netCash < 0)
			{
				MessageBox(bDDlg, L"Can not have negative cash balance. Please try again.", L"Build Error", 0);
				updateBuild(companyTurnorder[game.getTurn()]);
			}
			else
			{ 
				company = companyTurnorder[game.getTurn()];
				wcscpy_s(buildList, L"Building actions: \n ");
				// List trains to scrap
				for (int k = 0; k < companies.getTrainCount(company); k++)
				{
					if (IsDlgButtonChecked(bDDlg, IDC_TS0 + k))
					{
						wcscat_s(buildList, L"*Scrap train ");
						_itow_s(companies.getTrainOwned(company, k), intStr, 10);
						wcscat_s(buildList, intStr);
						wcscat_s(buildList, L". \n");
					}
				}

				// List next train for sale
				for (int k = 0; k < 4; k++)
				{
					if (IsDlgButtonChecked(bDDlg, IDC_TB0 + k))
					{
						wcscat_s(buildList, L"*Buy train ");
						_itow_s(companies.getTrainForSale() + k, intStr, 10);
						wcscat_s(buildList, intStr);
						wcscat_s(buildList, L". \n");
					}
				}

				track = GetDlgItemInt(bDDlg, IDC_TRACK_TO_BUY, NULL, false);
				wcscat_s(buildList, L"*Build $");
				_itow_s(track, intStr, 10);
				wcscat_s(buildList, intStr);
				wcscat_s(buildList, L" of track \n");
				profit = GetDlgItemInt(bDDlg, IDC_PROFIT, NULL, false);

				wcscat_s(buildList, L"*Set net profit to $");
				_itow_s(profit, intStr, 10);
				wcscat_s(buildList, intStr);
				wcscat_s(buildList, L". \n");

				int dividend;
				if (IsDlgButtonChecked(bDDlg, IDC_DIST))
				{
					dividend = 0;
					wcscpy_s(intStr, L"*Distribute dividiens. \n");
					wcscat_s(buildList, intStr);

				}
				else
				{
					dividend = 1;
					wcscpy_s(intStr, L"*Withhold dividens. \n");
					wcscat_s(buildList, intStr);
				}
				if (MessageBox(bDDlg, buildList, L"Confirm Buy", MB_OKCANCEL) == 1)
				{
					for (int k = 0; k < companies.getTrainCount(company); k++)
					{
						if (IsDlgButtonChecked(bDDlg, IDC_TS0 + k))
						{
							companies.changeCash(company, companies.scrapTrainOfCompany(companies.getTrainOwned(company, k), company));
						}
					}
					for (int k = 0; k < 4; k++)
					{
						if (IsDlgButtonChecked(bDDlg, IDC_TB0 + k))
						{
							companies.changeCash(company, -companies.buyTrainForSale(company, techLevel));
						}
					}
					BuildTrack(company, track); //Build track
					if (profit == 0)
					{
						netprofitChange = -2;
					}
					else if (profit > companies.getNetprofit(company))
					{
						netprofitChange = 1;
					}
					else if (profit < companies.getNetprofit(company))
					{
						netprofitChange = -1;
					}
					else
					{
						netprofitChange = 0;
					}
					companies.setNetprofit(company, profit);
					//				cout << "++" << companies.name[company] << " netprofit is now " << profit << ", \n";
					setCompanyDividens(company, dividend);
					setCompanyValuation(company, netprofitChange, dividend);
					//				cout << "++" << companies.name[company] << " has $ " << companies.getCash(company) << endl;
					updateInfo();
					if (incrementTurn(false) == 0)
					{
						updateBuild(companyTurnorder[game.getTurn()]);
					}
					else
					{
						EndDialog(bDDlg, LOWORD(wParam));
						return TRUE;
					}
				}
			}
			break;
		case IDC_CANCEL:
			EndDialog(bDDlg, LOWORD(wParam));
			return TRUE;
			break;


		}
		return TRUE;
	}
	return FALSE;
} 
INT_PTR CALLBACK BuyShares(HWND bSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{
	HWND hwndList2 = GetDlgItem(bSDlg, IDC_SHARES_TO_BUY1);
	HWND hwndList3 = GetDlgItem(bSDlg, IDC_SPIN_BUY1);
	HWND hwndList4 = GetDlgItem(bSDlg, IDC_SHARES_TO_BUY2);
	HWND hwndList5 = GetDlgItem(bSDlg, IDC_SPIN_BUY2);
	int Value1, Value2;
	int company;

	switch (message)
	{
	case WM_INITDIALOG:
	{
		SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		for (int j = 0; j <maxCompanies; j++)
		{
			if (companies.getAvailable(j, techLevel))
			{
				ShowWindow(GetDlgItem(bSDlg, IDC_RCB0 + j), SW_SHOW);
				SendMessageW(GetDlgItem(bSDlg, IDC_RCB0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + j)));
			}
		}
		SetFocus(hwndList3);
		return 0;
			break;
	}
	case WM_VSCROLL:
	{
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY1, val, true);
		val = LOWORD(SendMessage(hwndList5, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY2, val, true);
		return 0;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RCB0:
			company = 0;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB1:
			company = 1;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB2:
			company = 2;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB3:
			company = 3;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB4:
			company = 4;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB5:
			company = 5;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB6:
			company = 6;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB7:
			company = 7;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB8:
			company = 8;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_RCB9:
			company = 9;
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList5, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getShares(company), 0));
			SendMessage(hwndList5, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(companies.getOrphans(company), 0));
			break;
		case IDC_BUY:
			for (int j = 0; j < maxCompanies; j++)
			{
				if (IsDlgButtonChecked(bSDlg, IDC_RCB0 + j))
				{
					company = j;
				}
			}
			requestValuation = companies.getValuation(company);
			if (companies.getStarted(company) == 0)
			{
				DialogBox(hInst, MAKEINTRESOURCE(IDD_INITVALBOX), bSDlg, InitValuation);
			}
			Value1 = GetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY1, NULL, false);
			Value2 = GetDlgItemInt(bSDlg, IDC_SHARES_TO_BUY2, NULL, false);
			if (attemptBuy(playerTurnorder[game.getTurn()], company, Value1, Value2, bSDlg))
			{
				incrementTurn(true);
				EndDialog(bSDlg, LOWORD(wParam));
				updateInfo();
				return 1;
			}
			
				break;
		case IDC_CANCEL:
			EndDialog(bSDlg, LOWORD(wParam));
			return 0;
			break;
		}
		return 0;
	}
	return 0;
}
INT_PTR CALLBACK SellShares(HWND sSDlg, UINT message,WPARAM wParam, LPARAM lParam)
{

	HWND hwndList2 = GetDlgItem(sSDlg, IDC_SHARES_TO_SELL);
	HWND hwndList3 = GetDlgItem(sSDlg, IDC_SPIN_SELL);
	int numberShares, company;
	int order = playerTurnorder[game.getTurn()];



	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (int j = 0; j < players.getCompanyCount(order); j++)
		{
			ShowWindow(GetDlgItem(sSDlg, IDC_RCS0 + j), SW_SHOW);
			SendMessageW(GetDlgItem(sSDlg, IDC_RCS0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + players.getCompanyOwned(order,j))));
		}
		SetFocus(hwndList2);
		return TRUE;
	}
	case WM_VSCROLL:
	{
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		SetDlgItemInt(sSDlg, IDC_STOCKS_TO_SELL, val, true);
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RCS0:
			company = players.getCompanyOwned(order, 0);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares,0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		break;
		case IDC_RCS1:
			company = players.getCompanyOwned(order,1);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares,0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS2:
			company = players.getCompanyOwned(order, 2);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares,0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS3:
			company = players.getCompanyOwned(order, 3);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS4:
			company = players.getCompanyOwned(order, 4);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS5:
			company = players.getCompanyOwned(order, 5);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS6:
			company = players.getCompanyOwned(order, 6);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS7:
			company = players.getCompanyOwned(order, 7);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS8:
			company = players.getCompanyOwned(order, 8);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCS9:
			company = players.getCompanyOwned(order, 9);
			numberShares = players.getShares(order, company);
			if (numberShares >= players.totalSharesOwned(company))
				numberShares--;
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;	
		case IDC_SELL:
			for (int j = 0; j < players.getCompanyCount(order); j++)
			{
				if (IsDlgButtonChecked(sSDlg, IDC_RCS0 + j))
				{
					company = players.getCompanyOwned(order,j);
				}
			}
			Value = GetDlgItemInt(sSDlg, IDC_SHARES_TO_SELL, NULL, false);
			if (attemptSell(order, company, Value))
			{
				updateInfo();
				EndDialog(sSDlg, LOWORD(wParam));
				return TRUE;
			}
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
	HWND hwndList2 = GetDlgItem(orDlg, IDC_ORPHANS_TO_BUY);
	HWND hwndList3 = GetDlgItem(orDlg, IDC_SPIN_ORPHANS);
	int numberShares;
	int company; 
	switch (message)
	{
	case WM_INITDIALOG:
	{
		for (int j = 0; j < companies.getCompanyRunCount(playerTurnorder[game.getTurn()]); j++)
		{
			ShowWindow(GetDlgItem(orDlg, IDC_RCO0 + j), SW_SHOW);
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], j);
			SendMessageW(GetDlgItem(orDlg, IDC_RCO0 + j), BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)LoadBitmap(GetModuleHandle(NULL), MAKEINTRESOURCE(IDB_BOS + company)));
		}
		SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, val, true);
		SetFocus(hwndList2);
		break;
	}

	case WM_VSCROLL:
	{
		Value = LOWORD(wParam);
		UINT val = LOWORD(SendMessage(hwndList3, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
		Value = SetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, val, true);
		Value = Value;
		break;
	}
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_RCO0:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 0);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO1:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 1);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO2:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 2);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO3:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 3);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO4:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 4);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO5:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 5);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO6:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 6);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO7:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 7);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO8:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 8);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_RCO9:
			company = companies.getCompanyRun(playerTurnorder[game.getTurn()], 9);
			numberShares = companies.getOrphans(company);
			SendMessage(hwndList3, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(numberShares, 0));
			SendMessage(hwndList3, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
			break;
		case IDC_BUY:
			Value = GetDlgItemInt(orDlg, IDC_ORPHANS_TO_BUY, NULL, false);
			for (int j = 0; j < companies.getCompanyRunCount(playerTurnorder[game.getTurn()]); j++)
			{
				if (IsDlgButtonChecked(orDlg, IDC_RCO0 + j))
				{
					company = companies.getCompanyRun(playerTurnorder[game.getTurn()], j);
				}
			}
			if (attemptOrphan(playerTurnorder[game.getTurn()], company, Value, orDlg))
			{
				updateInfo();
				incrementTurn(true);
				EndDialog(orDlg, LOWORD(wParam));
				return TRUE;
			}
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
		wchar_t valStr[100];
		wchar_t intStr[100];
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.lowVal[techLevel-1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList1,valStr);
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.medVal[techLevel-1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList2, valStr);
		wcscpy_s(valStr, L"$ ");
		_itow_s(valuations[techLevels.highVal[techLevel-1]], intStr, 10);
		wcscat_s(valStr, intStr);
		wcscat_s(valStr, L" per share.");
		SetWindowText(hwndList3, valStr);
		return TRUE;
	}

	
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_OK:
			if (IsDlgButtonChecked(iVDlg, IDC_RADIOV1))
			{
				requestValuation = techLevels.lowVal[techLevel-1];
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV2))
			{
				requestValuation = techLevels.medVal[techLevel-1];
			}
			else if (IsDlgButtonChecked(iVDlg, IDC_RADIOV3))
			{
				requestValuation = techLevels.highVal[techLevel-1];
			}
			else
				MessageBox(iVDlg, L"Must select one option.", L"ERROR", MB_OKCANCEL);
			EndDialog(iVDlg, LOWORD(wParam));
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
bool attemptBuy(int player, int company, int numberShares, int numberOrphans, HWND hwnd)
{
	// Player requests to buy


	if ((numberShares + numberOrphans)*valuations[requestValuation] > players.getCash(player))
	{
		MessageBox(hwnd, L"Not enough funds to buy shares. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else if (players.getSold(player, company))
	{
		MessageBox(hwnd, L"Can not buy shares of company sold during round. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else
	{
		// Proceed with sale
		int test;
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
bool attemptOrphan(int player, int company, int numberShares, HWND hwnd)
{

	if (numberShares*valuations[companies.getValuation(company)] > companies.getCash(company))
	{
		MessageBox(hwnd, L"Not enough funds to buy shares. Please try again.", L"Buy Error", MB_OK);
		return false;
	}
	else
	{

		companies.changeOrphans(company, -numberShares);
		companies.changeShares(company, numberShares);
		companies.changeCash(company, -numberShares * valuations[companies.getValuation(company)]);
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
void BuildTrack(int company, int amount)
{
	companies.changeCash(company, -amount);
	cout << companies.name[company] << " now has $" << companies.getCash(company) << endl;
	return;
}

void setCompanyValuation(int company, int netProfitChange, char companyDividenPlan)
{
	if (netProfitChange < 0)
	{
		if (companies.getNetprofit(company) != 0)
		{
			companies.decreaseValuation(company);
			cout << "++" << companies.name[company] << " valuation decreases to $" << valuations[companies.getValuation(company)] << " per share. \n";
		}
		else
		{
			// Unprofitable Company
			int badPresident = companies.getPresident(company);
			companies.decreaseValuation(company);
			companies.decreaseValuation(company);
			cout << "++" << companies.name[company] << " valuation decreases to $" << valuations[companies.getValuation(company)] << " per share. \n";
			companies.changeShares(company, 1);
			players.changeShares(setCompanyPresident(company), company, -1);
			cout << "++" << players.name[badPresident] << " shares in " << companies.name[company] << " has been reduced to " << players.getShares(badPresident, company) << " shares. \n";
			if (players.getShares(setCompanyPresident(company), company) == 0)
			{
				// Company goes into recievership
				cout << "++" << companies.name[company] << " goes into recievership. \n";
				companies.setRecievership(company); // Place into revievership
				companies.clearStarted(company); // Clear started flag
				companies.setValuation(company, 4); //Set valuation to $50
				companies.scrapCompanyTrains(company); //Scrap all trains
				companies.setCash(company, 0); //Clear out treasurey
				companies.changeOrphans(company, -companies.getOrphans(company)); //Clear out orphans
				companies.changeShares(company, 10 - companies.getShares(company)); // Put all shares regular
				cout << "++" << companies.name[company] << " now has $" << companies.getCash(company) << " and " << companies.getShares(company) << " regular shares and " << companies.getOrphans(company) << " orphaned shares \n";
			}
		}
	}
	else if ((netProfitChange > 0) && (companyDividenPlan == 0))
	{
		companies.increaseValuation(company);
		cout << "++" << companies.name[company] << " valuation increases to $" << valuations[companies.getValuation(company)] << " per share. \n";

	}
	else
	{
		cout << "++" << companies.name[company] << " valuation remains the same at $" << valuations[companies.getValuation(company)] << " per share. \n";
	}
	return;
} 
void setCompanyDividens(int company, int option)
{
	if (option == 0)
	{
		int cDividend = (companies.getShares(company) * companies.getNetprofit(company)) / 10;
		companies.changeCash(company, cDividend);
		cout << "++" << companies.name[company] << " recieved $" << cDividend << " from " << companies.getShares(company) << " shares. \n";

		for (int i = 0; i < numberPlayers; i++)
		{
			if (players.getShares(i, company) != 0)
			{
				int pDividend = players.getShares(i, company)*companies.getNetprofit(company) / 10;
				players.changeCash(i, pDividend);
				cout << "++" << players.name[i] << " recieved $" << pDividend << " from " << players.getShares(i, company) << " shares of " << companies.name[company] << " and now has $" << players.getCash(i) << endl;
			}
		}
	}
	else
	{
		companies.changeCash(company, companies.getNetprofit(company));
		cout << "++" << companies.name[company] << " recieved net profit of  $" << companies.getNetprofit(company) << ". \n";

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
	game.setRound(atoi(testArray));
	findEOL(inDataStream);
	getChar = readValue(inDataStream, testArray);
	getChar = readValue(inDataStream, testArray);
	game.setTurn(atoi(testArray));
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
int updateInfo()
{
	wchar_t cashStr[10];

	HWND hwndPC1 = GetDlgItem(infoDlg, IDC_PC1);
	HWND hwndPC2 = GetDlgItem(infoDlg, IDC_PC2);
	HWND hwndPC3 = GetDlgItem(infoDlg, IDC_PC3);
	HWND hwndPC4 = GetDlgItem(infoDlg, IDC_PC4);
	HWND hwndPC5 = GetDlgItem(infoDlg, IDC_PC5);
	HWND hwndPC6 = GetDlgItem(infoDlg, IDC_PC6);

	HWND hwndPW1 = GetDlgItem(infoDlg, IDC_PW1);
	HWND hwndPW2 = GetDlgItem(infoDlg, IDC_PW2);
	HWND hwndPW3 = GetDlgItem(infoDlg, IDC_PW3);
	HWND hwndPW4 = GetDlgItem(infoDlg, IDC_PW4);
	HWND hwndPW5 = GetDlgItem(infoDlg, IDC_PW5);
	HWND hwndPW6 = GetDlgItem(infoDlg, IDC_PW6);

	HWND hwndCV1 = GetDlgItem(infoDlg, IDC_CV1);
	HWND hwndCV2 = GetDlgItem(infoDlg, IDC_CV2);
	HWND hwndCV3 = GetDlgItem(infoDlg, IDC_CV3);
	HWND hwndCV4 = GetDlgItem(infoDlg, IDC_CV4);
	HWND hwndCV5 = GetDlgItem(infoDlg, IDC_CV5);
	HWND hwndCV6 = GetDlgItem(infoDlg, IDC_CV6);
	HWND hwndCV7 = GetDlgItem(infoDlg, IDC_CV7);
	HWND hwndCV8 = GetDlgItem(infoDlg, IDC_CV8);
	HWND hwndCV9 = GetDlgItem(infoDlg, IDC_CV9);
	HWND hwndCV0 = GetDlgItem(infoDlg, IDC_CV0);


		_itow_s(players.getCash(0), cashStr, 10);
		SetWindowText(hwndPC1, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(0), cashStr, 10);
		SetWindowText(hwndPW1, (LPCWSTR)cashStr);

		_itow_s(players.getCash(1), cashStr, 10);
		SetWindowText(hwndPC2, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(1), cashStr, 10);
		SetWindowText(hwndPW2, (LPCWSTR)cashStr);

		_itow_s(players.getCash(2), cashStr, 10);
		SetWindowText(hwndPC3, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(2), cashStr, 10);
		SetWindowText(hwndPW3, (LPCWSTR)cashStr);

		_itow_s(players.getCash(3), cashStr, 10);
		SetWindowText(hwndPC4, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(3), cashStr, 10);
		SetWindowText(hwndPW4, (LPCWSTR)cashStr);

		_itow_s(players.getCash(4), cashStr, 10);
		SetWindowText(hwndPC5, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(4), cashStr, 10);
		SetWindowText(hwndPW5, (LPCWSTR)cashStr);

		_itow_s(players.getCash(5), cashStr, 10);
		SetWindowText(hwndPC6, (LPCWSTR)cashStr);
		_itow_s(players.getNetworth(5), cashStr, 10);
		SetWindowText(hwndPW6, (LPCWSTR)cashStr);

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



	return(0);
}
int updateBuild(int company)
{
	HWND hwndCC = GetDlgItem(buildDlg, IDC_CURRENT_COMPANY);
	HWND hwndList1 = GetDlgItem(buildDlg, IDC_SPIN_TRACK);
	HWND hwndList2 = GetDlgItem(buildDlg, IDC_SPIN_PROFIT);
	wchar_t intStr[20];
	wchar_t ncStr[20];
	SetWindowText(hwndCC, (LPCWSTR)companies.name[company]);
	netCash = companies.getCash(company);
	_itow_s(netCash, intStr, 10);
	wcscpy_s(ncStr, intStr);
	SetWindowText(GetDlgItem(buildDlg, IDC_NETCASH), ncStr);

	//Clean up train boxes
	for (int j = 0; j < 5; j++)
	{
		SendMessage(GetDlgItem(buildDlg, IDC_TS0 + j), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		SendMessage(GetDlgItem(buildDlg, IDC_TB0 + j), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);
		ShowWindow(GetDlgItem(buildDlg, IDC_TS0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TB0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TSN0 + j), SW_HIDE);
		ShowWindow(GetDlgItem(buildDlg, IDC_TBN0 + j), SW_HIDE);
	}
	// List trains to scrap
	for (int j = 0; j < companies.getTrainCount(company); j++)
	{
		ShowWindow(GetDlgItem(buildDlg, IDC_TS0 + j), SW_SHOW);
	}
	// List next train for sale
	int train = companies.getTrainForSale();
	ShowWindow(GetDlgItem(buildDlg, IDC_TB0), SW_SHOW);
	// Intitialize Spin Boxes
	SendMessage(hwndList1, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(20, 0));
	SendMessage(hwndList1, UDM_SETPOS, (WPARAM)0, (LPARAM)0);
	SendMessage(hwndList2, UDM_SETRANGE, (WPARAM)0, MAKELPARAM(60, 0));
	SendMessage(hwndList2, UDM_SETPOS, (WPARAM)0, (LPARAM)companies.getNetprofit(company) / 10);
	UINT val1 = LOWORD(SendMessage(hwndList1, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
	Value = SetDlgItemInt(buildDlg, IDC_TRACK_TO_BUY, val1 * 10, true);
	UINT val2 = LOWORD(SendMessage(hwndList2, UDM_GETPOS, (WPARAM)0, (LPARAM)0));
	Value = SetDlgItemInt(buildDlg, IDC_PROFIT, val2 * 10, true);
	SendMessage(GetDlgItem(buildDlg, IDC_DIST), BM_SETCHECK, (WPARAM)BST_CHECKED, (LPARAM)0);
	SendMessage(GetDlgItem(buildDlg, IDC_WITH), BM_SETCHECK, (WPARAM)BST_UNCHECKED, (LPARAM)0);




	SetFocus(hwndList1);
	return(0);
}
int incrementTurn(bool success)
{
	int round = game.getRound();
	int turn = game.getTurn();
	switch (round)
	{
	case 1:
	{
		if (!success)
		{
			numberPasses++;
		}
		if (numberPasses != numberPlayers)
		{
			if (turn == numberPlayers-1)
			{
				numberPasses = 0;
				turn = 0;
			}
			else
			{
				turn++;
			}
//			SetWindowText(GetDlgItem(marketDlg, IDC_CURRENT_PLAYER), (LPCWSTR)players.name[game.getTurn()]);
			game.setTurn(turn);
			game.setRound(round);
			return(0);
		}
		else
		{
			numberPasses = 0;
			turn = 0;
			round++;
			companies.getTurnorders(companyTurnorder);
			// Clear sold array
			for (int i = 0; i < numberPlayers; i++)
			{
				for (int j = 0; j < maxCompanies; j++)
				{
					players.clearSold(i, j);
				}
			}
			reduceForOrphans();
			game.setTurn(turn);
			game.setRound(round);
			return(1);
		}
	}
	case 2:
	{
		if (companies.getStarted(companyTurnorder[turn+1]))
		{
			turn++;
			game.setTurn(turn);
			game.setRound(round);
			return(0);
		}
		else
		{
			turn = 0;
			round++;
			companies.getTurnorders(companyTurnorder);
			game.setTurn(turn);
			game.setRound(round);
			return(1);
		}
	}
	case 3:
	{
		if (companies.getStarted(companyTurnorder[turn+1]))
		{
			turn++;
			game.setTurn(turn);
			game.setRound(round);
			return(0);
		}
		else
		{
			turn = 0;
			round = 1;
			players.getTurnorders(playerTurnorder, numberPlayers);
			game.setTurn(turn);
			game.setRound(round);
			return(1);
		}
	}
	}
	return(1);
}
int endGame()
{
	cout << "End of the game.";
	int winner = 0;
	for (int i = 0; i < numberPlayers; i++)
	{
		if (players.getNetworth(i) > players.getNetworth(winner))
		{
			winner = i;
		}
	}
	cout << " The winner is " << players.name[winner] << "! \n";
	return(0);
}