//=============================================================================
//
//	�^�C�g��	�J�����t�@�C��
//	�t�@�C����	light.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/26
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "lightDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"

//=============================================================================
//	�֐���	:CLightDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CLightDX::CLightDX()
{
	memset(m_aLight, NULL, sizeof(m_aLight));
}

//=============================================================================
//	�֐���	:~CLightDX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CLightDX::~CLightDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CLightDX::Init(void)
{
	
	D3DXVECTOR3 vecDir;

	// 1�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_aLight[0], sizeof(D3DLIGHT9));
	m_aLight[0].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_aLight[0].Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[0].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(0, &m_aLight[0]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(0, TRUE);										// ���C�g�̗L����
	
	// 2�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_aLight[1], sizeof(D3DLIGHT9));
	m_aLight[1].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_aLight[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[1].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(1, &m_aLight[1]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(1, TRUE);										// ���C�g�̗L����
	
	// 3�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_aLight[2], sizeof(D3DLIGHT9));
	m_aLight[2].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_aLight[2].Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_aLight[2].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(2, &m_aLight[2]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(2, TRUE);										// ���C�g�̗L����

	return S_OK;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CLightDX::Uninit(void)
{

}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CLightDX::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CLightDX::Draw(void)
{	
	// �f�o�b�O���\��
#ifdef _DEBUG
	//CDebugProc::DebugProc("���C�g�x�N�g��:(%5.2f:%5.2f:%5.2f)\n", m_aLight[0].Direction.x, m_aLight[0].Direction.y, m_aLight[0].Direction.z);
#endif
}