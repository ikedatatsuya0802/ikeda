#ifndef _INPUT_H_
#define _INPUT_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	input.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
#include <d3dx9.h>
#define DIRECTINPUT_VERSION	(0x0800)
#include <dinput.h>

//=============================================================================
//	�����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")

//=============================================================================
//	�}�N����`
//=============================================================================
#define KEYSTATE_NUM			(256)	// �L�[���̌�
#define MOUSESTATE_NUM			(3)	// �L�[���̌�

#define REPEAT_TIME				(20)	// ���s�[�g�܂ł̎���(�~���b)

#define REPEAT_INTERVAL_NORMAL	(1)	// �ʏ�̃��s�[�g���x
#define REPEAT_INTERVAL_PAUSE	(10)	// �|�[�Y���̃��s�[�g���x

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	RECT	moveRect;		// ��ʏ�œ�����͈�
	POINT	sPos;			// �X�N���[�����W
	D3DXVECTOR3	wPos;		// ���[���h���W
	bool	lButton;		// ���{�^��
	bool	rButton;		// �E�{�^��
	bool	cButton;		// �^�񒆃{�^��
	int		moveAdd;		// �ړ���
	RECT	imgRect;		// �}�E�X�p�摜��`
	int		imgWidth;		// �}�E�X�摜��
	int		imgHeight;		// �}�E�X�摜����
	int		Notch;
	int		WheelFraction;
} MOUSE_STATUS;	// �}�E�X���

typedef struct {
	BYTE	Press[KEYSTATE_NUM];		// �v���X���
	BYTE	Trigger[KEYSTATE_NUM];		// �g���K���
	BYTE	Release[KEYSTATE_NUM];		// �����[�X���
	BYTE	Repeat[KEYSTATE_NUM];		// ���s�[�g���

	int		RepeatCount[KEYSTATE_NUM];	// ���s�[�g�J�E���g
	int		RepeatInterval;				// ���s�[�g�Ԋu
} KEYSTATE;	// �L�[���

typedef struct {
	bool	Press[MOUSESTATE_NUM];			// �v���X���
	bool	Trigger[MOUSESTATE_NUM];		// �g���K���
	bool	Release[MOUSESTATE_NUM];		// �����[�X���
	bool	Repeat[MOUSESTATE_NUM];			// ���s�[�g���

	int		RepeatCount[MOUSESTATE_NUM];	// ���s�[�g�J�E���g
	int		RepeatInterval;					// ���s�[�g�Ԋu
} MOUSESTATE;	// �}�E�X���

typedef enum {
	MBTN_LEFT = 0,	// ���{�^��
	MBTN_CENTER,	// �����{�^��
	MBTN_RIGHT,		// �E�{�^��
	MBTN_MAX		// 
} MB;	// �}�E�X�{�^���R�[�h

//=============================================================================
//	�N���X��`
//=============================================================================
class CInput
{
public:
	static HRESULT	Init(HINSTANCE hInstance, HWND hWnd);
	static void		Uninit(void);
	static void		Update(void);

	// �L�[�{�[�h����
	static HRESULT	InitKeyboard(HINSTANCE hInstance, HWND hWnd);
	static void		UpdateKeyboard(void);
	static void		UninitKeyboard(void);
	static void		SetRepeatInterval(int nRepeatInterval){ m_KeyState.RepeatInterval = nRepeatInterval; }

	static bool	GetKeyPress(int nKey){ return (m_KeyState.Press[nKey] & 0x80) ? true : false; }
	static bool	GetKeyTrigger(int nKey){ return (m_KeyState.Trigger[nKey] & 0x80) ? true : false; }
	static bool	GetKeyRelease(int nKey){ return (m_KeyState.Release[nKey] & 0x80) ? true : false; }
	static bool	GetKeyRepeat(int nKey){ return (m_KeyState.Repeat[nKey] & 0x80) ? true : false; }
	

	// �}�E�X����
	static HRESULT	InitMouse(HINSTANCE hInstance, HWND hWnd);
	static void		UpdateMouse(void);
	static void		UninitMouse(void);

	static D3DXVECTOR3* MouseScreenToWorld(D3DXVECTOR3* pout, POINT point);
	static D3DXVECTOR3* CalcScreenToWorld(D3DXVECTOR3* pout,
		int Sx, int Sy, float fZ, int Screen_w, int Screen_h,
		D3DXMATRIX* View, D3DXMATRIX* Prj);
	static D3DXVECTOR3* CalcScreenToXZ(D3DXVECTOR3* pout,
		int Sx, int Sy, int Screen_w, int Screen_h,
		D3DXMATRIX* View, D3DXMATRIX* Prj);

	static POINT		GetMouseScreenPos(void) { return m_MState.sPos; }
	static D3DXVECTOR3	GetMouseWorldPos(void) { return m_MState.wPos; }
	static int			GetMouseNotch(void) { return m_MState.Notch; }
	static int			GetMouseFriction(void) { return m_MState.WheelFraction; }
	
	static bool	GetMousePress(int nKey) { return m_MouseState.Press[nKey]; }
	static bool	GetMouseTrigger(int nKey) { return m_MouseState.Trigger[nKey]; }
	static bool	GetMouseRelease(int nKey) { return m_MouseState.Release[nKey]; }
	static bool	GetMouseRepeat(int nKey) { return m_MouseState.Repeat[nKey]; }

	static void	SetMouseNotch(int notch) { m_MState.Notch = notch; }
	static void	SetMouseLButton(bool state) { m_MState.lButton = state; }
	static void	SetMouseCButton(bool state) { m_MState.cButton = state; }
	static void	SetMouseRButton(bool state) { m_MState.rButton = state; }
	static void	SetMouseFraction(int fraction) { m_MState.WheelFraction = fraction; }
	
private:
	static LPDIRECTINPUT8		m_pInput;		// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
	static LPDIRECTINPUTDEVICE8	m_pDevKeyboard;	// ����3D�f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
	static LPDIRECTINPUTDEVICE8 m_pDevMouse;	// �}�E�X�̃f�o�C�X
	
	static KEYSTATE		m_KeyState;				// �L�[�{�[�h�̓��͏�񃏁[�N
	static MOUSESTATE	m_MouseState;			// �L�[�{�[�h�̓��͏�񃏁[�N
	static MOUSE_STATUS	m_MState;				// �}�E�X���
};

//=============================================================================
//	�}�N����`
//=============================================================================
class CInputPress
{
public:
	#define	KH_A		(CInput::GetKeyPress(DIK_A))
	#define	KH_B		(CInput::GetKeyPress(DIK_B))
	#define	KH_C		(CInput::GetKeyPress(DIK_C))
	#define	KH_D		(CInput::GetKeyPress(DIK_D))
	#define	KH_E		(CInput::GetKeyPress(DIK_E))
	#define	KH_F		(CInput::GetKeyPress(DIK_F))
	#define	KH_G		(CInput::GetKeyPress(DIK_G))
	#define	KH_H		(CInput::GetKeyPress(DIK_H))
	#define	KH_I		(CInput::GetKeyPress(DIK_I))
	#define	KH_J		(CInput::GetKeyPress(DIK_J))
	#define	KH_K		(CInput::GetKeyPress(DIK_K))
	#define	KH_L		(CInput::GetKeyPress(DIK_L))
	#define	KH_M		(CInput::GetKeyPress(DIK_M))
	#define	KH_N		(CInput::GetKeyPress(DIK_N))
	#define	KH_O		(CInput::GetKeyPress(DIK_O))
	#define	KH_P		(CInput::GetKeyPress(DIK_P))
	#define	KH_Q		(CInput::GetKeyPress(DIK_Q))
	#define	KH_R		(CInput::GetKeyPress(DIK_R))
	#define	KH_S		(CInput::GetKeyPress(DIK_S))
	#define	KH_T		(CInput::GetKeyPress(DIK_T))
	#define	KH_U		(CInput::GetKeyPress(DIK_U))
	#define	KH_V		(CInput::GetKeyPress(DIK_V))
	#define	KH_W		(CInput::GetKeyPress(DIK_W))
	#define	KH_X		(CInput::GetKeyPress(DIK_X))
	#define	KH_Y		(CInput::GetKeyPress(DIK_Y))
	#define	KH_Z		(CInput::GetKeyPress(DIK_Z))
	#define	KH_1		(CInput::GetKeyPress(DIK_1))
	#define	KH_2		(CInput::GetKeyPress(DIK_2))
	#define	KH_3		(CInput::GetKeyPress(DIK_3))
	#define	KH_4		(CInput::GetKeyPress(DIK_4))
	#define	KH_5		(CInput::GetKeyPress(DIK_5))
	#define	KH_6		(CInput::GetKeyPress(DIK_6))
	#define	KH_7		(CInput::GetKeyPress(DIK_7))
	#define	KH_8		(CInput::GetKeyPress(DIK_8))
	#define	KH_9		(CInput::GetKeyPress(DIK_9))
	#define	KH_0		(CInput::GetKeyPress(DIK_0))
	#define	KH_UP		(CInput::GetKeyPress(DIK_UP))
	#define	KH_DOWN		(CInput::GetKeyPress(DIK_DOWN))
	#define	KH_LEFT		(CInput::GetKeyPress(DIK_LEFT))
	#define	KH_RIGHT	(CInput::GetKeyPress(DIK_RIGHT))
	#define	KH_ENTER	(CInput::GetKeyPress(DIK_RETURN))
	#define	KH_SPACE	(CInput::GetKeyPress(DIK_SPACE))
	#define	KH_LSHIFT	(CInput::GetKeyPress(DIK_LSHIFT))
	#define	KH_RSHIFT	(CInput::GetKeyPress(DIK_RSHIFT))
};

class CInputTrigger
{
public:
	#define	KT_A		(CInput::GetKeyTrigger(DIK_A))
	#define	KT_B		(CInput::GetKeyTrigger(DIK_B))
	#define	KT_C		(CInput::GetKeyTrigger(DIK_C))
	#define	KT_D		(CInput::GetKeyTrigger(DIK_D))
	#define	KT_E		(CInput::GetKeyTrigger(DIK_E))
	#define	KT_F		(CInput::GetKeyTrigger(DIK_F))
	#define	KT_G		(CInput::GetKeyTrigger(DIK_G))
	#define	KT_H		(CInput::GetKeyTrigger(DIK_H))
	#define	KT_I		(CInput::GetKeyTrigger(DIK_I))
	#define	KT_J		(CInput::GetKeyTrigger(DIK_J))
	#define	KT_K		(CInput::GetKeyTrigger(DIK_K))
	#define	KT_L		(CInput::GetKeyTrigger(DIK_L))
	#define	KT_M		(CInput::GetKeyTrigger(DIK_M))
	#define	KT_N		(CInput::GetKeyTrigger(DIK_N))
	#define	KT_O		(CInput::GetKeyTrigger(DIK_O))
	#define	KT_P		(CInput::GetKeyTrigger(DIK_P))
	#define	KT_Q		(CInput::GetKeyTrigger(DIK_Q))
	#define	KT_R		(CInput::GetKeyTrigger(DIK_R))
	#define	KT_S		(CInput::GetKeyTrigger(DIK_S))
	#define	KT_T		(CInput::GetKeyTrigger(DIK_T))
	#define	KT_U		(CInput::GetKeyTrigger(DIK_U))
	#define	KT_V		(CInput::GetKeyTrigger(DIK_V))
	#define	KT_W		(CInput::GetKeyTrigger(DIK_W))
	#define	KT_X		(CInput::GetKeyTrigger(DIK_X))
	#define	KT_Y		(CInput::GetKeyTrigger(DIK_Y))
	#define	KT_Z		(CInput::GetKeyTrigger(DIK_Z))
	#define	KT_1		(CInput::GetKeyTrigger(DIK_1))
	#define	KT_2		(CInput::GetKeyTrigger(DIK_2))
	#define	KT_3		(CInput::GetKeyTrigger(DIK_3))
	#define	KT_4		(CInput::GetKeyTrigger(DIK_4))
	#define	KT_5		(CInput::GetKeyTrigger(DIK_5))
	#define	KT_6		(CInput::GetKeyTrigger(DIK_6))
	#define	KT_7		(CInput::GetKeyTrigger(DIK_7))
	#define	KT_8		(CInput::GetKeyTrigger(DIK_8))
	#define	KT_9		(CInput::GetKeyTrigger(DIK_9))
	#define	KT_0		(CInput::GetKeyTrigger(DIK_0))
	#define	KT_UP		(CInput::GetKeyTrigger(DIK_UP))
	#define	KT_DOWN		(CInput::GetKeyTrigger(DIK_DOWN))
	#define	KT_LEFT		(CInput::GetKeyTrigger(DIK_LEFT))
	#define	KT_RIGHT	(CInput::GetKeyTrigger(DIK_RIGHT))
	#define	KT_ENTER	(CInput::GetKeyTrigger(DIK_RETURN))
	#define	KT_SPACE	(CInput::GetKeyTrigger(DIK_SPACE))
	#define	KT_LSHIFT	(CInput::GetKeyTrigger(DIK_LSHIFT))
	#define	KT_RSHIFT	(CInput::GetKeyTrigger(DIK_RSHIFT))
};

class CInputRelease
{
public:
	#define	KR_A		(CInput::GetKeyRelease(DIK_A))
	#define	KR_B		(CInput::GetKeyRelease(DIK_B))
	#define	KR_C		(CInput::GetKeyRelease(DIK_C))
	#define	KR_D		(CInput::GetKeyRelease(DIK_D))
	#define	KR_E		(CInput::GetKeyRelease(DIK_E))
	#define	KR_F		(CInput::GetKeyRelease(DIK_F))
	#define	KR_G		(CInput::GetKeyRelease(DIK_G))
	#define	KR_H		(CInput::GetKeyRelease(DIK_H))
	#define	KR_I		(CInput::GetKeyRelease(DIK_I))
	#define	KR_J		(CInput::GetKeyRelease(DIK_J))
	#define	KR_K		(CInput::GetKeyRelease(DIK_K))
	#define	KR_L		(CInput::GetKeyRelease(DIK_L))
	#define	KR_M		(CInput::GetKeyRelease(DIK_M))
	#define	KR_N		(CInput::GetKeyRelease(DIK_N))
	#define	KR_O		(CInput::GetKeyRelease(DIK_O))
	#define	KR_P		(CInput::GetKeyRelease(DIK_P))
	#define	KR_Q		(CInput::GetKeyRelease(DIK_Q))
	#define	KR_R		(CInput::GetKeyRelease(DIK_R))
	#define	KR_S		(CInput::GetKeyRelease(DIK_S))
	#define	KR_T		(CInput::GetKeyRelease(DIK_T))
	#define	KR_U		(CInput::GetKeyRelease(DIK_U))
	#define	KR_V		(CInput::GetKeyRelease(DIK_V))
	#define	KR_W		(CInput::GetKeyRelease(DIK_W))
	#define	KR_X		(CInput::GetKeyRelease(DIK_X))
	#define	KR_Y		(CInput::GetKeyRelease(DIK_Y))
	#define	KR_Z		(CInput::GetKeyRelease(DIK_Z))
	#define	KR_1		(CInput::GetKeyRelease(DIK_1))
	#define	KR_2		(CInput::GetKeyRelease(DIK_2))
	#define	KR_3		(CInput::GetKeyRelease(DIK_3))
	#define	KR_4		(CInput::GetKeyRelease(DIK_4))
	#define	KR_5		(CInput::GetKeyRelease(DIK_5))
	#define	KR_6		(CInput::GetKeyRelease(DIK_6))
	#define	KR_7		(CInput::GetKeyRelease(DIK_7))
	#define	KR_8		(CInput::GetKeyRelease(DIK_8))
	#define	KR_9		(CInput::GetKeyRelease(DIK_9))
	#define	KR_0		(CInput::GetKeyRelease(DIK_0))
	#define	KR_UP		(CInput::GetKeyRelease(DIK_UP))
	#define	KR_DOWN		(CInput::GetKeyRelease(DIK_DOWN))
	#define	KR_LEFT		(CInput::GetKeyRelease(DIK_LEFT))
	#define	KR_RIGHT	(CInput::GetKeyRelease(DIK_RIGHT))
	#define	KR_ENTER	(CInput::GetKeyRelease(DIK_RETURN))
	#define	KR_SPACE	(CInput::GetKeyRelease(DIK_SPACE))
	#define	KR_LSHIFT	(CInput::GetKeyRelease(DIK_LSHIFT))
	#define	KR_RSHIFT	(CInput::GetKeyRelease(DIK_RSHIFT))
};

class CInputRepeat
{
public:
	#define	KRP_A		(CInput::GetKeyRepeat(DIK_A))
	#define	KRP_B		(CInput::GetKeyRepeat(DIK_B))
	#define	KRP_C		(CInput::GetKeyRepeat(DIK_C))
	#define	KRP_D		(CInput::GetKeyRepeat(DIK_D))
	#define	KRP_E		(CInput::GetKeyRepeat(DIK_E))
	#define	KRP_F		(CInput::GetKeyRepeat(DIK_F))
	#define	KRP_G		(CInput::GetKeyRepeat(DIK_G))
	#define	KRP_H		(CInput::GetKeyRepeat(DIK_H))
	#define	KRP_I		(CInput::GetKeyRepeat(DIK_I))
	#define	KRP_J		(CInput::GetKeyRepeat(DIK_J))
	#define	KRP_K		(CInput::GetKeyRepeat(DIK_K))
	#define	KRP_L		(CInput::GetKeyRepeat(DIK_L))
	#define	KRP_M		(CInput::GetKeyRepeat(DIK_M))
	#define	KRP_N		(CInput::GetKeyRepeat(DIK_N))
	#define	KRP_O		(CInput::GetKeyRepeat(DIK_O))
	#define	KRP_P		(CInput::GetKeyRepeat(DIK_P))
	#define	KRP_Q		(CInput::GetKeyRepeat(DIK_Q))
	#define	KRP_R		(CInput::GetKeyRepeat(DIK_R))
	#define	KRP_S		(CInput::GetKeyRepeat(DIK_S))
	#define	KRP_T		(CInput::GetKeyRepeat(DIK_T))
	#define	KRP_U		(CInput::GetKeyRepeat(DIK_U))
	#define	KRP_V		(CInput::GetKeyRepeat(DIK_V))
	#define	KRP_W		(CInput::GetKeyRepeat(DIK_W))
	#define	KRP_X		(CInput::GetKeyRepeat(DIK_X))
	#define	KRP_Y		(CInput::GetKeyRepeat(DIK_Y))
	#define	KRP_Z		(CInput::GetKeyRepeat(DIK_Z))
	#define	KRP_1		(CInput::GetKeyRepeat(DIK_1))
	#define	KRP_2		(CInput::GetKeyRepeat(DIK_2))
	#define	KRP_3		(CInput::GetKeyRepeat(DIK_3))
	#define	KRP_4		(CInput::GetKeyRepeat(DIK_4))
	#define	KRP_5		(CInput::GetKeyRepeat(DIK_5))
	#define	KRP_6		(CInput::GetKeyRepeat(DIK_6))
	#define	KRP_7		(CInput::GetKeyRepeat(DIK_7))
	#define	KRP_8		(CInput::GetKeyRepeat(DIK_8))
	#define	KRP_9		(CInput::GetKeyRepeat(DIK_9))
	#define	KRP_0		(CInput::GetKeyRepeat(DIK_0))
	#define	KRP_UP		(CInput::GetKeyRepeat(DIK_UP))
	#define	KRP_DOWN	(CInput::GetKeyRepeat(DIK_DOWN))
	#define	KRP_LEFT	(CInput::GetKeyRepeat(DIK_LEFT))
	#define	KRP_RIGHT	(CInput::GetKeyRepeat(DIK_RIGHT))
	#define	KRP_ENTER	(CInput::GetKeyRepeat(DIK_RETURN))
	#define	KRP_SPACE	(CInput::GetKeyRepeat(DIK_SPACE))
	#define	KRP_LSHIFT	(CInput::GetKeyRepeat(DIK_LSHIFT))
	#define	KRP_RSHIFT	(CInput::GetKeyRepeat(DIK_RSHIFT))
};

#endif