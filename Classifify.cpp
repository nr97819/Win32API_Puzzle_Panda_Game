
CApp::CGame m_game;

void m_game->InitializePuzzleData()
{
	swprintf_s(clearMsg, L"Clear! 축하합니다!");
	swprintf_s(posMsg, L"( %d, %d )", pX, pY);
	swprintf_s(btnMsg, L"게임시작!", RAND_MIX_VALUE);
		
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
}

void m_app->DrawPieces()
{
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
}

void m_app->DrawAxisLines()
{
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
}

// render 함수로 따로 빼기
void m_app->RenderPuzzles()
{
	// 조각 그리기
	m_app->DrawPieces();
	
	// 격자 그리기
	m_app->DrawAxisLines();
}

void m_app->DrawStartButton()
{
	hOldPen = (HPEN)SelectObject(hdc, hGrayBrush);
	Rectangle(hdc, rBtnPos.left, rBtnPos.top, rBtnPos.right, rBtnPos.bottom);
	SelectObject(hdc, hOldPen);
	TextOut(hdc, rBtnPos.left + 5, rBtnPos.top + 5, btnMsg, wcslen(btnMsg));
}

void m_app->PrintClearMessage()
{
	MessageBox(hWnd, clearMsg, _T("Clear"), MB_OK);
	//TextOut(hdc, 250, 50, clearMsg, wcslen(clearMsg));
	TextOut(hdc, 250, 50, clearMsg, wcslen(clearMsg));
}

void m_game->ShufflePuzzleRandomly()
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

//==================================================================

int m_game->IsAllPiecesCorrect()
{
	idCnt = 0;
	for (int checkY = 0; checkY < 4; checkY++)
	{
		for (int checkX = 0; checkX < 4; checkX++)
		{
			// 나중에 함수로 빼냈을 때, 활성화 할 내용
			/*if (pieces[checkY][checkX].GetId() < 0)
				return bClearCheck;*/

			if (idCnt == 15)
				return TRUE;

			if (pieces[checkY][checkX].GetId() != idCnt)
			{
				return FALSE;
			}

			idCnt++;
		}
	}
	return TRUE;
}

void m_game->MovePiece()
{
	CPiece temp = pieces[pY][pX];
	pieces[pY][pX] = pieces[voidPos.y][voidPos.x];
	pieces[voidPos.y][voidPos.x] = temp;

	voidPos.x = pX;
	voidPos.y = pY;	
}
			
LRESULT CALLBACK CApp::MyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		hBitmap = (HBITMAP)LoadBitmap(((LPCREATESTRUCT)lParam)->hInstance, MAKEINTRESOURCE(IDB_BITMAP1));

		m_game->InitializePuzzleData();
		
		break;

	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		GetClientRect(hWnd, &rectView);
		memdc = CreateCompatibleDC(hdc);
		SelectObject(memdc, hBitmap);
		
		// 매순간 퍼즐 render하는 함수
		m_app->RenderPuzzles();

		/*if (bClearCheck && bGameStarted)*/
		if (bClearCheck)
		{
			m_app->PrintClearMessage();
		}
		//TextOut(hdc, 0, 0, posMsg, wcslen(posMsg));
		DeleteDC(memdc);

		// 버튼 UI 그리기
		if (!bGameStarted)
		{
			m_app->DrawStartButton();
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
			m_game->ShufflePuzzleRandomly();
		}

		// 인접한 위치로만, 퍼즐 이동 가능 !
		pX /= 150;
		pY /= 150;
		if ((abs(pX - voidPos.x) + abs(pY - voidPos.y)) == 1)
		{
			m_game->MovePiece();
		}

		// 게임 Clear (index : 0~15 )
		m_game->IsAllPiecesCorrect();
		
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
