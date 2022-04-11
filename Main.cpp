#include "pch.h"
#include "App.h"

int CALLBACK WinMain(
	_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPSTR     lpCmdLine,
	_In_ int       nCmdShow
)
{
	MSG msg;
	if (CApp::GetInst()->Initialize(hInstance, hPrevInstance, lpCmdLine, nCmdShow))
	{
		msg = CApp::GetInst()->RunLoop();
	}
	CApp::Deleteinst();
	
	return (int)msg.wParam;;
}