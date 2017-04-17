//=============================================================================
//
//	�^�C�g��	���C���t�@�C��
//	�t�@�C����	main.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/13
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "main.h"
#include "manager.h"

//=============================================================================
//	�v���g�^�C�v
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//=============================================================================
//	�O���[�o���ϐ�
//=============================================================================
static HWND		g_hWnd;						// �E�B���h�E�n���h��
static int		g_nCountFPS;				// FPS�J�E���^

//=============================================================================
//	�֐���	:WinMain
//	����	:HINSTANCE hInstance
//			:HINSTANCE hPrevInstance
//			:LPSTR lpCmdLine
//			:int nCmdLine
//	�߂�l	:int
//	����	:���C���֐�
//=============================================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex = {
		sizeof(WNDCLASSEX), CS_CLASSDC, WndProc, 0, 0, hInstance, NULL,
		LoadCursor(NULL, IDC_ARROW), (HBRUSH)(COLOR_WINDOW + 1), NULL, "NullWindow", NULL
	};
	MSG		msg;
	RECT	rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	DWORD	dwFrameCount;
	DWORD	dwCurrentTime;
	DWORD	dwExecLastTime;
	DWORD	dwFPSLastTime;
	int		frx, fry, cpy;


	timeBeginPeriod(1);
	dwFrameCount =
		dwCurrentTime = 0;
	dwExecLastTime =
		dwFPSLastTime = timeGetTime();

	// �E�B���h�E���쐬
	frx = GetSystemMetrics(SM_CXFIXEDFRAME);
	fry = GetSystemMetrics(SM_CYFIXEDFRAME);
	cpy = GetSystemMetrics(SM_CYCAPTION);
	RegisterClassEx(&wcex);
	g_hWnd = CreateWindowEx(
		0, "NullWindow", "�w�l�G�x TH�@AT13A284�@07 �@�r�c �B��", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left + frx * 2),
		(rect.bottom - rect.top + fry * 2 + cpy),
		NULL, NULL, hInstance, NULL);

	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW, false);
	rect.right -= rect.left;
	rect.bottom -= rect.top;

	// ����������
	CManager::Init(hInstance, g_hWnd, TRUE);

	ShowWindow(g_hWnd, nCmdShow);
	UpdateWindow(g_hWnd);

	// �������[�v
	while(1)
	{
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)
		{
			if(msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
		}
		else
		{
			dwCurrentTime = timeGetTime();
			if((dwCurrentTime - dwFPSLastTime) >= 500)
			{
				g_nCountFPS = (dwFrameCount * 1000) / (dwCurrentTime - dwFPSLastTime);
				dwFPSLastTime = dwCurrentTime;
				dwFrameCount = 0;
			}
			if((dwCurrentTime - dwExecLastTime) >= (1000 / GAME_FPS))
			{
				dwExecLastTime = dwCurrentTime;

				// �e��X�V����
				CManager::Update();

				// �e��`�揈��
				CManager::Draw();

				dwFrameCount++;
			}
		}
	}
	UnregisterClass("NullWindow", wcex.hInstance);

	// �e��I������
	CManager::Uninit();

	timeEndPeriod(1);

	return (int)msg.wParam;
}

//=============================================================================
//	�֐���	:WndProc
//	����	:HINSTANCE hWnd(�E�B���h�E�n���h��)
//			:UINT uMsg
//			:WPARAM wParam
//			:LPARAM lParam
//	�߂�l	:LRESULT
//	����	:�E�B���h�E�v���V�[�W��
//=============================================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			DestroyWindow(hWnd);
			break;
		}
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

//=============================================================================
//	�֐���	:GetFPS
//	����	:����
//	�߂�l	:int
//	����	:FPS�l�����^�[������B
//=============================================================================
int GetFPS(void)
{
	return g_nCountFPS;
}

//=============================================================================
//	�֐���	:GethWnd
//	����	:����
//	�߂�l	:HWND
//	����	:�E�B���h�E�n���h�������^�[������B
//=============================================================================
HWND GethWnd(void)
{
	return g_hWnd;
}

//=============================================================================
//	�֐���	:StringToCharP
//	����	:����
//	�߂�l	:����
//	����	:string�^��char*�^�ɕϊ�����B
//=============================================================================
char* StringToCharP(const string str)
{
	char* name = new char[str.length() + 1];
	memcpy(name, str.c_str(), (str.length() + 1));

	return name;
}

//=============================================================================
//	�֐���	:CharPToString
//	����	:����
//	�߂�l	:����
//	����	:char*�^��string�^�ɕϊ�����B
//=============================================================================
string CharPToString(const char* str)
{
	string name(str);

	return name;
}

// �����񕪊�
vector<std::string> split(const string &str, char sep)
{
	vector<string> v;
	stringstream ss(str);
	string buffer;
	while(getline(ss, buffer, sep)) {
		v.push_back(buffer);
	}
	return v;
}