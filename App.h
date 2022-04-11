#pragma once

#include "pch.h"
#include "Game.h"

class CApp
{
private:
	// =============================================
	static CApp* m_app;
	CGame* m_game;
	// =============================================

	TCHAR m_szWindowClass[50] = _T("판다 퍼즐 게임");
	TCHAR m_szTitle[50] = _T("즐거운 판다 퍼즐 게임");

	// 회색 배경 그리기
	HBRUSH m_hBlueBrush = CreateSolidBrush(RGB(30, 30, 150));

	HINSTANCE hInst;

	HDC memdc;
	RECT rectView;
	HBITMAP hBitmap;

	int x = 0, y = 0;
	int delta = 150;
	//int delta2 = 150 / (1.5);

	WCHAR posMsg[30] = {};
	int pX = 0, pY = 0;
	// 조각을 넣는 부분
	int index = 0;
	CPiece pieces[4][4] = {};

	// 격자 두께 설정
	HPEN hPen = CreatePen(PS_DOT, 6, RGB(30, 30, 150));
	HPEN hOldPen;

	// 게임 퍼즐 옮기기 로직
	POINT voidPos{ 3, 3 };
	// static int dirArr[4] = { -4, +4, +1, -1 };

	// 버튼 UI 좌표 지정
	HBRUSH hGrayBrush = (HBRUSH)CreateSolidBrush(RGB(100, 100, 200));
	RECT rBtnPos = { 500, 20, 580, 50 };
	int RAND_MIX_VALUE = 1000;
	WCHAR btnMsg[30] = {};

	// 게임 Clear 처리
	bool bClearCheck = true;
	bool bGameStarted = false;
	int idCnt = 0;
	WCHAR clearMsg[50] = {};

private:
	CApp();
	~CApp();

public:
	static CApp* GetInst()
	{
		if (nullptr == m_app)
			m_app = new CApp();

		return m_app;
	}

	static void Deleteinst()
	{
		if (nullptr != m_app)
		{
			delete m_app;
			m_app = nullptr;
		}
	}

public:
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK MyProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	int Initialize(
		_In_ HINSTANCE hInstance,
		_In_opt_ HINSTANCE hPrevInstance,
		_In_ LPSTR     lpCmdLine,
		_In_ int       nCmdShow);

	MSG RunLoop();
};

