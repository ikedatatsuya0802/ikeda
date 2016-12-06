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

//D3DLIGHT9 CLightDX::m_Light[3];
vector<D3DLIGHT9> CLightDX::m_Light(3);
int	CLightDX::m_LightNum;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CLightDX::Init(void)
{
	m_LightNum = 0;

	AddHolizontalLight(D3DXVECTOR3(0.7f, -0.5f, 0.2f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(-0.75f, -0.55f, -0.25f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	
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
{
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// �z��ǉ�
	m_Light.push_back(light);

	// ���s�����̐ݒ�
	m_Light[m_LightNum].Type = D3DLIGHT_DIRECTIONAL;
	
	// �f�B�t���[�Y�J���[�̐ݒ�
	m_Light[m_LightNum].Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// �����x�N�g���̐��K��
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light[m_LightNum].Direction, &vec);

	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(m_LightNum, &m_Light[m_LightNum]);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(m_LightNum, TRUE);

	// ���C�g�̐��𑝂₷
	m_LightNum++;
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