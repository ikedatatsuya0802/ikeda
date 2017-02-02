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

	AddHolizontalLight(D3DXVECTOR3(0.7f, -0.5f, 0.2f), D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(-0.75f, -0.55f, -0.25f), D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	AddHolizontalLight(D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));

	AddSpotLight(D3DXVECTOR3(0.0f, 100.0f, 400.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f),
		D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f), D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f), D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f));
	//ChangeLight(m_LightNum - 1, false);
	
	return S_OK;
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

	// ���s�����̐ݒ�
	light.Type = D3DLIGHT_DIRECTIONAL;
	
	// �f�B�t���[�Y�J���[�̐ݒ�
	light.Diffuse = D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);

	// �����x�N�g���̐��K��
	D3DXVec3Normalize((D3DXVECTOR3*)&light.Direction, &vec);

	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(m_LightNum, &light);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(m_LightNum, TRUE);

	// �z��ǉ�
	m_Light.push_back(light);

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

	// ���s�����̐ݒ�
	//light.Type = D3DLIGHT_SPOT;
	light.Type = D3DLIGHT_POINT;

	// �f�B�t���[�Y�J���[�̐ݒ�
	light.Diffuse	= D3DXCOLOR(dif.r, dif.g, dif.b, dif.a);
	light.Specular	= D3DXCOLOR(spec.r, spec.g, spec.b, spec.a);
	light.Ambient	= D3DXCOLOR(amb.r, amb.g, amb.b, amb.a);

	// �ʒu�̐ݒ�
	light.Position = pos;

	// �����x�N�g���̐ݒ�
	light.Direction = vec;
	
	light.Range			= 1000.0f;
	light.Falloff		= 1.0f;
	light.Attenuation0	= 0.5f;
	light.Attenuation1	= 0.2f;
	light.Attenuation2	= 0.1f;
	light.Theta			= D3DXToRadian(30.0f);
	light.Phi			= D3DXToRadian(60.0f);

	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(2, &light);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(2, TRUE);

	// �z��ǉ�
	m_Light.push_back(light);

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
	if((num >= m_Light.size()) || (m_Light.size() == 0))
	{// 臒l�`�F�b�N

		return;
	}

	// �ʒu�̐ݒ�
	m_Light[num].Position = pos;

	// �����x�N�g���̐ݒ�
	m_Light[num].Direction = vec;
	
	// ���C�g�̃Z�b�g
	D3D_DEVICE->SetLight(num, &m_Light[num]);

	// ���C�g�̗L����
	D3D_DEVICE->LightEnable(num, TRUE);
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