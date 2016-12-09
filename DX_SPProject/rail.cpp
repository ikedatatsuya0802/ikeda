//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	rail.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "rail.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"
#include "game.h"
#include "railLine.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CRail::m_pTexture;

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CRail::CRail(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CRail::~CRail()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CRail::Init(int line, D3DXVECTOR3 pos)
{	
	char			*str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_RailLine = line;
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (m_Spline->PosHermite.size() * 2 + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture);
	
	// ���[�����Z�b�g
	SetVtxBuff();

	Load();
}

//=============================================================================
//	�֐���	:SetRailData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CRail::SetVtxBuff(void)
{
	VERTEX_3D			*pVtx;		// 3D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;
		//D3DXVECTOR3 a[RAIL_VERTEX];

		// �`����W�ݒ�
		rot = atan2f((m_Spline->PosHermite[1].x - m_Spline->PosHermite[0].x), (m_Spline->PosHermite[1].z - m_Spline->PosHermite[0].z));
		pVtx[0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
		pVtx[1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z + sinf(rot) * (RAIL_WIDTH * 0.5f));
		for(int i = 1 ; i < (int)m_Spline->PosHermite.size() ; i++)
		{
			// �p�x�ݒ�
			if(i != ((int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[i + 1].x - m_Spline->PosHermite[i].x), (m_Spline->PosHermite[i + 1].z - m_Spline->PosHermite[i].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[i].x - m_Spline->PosHermite[i - 1].x), (m_Spline->PosHermite[i].z - m_Spline->PosHermite[i - 1].z));
			}

			if(i == ((int)m_Spline->PosHermite.size() - 1))
			{
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + (cosf(rot) * (RAIL_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[0].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (RAIL_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[0].z + sinf(rot) * (RAIL_WIDTH * 0.5f));
			}
			else
			{
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x + (cosf(rot) * (RAIL_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[i].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x - cosf(rot) * (RAIL_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[i].z + sinf(rot) * (RAIL_WIDTH * 0.5f));
			}
		}

		for(int i = 0 ; i < ((int)m_Spline->PosHermite.size() * 2 + 2) ; i++)
		{
			// �@���ݒ�
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`���\�t���W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		for(int i = 1 ; i < (int)m_Spline->PosHermite.size() ; i++)
		{
			pVtx[i * 2 + 0].tex = D3DXVECTOR2(0.0f, (float)i * 0.75f);
			pVtx[i * 2 + 1].tex = D3DXVECTOR2(1.0f, (float)i * 0.75f);
		}

	}
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CRail::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pTexture);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CRail::Update(void)
{
	// �`��t���O�ݒ�
	m_flgDraw = DX_CAMERA->GetCameraMode() ? false : true;

	// ���[�����Z�b�g
	SetVtxBuff();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CRail::Draw(void)
{
	if(m_flgDraw)
	{
		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// ���C�e�B���O�ݒ���I�t��
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ((int)m_Spline->PosHermite.size() * 2));	// �`��

		// ���C�e�B���O�ݒ���I����
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CRail *CRail::Create(int line, D3DXVECTOR3 pos)
{
	CRail *scene3D;

	scene3D = new CRail;
	
	scene3D->Init(line, pos);

	return scene3D;
}