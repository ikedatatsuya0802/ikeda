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

D3DLIGHT9 CLightDX::m_Light[3];
//list<D3DLIGHT9> CLightDX::m_Light(3);
list<D3DLIGHT9>::iterator CLightDX::m_ItrLight;
int				CLightDX::m_LightNum;

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
	ZeroMemory(&m_Light[0], sizeof(D3DLIGHT9));
	m_Light[0].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_Light[0].Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[0].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(0, &m_Light[0]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(0, TRUE);										// ���C�g�̗L����

																		// 2�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_Light[1], sizeof(D3DLIGHT9));
	m_Light[1].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_Light[1].Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[1].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(1, &m_Light[1]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(1, TRUE);										// ���C�g�̗L����

																		// 3�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_Light[2], sizeof(D3DLIGHT9));
	m_Light[2].Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_Light[2].Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[2].Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(2, &m_Light[2]);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(2, TRUE);										// ���C�g�̗L����

	/*
	m_LightNum = 0;

	//AddHolizontalLight(D3DXVECTOR3(1.0, 1.0, 1.0), D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f));
	
	D3DXVECTOR3 vecDir;
	m_ItrLight = m_Light.begin();

	// 1�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	//m_ItrLight->Diffuse = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);			// ���̐F
	m_ItrLight->Diffuse = D3DXCOLOR(0.3f, 0.3f, 0.3f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.7f, -0.5f, 0.2f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(0, &*m_ItrLight);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(0, TRUE);										// ���C�g�̗L����
	m_ItrLight++;
	
	// 2�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_ItrLight->Diffuse = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(-0.75f, -0.55f, -0.25f);						// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(1, &*m_ItrLight);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(1, TRUE);										// ���C�g�̗L����
	m_ItrLight++;
	
	// 3�Ԗڃ��C�g�̐ݒ�
	ZeroMemory(&m_ItrLight, sizeof(D3DLIGHT9));
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;							// ���s����
	m_ItrLight->Diffuse = D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f);			// ���̐F
	vecDir = D3DXVECTOR3(0.0f, 0.0f, -1.0f);							// ���̌���
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vecDir);	// ���K��
	D3D_DEVICE->SetLight(2, &*m_ItrLight);									// ���C�g�̔��f
	D3D_DEVICE->LightEnable(2, TRUE);										// ���C�g�̗L����
	*/
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
	//CDebugProc::DebugProc("���C�g�x�N�g��:(%5.2f:%5.2f:%5.2f)\n", m_ItrLight->Direction.x, m_ItrLight->Direction.y, m_ItrLight->Direction.z);
#endif
}

//=============================================================================
//	�֐���	:AddHolizontalLight
//	����	:����
//	�߂�l	:����
//	����	:���s������ǉ�����B
//=============================================================================
void CLightDX::AddHolizontalLight(D3DXVECTOR3 vec, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{/*
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// �z��ǉ�
	m_Light.push_back(light);

	m_ItrLight = m_Light.end();

	// ���s�����̐ݒ�
	m_ItrLight->Type = D3DLIGHT_DIRECTIONAL;
					
	// �f�B�t���[�Y�J���[�̐ݒ�
	m_ItrLight->Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// �����x�N�g���̐��K��
	D3DXVec3Normalize((D3DXVECTOR3*)&m_ItrLight->Direction, &vec);

	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(0, &*m_ItrLight);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(0, TRUE);*/
}

//=============================================================================
//	�֐���	:AddPointLight
//	����	:����
//	�߂�l	:����
//	����	:�_������ǉ�����B
//=============================================================================
void CLightDX::AddPointLight(D3DXVECTOR3 pos, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{

}

//=============================================================================
//	�֐���	:AddSpotLight
//	����	:����
//	�߂�l	:����
//	����	:�X�|�b�g������ǉ�����B
//=============================================================================
void CLightDX::AddSpotLight(D3DXVECTOR3 pos, D3DXVECTOR3 vec, D3DCOLORVALUE dif, D3DCOLORVALUE spec, D3DCOLORVALUE amb)
{

}