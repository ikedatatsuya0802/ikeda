//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(3D)
//	�t�@�C����	input.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "input.h"
#include "main.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECTINPUT8			CInput::m_pInput;			// DirectInput�I�u�W�F�N�g�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	CInput::m_pDevKeyboard;		// ����3D�f�o�C�X(�L�[�{�[�h)�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	CInput::m_pDevMouse;		// ����3D�f�o�C�X(�}�E�X)�ւ̃|�C���^

KEYSTATE				CInput::m_KeyState;			// �L�[�{�[�h�̓��͏�񃏁[�N

MOUSE_STATUS			CInput::m_MState;			// �}�E�X���
MOUSESTATE				CInput::m_MouseState;		// �}�E�X�̓��͏�񃏁[�N

//=============================================================================
//	�֐���	:InitInput
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	// �ϐ�������
	m_pInput = NULL;
	m_pDevKeyboard = NULL;

	memset(&m_KeyState, 0, sizeof(m_KeyState));
	memset(&m_MouseState, 0, sizeof(m_MouseState));

	if(m_pInput == NULL)
	{
		if(FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&m_pInput, NULL)))
		{
			MessageBox(hWnd, "�E�B���h�E����܂����H", "�E�B���h�E�̏I��", MB_YESNO);
			return E_FAIL;
		}
	}

	InitKeyboard(hInstance, hWnd);
	InitMouse(hInstance, hWnd);

	return S_OK;
}

//=============================================================================
//	�֐���	:UnInitInput
//	����	:����
//	�߂�l	:����
//	����	:���͂̏I���������s���B
//=============================================================================
void CInput::Uninit(void)
{
	SafetyRelease(m_pInput);

	UninitKeyboard();
	UninitMouse();
}

//=============================================================================
//	�֐���	:UpdateInput
//	����	:����
//	�߂�l	:����
//	����	:���͂̍X�V�������s���B
//=============================================================================
void CInput::Update(void)
{
	UpdateKeyboard();
	UpdateMouse();
}

//=============================================================================
//	�֐���	:InitKeyboard
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CInput::InitKeyboard(HINSTANCE hInstance, HWND hWnd)
{
	if(FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevKeyboard, NULL)))							// 3D�f�o�C�X�̐���
	{
		MessageBox(hWnd, "m_pInput->CreateDevice�Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))											// �f�[�^�t�H�[�}�b�g�̐ݒ�
	{
		MessageBox(hWnd, "m_pDevKeyboard->SetDataFormat�Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))		// �������[�h�̐ݒ�
	{
		MessageBox(hWnd, "m_pDevKeyboard->SetCooperativeLevel�Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
		return E_FAIL;
	}
	m_pDevKeyboard->Acquire();		// �A�N�Z�X�����擾

	// �L�[�{�[�h�̃��s�[�g�J�E���g������
	memset(m_KeyState.RepeatCount, 0, sizeof(m_KeyState.RepeatCount));

	// ���s�[�g�Ԋu�̏�����
	m_KeyState.RepeatInterval = REPEAT_INTERVAL_NORMAL;

	return S_OK;
}

//=============================================================================
//	�֐���	:UpdateKeyboard
//	����	:����
//	�߂�l	:����
//	����	:���͂̍X�V�������s���B
//=============================================================================
void CInput::UpdateKeyboard(void)
{
	BYTE aKeyState[KEYSTATE_NUM];

	if(SUCCEEDED(m_pDevKeyboard->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for(int nCntKey = 0 ; nCntKey < KEYSTATE_NUM ; nCntKey++)
		{
			// �g���K����
			m_KeyState.Trigger[nCntKey] = aKeyState[nCntKey] & (m_KeyState.Press[nCntKey] ^ aKeyState[nCntKey]);
			
			// �����[�X����
			m_KeyState.Release[nCntKey] = m_KeyState.Press[nCntKey] & (m_KeyState.Press[nCntKey] ^ aKeyState[nCntKey]);

			// ���s�[�g����
			if(aKeyState[nCntKey])
			{
				m_KeyState.RepeatCount[nCntKey]++;
				if(m_KeyState.RepeatCount[nCntKey] > REPEAT_TIME)
				{
					// ���s�[�g�Ԋu�ɂ���ă��s�[�g���s��
					if(m_KeyState.RepeatCount[nCntKey] % m_KeyState.RepeatInterval == 0)
					{
						m_KeyState.Repeat[nCntKey] = aKeyState[nCntKey];
					}
					else
					{
						m_KeyState.Repeat[nCntKey] = 0;
					}
				}
			}
			else if(m_KeyState.Release[nCntKey])
			{
				m_KeyState.RepeatCount[nCntKey] = 0;
				m_KeyState.Repeat[nCntKey] = 0;
			}

			m_KeyState.Press[nCntKey] = aKeyState[nCntKey];
		}
	}
	else
	{
		m_pDevKeyboard->Acquire();
	}
}

//=============================================================================
//	�֐���	:UnInitKeyboard
//	����	:����
//	�߂�l	:����
//	����	:���͂̏I���������s���B
//=============================================================================
void CInput::UninitKeyboard(void)
{
	if(m_pDevKeyboard != NULL)
	{
		m_pDevKeyboard->Unacquire();	// �A�N�Z�X���̊J��
		m_pDevKeyboard->Release();
		m_pDevKeyboard = NULL;
	}
}

//=============================================================================
//	�֐���	:InitMouse
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CInput::InitMouse(HINSTANCE hInstance, HWND hWnd)
{
	// �}�E�X�|�C���^���̏�����
	SetRect(&m_MState.moveRect, 10, 10, (int)SCREEN_WIDTH-10, (int)SCREEN_HEIGHT-10);		// �}�E�X�J�[�\���̓����͈�
	m_MState.sPos.x		= (LONG)m_MState.moveRect.left;	// �}�E�X�J�[�\����X���W��������
	m_MState.sPos.y		= (LONG)m_MState.moveRect.top;		// �}�E�X�J�[�\����Y���W��������
	m_MState.wPos		= VEC3_ZERO;	// ���[���h���W�̏�����
	m_MState.lButton	= false;							// ���{�^���̏���������
	m_MState.rButton	= false;							// �E�{�^���̏���������
	m_MState.cButton	= false;							// �����{�^���̏���������
	m_MState.moveAdd	= 2;								// �}�E�X�J�[�\���̈ړ��ʂ�ݒ�
	SetRect(&m_MState.imgRect, 400, 0, 420, 20);							// �}�E�X�J�[�\���摜�̋�`��ݒ�
	m_MState.imgWidth = m_MState.imgRect.right - m_MState.imgRect.left;		// �摜�̕����v�Z
	m_MState.imgHeight = m_MState.imgRect.bottom - m_MState.imgRect.top;	// �摜�̍������v�Z
	m_MState.Notch			= 0;
	m_MState.WheelFraction	= 0;


	// �}�E�X�̃��s�[�g�J�E���g������
	memset(m_MouseState.RepeatCount, 0, sizeof(m_MouseState.RepeatCount));

	// ���s�[�g�Ԋu�̏�����
	m_MouseState.RepeatInterval = REPEAT_INTERVAL_NORMAL;

	return S_OK;
}

//=============================================================================
//	�֐���	:UpdateKeyboard
//	����	:����
//	�߂�l	:����
//	����	:���͂̍X�V�������s���B
//=============================================================================
void CInput::UpdateMouse(void)
{
	static bool aMouseState[MOUSESTATE_NUM]	= { false, false, false };
	static bool aMouseStateOld[MOUSESTATE_NUM]	= { false, false, false };


	// �}�E�X�̃X�N���[�����W�̎擾
	GetCursorPos(&m_MState.sPos);
	
	// ���[���h���W�̎擾
	MouseScreenToWorld(&m_MState.wPos, m_MState.sPos);
	//m_MState.wPos.x *= 1.01f;
	//m_MState.wPos.z *= 1.03f;

	// ���̑ޔ�
	aMouseStateOld[0] = aMouseState[0];
	aMouseStateOld[1] = aMouseState[1];
	aMouseStateOld[2] = aMouseState[2];

	// �}�E�X���擾
	aMouseState[0] = m_MState.lButton;
	aMouseState[1] = m_MState.cButton;
	aMouseState[2] = m_MState.rButton;

	for(int nCntButton = 0 ; nCntButton < MBTN_MAX ; nCntButton++)
	{
		// �g���K����
		m_MouseState.Trigger[nCntButton] = (aMouseState[nCntButton] && !aMouseStateOld[nCntButton]) ? true : false;

		// �����[�X����
		m_MouseState.Release[nCntButton] = (!aMouseState[nCntButton] && aMouseStateOld[nCntButton]) ? true : false;

		// ���s�[�g����
		if(aMouseState)
		{
			m_MouseState.RepeatCount[nCntButton]++;
			if(m_MouseState.RepeatCount[nCntButton] > REPEAT_TIME)
			{
				// ���s�[�g�Ԋu�ɂ���ă��s�[�g���s��
				if(m_MouseState.RepeatCount[nCntButton] % m_MouseState.RepeatInterval == 0)
				{
					m_MouseState.Repeat[nCntButton] = aMouseState[nCntButton];
				}
				else
				{
					m_MouseState.Repeat[nCntButton] = 0;
				}
			}
		}
		else if(m_MouseState.Release)
		{
			m_MouseState.RepeatCount[nCntButton] = 0;
			m_MouseState.Repeat[nCntButton] = 0;
		}

		m_MouseState.Press[nCntButton] = aMouseState[nCntButton];
	}
	

	CDebugProc::DebugProc("�}�E�X(2D):(%ld, %ld)\n", m_MState.sPos.x, m_MState.sPos.y);
	CDebugProc::DebugProc("�}�E�X(3D):(%.2f, %.2f, %.2f)\n", m_MState.wPos.x, m_MState.wPos.y, m_MState.wPos.z);
}

//=============================================================================
//	�֐���	:UnInitKeyboard
//	����	:����
//	�߂�l	:����
//	����	:���͂̏I���������s���B
//=============================================================================
void CInput::UninitMouse(void)
{
	if(m_pDevMouse != NULL)
	{
		m_pDevMouse->Unacquire();	// �A�N�Z�X���̊J��
		m_pDevMouse->Release();
		m_pDevMouse = NULL;
	}
}

//=============================================================================
//	�֐���	:MouseScreenToWorld
//	����	:����
//	�߂�l	:����
//	����	:�}�E�X�̃X�N���[�����W�����[���h���W�ɕϊ�����B
//=============================================================================
D3DXVECTOR3* CInput::MouseScreenToWorld(D3DXVECTOR3* pOut, POINT point)
{
	if((point.x > 0 && point.x < SCREEN_WIDTH) && (point.y > 0 && point.y < SCREEN_HEIGHT))
	{// �X�N���[���͈͓̔��������ꍇ

		D3DXMATRIX mtxView, mtxPrj;	// �}�g���b�N�X

		// �}�E�X�̃N���C�A���g���W�擾
		ScreenToClient(GethWnd(), &point);

		// �}�g���b�N�X�擾
		D3D_DEVICE->GetTransform(D3DTS_VIEW, &mtxView);
		D3D_DEVICE->GetTransform(D3DTS_PROJECTION, &mtxPrj);

		// �}�E�X�̃��[���h���W�v�Z
		CalcScreenToXZ(pOut, (int)point.x, (int)point.y, (int)SCREEN_WIDTH, (int)SCREEN_HEIGHT, &mtxView, &mtxPrj);
	}
	else
	{// �X�N���[���͈͓̔��łȂ��ꍇ

		*pOut = VEC3_ZERO;
	}


	return pOut;
}

//=============================================================================
//	�֐���	:CalcScreenToWorld
//	����	:����
//	�߂�l	:����
//	����	:�X�N���[�����W�����[���h���W�ɕϊ�����B
//=============================================================================
D3DXVECTOR3* CInput::CalcScreenToWorld(D3DXVECTOR3* pout, int Sx, int Sy, float fZ, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	// �e�s��̋t�s����Z�o
	D3DXMATRIX InvView, InvPrj, VP, InvViewport;
	D3DXMatrixInverse(&InvView, NULL, View);
	D3DXMatrixInverse(&InvPrj, NULL, Prj);
	D3DXMatrixIdentity(&VP);
	VP._11 = Screen_w / 2.0f; VP._22 = -Screen_h / 2.0f;
	VP._41 = Screen_w / 2.0f; VP._42 = Screen_h / 2.0f;
	D3DXMatrixInverse(&InvViewport, NULL, &VP);

	// �t�ϊ�
	D3DXMATRIX tmp = InvViewport * InvPrj * InvView;
	D3DXVec3TransformCoord(pout, &D3DXVECTOR3((float)Sx, (float)Sy, (float)fZ), &tmp);

	return pout;
}

//=============================================================================
//	�֐���	:CalcScreenToXZ
//	����	:����
//	�߂�l	:����
//	����	:�X�N���[�����W��XZ���ʂ̃��[���h���W�̌�_���Z�o����B
//=============================================================================
D3DXVECTOR3* CInput::CalcScreenToXZ(D3DXVECTOR3* pout, int Sx, int Sy, int Screen_w, int Screen_h, D3DXMATRIX* View, D3DXMATRIX* Prj)
{
	D3DXVECTOR3 nearpos;
	D3DXVECTOR3 farpos;
	D3DXVECTOR3 ray;
	CalcScreenToWorld(&nearpos, Sx, Sy, 0.0f, Screen_w, Screen_h, View, Prj);
	CalcScreenToWorld(&farpos, Sx, Sy, 1.0f, Screen_w, Screen_h, View, Prj);
	ray = farpos - nearpos;
	D3DXVec3Normalize(&ray, &ray);

	// ���Ƃ̌������N���Ă���ꍇ�͌�_��
	// �N���Ă��Ȃ��ꍇ�͉����̕ǂƂ̌�_���o��
	if(ray.y <= 0)
	{
		// ����_
		float Lray = D3DXVec3Dot(&ray, &D3DXVECTOR3(0, 1, 0));
		float LP0 = D3DXVec3Dot(&(-nearpos), &D3DXVECTOR3(0, 1, 0));
		*pout = nearpos + (LP0 / Lray)*ray;
	}
	else
	{
		*pout = farpos;
	}

	return pout;
}
