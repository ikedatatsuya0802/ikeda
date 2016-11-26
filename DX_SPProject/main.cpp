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
		0, "NullWindow", "Everlasting Rail", WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, (rect.right - rect.left + GetSystemMetrics(SM_CXFIXEDFRAME) * 2),
		(rect.bottom - rect.top + GetSystemMetrics(SM_CYFIXEDFRAME) * 2 + GetSystemMetrics(SM_CYCAPTION)),
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
			if((dwCurrentTime - dwExecLastTime) >= (1000 / 60))
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
	int	nID;

	const RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };// ��ʕ������p

	switch(uMsg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "�E�B���h�E����܂����H", "�E�B���h�E�̏I��", MB_YESNO);
			if(nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;
	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;
	default:
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