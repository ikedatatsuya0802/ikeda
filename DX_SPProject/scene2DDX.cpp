//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(2DDX)
//	�t�@�C����	scene2DDX.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	�֐���	:CScene2DDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CScene2DDX::CScene2DDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
{
	m_fLength	= 0.0f;
	m_fAngle	= 0.0f;
	m_pTexture	= NULL;
	m_pVtxBuff	= NULL;
}

//=============================================================================
//	�֐���	:~CScene2DDX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CScene2DDX::~CScene2DDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CScene2DDX::Init(cchar *str, cVec3 pos, cVec2 size, cfloat rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, 0.0f));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, rot));
	m_fLength	= hypotf(size.x, size.y) * 0.5f;
	m_fAngle	= atan2f(size.x, size.y);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// ���_�o�b�t�@�ݒ�
	SetVtxBuff();

	// �e�N�X�`���̃��[�h
	Load(str);
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CScene2DDX::SetVtxBuff(void)
{
	VERTEX_2D	*pVtx;	// 3D���_���


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

	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		// ���Z�W���ݒ�
		pVtx[i].rhw = 1.0f;

		// ���_�F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CScene2DDX::Uninit(void)
{
	Unload();

	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CScene2DDX::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CScene2DDX::Draw(void)
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
//	�֐���	:Create
//	����	:D3DXVECTOR3	pos		-> �����ʒu
//			:D3DXVECTOR2	size	-> �|���S���̃T�C�Y
//			:char			*str	-> �e�N�X�`���̃t�@�C���p�X
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CScene2DDX *CScene2DDX::Create(cchar *str, cVec3 pos, cVec2 size, cfloat rot)
{
	CScene2DDX *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CScene2DDX();

	// ����������
	instance->Init(str, pos, size, rot);

	// �C���X�^���X�����^�[��
	return instance;
}

//=============================================================================
//	�֐���	:SetColor
//	����	:bool	mode	-> �ύX���[�h�Atrue�Ŕ{���w��Afalse�Œl�w��
//			:float	x		-> ��(�{��)
//			:float	y		-> ����(�{��)
//	�߂�l	:����
//	����	:�|���S���F��ݒ肷��B
//=============================================================================
void CScene2DDX::SetSize(cbool mode, cfloat x, cfloat y)
{
	VERTEX_2D	*pVtx;	// 2D���_���

	if(mode)
	{// �{���w��
		
		float nx = (x * sinf(m_fAngle - m_Rot.z) * m_fLength);
		float ny = (y * cosf(m_fAngle - m_Rot.z) * m_fLength);

		m_fLength = hypotf(nx, ny);
		m_fAngle = atan2f(nx, ny);
	}
	else
	{// �l�w��

		m_fLength = hypotf(x, y) * 0.5f;
		m_fAngle = atan2f(x, y);
	}


	// ���_�o�b�t�@�ɔ��f
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z) * m_fLength));
	pVtx[0].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z) * m_fLength));

	pVtx[1].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z) * m_fLength));
	pVtx[1].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z) * m_fLength));

	pVtx[2].Pos.x = (m_Pos.x - (sinf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));
	pVtx[2].Pos.y = (m_Pos.y - (cosf(-m_fAngle - m_Rot.z + D3DX_PI) * m_fLength));

	pVtx[3].Pos.x = (m_Pos.x - (sinf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));
	pVtx[3].Pos.y = (m_Pos.y - (cosf(m_fAngle - m_Rot.z - D3DX_PI) * m_fLength));

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:SetColor
//	����	:float	a		-> �A���t�@�l
//			:float	r		-> R�l
//			:float	g		-> G�l
//			:float	b		-> B�l
//	�߂�l	:����
//	����	:�|���S���F��ݒ肷��B
//=============================================================================
void CScene2DDX::SetColor(cfloat a, cfloat r, cfloat g, cfloat b)
{
	VERTEX_2D	*pVtx;	// 2D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].col = D3DCOLOR_COLORVALUE(r, g, b, a);
	}
	
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:SetUV
//	����	:cVec2	leftTop		-> ����UV���W
//			:cVec2	rightButtom	-> �E��UV���W
//	�߂�l	:����
//	����	:�|���S����UV���W��ݒ肷��B
//=============================================================================
void CScene2DDX::SetUV(cVec2 leftTop, cVec2 rightButtom)
{
	VERTEX_2D	*pVtx;	// 2D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	pVtx[0].tex = D3DXVECTOR2(leftTop.x, leftTop.y);
	pVtx[1].tex = D3DXVECTOR2(rightButtom.x, leftTop.y);
	pVtx[2].tex = D3DXVECTOR2(leftTop.x, rightButtom.y);
	pVtx[3].tex = D3DXVECTOR2(rightButtom.x, rightButtom.y);

	m_pVtxBuff->Unlock();
}