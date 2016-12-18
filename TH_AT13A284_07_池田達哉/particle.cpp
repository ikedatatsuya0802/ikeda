//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(3D)
//	�t�@�C����	scene3D.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "main.h"

//=============================================================================
//	�ÓI�����o
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture[PARTICLE_PATTERN];

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CParticle::CParticle(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CParticle::Init(int pattern)
{
	if((pattern < 0) || (pattern >= 4))
		pattern = 0;

	random_device rd;	// �����_���f�o�C�X
	mt19937 mt(rd());	// ���������@

	// �p�^�[���ݒ�
	m_Pattern = pattern;

	// ������]��0
	m_Rot = VEC3_ZERO;

	// ���ݒ�
	switch(m_Pattern)
	{
	case 0:// �t
	{
		// ���������@�쐬
		uniform_real_distribution<float> posX(-500, 500);
		uniform_real_distribution<float> posZ(-500, 500);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-10, -2);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.3f, 0.3f);
		uniform_real_distribution<float> rotY(-0.3f, 0.3f);
		uniform_real_distribution<float> rotZ(-0.3f, 0.3f);

		// �e����ݒ�
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 700.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Life		= 120;
	}
	break;
	case 1:// ��
	{
		// ���������@�쐬
		uniform_real_distribution<float> posX(300, 800);
		uniform_real_distribution<float> posZ(-800, -300);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-10, -2);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.1f, 0.1f);
		uniform_real_distribution<float> rotY(-0.1f, 0.1f);
		uniform_real_distribution<float> rotZ(-0.1f, 0.1f);

		// �e����ݒ�
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 500.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
		m_Life		= 60;
	}
	break;
	case 2:// �H
	{
		// ���������@�쐬
		uniform_real_distribution<float> posX(300, 500);
		uniform_real_distribution<float> posZ(-500, -300);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-5, -1);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.3f, 0.3f);
		uniform_real_distribution<float> rotY(-0.3f, 0.3f);
		uniform_real_distribution<float> rotZ(-0.3f, 0.3f);

		// �e����ݒ�
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 500.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(-0.1f, 0.0f, 0.1f);
		m_Life		= 120;
	}
	break;
	case 3:// �~
	{
		// ���������@�쐬
		uniform_real_distribution<float> posX(-500, 500);
		uniform_real_distribution<float> posZ(-500, 500);
		uniform_real_distribution<float> moveX(-5, 5);
		uniform_real_distribution<float> moveY(-3, -1);
		uniform_real_distribution<float> moveZ(-5, 5);

		// �e����ݒ�
		m_Size		= D3DXVECTOR2(20.0f, 20.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 700.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= VEC3_ZERO;
		m_Wind		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Life		= 180;
	}
	break;
	default:
		break;
	}

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);	
	SetVtxBuff();
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CParticle::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D���_���


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���_���W�ݒ�
	pVtx[0].Pos.x = -(m_Size.x * 0.5f);
	pVtx[0].Pos.y = (m_Size.y * 0.5f);
	pVtx[0].Pos.z = 0.0f;

	pVtx[1].Pos.x = (m_Size.x * 0.5f);
	pVtx[1].Pos.y = (m_Size.y * 0.5f);
	pVtx[1].Pos.z = 0.0f;

	pVtx[2].Pos.x = -(m_Size.x * 0.5f);
	pVtx[2].Pos.y = -(m_Size.y * 0.5f);
	pVtx[2].Pos.z = 0.0f;

	pVtx[3].Pos.x = (m_Size.x * 0.5f);
	pVtx[3].Pos.y = -(m_Size.y * 0.5f);
	pVtx[3].Pos.z = 0.0f;

	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		// �@���ݒ�
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		switch(m_Pattern)
		{
		case 0:// �t
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(FCOLOR(255), FCOLOR(182), FCOLOR(193), 1.0f);
			break;
		case 1:// ��
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(FCOLOR(34), FCOLOR(139), FCOLOR(34), 1.0f);
			break;
		case 2:// �H
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 0.3f, 0.3f, 1.0f);
			break;
		case 3:// �~
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		default:
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
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
void CParticle::Uninit(void)
{
	// �C���X�^���X�폜
	SafetyRelease(m_pVtxBuff);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CParticle::Update(void)
{
	// �e�폈��
	m_Move	+= m_Wind;
	m_Pos	+= m_Move;
	m_Rot	+= m_RotMove;

	m_Life--;
	if(m_Life <= 0)
	{
		// �C���X�^���X�폜
		Release();
		return;
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CParticle::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	if(m_Pattern < 3)
	{
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	}
	else
	{
		CRendererDX::SetMatrixBB(&m_mtxWorld, m_Pos, m_Rot);
	}

	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �J�����O�I�t
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 100);

	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	
	// �e�N�X�`���̐ݒ�
	switch(m_Pattern)
	{
	case 0:// �t
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		break;
	case 1:// ��
		D3D_DEVICE->SetTexture(0, m_pTexture[1]);
		break;
	case 2:// �H
		D3D_DEVICE->SetTexture(0, m_pTexture[2]);
		break;
	case 3:// �~
		D3D_DEVICE->SetTexture(0, m_pTexture[3]);
		break;
	default:
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		break;
	}
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// �`��

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	// �J�����O�I��
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CParticle *CParticle::Create(int pattern)
{
	CParticle *instance;	// �C���X�^���X

	// �C���X�^���X����
	instance = new CParticle();

	// ����������
	instance->Init(pattern);

	// �C���X�^���X�����^�[��
	return instance;
}