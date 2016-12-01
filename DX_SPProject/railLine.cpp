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
#include <time.h>
#include "readfile.h"
#include "railLine.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "input.h"
#include "cameraDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CRailLine::m_pTexture;

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CRailLine::CRailLine(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
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
	char			*str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_RailLineLine = line;
	
	LoadSpline(line);
	CalcSpline(line);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.PosHermite.size() + 1)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * ((int)m_Spline.Pos.size() * 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffVec, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffSPoints, NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuffLPoints, NULL);


	// ���_�f�[�^�Z�b�g
	SetSplineVtx();
	SetSplineVtxVec();
	SetSplineVtxSPoints();
	SetSplineVtxLPoints();

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
	VERTEX_3D	*pVtx;	// 3D���_���
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i <= (int)m_Spline.PosHermite.size() ; i++)
		{
			// ���W�ݒ�
			if(i == (int)m_Spline.PosHermite.size())
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.PosHermite[0].x, 0.1f, m_Spline.PosHermite[0].z);
			}
			else
			{
				pVtx[i].Pos = D3DXVECTOR3(m_Spline.PosHermite[i].x, 0.1f, m_Spline.PosHermite[i].z);
			}

			// �@���ݒ�
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);

			// �e�N�X�`���\�t���W�ݒ�
			pVtx[i].tex = D3DXVECTOR2(0.0f, 0.0f);
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
	VERTEX_3D	*pVtx;	// 3D���_���

	m_pVtxBuffVec->Lock(0, 0, (void**)&pVtx, 0);
	{
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			// ���W�ݒ�
			pVtx[i * 2 + 0].Pos = D3DXVECTOR3(m_Spline.Pos[i].x, 0.1f, m_Spline.Pos[i].z);
			pVtx[i * 2 + 1].Pos = D3DXVECTOR3(m_Spline.Pos[i].x + m_Spline.Vec[i].x, 0.1f, m_Spline.Pos[i].z + m_Spline.Vec[i].z);

			// �@���ݒ�
			pVtx[i * 2 + 0].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			pVtx[i * 2 + 1].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i * 2 + 0].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
			pVtx[i * 2 + 1].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);

			// �e�N�X�`���\�t���W�ݒ�
			pVtx[i * 2 + 0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[i * 2 + 1].tex = D3DXVECTOR2(0.0f, 0.0f);
		}
	}
	m_pVtxBuffVec->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtx
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxSPoints(int line)
{
	VERTEX_3D	*pVtx;	// ���_���
	

	m_pVtxBuffSPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�ݒ�
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), (RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_SPOINT_SIZE / 2), -(RAILLINE_SPOINT_SIZE / 2), 0.0f);

		// �@���ݒ�
		for(int i = 0 ; i < VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 0.0f, 1.0f);
		}

		// �e�N�X�`���\�t���W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffSPoints->Unlock();
}

//=============================================================================
//	�֐���	:SetSplineVtx
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�X�v���C���̒��_�f�[�^���Z�b�g����B
//=============================================================================
void CRailLine::SetSplineVtxLPoints(int line)
{
	VERTEX_3D	*pVtx;	// ���_���
	
	
	m_pVtxBuffLPoints->Lock(0, 0, (void**)&pVtx, 0);
	{
		// ���W�ݒ�
		pVtx[0].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[1].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), (RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[2].Pos = D3DXVECTOR3(-(RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);
		pVtx[3].Pos = D3DXVECTOR3((RAILLINE_LPOINT_SIZE / 2), -(RAILLINE_LPOINT_SIZE / 2), 0.0f);

		// �@���ݒ�
		for(int i = 0 ; i < VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 0.0f, 0.0f, 1.0f);
		}

		// �e�N�X�`���\�t���W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}
	m_pVtxBuffLPoints->Unlock();
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
	SafetyRelease(m_pTexture);

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
	m_flgDraw = CManager::GetCamera()->GetCameraMode() ? true : false;

	if(CManager::GetCamera()->GetCameraMode())
	{// �G�f�B�b�g���[�h�̏ꍇ�̂ݏ���

		// �X�v���C�����Z�[�u
		if(CInput::GetKeyTrigger(DIK_F1))
		{
			SaveSpline();
		}

		if(CInput::GetMousePress(MS_LB))
		{// �}�E�X��������Ă�����

			if(CInput::GetMouseTrigger(MS_LB))
			{// �h���b�O�t���O�ݒ�

				// ��̒��_��D��I�ɑI�����A�h���b�O�t���O���I����
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos(), b = m_Spline.Pos[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 0.0f, a.z) - D3DXVECTOR3(b.x, 0.0f, b.z)));

					if(length < RAILLINE_DRAG_SIZE)
					{
						m_Spline.ifHold[i] = true;
						break;
					}
				}
			}

			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				if(m_Spline.ifHold[i])
				{
					m_Spline.Pos[i] = CInput::GetMouseWorldPos();
				}
			}

			// �X�v���C���̍Čv�Z
			CalcSpline();
		}
		else if(CInput::GetMouseRelease(MS_LB))
		{// �}�E�X�������[�X���ꂽ�ꍇ

			// �S���_�̃z�[���h������
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				m_Spline.ifHold[i] = false;
			}
		}

		if(CInput::GetMousePress(MS_RB))
		{// �}�E�X��������Ă�����

			if(CInput::GetMouseTrigger(MS_RB))
			{// �h���b�O�t���O�ݒ�

				// ��̒��_��D��I�ɑI�����A�h���b�O�t���O���I����
				for(int i = ((int)m_Spline.Pos.size() - 1) ; i >= 0 ; i--)
				{
					D3DXVECTOR3 a = CInput::GetMouseWorldPos();
					D3DXVECTOR3 b = m_Spline.Pos[i] + m_Spline.Vec[i];
					float length = D3DXVec3Length(&(D3DXVECTOR3(a.x, 0.0f, a.z) - D3DXVECTOR3(b.x, 0.0f, b.z)));

					if(length < RAILLINE_DRAG_SIZE)
					{
						m_Spline.ifHold[i] = true;
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

			// �X�v���C���̍Čv�Z
			CalcSpline();
		}
		else if(CInput::GetMouseRelease(MS_RB))
		{// �}�E�X�������[�X���ꂽ�ꍇ

		 // �S���_�̃z�[���h������
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				m_Spline.ifHold[i] = false;
			}
		}

		// ���`�̍Čv�Z
		SetSplineVtx();
		SetSplineVtxVec();
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
	if(m_flgDraw)
	{
		// �}�g���b�N�X�ݒ�
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

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

		D3D_DEVICE->SetTexture(0, m_pTexture);	// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffSPoints, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++)
		{
			if((i % RAILLINE_SET) != 0)
			{
				// �}�g���b�N�X�ݒ�
				CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.PosHermite[i]);

				// �`��
				D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);
			}
		}

		D3D_DEVICE->SetStreamSource(0, m_pVtxBuffLPoints, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
		for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
		{
			// �}�g���b�N�X�ݒ�
			CRendererDX::SetMatrixBB(&m_mtxWorld, m_Spline.Pos[i]);

			// �`��
			D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, VERTEX_NUM);
		}

		// ���C�e�B���O�ݒ���I����
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

		// �����_�[�X�e�[�g�ݒ��߂�
		D3D_DEVICE->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
		D3D_DEVICE->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		D3D_DEVICE->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);


		// �f�o�b�O���\��
		if(CManager::GetCamera()->GetCameraMode())
		{
			for(int i = 0 ; i < (int)m_Spline.Pos.size() ; i++)
			{
				CDebugProc::DebugProc("�X�v���C�����W[%d]:(%5.2f:%5.2f:%5.2f)\n", i, m_Spline.Pos[i].x, m_Spline.Pos[i].y, m_Spline.Pos[i].z);
			}
		}
	}
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
	FILE	*fp = NULL;					// �t�@�C���|�C���^
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
	FILE	*fp = NULL;	// �t�@�C���|�C���^

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

			D3DXVECTOR3 pos;
			fscanf(fp, " = %f %f %f\n", &pos.x, &pos.y, &pos.z);
			m_Spline.Pos.push_back(pos);
			m_Spline.ifHold.push_back(false);
		}
		else if(strcmp(str, "VEC") == 0)
		{// �X�v���C������x�N�g����ǉ�
			
			D3DXVECTOR3 vec;
			fscanf(fp, " = %f %f %f\n", &vec.x, &vec.y, &vec.z);
			m_Spline.Vec.push_back(vec);
		}
		else if(strcmp(str, "END_SCRIPT") == 0)
		{// �X�v���C���ǂݍ��݂��I��

			break;
		}
	}

	// �G���~�[�g���W��ǉ�
	m_Spline.PosHermite.resize((m_Spline.Pos.size() - 1) * RAILLINE_SET);

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
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if((int)m_Spline.Pos.size() >= 2)
	{
		for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++, t += (1.0f / RAILLINE_SET))
		{
			float nowt = (t - ((int)t));

			m_Spline.PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[(int)t + 1].x)
								+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[(int)t + 1].x);
			m_Spline.PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline.Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline.Pos[(int)t + 1].z)
								+ (pow((1 - nowt), 2) * nowt * m_Spline.Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline.Vec[(int)t + 1].z);	
			m_Spline.PosHermite[i].y = 1.0f;
		}
	}

	
	for(int i = 0 ; i < (int)m_Spline.PosHermite.size() ; i++, t += (1.0f / RAILLINE_SET))
	{
		float nowt = (t - ((int)t));

		if((int)t == 0)
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t + 1].x - m_Spline.PosHermite[(int)t].x), 0.0f, (m_Spline.PosHermite[(int)t + 1].z - m_Spline.PosHermite[(int)t].z));
		}
		else if(i == (RAILLINE_SET - 1))
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t].x - m_Spline.PosHermite[(int)t - 1].x), 0.0f, (m_Spline.PosHermite[(int)t].z - m_Spline.PosHermite[(int)t - 1].z));
		}
		else
		{
			vec = D3DXVECTOR3((m_Spline.PosHermite[(int)t + 1].x - m_Spline.PosHermite[(int)t - 1].x), 0.0f, (m_Spline.PosHermite[(int)t + 1].z - m_Spline.PosHermite[(int)t - 1].z));
		}
		D3DXVec3Normalize(&vec, &vec);
		vec *= RAILLINE_MARGIN;

		if(line > 0)
		{
			m_Spline.PosHermite[i].x -= cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline.PosHermite[i].z -= sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}
		else if(line < 0)
		{
			m_Spline.PosHermite[i].x += cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline.PosHermite[i].z += sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}

		t += (1.0f / RAILLINE_SET);
	}
}