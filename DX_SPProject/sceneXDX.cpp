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
LPDIRECT3DTEXTURE9	CSceneXDX::m_pTexture[MODEL_TEXTURENUM];

//=============================================================================
//	�֐���	:CSceneX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneXDX::CSceneXDX(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{
	D3DXMatrixIdentity(&m_mtxWorld);
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
void CSceneXDX::Init(char* fileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	LoadModel(fileName);
}

//=============================================================================
//	�֐���	:Load
//	����	:����
//	�߂�l	:����
//	����	:�B
//=============================================================================
void CSceneXDX::Load(void)
{

}

//=============================================================================
//	�֐���	:Load
//	����	:����
//	�߂�l	:����
//	����	:�B
//=============================================================================
void CSceneXDX::Unload(void)
{

}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSceneXDX::Uninit(void)
{

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
	D3DXMATERIAL		*pMat		= NULL;		// �}�e���A��
	D3DMATERIAL9		matDef;					// �f�t�H���g�̃}�e���A��


	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	
	// �A���t�@�e�X�g�J�n
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 250);

	// �`�揈��
	D3D_DEVICE->GetMaterial(&matDef);	// ���݂̃}�e���A�����擾

	// �}�e���A���ϊ�
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();
	
	// �v���C���[�`��
	for(int nCntMat = 0 ; nCntMat < (int)m_ModelStatus.NumMat ; nCntMat++)
	{
		D3D_DEVICE->SetMaterial(&pMat[nCntMat].MatD3D);	// �}�e���A���Z�b�g

		// �e�N�X�`���ǂݍ���
		if(pMat[nCntMat].pTextureFilename)
		{
			if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player000.png") == 0)
			{// ���[���e�N�X�`��
				D3D_DEVICE->SetTexture(0, m_pTexture[0]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player001.png") == 0)
			{// ���[���e�N�X�`��
				D3D_DEVICE->SetTexture(0, m_pTexture[1]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player002.jpg") == 0)
			{// ���[���e�N�X�`��
				D3D_DEVICE->SetTexture(0, m_pTexture[2]);
			}
			else if(strcmp(pMat[nCntMat].pTextureFilename, "..\\data\\TEXTURE\\player003.jpg") == 0)
			{// ���[���e�N�X�`��
				D3D_DEVICE->SetTexture(0, m_pTexture[3]);
			}
			else
			{
				D3D_DEVICE->SetTexture(0, NULL);
			}
		}
		else
		{// �e�N�X�`������
			D3D_DEVICE->SetTexture(0, NULL);
		}

		m_ModelStatus.pMesh->DrawSubset(nCntMat);
	}

	// �}�e���A�������ɖ߂�
	D3D_DEVICE->SetMaterial(&matDef);

	// �A���t�@�e�X�g�I��
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);
	
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
CSceneXDX *CSceneXDX::Create(char* fileName, D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	CSceneXDX *instance;

	instance = new CSceneXDX(true);

	instance->Init(fileName, pos, rot);

	return instance;
}

//=============================================================================
//	�֐���	:LoadModel
//	����	:char *filename -> �t�@�C����
//			:LPDIRECT3DDEVICE9 D3D_DEVICE -> 3D�f�o�C�X
//			:MODELSTATUS ms -> 3D���f���̊e����
//	�߂�l	:����
//	����	:���f����ǂݍ��ށB�G���[���t���B
//=============================================================================
void CSceneXDX::LoadModel(char *filename)
{
	FILE *fp;	// �t�@�C���|�C���^
	char str[1024] = ".\\data\\MODEL\\";
	strcat(str, filename);

	// ����3D���f���t�@�C���̃t�@�C�������Ԉ���Ă����ꍇ�A�_�~�[�̃��f����ǂݍ��ށB
	if(fopen_s(&fp, str, "r") == NULL)
	{// �t�@�C����������
		fclose(fp);
		D3DXLoadMeshFromX(str, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}
	else
	{// �w�肵���t�@�C�������݂��Ă��Ȃ�
		D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}

}