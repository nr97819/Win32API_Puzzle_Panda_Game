#include "pch.h"

#include "App.h"

CApp* CApp::m_app = nullptr;

CApp::CApp() 
	: m_szWindowClass(_T("판다 퍼즐 게임"))
	, m_szTitle(_T("즐거운 판다 퍼즐 게임"))
	, m_pX(0), m_pY(0)
	, m_delta(150)
	, m_bGameStarted(false)
	, m_clearMsg{}
	, m_btnMsg{}
{
	srand(static_cast<unsigned int>(time(0)));

	m_game = new CGame();

	m_blueBrush = CreateSolidBrush(RGB(30, 30, 150));
	m_grayBrush = (HBRUSH)CreateSolidBrush(RGB(100, 100, 200));
	m_blackSlimPen = CreatePen(PS_DOT, 6, RGB(30, 30, 150));
	m_rBtnPos = { 500, 20, 580, 50 };
}

CApp::~CApp()
{
	if (nullptr != m_game) 
	{
		delete m_game;
		m_game = nullptr;
	}
}

LRESULT CALLBACK CApp::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_app->MyProc(hWnd, message, wParam, lParam);
}

LRESULT CALLBACK CApp::MyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE:
		InitializePuzzle(lParam);
		break;

	case WM_PAINT:
		m_hdc = BeginPaint(hWnd, &m_ps);
		GetClientRect(hWnd, &rectView);
		memdc = CreateCompatibleDC(m_hdc);
		SelectObject(memdc, hBitmap);

		DrawPieces(); // 조각 그리기
		DrawAxisLines(); // 격자 그리기
		if (IsGameStarted())
		{
			if (IsAllPiecesCorrect())
				PrintClearMessage();
		}
		else
		{
			DrawStartButton();
		}

		DeleteDC(memdc);
		EndPaint(hWnd, &m_ps);
		break;

	case WM_LBUTTONDOWN:
		m_pX = LOWORD(lParam);
		m_pY = HIWORD(lParam);
		m_dotX = m_pX / 150;
		m_dotY = m_pY / 150;

		// 버튼 생성 (무작위 섞기)
		if (IsButtonClicked())
		{
			m_game->ShufflePuzzleRandomly();
		}
		if (m_pX >= m_rBtnPos.left && m_pX <= m_rBtnPos.right &&
			m_pY >= m_rBtnPos.top && m_pY <= m_rBtnPos.bottom) // 버튼 좌표 지정
		{
			m_game->ShufflePuzzleRandomly();
		}
		m_game->MovePiece(m_dotX, m_dotY);

		// 게임 클리어 (index : 0~15 )
		IsAllPiecesCorrect();
		InvalidateRgn(hWnd, NULL, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_TIMER:
		break;

	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}

int CApp::Initialize(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, IDI_APPLICATION);
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = m_blueBrush;
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, IDI_APPLICATION);

	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL,
			_T("Call to RegisterClassEx failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	hInst = hInstance;

	m_hWnd = CreateWindow(
		m_szWindowClass,
		m_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		630, 650,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!m_hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(m_hWnd, nCmdShow);
	UpdateWindow(m_hWnd);
}

MSG CApp::RunLoop()
{
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg;
}

bool CApp::InitializePuzzle(LPARAM lParam)
{
	hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));
	
	swprintf_s(m_clearMsg, L"클리어하셨습니다! 축하합니다!");
	swprintf_s(m_btnMsg, L"게임시작");

	if (m_game->InitializeGame(m_delta))
		return false;
}

void CApp::DrawPieces()
{
	int x, y;
	for (y = 0; y < 4; ++y) // max : 600
	{
		for (x = 0; x < 4; ++x) // 빈 자리의 id=-1 이므로, 출력하지 않기
		{
			CPiece(*pPieces)[4] = m_game->GetPieces();
			if (pPieces[y][x].GetId() < 0)
				continue;

			BitBlt(m_hdc, x * m_delta,
				y * m_delta,
				m_delta,
				m_delta, memdc,
				pPieces[y][x].GetPos().x,
				pPieces[y][x].GetPos().y, SRCCOPY);
		}
	}
}

void CApp::DrawAxisLines()
{
	HPEN m_oldPen = (HPEN)SelectObject(m_hdc, m_blackSlimPen);

	int x, y;
	for (y = 0; y < 5; ++y)
	{
		for (x = 0; x < 5; ++x)
		{
			MoveToEx(m_hdc, x * m_delta, 0 * m_delta, NULL);
			LineTo(m_hdc, x * m_delta, 4 * m_delta);
		}
		MoveToEx(m_hdc, 0 * m_delta, y * m_delta, NULL);
		LineTo(m_hdc, 4 * m_delta, y * m_delta);
	}
	SelectObject(m_hdc, m_oldPen);
}

void CApp::DrawStartButton() // 버튼 그리기
{
	HPEN hOldPen = (HPEN)SelectObject(m_hdc, m_grayBrush);
	Rectangle(m_hdc, m_rBtnPos.left, m_rBtnPos.top, m_rBtnPos.right, m_rBtnPos.bottom);
	SelectObject(m_hdc, hOldPen);
	TextOut(m_hdc, m_rBtnPos.left + 5, m_rBtnPos.top + 5, m_btnMsg, wcslen(m_btnMsg));
}

void CApp::PrintClearMessage()
{
	MessageBox(m_hWnd, m_clearMsg, _T("Clear"), MB_OK);
	//TextOut(hdc, 250, 50, clearMsg, wcslen(clearMsg));
	TextOut(m_hdc, 250, 50, m_clearMsg, wcslen(m_clearMsg));
}

bool CApp::IsAllPiecesCorrect()
{
	int idCnt = 0;
	CPiece(*pPieces)[4] = m_game->GetPieces();

	for (int checkY = 0; checkY < 4; checkY++)
	{
		for (int checkX = 0; checkX < 4; checkX++)
		{
			// 나중에 함수로 빼냈을 때, 활성화 할 내용
			/*if (pieces[checkY][checkX].GetId() < 0)
				return bClearCheck;*/

			if (idCnt == 15)
				return TRUE;

			if (pPieces[checkY][checkX].GetId() != idCnt)
			{
				return FALSE;
			}

			idCnt++;
		}
	}
	return TRUE;
}