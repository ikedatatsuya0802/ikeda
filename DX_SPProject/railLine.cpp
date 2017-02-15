//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	railLine.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "railLine.h"
#include "main.h"
#include <time.h>
#include "input.h"
#include "manager.h"
#include "cameraDX.h"

//=============================================================================
//	�ÓI�����o
//=============================================================================
LPDIRECT3DTEXTURE9 CRailLine::m_pTextures[2];

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CRailLine::CRailLine(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CRailLine::~CRailLine()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CRailLine::Init(int line, D3DXVECTOR3 pos)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);
	m_RailLine = line;
	m_EditType = ET_POSVEC;
	
	LoadSpline(line);
	CalcSpline(line);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffPointer, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffDrift, NULL);

	// ���_�f�[�^�Z�b�g
	SetSplineVtx();
	SetSplineVtxVec();
	SetSplineVtxSPoints();
	SetSplineVtxLPoints();
	SetSplineVtxPointer();
	SetSplineVtxDrift();

	// �e�N�X�`���̃��[�h
	Load();
}

//=============================================================================
//	�֐���	:SetSplineVtx
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtx(int line)
{
	VERTEX_3D* pVtx;	// 3D���_���
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i <= (int)m_Spline.PosHermite.size() ; i++)
		{
			// ���W�ݒ�
			if(i == (int)m_Spline.PosHermite.size())
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.Pos[(int)m_Spline.Pos.size() - 1].x, 1.0f, m_Spline.Pos[(int)m_Spline.Pos.size() - 1].z);
			}
			else
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.PosHermite[i].x, 1.0f, m_Spline.PosHermite[i].z);
			}

			// �@���ݒ�
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`�����W�ݒ�
			pVtx[i].tex = VEC2_ZERO;
		}
	}
	m_pVtxBuff->Unlock();	
}

//=============================================================================
//	�֐���	:SetSplineVtxVec
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̐���x�N�g���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxVec(int line)
{
	VERTEX_3D* pVtx;	// 3D���_���

	m_pVtxBuffVec->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			// ���W�ݒ�
			pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline.Pos[i].x, 1.0f, m_Spline.Pos[i].z);
			pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline.Pos[i].x + m_Spline.Vec[i].x, 1.0f, m_Spline.Pos[i].z + m_Spline.Vec[i].z);

			// �@���ݒ�
			pVtx[i * 2 + 0].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i * 2 + 1].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i * 2 + 0].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[i * 2 + 1].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`�����W�ݒ�
			pVtx[i * 2 + 0].tex = VEC2_ZERO;
			pVtx[i * 2 + 1].tex = VEC2_ZERO;
		}
	}
	m_pVtxBuffVec->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtxSPoints
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxSPoints(int line)
{
	VERTEX_3D* pVtx;	// ���_���
	

	m_pVtxBuffSPoints->Lock(0, 0, (void**)&pVtx, 0);

	// ���W�ݒ�
	pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[1].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);
	pVtx[3].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);

	// �@���ݒ�
	for(int i = 0 ; i < VERTEX_SQUARE ; i++)
	{
		pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuffSPoints->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtxLPoints
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxLPoints(int line)
{
	VERTEX_3D* pVtx;	// ���_���
	
	
	m_pVtxBuffLPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�ݒ�
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);

		// �@���ݒ�
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// �e�N�X�`�����W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffLPoints->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtxPointer
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�}�E�X�̃h���b�O�͈͂̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxPointer(int line)
{
	VERTEX_3D* pVtx;	// ���_���


	m_pVtxBuffPointer->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�ݒ�
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, (RAILLINE_DRAG_SIZE / 2));
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_DRAG_SIZE / 2), 0.5f, -(RAILLINE_DRAG_SIZE / 2));

		// �@���ݒ�
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffPointer->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtxDrift
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�h���t�g�̊J�n�E�I�_�̃}�[�N�B
//=============================================================================
void CRailLine::SetSplineVtxDrift(int line)
{
	VERTEX_3D* pVtx;	// ���_���


	m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�ݒ�
		pVtx[0].Pos = D3DXVECTOR3(-75.0f, 150.0f, 0.0f);
		pVtx[1].Pos = D3DXVECTOR3(75.0f, 150.0f, 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-75.0f, 0.0f, 0.0f);
		pVtx[3].Pos = D3DXVECTOR3(75.0f, 0.0f, 0.0f);

		// �@���ݒ�
		for(int i = 0 ; i < VERTEX_SQUARE ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`�����W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.25f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.25f, 1.0f);
	}
	m_pVtxBuffDrift->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CRailLine::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pVtxBuffVec);
	SafetyRelease(m_pVtxBuffSPoints);
	SafetyRelease(m_pVtxBuffLPoints);
	SafetyRelease(m_pVtxBuffPointer);
	SafetyRelease(m_pVtxBuffDrift);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CRailLine::Update(void)
{
	// �`��t���O�ݒ�
	m_flgDraw = CManager::GetEdhitMode() ? true : false;

	if(CManager::GetEdhitMode())
	{// �G�f�B�b�g���[�h�̏ꍇ�̂ݏ���

		// �G�f�B�b�g���[�h�ύX
		if(CInput::GetMouseTrigger(MBTN_CENTER))
		{
			if(m_EditType == ET_POSVEC) m_EditType = ET_DRIFT;
			else if(m_EditType == ET_DRIFT) m_EditType = ET_POSVEC;
		}

		// �J�����̍����ɉ������X�P�[����ݒ�
		m_YScale = (DX_CAMERA->GetCameraPosV().y / 2000.0f);
		
		// �X�v���C�����Z�[�u
		if(CInput::GetKeyTrigger(DIK_F1))
		{
			// �X�v���C�������Z�[�u
			SaveSpline();

			// �f�o�b�O���\���J�E���g���Z�b�g
			m_DebugProcCnt = DEBUGPROC_CNT;
		}
		
		// �X�v���C���̐���_�𑝂₷
		if(CInput::GetKeyTrigger(DIK_F2))
		{
			AddPoints();
		}
		else if(CInput::GetKeyTrigger(DIK_F3))
		{
			DeletePoints();
		}


		// �}�E�X�ɂ��G�f�B�b�g����
		MouseEdit();
	}
}

//=============================================================================
//	�֐���	:MouseEdit
//	����	:����
//	�߂�l	:����
//	����	:�}�E�X�ɂ��G�f�B�b�g����B
//=============================================================================
void CRailLine::MouseEdit(void)
{
	static POINT holdPos = {0};	// �z�[���h�J�n���̃}�E�X�̃X�N���[�����W

	if(CInput::GetMousePress(MBTN_LEFT))
	{// �}�E�X��������Ă�����

		switch(m_EditType)
		{
		case ET_POSVEC:
			if(CInput::GetMouseTrigger(MBTN_LEFT))
			{// �h���b�O�t���O�ݒ�

				// ��̒��_��D��I�ɑI�����A�h���b�O�t���O���I����
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos(), b = m_Spline.Pos[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));

					if(length < RAILLINE_DRAG_SIZE)
					{
						holdPos = CInput::GetMouseScreenPos();
						m_Spline.ifHold[i] = 1;
						break;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					if(KH_LSHIFT)
					{
						// �����G�f�B�b�g
						m_Spline.Pos[i].y = ((float)holdPos.y - CInput::GetMouseScreenPos().y) * 2.0f;
						
						// �����̉����l�ݒ�
						if(m_Spline.Pos[i].y < 1.0f)
						{
							m_Spline.Pos[i].y = 1.0f;
						}
					}
					else
					{
						m_Spline.Pos[i].x = CInput::GetMouseWorldPos().x;
						m_Spline.Pos[i].z = CInput::GetMouseWorldPos().z;
					}
				}
			}
			break;
		case ET_DRIFT:
			if(CInput::GetMouseTrigger(MBTN_LEFT))
			{// �h���b�O�t���O�ݒ�

				// ��̒��_��D��I�ɑI�����A�h���b�O�t���O���I����
				for(int i = ((int)m_Spline.Drift.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos();
					D3DXVECTOR3 b = GetSplinePos(m_Spline.Drift[i].Begin);
					D3DXVECTOR3 c = GetSplinePos(m_Spline.Drift[i].End);
					float lengthBegin = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));
					float lengthEnd = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(c.x, 1.0f, c.z)));

					if(lengthBegin < RAILLINE_DRAG_SIZE)
					{
						if(lengthBegin < lengthEnd) m_Spline.Drift[i].ifHoldBegin = true;
						break;
					}
					else if(lengthEnd < RAILLINE_DRAG_SIZE)
					{
						if(lengthEnd < lengthBegin) m_Spline.Drift[i].ifHoldEnd = true;
						break;
					}
				}
			}

			// �h���t�g�����؂�ւ�
			if(CInput::GetKeyTrigger(DIK_F4))
			{
#pragma omp parallel for
				for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
				{
					if(m_Spline.Drift[i].ifHoldBegin || m_Spline.Drift[i].ifHoldEnd)
					{
						//ChangeFlag(m_Spline.Drift[i].Curve);
						m_Spline.Drift[i].Curve = m_Spline.Drift[i].Curve ? false : true;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				// �X�v���C���̍ŋߓ_�T��
				float per = 0.0f;
				int maxHermite = 0;

#pragma omp parallel for
				for(int j = 0 ; j < RAILLINE_DRIFT_SEARCH ; j++)
				{
					float tMax = (1.0f / RAILLINE_DRIFT_SEARCH * maxHermite);
					float t = (1.0f / RAILLINE_DRIFT_SEARCH * j);
					float lengthMin = D3DXVec3Length(&(CInput::GetMouseWorldPos() - GetSplinePos(tMax)));
					float length = D3DXVec3Length(&(CInput::GetMouseWorldPos() - GetSplinePos(t)));
					if(lengthMin > length)
					{
						maxHermite = j;
					}
				}

				if(m_Spline.Drift[i].ifHoldBegin)
				{
					m_Spline.Drift[i].Begin = ((1.0f / RAILLINE_DRIFT_SEARCH) * maxHermite);

					// ���͈͐ݒ�
					if(i > 0)
					{
						if(m_Spline.Drift[i].Begin <= m_Spline.Drift[i - 1].End + RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].Begin = m_Spline.Drift[i - 1].End + RAILLINE_DRIFT_DRAG;
						}
					}

					if(i < (int)m_Spline.Drift.size() - 1)
					{
						if(m_Spline.Drift[i].Begin >= m_Spline.Drift[i].End - RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].Begin = m_Spline.Drift[i].End - RAILLINE_DRIFT_DRAG;
						}
					}
				}
				else if(m_Spline.Drift[i].ifHoldEnd)
				{
					m_Spline.Drift[i].End = ((1.0f / RAILLINE_DRIFT_SEARCH) * maxHermite);

					// ���͈͐ݒ�
					if(i < (int)m_Spline.Drift.size() - 1)
					{
						if(m_Spline.Drift[i].End >= m_Spline.Drift[i + 1].Begin - RAILLINE_DRIFT_DRAG)
						{
							m_Spline.Drift[i].End = m_Spline.Drift[i + 1].Begin - RAILLINE_DRIFT_DRAG;
						}
					}

					if(m_Spline.Drift[i].End <= m_Spline.Drift[i].Begin + RAILLINE_DRIFT_DRAG)
					{
						m_Spline.Drift[i].End = m_Spline.Drift[i].Begin + RAILLINE_DRIFT_DRAG;
					}
				}
			}
			break;
		default:
			break;
		}

		if(KT_X)
		{// X���W��0��
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Pos[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}
		else if(KT_Z)
		{// Z���W��0��
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Pos[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}

		// �X�v���C���̍Čv�Z
		CalcSpline();
	}
	else if(CInput::GetMousePress(MBTN_RIGHT))
	{
		switch(m_EditType)
		{
		case ET_POSVEC:
			if(CInput::GetMouseTrigger(MBTN_RIGHT))
			{// �h���b�O�t���O�ݒ�

				// ��̒��_��D��I�ɑI�����A�h���b�O�t���O���I����
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos();
					D3DXVECTOR3 b = m_Spline.Pos[i] + m_Spline.Vec[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 1.0f, a.z) - D3DXVECTOR3(b.x, 1.0f, b.z)));

					if(length < (RAILLINE_DRAG_SIZE * m_YScale))
					{
						m_Spline.ifHold[i] = 1;
						break;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Vec[i] = CInput::GetMouseWorldPos() - m_Spline.Pos[i];
				}
			}
			break;
		default:
			break;
		}

		
		if(KT_X)
		{// X���W��0��
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Vec[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}		
		else if(KT_Z)
		{// Z���W��0��
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Vec[i].x = 0.0f;
					m_Spline.ifHold[i] = 0;
					break;
				}
			}
		}

		// �X�v���C���̍Čv�Z
		CalcSpline();
	}
	else if(CInput::GetMouseRelease(MBTN_LEFT) || CInput::GetMouseRelease(MBTN_RIGHT))
	{// �}�E�X�������[�X���ꂽ�ꍇ

		// �S���_�̃z�[���h������
#pragma omp parallel for
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			m_Spline.ifHold[i] = 0;
		}
#pragma omp parallel for
		for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
		{
			m_Spline.Drift[i].ifHoldBegin	= false;
			m_Spline.Drift[i].ifHoldEnd		= false;
		}

		// �X�v���C���̍Čv�Z
		CalcSpline();
	}


	// ���`�̍Čv�Z
	SetSplineVtx();
	SetSplineVtxVec();
}

//=============================================================================
//	�֐���	:AddPoints
//	����	:����
//	�߂�l	:����
//	����	:�X�v���C������_�𑝂₷�B
//=============================================================================
void CRailLine::AddPoints(void)
{
	// ������W��ǉ�
	D3DXVECTOR3 pos = m_Spline.Pos[m_Spline.Pos.size() - 1];
	m_Spline.Pos.push_back(D3DXVECTOR3(pos.x, pos.y, pos.z));
	m_Spline.ifHold.push_back(false);
	m_Spline.LengthMin.push_back(0);

	// ����x�N�g����ǉ�
	D3DXVECTOR3 vec = m_Spline.Vec[m_Spline.Vec.size() - 1];
	m_Spline.Vec.push_back(D3DXVECTOR3(vec.x, vec.y, vec.z));

	// ���_�o�b�t�@�̃��T�C�Y
	SafetyRelease(m_pVtxBuff);
	SafetyRelease(m_pVtxBuffVec);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	SetSplineVtx();
	SetSplineVtxVec();

	// �X�v���C���̍Čv�Z
	CalcSpline();
}

//=============================================================================
//	�֐���	:DeletePoints
//	����	:����
//	�߂�l	:����
//	����	:�X�v���C������_���폜����B
//=============================================================================
void CRailLine::DeletePoints(void)
{
	// �Œ��2�̐���_�͕ێ�����
	if(m_Spline.Pos.size() > 2)
	{
		// ������W���폜
		m_Spline.Pos.pop_back();
		m_Spline.ifHold.pop_back();
		m_Spline.LengthMin.pop_back();

		// ����x�N�g�����폜
		m_Spline.Vec.pop_back();

		// ���_�o�b�t�@�̃��T�C�Y
		SafetyRelease(m_pVtxBuff);
		SafetyRelease(m_pVtxBuffVec);
		D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
		D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
		SetSplineVtx();
		SetSplineVtxVec();

		// �X�v���C���̍Čv�Z
		CalcSpline();
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CRailLine::Draw(void)
{
	static int dpCnt = 0;
	dpCnt++;
	if(dpCnt > 120 * 100000)
	{
		dpCnt = 0;
	}

	if(m_flgDraw)
	{
		// �}�E�X���W�ɑΉ�����X�P�[��
		D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f);
		if(m_YScale < 1.0f) m_YScale = 1.0f;
		scl *= m_YScale;

		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// Z�e�X�g�J�n
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_ALWAYS);

		// �A���t�@�e�X�g�J�n
		D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
		D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

		// ���C�e�B���O�ݒ���I�t��
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// ���`�`��
		D3D_DEVICE->SetTexture(0, NULL);	// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->DrawPrimitive(D3DPT_LINESTRIP, 0, m_Spline.PosHermite.size());	// �`��

		// �x�N�g���`��
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffVec, 0, sizeof(VERTEX_3D));		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->DrawPrimitive(D3DPT_LINELIST, 0, (m_Spline.Pos.size() * 2));	// �`��

		// ���Z�����ݒ�
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTextures[0]);


		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffSPoints, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			if((i % RAILLINE_SET) != 0)
			{
				// �}�g���b�N�X�ݒ�
				CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.PosHermite[i], VEC3_ZERO, scl);

				// �`��
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
			}
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffLPoints, 0, sizeof(VERTEX_3D));
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			D3DXVECTOR3 pos = m_Spline.Pos[i];
			pos.y += 1.0f;

			// �}�g���b�N�X�ݒ�
			CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);

			// �`��
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
		}

		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffPointer, 0, sizeof(VERTEX_3D));
		// �}�g���b�N�X�ݒ�
		scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f) * (m_YScale / 2);
		CRendererDX::SetMatrix(&m_mtxWorld, CInput::GetMouseWorldPos(), VEC3_ZERO, scl);
		// �`��
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);

		if(m_EditType == ET_DRIFT)
		{
			// �e�N�X�`���̐ݒ�
			D3D_DEVICE->SetTexture(0, m_pTextures[1]);
			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				D3DXVECTOR3 pos;
				VERTEX_3D *pVtx;
				float tex;

				// �e�N�X�`�����W�̐ݒ�
				m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
				tex = m_Spline.Drift[i].Curve ? 0.25f : 0.0f;
				for(int texture = 0 ; texture < VERTEX_SQUARE ; texture++)
					pVtx[texture].tex = D3DXVECTOR2(tex + ((texture % 2) * 0.25f), ((float)(texture / 2)));
				m_pVtxBuffDrift->Unlock();
				// ���_�t�H�[�}�b�g�̐ݒ�
				D3D_DEVICE->SetStreamSource(0, m_pVtxBuffDrift, 0, sizeof(VERTEX_3D));
				// �X�v���C�����W�擾
				pos = GetSplinePos(m_Spline.Drift[i].Begin);
				// �}�g���b�N�X�ݒ�
				CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);
				// �`��
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);

				// �e�N�X�`�����W�̐ݒ�
				m_pVtxBuffDrift->Lock(0, 0, (void**)&pVtx, 0);
				tex = m_Spline.Drift[i].Curve ? 0.75f : 0.5f;
				for(int texture = 0 ; texture < VERTEX_SQUARE ; texture++)
					pVtx[texture].tex = D3DXVECTOR2(tex + ((texture % 2) * 0.25f), ((float)(texture / 2)));
				m_pVtxBuffDrift->Unlock();
				// ���_�t�H�[�}�b�g�̐ݒ�
				D3D_DEVICE->SetStreamSource(0, m_pVtxBuffDrift, 0, sizeof(VERTEX_3D));
				// �X�v���C�����W�擾
				pos = GetSplinePos(m_Spline.Drift[i].End);
				// �}�g���b�N�X�ݒ�
				CRendererDX::SetMatrixBB(&m_mtxWorld, pos, VEC3_ZERO, scl);
				// �`��
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_SQUARE);
			}
		}

		// ���C�e�B���O�ݒ���I����
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �����_�[�X�e�[�g�ݒ��߂�
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

		// Z�E�A���t�@�e�X�g�I��
		CRendererDX::DisableZTest();
		CRendererDX::DisableAlphaTest();
	}

	// �f�o�b�O���\��
#ifdef _DEBUG
	if(CManager::GetEdhitMode())
	{
		CDebugProc::DebugProc("�G�f�B�b�g�^�C�v:%d\n", m_EditType);
		if(dpCnt / 120 % 2 == 0)
		{
			for(int i = 0 ; i < (int)m_Spline.Pos.size() / 2 ; i++)
			{
				CDebugProc::DebugProc("spl_pos[%2d]:(%5.1f:%5.1f:%5.1f) ", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
				CDebugProc::DebugProc("vec:(%5.1f:%5.1f:%5.1f)\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
			}
			if((int)m_Spline.Pos.size() % 2 == 1)
			{
				CDebugProc::DebugProc("\n");
			}
		}
		else
		{
			for(int i = (int)m_Spline.Pos.size() / 2 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				CDebugProc::DebugProc("spl_pos[%2d]:(%5.1f:%5.1f:%5.1f) ", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
				CDebugProc::DebugProc("vec:(%5.1f:%5.1f:%5.1f)\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
			}
		}
	}
	CDebugProc::DebugProc("�X�v���C����:%.1f\n", m_Spline.Length);

	CDebugProc::DebugProc("�X�v���C����Ԓ�\n");
	for(int i = 0 ; i < (int)m_Spline.LengthMin.size() - 1 ; i++)
	{
		CDebugProc::DebugProc("[%2d->%2d]:%4.0f, ", i, (i + 1), m_Spline.LengthMin[i]);

		if(i % 4 == 3)
		{
			CDebugProc::DebugProc("\n");
		}
	}
	CDebugProc::DebugProc("\n");

	if(m_DebugProcCnt > 0)
	{
		CDebugProc::DebugProc("�X�v���C�������Z�[�u���܂���\n");
		m_DebugProcCnt--;
	}
#endif
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CRailLine *CRailLine::Create(int line, D3DXVECTOR3 pos)
{
	CRailLine *scene3D;

	scene3D = new CRailLine;
	
	scene3D->Init(line, pos);

	return scene3D;
}

//=============================================================================
//	�֐���	:SaveSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C�������t�@�C���ɃZ�[�u����B
//=============================================================================
void CRailLine::SaveSpline(void)
{
	FILE* fp = NULL;					// �t�@�C���|�C���^
	time_t now = time(NULL);			// ���ݎ���
	struct tm *pnow = localtime(&now);	// ���ݎ���

	fp = fopen("./data/spline.txt", "w");

	fprintf_s(fp, "//==============================================================================\n");
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//	�X�v���C���f�[�^�p�X�N���v�g�t�@�C��\n");
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//	�f�[�^�o�͓����F%2d/%2d/%2d %2d:%2d:%2d\n", pnow->tm_year, pnow->tm_mon, pnow->tm_mday, pnow->tm_hour, pnow->tm_min, pnow->tm_sec);
	fprintf_s(fp, "//\n");
	fprintf_s(fp, "//==============================================================================\n");
	fprintf_s(fp, "SCRIPT				// �����Ȃ�I��Ώ����Ȃ�I\n\n");

	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// ���W���\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");

	for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
	{
		fprintf_s(fp, "POS = %.1f %.1f %.1f\n", m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
	}
	fprintf_s(fp, "\n");


	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// �x�N�g�����\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	for(int i = 0 ; i < (int)m_Spline.Vec.size() ; i++)
	{
		fprintf_s(fp, "VEC = %.1f %.1f %.1f\n", m_Spline.Vec[i].x, m_Spline.Vec[i].y, m_Spline.Vec[i].z);
	}
	fprintf_s(fp, "\n");


	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	fprintf_s(fp, "// �h���t�g�_���\n");
	fprintf_s(fp, "//------------------------------------------------------------------------------\n");
	for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
	{
		fprintf_s(fp, "DRIFT = %.4f %.4f %d\n", m_Spline.Drift[i].Begin, m_Spline.Drift[i].End, (int)m_Spline.Drift[i].Curve);
	}
	fprintf_s(fp, "\n");

	fprintf_s(fp, "END_SCRIPT			// �����Ȃ�I��Ώ����Ȃ�I");

	fclose(fp);
}

//=============================================================================
//	�֐���	:LoadSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�t�@�C������X�v���C������ǂݍ��ށB
//=============================================================================
void CRailLine::LoadSpline(int line)
{
	FILE* fp = NULL;	// �t�@�C���|�C���^
	int posnum = 0;

	fp = fopen("./data/spline.txt", "r");

	// END_SCRIPT�܂œǂݍ���
	while(!feof(fp))
	{
		char str[65535] = {NULL};
		memset(str, NULL, sizeof(str));

		// �P����擾
		fscanf(fp, "%s", str);

		if(strcmp(str, "POS") == 0)
		{// �X�v���C������_��ǉ�

			posnum++;
			D3DXVECTOR3 pos;
			fscanf(fp, " = %f %f %f\n", &pos.x, &pos.y, &pos.z);
			m_Spline.Pos.push_back(pos);
			m_Spline.ifHold.push_back(0);
			m_Spline.LengthMin.push_back(0.0f);
		}
		else if(strcmp(str, "VEC") == 0)
		{// �X�v���C������x�N�g����ǉ�
			
			D3DXVECTOR3 vec;
			fscanf(fp, " = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			m_Spline.Vec.push_back(vec);
		}
		else if(strcmp(str, "DRIFT") == 0)
		{// �X�v���C������x�N�g����ǉ�

			DRIFT_POINT drift;
			fscanf(fp, " = %f %f %d\n", &drift.Begin, &drift.End, &drift.Curve);
			m_Spline.Drift.push_back(drift);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{// �X�v���C���ǂݍ��݂��I��

			break;
		}
	}

	// �G���~�[�g���W��ǉ�
	m_Spline.PosHermite.resize(RAILLINE_SET);

	// �t�@�C���N���[�Y
	fclose(fp);
}

//=============================================================================
//	�֐���	:CalcSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C�����v�Z����B
//=============================================================================
void CRailLine::CalcSpline(int line)
{
	float t		= 0.0f;
	float rot	= 0.0f;
	D3DXVECTOR3 vec = VEC3_ZERO;
	
	if((int)m_Spline.Pos.size() >= 2)
	{
		// �X�v���C�����v�Z����
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			int		posNum	= (int)(t * ((int)m_Spline.Pos.size() - 1));
			float	nowt	= t * ((int)m_Spline.Pos.size() - 1) - posNum;

			m_Spline.PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].x)
				+ (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].x)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].x)
				+ ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].x);

			m_Spline.PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].z)
				+ (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].z)
				+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].z)
				+ ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].z);
			
			// �����ݒ�
			if(posNum < (int)m_Spline.Pos.size())
			{
				// ���_�ւ̍����̍����v�Z
				float high = m_Spline.Pos[posNum + 1].y - m_Spline.Pos[posNum].y;

				if(high > 0.0f)
				{// ����

					float rad = nowt * D3DX_PI;
					float pow = (-cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

					m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y + high * pow;
					rad = 0;
				}
				else if(high < 0.0f)
				{// �����

					high *= -1.0f;
					float rad = nowt * D3DX_PI;
					float pow = (cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

					m_Spline.PosHermite[i].y = (m_Spline.Pos[posNum].y - high) + high * pow;
				}
				else
				{
					m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y;
				}
			}
			else
			{
				m_Spline.PosHermite[i].y = m_Spline.Pos[posNum].y;
			}

			// �����Ԃ�i�߂�
			t += (1.0f / RAILLINE_SET);
		}

		// �X�v���C�������v�Z����
		m_Spline.Length = 0.0f;
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for(int i = 1 ; i < RAILLINE_DRIFT_SEARCH ; i++)
		{
			float oldT = (1.0f / RAILLINE_DRIFT_SEARCH * (i - 1));	// i-1�̃X�v���C���ʒu
			float t = (1.0f / RAILLINE_DRIFT_SEARCH * i);			// i�̃X�v���C���ʒu

			// 1��Ԃ̒����Z�o
			float length = D3DXVec3Length(&(GetSplinePos(oldT) - GetSplinePos(t)));

			// �������Z
			m_Spline.Length += length;
		}

		// �X�v���C����Ԓ����v�Z����
#ifdef _OPENMP
#pragma omp parallel for
#endif
		for(int i = 0 ; i < ((int)m_Spline.LengthMin.size() - 1) ; i++)
		{
			m_Spline.LengthMin[i] = 0.0f;
			float prevT = (1.0f / (m_Spline.Pos.size() - 1) * i);
			float nextT = (1.0f / (m_Spline.Pos.size() - 1) * (i + 1));
			for(int j = 1 ; j < RAILLINE_DRIFT_SEARCH ; j++)
			{
				float oldT = ((nextT - prevT) / RAILLINE_DRIFT_SEARCH * (j - 1)) + prevT;	// i-1�̃X�v���C���ʒu
				float t = ((nextT - prevT) / RAILLINE_DRIFT_SEARCH * j) + prevT;			// i�̃X�v���C���ʒu

				// 1��Ԃ̒����Z�o
				float length = D3DXVec3Length(&(GetSplinePos(oldT) - GetSplinePos(t)));

				// �������Z
				m_Spline.LengthMin[i] += length;
			}
		}
	}
}

//=============================================================================
//	�֐���	:GetSplinePos
//	����	:float t	->	�X�v���C���}��ϐ�
//	�߂�l	:�Ȃ�
//	����	:�}��ϐ��̒l�ɉ������X�v���C����̓_��Ԃ��B
//=============================================================================
D3DXVECTOR3 CRailLine::GetSplinePos(float t)
{
	D3DXVECTOR3 pos;

	if(((int)m_Spline.Pos.size() >= 2) && ((t >= 0.0f) && (t <= 1.0f)))
	{
		int		posNum = (int)(t * ((int)m_Spline.Pos.size() - 1));
		float	nowt = t * ((int)m_Spline.Pos.size() - 1) - posNum;

		pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].x)
			+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].x);
		pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[posNum].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[posNum + 1].z)
			+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[posNum].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[posNum + 1].z);
		

		if(posNum < (int)m_Spline.Pos.size())
		{
			// ���_�ւ̍����̍����v�Z
			float high = m_Spline.Pos[posNum + 1].y - m_Spline.Pos[posNum].y;

			if(high > 0.0f)
			{// ����

				float rad = nowt * D3DX_PI;
				float pow = (-cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

				pos.y = m_Spline.Pos[posNum].y + high * pow;
			}
			else if(high < 0.0f)
			{// �����

				high *= -1.0f;
				float rad = nowt * D3DX_PI;
				float pow = (cosf(rad) * RAILLINE_SLOPE + RAILLINE_SLOPE) * 0.5f;

				pos.y = (m_Spline.Pos[posNum].y - high) + high * pow;
			}
			else
			{
				pos.y = m_Spline.Pos[posNum].y;
			}
		}
		else
		{
			pos.y = m_Spline.Pos[posNum].y;
		}
	}

	return pos;
}

//=============================================================================
//	�֐���	:GetDriftStatus
//	����	:float oldt	->	�X�v���C���}��ϐ�(��)
//			:float t	->	�X�v���C���}��ϐ�(�V)
//	�߂�l	:�Ȃ�
//	����	:�h���t�g�����ۂ��A�܂��ǂ̏�Ԃ���Ԃ��B
//=============================================================================
DRIFT_STATUS CRailLine::GetDriftStatus(float oldt, float t)
{
	DRIFT_STATUS ds = { false, false, 0 };	// �h���t�g�X�e�[�^�X
	bool ifOldInDrift = false;		// �O��X�v���C�����W���h���t�g�n�_�ɓ����Ă��邩
	bool ifNewInDrift = false;		// ����X�v���C�����W���h���t�g�n�_�ɓ����Ă��邩

	if((int)m_Spline.Pos.size() >= 2)
	{
		if((t >= 0.0f) && (t <= 1.0f) && (oldt >= 0.0f) && (oldt <= 1.0f))
		{
			for(int i = 0 ; i < (int)m_Spline.Drift.size() ; i++)
			{
				if((oldt >= m_Spline.Drift[i].Begin) && (oldt <= m_Spline.Drift[i].End))
				{// �O����W���h���t�g��Ԃ̒��ɂ���

					ifOldInDrift = true;
					ds.Curve = m_Spline.Drift[i].Curve;
				}
				if((t >= m_Spline.Drift[i].Begin) && (t <= m_Spline.Drift[i].End))
				{// ������W���h���t�g��Ԃ̒��ɂ���

					ifNewInDrift = true;
					ds.Curve = m_Spline.Drift[i].Curve;
				}
			}
		}

		if(!ifOldInDrift && !ifNewInDrift)
		{// �h���t�g��Ԃɓ����Ă��Ȃ�

			ds.ifDrift	= false;
			ds.Status	= 0;
		}
		else if(!ifOldInDrift && ifNewInDrift)
		{// �h���t�g�̎n�_

			ds.ifDrift = true;
			ds.Status = -1;
		}
		else if(ifOldInDrift && !ifNewInDrift)
		{// �h���t�g�̏I�_

			ds.ifDrift = true;
			ds.Status = 1;
		}
		else if(ifOldInDrift && ifNewInDrift)
		{// �h���t�g��Ԃ̒�

			ds.ifDrift = true;
			ds.Status = 0;
		}
	}

	return ds;
}

//=============================================================================
//	�֐���	:GetMoveVec
//	����	:float t	->	�X�v���C���}��ϐ�
//	�߂�l	:�Ȃ�
//	����	:t�̈ʒu�ł̐i�s������Ԃ��B
//=============================================================================
D3DXVECTOR3 CRailLine::GetMoveVec(float t)
{
	D3DXVECTOR3 vec = GetSplinePos(t + 0.01f) - GetSplinePos(t);

	return vec;
}

//=============================================================================
//	�֐���	:AngleOf2Vector
//	����	:D3DXVECTOR3 a	->	�x�N�g��1
//			:D3DXVECTOR3 b	->	�x�N�g��2
//	�߂�l	:�Ȃ�
//	����	:2�̃x�N�g���̂Ȃ��p��Ԃ��B
//=============================================================================
float CRailLine::AngleOf2Vector(D3DXVECTOR3 a, D3DXVECTOR3 b)
{
	//���ςƃx�N�g���������g����cos�Ƃ����߂�
	float length = D3DXVec3Length(&a) * D3DXVec3Length(&b);

	if(length > 0.0f)
	{
		float rad = D3DXVec3Dot(&a, &b) / length;

		if(rad > 1.0f)
		{
			rad = 1.0f;
		}

		//cos�Ƃ���Ƃ����߂�
		float angle = acos(rad);

		// �p�x�␳
		if(a.y < 0.0f)
		{
			angle *= -1;
		}

		return angle;
	}
	else
	{
		return 0;
	}
}