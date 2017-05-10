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
//	�֐���	:CSceneX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneXDX::CSceneXDX(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneDX(ifListAdd, priority, objtype)
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
void CSceneXDX::Init(cchar* fileName, MODELSTATUS* mesh, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	if(mesh)
	{
		m_ModelStatus = mesh;
	}
	else
	{
		m_ModelStatus = new MODELSTATUS;
		LoadModel(fileName, m_ModelStatus);
	}
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSceneXDX::Uninit(void)
{
	/*
	SafetyRelease(m_ModelStatus->pBuffMat);
	SafetyRelease(m_ModelStatus->pMesh);*/
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
	D3DXMATERIAL	*pMat = NULL;		// �}�e���A��
	D3DMATERIAL9	matDef;				// �f�t�H���g�̃}�e���A��


	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// Z�e�X�g�J�n
	CRendererDX::EnableZTest();

	// �A���t�@�e�X�g�J�n
	CRendererDX::EnableAlphaTest();

	// �`�揈��
	D3D_DEVICE->GetMaterial(&matDef);	// ���݂̃}�e���A�����擾

	// �}�e���A���ϊ�
	pMat = (D3DXMATERIAL *)m_ModelStatus->pBuffMat->GetBufferPointer();

	// 3D���f���`��
	for(int i = 0 ; i < (int)m_ModelStatus->NumMat ; i++)
	{
		// �}�e���A���Z�b�g
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���ǂݍ���
		if(pMat[i].pTextureFilename)
		{// �e�N�X�`���L��

			// ���X�g���瓯���̃e�N�X�`����T�����A�Z�b�g
			for each(TEXTURE list in m_ModelStatus->Tex)
			{
				if(list.FileName == CharPToString(pMat[i].pTextureFilename))
				{
					D3D_DEVICE->SetTexture(0, list.pTexture);
				}
			}
		}
		else
		{// �e�N�X�`������

		 // �e�N�X�`�����Z�b�g���Ȃ�
			D3D_DEVICE->SetTexture(0, NULL);
		}

		// ���f���`��
		m_ModelStatus->pMesh->DrawSubset(i);
	}

	// �}�e���A�������ɖ߂�
	D3D_DEVICE->SetMaterial(&matDef);

	// �A���t�@�e�X�g�I��
	CRendererDX::DisableAlphaTest();

	// Z�e�X�g�I��
	CRendererDX::DisableZTest();
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CSceneXDX *CSceneXDX::Create(cchar* fileName, MODELSTATUS* mesh, const D3DXVECTOR3 pos, const D3DXVECTOR3 rot)
{
	CSceneXDX *instance;

	instance = new CSceneXDX(true);

	instance->Init(fileName, mesh, pos, rot);

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
void CSceneXDX::LoadModel(cchar* filename, MODELSTATUS* modelStatus)
{
	FILE *fp;	// �t�@�C���|�C���^

	// ����3D���f���t�@�C���̃t�@�C�������Ԉ���Ă����ꍇ�A�_�~�[�̃��f����ǂݍ��ށB
	if(fopen_s(&fp, filename, "r") == NULL)
	{// �t�@�C����������
		fclose(fp);
		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&modelStatus->pBuffMat, NULL, &modelStatus->NumMat, &modelStatus->pMesh);
	}
	else
	{// �w�肵���t�@�C�������݂��Ă��Ȃ�
		D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&modelStatus->pBuffMat, NULL, &modelStatus->NumMat, &modelStatus->pMesh);
	}

	AutomaticSetTexture(modelStatus);
}

//=============================================================================
//	�֐���	:AutomaticSetTexture
//	����	:����
//	�߂�l	:����
//	����	:�}�e���A������莩���Ńe�N�X�`����ǉ�����B
//=============================================================================
void CSceneXDX::AutomaticSetTexture(MODELSTATUS* modelStatus)
{
	D3DXMATERIAL* pMat = NULL;	// �}�e���A��

	// �}�e���A���ϊ�
	pMat = (D3DXMATERIAL *)modelStatus->pBuffMat->GetBufferPointer();

	// �v���C���[�`��
	for(int i = 0 ; i < (int)modelStatus->NumMat ; i++)
	{
		// �e�N�X�`���ǂݍ���
		if(pMat[i].pTextureFilename)
		{
			AddTexture(modelStatus->Tex, pMat[i].pTextureFilename);
		}
	}
}

//=============================================================================
//	�֐���	:AddTexture
//	����	:����
//	�߂�l	:����
//	����	:�e�N�X�`����ǉ�����B
//=============================================================================
void CSceneXDX::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	char optional[1024] = ".\\";
	string fName;
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);

	if(fileName[0] != '.')
	{
		fName = strcat(optional, fileName);
	}
	else
	{
		fName.append(fileName);
	}

	D3DXCreateTextureFromFile(D3D_DEVICE, fName.c_str(), &texture[texture.size() - 1].pTexture);
}