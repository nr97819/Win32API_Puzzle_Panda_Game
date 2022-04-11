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

	TCHAR m_szWindowClass[50] = _T("�Ǵ� ���� ����");
	TCHAR m_szTitle[50] = _T("��ſ� �Ǵ� ���� ����");

	// ȸ�� ��� �׸���
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
	// ������ �ִ� �κ�
	int index = 0;
	CPiece pieces[4][4] = {};

	// ���� �β� ����
	HPEN hPen = CreatePen(PS_DOT, 6, RGB(30, 30, 150));
	HPEN hOldPen;

	// ���� ���� �ű�� ����
	POINT voidPos{ 3, 3 };
	// static int dirArr[4] = { -4, +4, +1, -1 };

	// ��ư UI ��ǥ ����
	HBRUSH hGrayBrush = (HBRUSH)CreateSolidBrush(RGB(100, 100, 200));
	RECT rBtnPos = { 500, 20, 580, 50 };
	int RAND_MIX_VALUE = 1000;
	WCHAR btnMsg[30] = {};

	// ���� Clear ó��
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

