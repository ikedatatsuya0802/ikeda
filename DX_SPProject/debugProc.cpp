//=============================================================================
//
//	�^�C�g��	�f�o�b�O��񏈗�
//	�t�@�C����	debugProc.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "debugProc.h"
#include "manager.h"
#include "rendererDX.h"
#include <stdio.h>

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPD3DXFONT	CDebugProc::m_pFont;
char		CDebugProc::m_aStrDebug[DEBUGSTR_MAX];

//=============================================================================
//	�֐���	:InitDebugProc
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CDebugProc::Init(void)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3D�f�o�C�X

	// �t�H���g�ݒ�
	D3DXCreateFont(pDevice, 18, 0, 0, 0, FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, "Terminal", &m_pFont);
	
	// ������������
	memset(m_aStrDebug, 0, sizeof(m_aStrDebug));

	return S_OK;
}

//=============================================================================
//	�֐���	:UnInitDebugProc
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CDebugProc::Uninit(void)
{
	if(m_pFont != NULL)
	{
		m_pFont->Release();
		m_pFont = NULL;
	}
}

//=============================================================================
//	�֐���	:UpdateDebugProc
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CDebugProc::Update(void)
{

}

//=============================================================================
//	�֐���	:DrawDebugProc
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CDebugProc::Draw(void)
{
	RECT rect = { 0, 0, (LONG)SCREEN_WIDTH, (LONG)SCREEN_HEIGHT };
	
	// �f�o�b�O���`��
	m_pFont->DrawText(NULL, &m_aStrDebug[0], -1, &rect, DT_LEFT, D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f));

	// ������������
	memset(m_aStrDebug, 0, sizeof(m_aStrDebug));
}

//=============================================================================
//	�֐���:	PrintfDebugProc
//	����	:char *fmt(�ψ����A�`�悷�镶����)
//	�߂�l	:����
//	����	:�^��printf�B
//=============================================================================
void CDebugProc::DebugProc(char *fmt, ...)
{
	va_list list;
	char str[256];

	va_start(list, fmt);

	vsprintf_s(str, fmt, list);

	strcat_s(m_aStrDebug, str);

	va_end(list);
}