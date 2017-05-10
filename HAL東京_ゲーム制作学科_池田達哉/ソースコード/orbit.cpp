//=============================================================================
//
//	�^�C�g��	�O�ՃG�t�F�N�g
//	�t�@�C����	orbit.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/11/10
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "orbit.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	COrbit::m_pTexture;

//=============================================================================
//	�֐���	:COrbit()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
COrbit::COrbit(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~COrbit()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
COrbit::~COrbit()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void COrbit::Init(D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_LocPos[0] = VEC3_ZERO;
	m_LocPos[1] = D3DXVECTOR3(0.0f, 50.0f, 0.0f);
	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ORBIT_VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// ���_���Z�b�g
	SetOrbitData();

	// �C���f�b�N�X�o�b�t�@�̊m��
	D3D_DEVICE->CreateIndexBuffer((sizeof(WORD) * ORBIT_INDEX_NUM), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &m_pIdxBuff, NULL);

	WORD *pIdx;

	m_pIdxBuff->Lock(0, 0, (void**)&pIdx, 0);
	{
		int buff = (ORBIT_VERTEX_NUM / (ORBIT_VERTICAL + 1));

		for(int nCntVertical = 0 ; nCntVertical < ORBIT_VERTICAL ; nCntVertical++)
		{
			for(int nCntIdx = 0 ; nCntIdx < ORBIT_INDEX_STANDARD ; nCntIdx++)
			{
				if((nCntVertical == (ORBIT_VERTICAL - 1)) && (nCntIdx == (ORBIT_INDEX_STANDARD - 2)))
				{// �����ł��؂�
					break;
				}

				if(nCntIdx == (ORBIT_INDEX_STANDARD - 1))
				{
					buff += (ORBIT_HORIZONTAL + 2);
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				}
				else if(nCntIdx == (ORBIT_INDEX_STANDARD - 2))
				{
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				}
				else
				{
					pIdx[nCntVertical * ORBIT_INDEX_STANDARD + nCntIdx] = buff;
				
					if(nCntIdx % 2 == 0)
					{
						buff -= (ORBIT_HORIZONTAL + 1);
					}
					else
					{
						if(nCntIdx != (ORBIT_INDEX_STANDARD - 3))
						{
							buff += (ORBIT_HORIZONTAL + 2);
						}
					}
				}
			}
		}
	}
	m_pIdxBuff->Unlock();

	// ���\�[�X�̃��[�h
	Load();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void COrbit::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pIdxBuff);

	// ���\�[�X�̃A�����[�h
	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void COrbit::Update(void)
{
	VERTEX_3D *pVtx;	// ���_�o�b�t�@
	CPlayer *player = CGame::GetPlayer1();

	if(player != NULL)
	{
		m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

		for(int i = 0 ; i < (ORBIT_VERTEX_NUM / 2 - 1) ; i++)
		{
			m_OldPos[i * 2] = m_OldPos[(i + 1) * 2];
			m_OldPos[i * 2 + 1] = m_OldPos[(i + 1) * 2 + 1];
		}

		D3DXVECTOR3 locPos1 = m_LocPos[0];
		D3DXVECTOR3 locPos2 = m_LocPos[1];

		D3DXVec3TransformCoord(&locPos1, &locPos1, player->GetWMatrix());
		D3DXVec3TransformCoord(&locPos2, &locPos2, player->GetWMatrix());

		m_OldPos[ORBIT_VERTEX_NUM - 1] = locPos1;
		m_OldPos[ORBIT_VERTEX_NUM - 2] = locPos2;

		for(int i = 0 ; i < ORBIT_VERTEX_NUM ; i++)
		{
			pVtx[i].Pos = m_OldPos[i];
		}

		m_pVtxBuff->Unlock();
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void COrbit::Draw(void)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// �}�g���b�N�X
	static float starttime = 20;


	// �}�g���b�N�X�̐ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// �J�����O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	
	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetIndices(m_pIdxBuff);								// �C���f�b�N�X�o�b�t�@�̃o�C���h
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, NULL);									// �e�N�X�`���̐ݒ�

	if(starttime <= 0)
	{
		D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, ORBIT_VERTEX_NUM, 0, ORBIT_POLYGON_NUM);	// �`��
	}

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �J�����O�ݒ��߂�
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	if(starttime > 0) starttime--;
}

//=============================================================================
//	�֐���:SetOrbitData
//	����:VERTEX_3D *pVtx(���_�f�[�^)
//	�߂�l:����
//	����:���_�f�[�^���Z�b�g����B
//=============================================================================
void COrbit::SetOrbitData(void)
{
	VERTEX_3D *pVtx;
	static float rot = 1.0f;
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= ORBIT_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= ORBIT_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].Pos = VEC3_ZERO;
		}
	}
	
	
	// �@���E�F�ݒ�
	for(int nCntVertex = 0 ; nCntVertex < ORBIT_VERTEX_NUM ; nCntVertex++)
	{
		pVtx[nCntVertex].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		pVtx[nCntVertex].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.7f, (((float)nCntVertex / (float)ORBIT_VERTEX_NUM) * 0.5f));
	}
	
	// �e�N�X�`�����W�ݒ�
	for(int nCntVtxVertical = 0 ; nCntVtxVertical <= ORBIT_VERTICAL ; nCntVtxVertical++)
	{
		for(int nCntVtxHorizontal = 0 ; nCntVtxHorizontal <= ORBIT_HORIZONTAL ; nCntVtxHorizontal++)
		{
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.x = (float)nCntVtxHorizontal * 0.5f;
			pVtx[(nCntVtxVertical * (ORBIT_HORIZONTAL + 1) + nCntVtxHorizontal)].tex.y = (float)nCntVtxVertical * 0.5f;
		}
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
COrbit *COrbit::Create(bool ifListAdd, int priority, OBJTYPE objtype, D3DXVECTOR3 pos)
{
	COrbit *orbit;
	
	orbit = new COrbit(ifListAdd, priority, objtype);

	orbit->Init();

	return orbit;
}