//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	number.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "number.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CNumber::m_pTexture = NULL;

//=============================================================================
//	�֐���	:CNumber()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CNumber::CNumber(bool ifListAdd, int priority, OBJTYPE objType) : CScene2DDX(ifListAdd, priority, objType)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
	m_pTexture	= NULL;
}

//=============================================================================
//	�֐���	:~CNumber()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CNumber::~CNumber()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CNumber::Init(int value, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	VERTEX_2D *pVtx;	// 2D���_���

	// 臒l�`�F�b�N
	(value) > 9 ? value = 9 : 0;
	(value) < 0 ? value = 0 : 0;

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.z = 0.0f;
	
	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.z = 0.0f;
	
	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.z = 0.0f;
	
	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.z = 0.0f;
	
	// ���Z�W���ݒ�
	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].rhw = 1.0f;

		// �X�R�A�F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}
	
	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 1.0f);
	
	m_pVtxBuff->Unlock();

	Load();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CNumber::Draw(void)
{
	// �A���t�@�e�X�g�J�n
	CRendererDX::EnableAlphaTest();

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_2D);

	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// �e�N�X�`���̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// �`��
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);

	// �A���t�@�e�X�g�I��
	CRendererDX::DisableAlphaTest();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CNumber::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
	//Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CNumber::Update(void)
{

}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CNumber *CNumber::Create(int value, D3DXVECTOR3 pos, D3DXVECTOR2 size)
{
	CNumber *number;	// �C���X�^���X
	
	// �C���X�^���X����
	number = new CNumber();
	
	// ����������
	number->Init(value, pos, size);
	
	// �C���X�^���X�����^�[��
	return number;
}

//=============================================================================
//	�֐���	:Create
//	����	:int value(���l)
//	�߂�l	:����
//	����	:�����l�A���邢�͏����_���̑��̋L�����Z�b�g����B
//=============================================================================
void CNumber::SetNumber(int value)
{
	VERTEX_2D	*pVtx;	// 2D���_���

	// 臒l�`�F�b�N
	(value) > 9 ? value = 9 : 0;
	(value) < 0 ? value = 0 : 0;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(((float)value * 0.1f), 0.0f);
	pVtx[1].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 0.0f);
	pVtx[2].tex = D3DXVECTOR2(((float)value * 0.1f), 1.0f);
	pVtx[3].tex = D3DXVECTOR2(((float)value * 0.1f + 0.1f), 1.0f);

	m_pVtxBuff->Unlock();
}