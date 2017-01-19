//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	EStructure.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "EStructure.h"
#include "manager.h"
#include "rendererDX.h"
#include "manager.h"
#include "cameraDX.h"
#include "game.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CEStructure::m_pTexture;

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CEStructure::CEStructure(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CEStructure::~CEStructure()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CEStructure::Init(int line, D3DXVECTOR3 pos)
{	
	char			*str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_EStructureLine = line;
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (m_Spline->PosHermite.size() * 2 + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture);
	
	// ���[�����Z�b�g
	SetVtxBuff();

	Load();
}

//=============================================================================
//	�֐���	:SetEStructureData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CEStructure::SetVtxBuff(void)
{
	VERTEX_3D			*pVtx;		// 3D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;
		//D3DXVECTOR3 a[ESTRUCTURE_VERTEX];

		// �`����W�ݒ�
		rot = atan2f((m_Spline->PosHermite[1].x - m_Spline->PosHermite[0].x), (m_Spline->PosHermite[1].z - m_Spline->PosHermite[0].z));
		pVtx[0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
		pVtx[1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
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
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[0].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[0].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
			}
			else
			{
				pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
					0.1f,
					m_Spline->PosHermite[i].z - sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
				pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[i].x - cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f),
					0.1f,
					m_Spline->PosHermite[i].z + sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));
			}
		}

		for(int i = 0 ; i < ((int)m_Spline->PosHermite.size() * 2 + 2) ; i++)
		{
			// �@���ݒ�
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�ݒ�
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
void CEStructure::Uninit(void)
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
void CEStructure::Update(void)
{
	static SPLINE spline = *CGame::GetRailLine()->GetSpline();

	// �`��t���O�ݒ�
	m_flgDraw = DX_CAMERA->GetCameraMode() ? false : true;

	// ���[�����G�f�B�b�g���ꂽ�ꍇ�A�X�V
	if(fabsf(spline.Length - m_Spline->Length) < 0.01f)
	{
		// ���[�����Z�b�g
		SetVtxBuff();
	}

	spline = (*m_Spline);
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CEStructure::Draw(void)
{
	if(m_flgDraw)
	{
		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// Z�e�X�g�J�n
		CRendererDX::EnableZTest();

		// ���C�e�B���O�ݒ���I�t��
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, ((int)m_Spline->PosHermite.size() * 2));	// �`��

		// ���C�e�B���O�ݒ���I����
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// Z�e�X�g�I��
		CRendererDX::DisableZTest();
	}
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CEStructure *CEStructure::Create(int line, D3DXVECTOR3 pos)
{
	CEStructure *scene3D;

	scene3D = new CEStructure;
	
	scene3D->Init(line, pos);

	return scene3D;
}