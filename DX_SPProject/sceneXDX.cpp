//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��(X�t�@�C��)
//	�t�@�C����	sceneX.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "sceneXDX.h"
#include "manager.h"
#include "main.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CSceneXDX::m_Texture[MODEL_TEXTURENUM];
LPD3DXMESH			CSceneXDX::m_pMesh;
LPD3DXBUFFER		CSceneXDX::m_pBuffMat;
DWORD				CSceneXDX::m_NumMat;

//=============================================================================
//	�֐���	:CSceneX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneXDX::CSceneXDX(int priority, OBJTYPE objtype) : CScene3DDX(priority, objtype)
{
	D3DXMatrixIdentity(&m_mtxWorld);
	m_pMesh		= NULL;
	m_pBuffMat	= NULL;
	m_NumMat	= 0;
}

//=============================================================================
//	�֐���	:~CSceneX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSceneXDX::~CSceneXDX()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CSceneXDX::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9	pDevice = CRendererDX::GetDevice();			// 3D�f�o�C�X

	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	m_Move		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RotMove	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_nCntMove = 0;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSceneXDX::Uninit(void)
{
	if(m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	if(m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSceneXDX::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSceneXDX::Draw(void)
{
	LPDIRECT3DDEVICE9	pDevice		= CRendererDX::GetDevice();		// 3D�f�o�C�X
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;					// �}�g���b�N�X
	D3DXMATERIAL		*pMat		= NULL;							// �}�e���A��
	D3DMATERIAL9		matDef;										// �f�t�H���g�̃}�e���A��


	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
	// �A���t�@�e�X�g�J�n
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 250);

	// �`�揈��
	pDevice->GetMaterial(&matDef);	// ���݂̃}�e���A�����擾

	// �}�e���A���ϊ�
	pMat = (D3DXMATERIAL *)m_pBuffMat->GetBufferPointer();	
	
	// �v���C���[�`��
	for(int nCntMat = 0 ; nCntMat < (int)m_NumMat ; nCntMat++)
	{
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);	// �}�e���A���Z�b�g

		// �e�N�X�`���ǂݍ���
		if(pMat[nCntMat].pTextureFilename)
		{
			if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player000.png") == 0)
			{// ���[���e�N�X�`��
				pDevice->SetTexture(0, m_Texture[0]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player001.png") == 0)
			{// ���[���e�N�X�`��
				pDevice->SetTexture(0, m_Texture[1]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player002.jpg") == 0)
			{// ���[���e�N�X�`��
				pDevice->SetTexture(0, m_Texture[2]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player003.jpg") == 0)
			{// ���[���e�N�X�`��
				pDevice->SetTexture(0, m_Texture[3]);
			}
			else
			{
				pDevice->SetTexture(0, NULL);
			}
		}
		else
		{// �e�N�X�`������
			pDevice->SetTexture(0, NULL);
		}

		m_pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�������ɖ߂�
	pDevice->SetMaterial(&matDef);

	// �A���t�@�e�X�g�I��
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	
	// �f�o�b�O���\��
#ifdef _DEBUG
	/*CDebugProc::DebugProc("�v���C���[���W:(%5.2f:%5.2f:%5.2f)\n", m_Pos.x, m_Pos.y, m_Pos.z);
	int i = mesh->GetFrontMesh(m_Pos);
	CDebugProc::DebugProc("����Ă���|���S��:(%2d:%2d:%2d)\n", i + 0, i + 1, i + 2);*/
#endif
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CSceneXDX *CSceneXDX::Create(D3DXVECTOR3 pos)
{
	CSceneXDX *sceneX;

	sceneX = new CSceneXDX;

	sceneX->Init(pos);

	return sceneX;
}