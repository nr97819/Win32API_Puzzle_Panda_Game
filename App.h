#pragma once

#include "Game.h"
#include <time.h>
#include "resource.h"
#include "Piece.h"

class CApp
{
private:
	static CApp*	m_app;
	CGame*			m_game;

private:
	HDC				m_hdc;
	PAINTSTRUCT		m_ps;
	HWND			m_hWnd;

	HINSTANCE		hInst;
	HDC				memdc;
	RECT			rectView;
	HBITMAP			hBitmap;

	TCHAR			m_szWindowClass[50] = {};
	TCHAR			m_szTitle[50];

	HPEN			m_blackSlimPen; // ���� �β� ������ ��
	HBRUSH			m_blueBrush; // �Ķ�
	HBRUSH			m_grayBrush; // ȸ��

	WCHAR			m_clearMsg[50];
	WCHAR			m_btnMsg[30];
	RECT			m_rBtnPos; // ��ư UI ��ǥ ����

	int				m_delta;
	int				m_pX,	m_pY; // POINT(x, y) �ӽ� �����
	int				m_dotX,	m_dotY;
	bool			m_bGameStarted;

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

	int Initialize(	_In_ HINSTANCE hInstance,
					_In_opt_ HINSTANCE hPrevInstance,
					_In_ LPSTR     lpCmdLine,
					_In_ int       nCmdShow);
	MSG RunLoop();

public:
	bool InitializePuzzle(LPARAM lParam);

	void DrawAxisLines();
	void DrawPieces();

	void DrawStartButton();
	void PrintClearMessage();

	bool IsAllPiecesCorrect();
	bool IsGameStarted() { return this->m_bGameStarted; }
	bool IsButtonClicked()
	{
		if (this->m_pX >= this->m_rBtnPos.left &&
			this->m_pX <= this->m_rBtnPos.right &&
			this->m_pY >= this->m_rBtnPos.top &&
			this->m_pY <= this->m_rBtnPos.bottom)
		{
			m_bGameStarted = true;
			return true;
		}
		else
			return false;
	}
};

