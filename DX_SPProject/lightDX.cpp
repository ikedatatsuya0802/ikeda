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
vector<D3DLIGHT9>	CLightDX::m_Light;
vector<D3DLIGHT9>	CLightDX::m_SpotLight;
uint				CLightDX::m_LightNum;

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
HRESULT CLightDX::Init(void)
{
	m_LightNum = 0;

	AddHolizontalLight(D3DXVECTOR3(0.7f, -0.5f, 0.2f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(-0.75f, -0.55f, -0.25f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));

	AddSpotLight(D3DXVECTOR3(30.0f, 10.0f, 100.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	//ChangeLight(m_LightNum - 1, false);
	
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
	D3DLIGHT9 light;
	ZeroMemory(&light, sizeof(D3DLIGHT9));

	// �z��ǉ�
	m_SpotLight.push_back(light);

	// ���s�����̐ݒ�
	//m_SpotLight[m_SpotLight.size() - 1].Type = D3DLIGHT_SPOT;
	m_SpotLight[m_SpotLight.size() - 1].Type = D3DLIGHT_POINT;

	// �f�B�t���[�Y�J���[�̐ݒ�
	m_SpotLight[m_SpotLight.size() - 1].Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	//m_SpotLight[m_SpotLight.size() - 1].Specular = D3DXCOLOR(spec.r, spec.g, spec.b, spec.a);
	//m_SpotLight[m_SpotLight.size() - 1].Ambient = D3DXCOLOR(amb.r, amb.g, amb.b, amb.a);

	// �ʒu�̐ݒ�
	m_SpotLight[m_SpotLight.size() - 1].Position = pos;

	// �����x�N�g���̐ݒ�
	m_SpotLight[m_SpotLight.size() - 1].Direction = vec;

	m_SpotLight[m_SpotLight.size() - 1].Theta = 0.1f;
	m_SpotLight[m_SpotLight.size() - 1].Phi = 0.2f;
	//m_SpotLight[m_SpotLight.size() - 1].Falloff = 1.0f;

	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(m_LightNum, &m_SpotLight[m_SpotLight.size() - 1]);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(m_LightNum, TRUE);

	m_LightNum++;
}

void CLightDX::ChangeLight(cuint num, cbool flug)
{
	if((num >= m_LightNum) || (m_LightNum == 0))
	{// 臒l�`�F�b�N

		return;
	}

	D3D_DEVICE->LightEnable(num, flug);
}

void CLightDX::SetSpotLight(cuint num, const D3DXVECTOR3 pos, const D3DXVECTOR3 vec)
{
	if((num >= m_SpotLight.size()) || (m_SpotLight.size() == 0))
	{// 臒l�`�F�b�N

		return;
	}

	// �ʒu�̐ݒ�
	m_SpotLight[num].Position = pos;

	// �����x�N�g���̐ݒ�
	m_SpotLight[num].Direction = vec;
	
	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(m_LightNum - 1, &m_SpotLight[num]);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(m_LightNum - 1, TRUE);
}

//=============================================================================
//	�֐���	:ChangeHolLight
//	����	:����
//	�߂�l	:����
//	����	:���s�����̐F��ς���B
//=============================================================================
void CLightDX::ChangeHolLight(cuint num, D3DCOLORVALUE col)
{
	if(num < m_Light.size())
	{
		// �F�ύX
		m_Light[num].Diffuse = D3DXCOLOR(col.r, col.g, col.b, col.a);

		// ���C�g�̃Z�b�g
		D3D_DEVICE->SetLight(num, &m_Light[num]);
	}
}