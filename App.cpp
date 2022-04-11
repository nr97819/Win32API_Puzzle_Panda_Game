#include "pch.h"
#include "App.h"

CApp* CApp::m_app = nullptr;

CApp::CApp()
{
	srand(static_cast<unsigned int>(time(0)));

	m_game = new CGame();
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
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		for (y = 0; y < 4; ++y)
		{
			for (x = 0; x < 4; ++x)
			{
				if (index >= 15) // 마지막 칸은 비워두기 (게임용)
				{
					pieces[y][x] = CPiece(-1, POINT{ x * delta, y * delta });
					break;
				}

				pieces[y][x] = CPiece(index, POINT{ x * delta, y * delta });
				index++;
			}
		}
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rectView);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		for (y = 0; y < 4; ++y) // max : 600
		{
			for (x = 0; x < 4; ++x) // 빈 자리의 id=-1 이므로, 출력하지 않기
			{
				if (pieces[y][x].GetId() < 0)
					continue;

				BitBlt(hdc, x * delta,
					y * delta,
					delta,
					delta, memdc,
					pieces[y][x].GetPos().x,
					pieces[y][x].GetPos().y, SRCCOPY);
			}
		}

		// <- render 함수로 따로 빼기 @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
		hOldPen = (HPEN)SelectObject(hdc, hPen);
		for (y = 0; y < 5; ++y)
		{
			for (x = 0; x < 5; ++x)
			{
				MoveToEx(hdc, x * delta, 0 * delta, NULL);
				LineTo(hdc, x * delta, 4 * delta);
			}
			MoveToEx(hdc, 0 * delta, y * delta, NULL);
			LineTo(hdc, 4 * delta, y * delta);
		}
		SelectObject(hdc, hOldPen);

		swprintf_s(clearMsg, L"Clear! 축하합니다!");
		/*if (bClearCheck && bGameStarted)*/
		if (bClearCheck)
		{
			MessageBox(hWnd, clearMsg, _T("Clear"), MB_OK);
			//TextOut(hdc, 250, 50, clearMsg, wcslen(clearMsg));
			TextOut(hdc, 250, 50, clearMsg, wcslen(clearMsg));
		}
		DeleteDC(memdc);

		swprintf_s(posMsg, L"( %d, %d )", pX, pY);
		TextOut(hdc, 0, 0, posMsg, wcslen(posMsg));

		// 버튼 UI 그리기
		swprintf_s(btnMsg, L"게임시작!", RAND_MIX_VALUE);
		if (!bGameStarted)
		{
			hOldPen = (HPEN)SelectObject(hdc, hGrayBrush);
			Rectangle(hdc, rBtnPos.left, rBtnPos.top, rBtnPos.right, rBtnPos.bottom);
			SelectObject(hdc, hOldPen);
			TextOut(hdc, rBtnPos.left + 5, rBtnPos.top + 5, btnMsg, wcslen(btnMsg));
		}

		EndPaint(hWnd, &ps);
		break;

	case WM_LBUTTONDOWN:
		pX = LOWORD(lParam);
		pY = HIWORD(lParam);

		// 버튼 생성 (무작위 섞기)
		if (pX >= rBtnPos.left && pX <= rBtnPos.right &&
			pY >= rBtnPos.top && pY <= rBtnPos.bottom) // 버튼 좌표 지정
		{
			for (int k = 0; k < RAND_MIX_VALUE; k++)
			{
				pX = static_cast<int>(rand() % 4);
				pY = static_cast<int>(rand() % 4);

				if ((abs(pX - voidPos.x) + abs(pY - voidPos.y)) == 1)
				{
					CPiece temp = pieces[pY][pX];
					pieces[pY][pX] = pieces[voidPos.y][voidPos.x];
					pieces[voidPos.y][voidPos.x] = temp;

					voidPos.x = pX;
					voidPos.y = pY;
				}
			}

			bGameStarted = true;
		}

		pX /= 150;
		pY /= 150;
		// 인접한 위치로만, 퍼즐 이동 가능 !
		if ((abs(pX - voidPos.x) + abs(pY - voidPos.y)) == 1)
		{
			CPiece temp = pieces[pY][pX];
			pieces[pY][pX] = pieces[voidPos.y][voidPos.x];
			pieces[voidPos.y][voidPos.x] = temp;

			voidPos.x = pX;
			voidPos.y = pY;
		}

		// 게임 Clear (index : 0~15 )
		bClearCheck = true;
		idCnt = 0;
		for (int checkY = 0; checkY < 4; checkY++)
		{
			for (int checkX = 0; checkX < 4; checkX++)
			{
				// 나중에 함수로 빼냈을 때, 활성화 할 내용
				/*if (pieces[checkY][checkX].GetId() < 0)
					return bClearCheck;*/

				if (idCnt == 15)
					break;

				if (pieces[checkY][checkX].GetId() != idCnt)
				{
					bClearCheck = false;
					break;
				}


				idCnt++;
			}

			if (!bClearCheck)
				break;
		}

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
	wcex.hbrBackground = m_hBlueBrush;
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

	HWND hWnd = CreateWindow(
		m_szWindowClass,
		m_szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		630, 650,
		NULL,
		NULL,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL,
			_T("Call to CreateWindow failed!"),
			_T("Windows Desktop Guided Tour"),
			NULL);

		return 1;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
}

MSG CApp::RunLoop()
{
	// Main message loop:
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg;
}