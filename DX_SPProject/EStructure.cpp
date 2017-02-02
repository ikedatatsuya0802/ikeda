//=============================================================================
//
//	�^�C�g��	����
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
void CEStructure::Init(D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ESTRUCTURE_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\wall000.jpg", &m_pTexture);
	
	// ���[�����Z�b�g
	SetVtxBuff();

	// �C���f�b�N�X�Z�b�g
	SetMeshIndex(&m_pIdxBuff, ESTRUCTURE_HORIZONTAL, ESTRUCTURE_VERTICAL);

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
	VERTEX_3D *pVtx;		// 3D���_���

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;

		for(int y = 0 ; y < ESTRUCTURE_VERTICAL ; y++)
		{			
			D3DXVECTOR3 splPos = m_Spline->PosHermite[y];			
			splPos.y = (m_Spline->PosHermite[y].y < 2.0f) ? -100.0f : (splPos.y - 1.0f);
			D3DXVECTOR3 vec = VEC3_ZERO;

			// �p�x�ݒ�
			if(y != ((int)m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[y + 1].x - m_Spline->PosHermite[y].x), (m_Spline->PosHermite[y + 1].z - m_Spline->PosHermite[y].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[y].x - m_Spline->PosHermite[y - 1].x), (m_Spline->PosHermite[y].z - m_Spline->PosHermite[y - 1].z));
			}

			float cosWidth = (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f));	// ���H����X����
			float sinWidth = (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f));	// ���H����Z����
			float cosWidthOuter = cosWidth * 1.1f;						// �O�Ǖ���X����
			float sinWidthOuter = sinWidth * 1.1f;						// �O�Ǖ���Z����


			for(int x = 0 ; x <= ESTRUCTURE_HORIZONTAL ; x++)
			{
				D3DXVECTOR3 pos = VEC3_ZERO;

				switch(x)
				{
				case 0:		// ��_
					pos = D3DXVECTOR3(splPos.x, splPos.y, splPos.z);
					break;
				case 1:		// ����E���ǉ���
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						splPos.y,
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 2:		// ����E���Ǐ㕔
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 3:		// ����E�O�Ǐ㕔
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 4:		// ����E�O�ǉ���
					pos = D3DXVECTOR3(
						splPos.x - (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.2f)),
						splPos.z + (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 5:		// ��^���̓_
					pos = D3DXVECTOR3(splPos.x,	(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.3f)), splPos.z);
					break;
				case 6:		// ��E�E�O�ǉ���
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y - (ESTRUCTURE_WALL_HEIGHT * 0.2f)),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 7:		// ��E�E�O�Ǐ㕔
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.6f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.6f)));
					break;
				case 8:		// ��E�E���Ǐ㕔
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						(splPos.y + ESTRUCTURE_WALL_HEIGHT),
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 9:		// ��E�E���ǉ���
					pos = D3DXVECTOR3(
						splPos.x + (cosf(rot) * (ESTRUCTURE_WIDTH * 0.5f)),
						splPos.y,
						splPos.z - (sinf(rot) * (ESTRUCTURE_WIDTH * 0.5f)));
					break;
				case 10:	// ��_
					pos = D3DXVECTOR3(splPos.x, splPos.y, splPos.z);
					break;
				default:
					break;
				}

				int i = y * (ESTRUCTURE_HORIZONTAL + 1) + x;
				pVtx[i].Pos = pos;
			}
		}

		for(int i = 0 ; i < ESTRUCTURE_VERTEX_NUM ; i++)
		{
			// �@���ݒ�
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�ݒ�
			pVtx[i].tex = D3DXVECTOR2((float)(i % 2), (float)i * 0.75f);
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
	//if(m_flgDraw)
	{
		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO);

		// Z�e�X�g�J�n
		CRendererDX::EnableZTest();

		// ���C�e�B���O�ݒ���I�t��
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// �`�揈��
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetIndices(m_pIdxBuff);									// �C���f�b�N�X�o�b�t�@�̃o�C���h
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
		D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ESTRUCTURE_VERTEX_NUM, 0, ESTRUCTURE_POLYGON_NUM);

		// ���C�e�B���O�ݒ���I����
		//D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

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
CEStructure *CEStructure::Create(D3DXVECTOR3 pos)
{
	CEStructure *instance;

	instance = new CEStructure;
	
	instance->Init(pos);

	return instance;
}