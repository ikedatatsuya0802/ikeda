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
CRail::CRail(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
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
	VERTEX_3D			*pVtx;										// 3D���_���
	
	char			*str	= NULL;	// �t�@�C�����e�i�[�z��
	unsigned int	offset	= 0;	// ������w��q

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_RailLine = line;
	
	//LoadSpline(line);
	m_Spline = CGame::GetRailLine()->GetSpline();
	CalcSpline(line);

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * (m_Spline->PosHermite.size() * 2 + 2)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// �e�N�X�`���̃��[�h
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"RAIL_TEXFILENAME000, &m_pTexture);
	
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float rot = 0.0f;
		//D3DXVECTOR3 a[RAIL_VERTEX];

		// �`����W�ݒ�
		rot = atan2f((m_Spline->PosHermite[1].x - m_Spline->PosHermite[0].x), (m_Spline->PosHermite[1].z - m_Spline->PosHermite[0].z));
		pVtx[0].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x + cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
		pVtx[1].Pos = D3DXVECTOR3(m_Spline->PosHermite[0].x - cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, m_Spline->PosHermite[0].z + sinf(rot) * (RAIL_WIDTH * 0.5f));
		for(int nCntInit = 1 ; nCntInit < ((m_Spline->nNum - 1) * RAIL_SET) ; nCntInit++)
		{
			// �p�x�ݒ�
			if(nCntInit != (m_Spline->PosHermite.size() - 1))
			{
				rot = atan2f((m_Spline->PosHermite[nCntInit + 1].x - m_Spline->PosHermite[nCntInit].x), (m_Spline->PosHermite[nCntInit + 1].z - m_Spline->PosHermite[nCntInit].z));
			}
			else
			{
				rot = atan2f((m_Spline->PosHermite[nCntInit].x - m_Spline->PosHermite[nCntInit - 1].x), (m_Spline->PosHermite[nCntInit].z - m_Spline->PosHermite[nCntInit - 1].z));
			}

			pVtx[nCntInit * 2 + 0].Pos = D3DXVECTOR3(m_Spline->PosHermite[nCntInit].x + (cosf(rot) * (RAIL_WIDTH * 0.5f)),
													1.0f,
													m_Spline->PosHermite[nCntInit].z - sinf(rot) * (RAIL_WIDTH * 0.5f));
			pVtx[nCntInit * 2 + 1].Pos = D3DXVECTOR3(m_Spline->PosHermite[nCntInit].x - cosf(rot) * (RAIL_WIDTH * 0.5f),
													1.0f,
													m_Spline->PosHermite[nCntInit].z + sinf(rot) * (RAIL_WIDTH * 0.5f));

			//a[nCntInit * 2 + 0] = D3DXVECTOR3(cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, sinf(rot) * (RAIL_WIDTH * 0.5f));
			//a[nCntInit * 2 + 1] = D3DXVECTOR3(-cosf(rot) * (RAIL_WIDTH * 0.5f), 1.0f, -sinf(rot) * (RAIL_WIDTH * 0.5f));
		}
	
		for(int nCntInit = 0 ; nCntInit < ((int)m_Spline->PosHermite.size() * 2 + 2) ; nCntInit++)
		{
			// �@���ݒ�
			pVtx[nCntInit].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[nCntInit].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}
	
		// �e�N�X�`���\�t���W�ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		for(int nCntInit = 1 ; nCntInit < (int)m_Spline->PosHermite.size() ; nCntInit++)
		{
			pVtx[nCntInit * 2 + 0].tex = D3DXVECTOR2(0.0f, (float)nCntInit * 2.0f);
			pVtx[nCntInit * 2 + 1].tex = D3DXVECTOR2(1.0f, (float)nCntInit * 2.0f);
		}

	}
	m_pVtxBuff->Unlock();

	Load();
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
	/*static float t = 0.0f;
	D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	float nowt = (t - ((int)t));

	pos.x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].x)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].x);
	pos.z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].z)
						+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].z);	
	pos.y = 20.0f;

	CManager::GetCamera()->SetCameraPos(pos, D3DXVECTOR3(0.0f, pos.y + 100.0f, pos.z - 200.0f));


	t += (1.0f / RAIL_SET) * 0.1f;
	if(t > (m_Spline->nNum - 1))
	{
		t -= m_Spline->nNum;
	}*/
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CRail::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �`�揈��
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, (((m_Spline->nNum - 1) * RAIL_SET) * 2));	// �`��

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);

	// �f�o�b�O���\��
#ifdef _DEBUG
	for(int i = 0 ; i < m_Spline->nNum ; i++)
	{
		CDebugProc::DebugProc("�X�v���C�����W[%d]:(%5.2f:%5.2f:%5.2f)\n", i, m_Spline->Pos[i].x, m_Spline->Pos[i].y, m_Spline->Pos[i].z);
	}
#endif
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

//=============================================================================
//	�֐���	:LoadSpline
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�Ȃ�
//=============================================================================
void CRail::LoadSpline(int line)
{/*
	FILE	*fp = NULL;	// �t�@�C���|�C���^

	fp = fopen("./data/spline.txt", "r");

	// �t�@�C���I���܂œǂݍ���
	while(feof(fp) != EOF)
	{
		char str[65535] = {NULL};
		memset(str, NULL, sizeof(str));

		fscanf(fp, "%s", str);

		if(strcmp(str, "NUM_POSITION") == 0)
		{
			fscanf(fp, " = %d", &m_Spline->nNum);

			if(m_Spline->nNum > 0)
			{
				m_Spline->PosHermite = new D3DXVECTOR3[(m_Spline->nNum - 1) * RAIL_SET];
				m_Spline->Pos = new D3DXVECTOR3[m_Spline->nNum];
				m_Spline->Vec = new D3DXVECTOR3[m_Spline->nNum];
			}
		}
		else if(strcmp(str, "POS") == 0)
		{
			// �G���~�[�g���W�ǂݎ��
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "POS = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Pos[i].x, &m_Spline->Pos[i].y, &m_Spline->Pos[i].z) == EOF)
					{
						m_Spline->Pos[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}
		}
		else if(strcmp(str, "VEC") == 0)
		{
			// �G���~�[�g�x�N�g���ǂݎ��
			for(int i = 0 ; i < m_Spline->nNum ; i++)
			{
				if(i > 0)
				{
					if(fscanf(fp, "VEC = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
				else
				{
					if(fscanf(fp, " = %f %f %f\n", &m_Spline->Vec[i].x, &m_Spline->Vec[i].y, &m_Spline->Vec[i].z) == EOF)
					{
						m_Spline->Vec[i] = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
					}
				}
			}

			// �ǂݍ��ݏI��
			break;
		}
	}

	fclose(fp);*/
}

//=============================================================================
//	�֐���	:CalcBezier
//	����	:�Ȃ�
//	�߂�l	:�Ȃ�
//	����	:�Ȃ�
//=============================================================================
void CRail::CalcSpline(int line)
{
	float t		= 0.0f;
	float rot	= 0.0f;
	D3DXVECTOR3 vec = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	
	if(m_Spline->nNum >= 2)
	{
		for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
		{
			float nowt = (t - ((int)t));

			m_Spline->PosHermite[i].x = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].x) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].x)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].x) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].x);
			m_Spline->PosHermite[i].z = (pow((nowt - 1), 2) * (2 * nowt + 1) * m_Spline->Pos[(int)t + 0].z) + (powf(nowt, 2) * (3 - 2 * nowt) * m_Spline->Pos[(int)t + 1].z)
								+ (pow((1 - nowt), 2) * nowt * m_Spline->Vec[(int)t + 0].z) + ((nowt - 1) * powf(nowt, 2) * m_Spline->Vec[(int)t + 1].z);	
			m_Spline->PosHermite[i].y = 1.0f;
		}
	}

	
	for(int i = 0 ; i < (int)m_Spline->PosHermite.size() ; i++, t += (1.0f / RAIL_SET))
	{
		float nowt = (t - ((int)t));

		if((int)t == 0)
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t].z));
		}
		else if(i == (RAIL_SET - 1))
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		else
		{
			vec = D3DXVECTOR3((m_Spline->PosHermite[(int)t + 1].x - m_Spline->PosHermite[(int)t - 1].x), 0.0f, (m_Spline->PosHermite[(int)t + 1].z - m_Spline->PosHermite[(int)t - 1].z));
		}
		D3DXVec3Normalize(&vec, &vec);
		vec *= RAIL_MARGIN;

		if(line > 0)
		{
			m_Spline->PosHermite[i].x -= cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z -= sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}
		else if(line < 0)
		{
			m_Spline->PosHermite[i].x += cosf(D3DX_PI * 0.5f) * (line * vec.x) - sinf(D3DX_PI * 0.5f) * (line * vec.z);
			m_Spline->PosHermite[i].z += sinf(D3DX_PI * 0.5f) * (line * vec.x) + cosf(D3DX_PI * 0.5f) * (line * vec.z);
		}

		t += (1.0f / RAIL_SET);
	}
}