//=============================================================================
//
//	�^�C�g��	�v���C���[
//	�t�@�C����	model.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/14
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include <stdio.h>
#include "model.h"
#include "main.h"
#include "player.h"
#include "cameraDX.h"
#include "rendererDX.h"
#include "scene3DDX.h"
#include "meshfield.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
//	�֐���	:CModel()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CModel::CModel(int priority)
{

}

//=============================================================================
//	�֐���	:~CModel()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CModel::~CModel()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�������������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
void CModel::Init(char *filename, D3DXVECTOR3 pos)
{
	// �e�평��������
	m_Parent	= NULL;
	m_PosDef	= D3DXVECTOR3(pos.x, pos.y, pos.z);
	m_RotDef	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Pos		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	D3DXMatrixIdentity(&m_mtxWorld);

	// ���f���ǂݍ���
	LoadModel(filename);

	AutomaticSetTexture();

	/*
	AddTexture(m_Texture, "hakatanozomi.bmp");
	AddTexture(m_Texture, "n700.bmp");
	AddTexture(m_Texture, "n700mado.bmp");
	AddTexture(m_Texture, "n700rogo.bmp");
	AddTexture(m_Texture, "n700yane.bmp");
	AddTexture(m_Texture, "ta.bmp");*/
}

void CModel::AutomaticSetTexture(void)
{
	D3DXMATERIAL	*pMat = NULL;	// �}�e���A��

	// �}�e���A���ϊ�
	pMat = (D3DXMATERIAL *)m_ModelStatus.pBuffMat->GetBufferPointer();

	// �v���C���[�`��
	for(int i = 0 ; i < (int)m_ModelStatus.NumMat ; i++)
	{
		// �}�e���A���Z�b�g
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���ǂݍ���
		if(pMat[i].pTextureFilename)
		{
			AddTexture(m_Texture, pMat[i].pTextureFilename);
		}
	}
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CModel::AddTexture(vector<TEXTURE> &texture, char* fileName)
{
	char optional[] = "./data/MODEL/n700";
	char* fName = strcat(optional, fileName);
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);


	D3DXCreateTextureFromFile(D3D_DEVICE, fName, &texture[texture.size() - 1].pTexture);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CModel::Uninit(void)
{
	SafetyRelease(m_ModelStatus.pMesh);
	SafetyRelease(m_ModelStatus.pBuffMat);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CModel::Update(void)
{

}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CModel::Draw(void)
{
	D3DXMATRIX		mtxView, mtxScl, mtxRot, mtxTrans;			// �}�g���b�N�X
	D3DXMATRIX		parentMatrix;								// �e�I�u�W�F�N�g�̃}�g���b�N�X
	D3DXMATERIAL	*pMat		= NULL;							// �}�e���A��
	D3DMATERIAL9	matDef;										// �f�t�H���g�̃}�e���A��

	
	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, 1.0f, 1.0f, 1.0f);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	if(m_Parent != NULL)
	{
		parentMatrix = m_Parent->GetWorldMatrix();
	}
	else
	{
		D3D_DEVICE->GetTransform(D3DTS_WORLD, &parentMatrix);
	}

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotDef.y, m_RotDef.x, m_RotDef.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	D3DXMatrixIdentity(&mtxRot);
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_Rot.y, m_Rot.x, m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_PosDef.x, m_PosDef.y, m_PosDef.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	D3DXMatrixIdentity(&mtxTrans);
	D3DXMatrixTranslation(&mtxTrans, m_Pos.x, m_Pos.y, m_Pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &parentMatrix);
	D3D_DEVICE->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	
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
		{// �e�N�X�`���L��

			D3D_DEVICE->SetTexture(0, NULL);
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
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CModel *CModel::Create(char *filename, D3DXVECTOR3 pos)
{
	CModel *model;	// �C���X�^���X

	// �C���X�^���X����
	model = new CModel;

	// ����������
	model->Init(filename, pos);

	// �C���X�^���X��Ԃ�
	return model;
}

//=============================================================================
//	�֐���	:LoadModel
//	����	:char *filename -> �t�@�C����
//			:LPDIRECT3DDEVICE9 D3D_DEVICE -> 3D�f�o�C�X
//			:MODELSTATUS ms -> 3D���f���̊e����
//	�߂�l	:����
//	����	:���f����ǂݍ��ށB�G���[���t���B
//=============================================================================
void CModel::LoadModel(char *filename)
{
	FILE *fp;	// �t�@�C���|�C���^


	// ����3D���f���t�@�C���̃t�@�C�������Ԉ���Ă����ꍇ�A�_�~�[�̃��f����ǂݍ��ށB
	if(fopen_s(&fp, filename, "r") == NULL)
	{// �t�@�C����������
		fclose(fp);
		D3DXLoadMeshFromX(filename, D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}
	else
	{// �w�肵���t�@�C�������݂��Ă��Ȃ�
		D3DXLoadMeshFromX("data/MODEL/dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
			&m_ModelStatus.pBuffMat, NULL, &m_ModelStatus.NumMat, &m_ModelStatus.pMesh);
	}

}