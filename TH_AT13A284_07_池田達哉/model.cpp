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
#include "model.h"
#include "main.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================

//=============================================================================
//	�֐���	:CModel()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CModel::CModel(bool ifListAdd, int priority, OBJTYPE objtype) : CSceneXDX(ifListAdd, priority, objtype)
{
	D3DXMatrixIdentity(&m_mtxWorld);
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
void CModel::Init(string fileDir, string filename)
{
	// �e�평��������
	m_FileDir = fileDir;
	m_Parent	= NULL;
	m_PosDef	= VEC3_ZERO;
	m_RotDef	= VEC3_ZERO;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;

	m_MorphCount	= 0;
	m_MorphStatus	= 0;

	// ���f���ǂݍ���
	LoadModel(m_FileDir, filename);
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CModel::Uninit(void)
{
	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		SafetyRelease(m_ModelStatus[i].pMesh);
		SafetyRelease(m_ModelStatus[i].pBuffMat);
	}
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


	// �e�I�u�W�F�N�g�̃��[���h�}�g���b�N�X�擾
	if(m_Parent != NULL)
	{
		parentMatrix = *m_Parent->GetWMatrix();
	}
	else
	{
		D3D_DEVICE->GetTransform(D3DTS_WORLD, &parentMatrix);
	}
	
	// �}�g���b�N�X������
	D3DXMatrixIdentity(&m_mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, m_Scl.x, m_Scl.y, m_Scl.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_RotDef.y + m_Rot.y, m_RotDef.x + m_Rot.x, m_RotDef.z + m_Rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, m_PosDef.x + m_Pos.x, m_PosDef.y + m_Pos.y, m_PosDef.z + m_Pos.z);
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
	pMat = (D3DXMATERIAL *)m_ModelStatus[m_MorphStatus].pBuffMat->GetBufferPointer();

	// 3D���f���`��
	for(int i = 0 ; i < (int)m_ModelStatus[m_MorphStatus].NumMat ; i++)
	{
		// �}�e���A���Z�b�g
		D3D_DEVICE->SetMaterial(&pMat[i].MatD3D);

		// �e�N�X�`���ǂݍ���
		if(pMat[i].pTextureFilename)
		{// �e�N�X�`���L��

		 // ���X�g���瓯���̃e�N�X�`����T�����A�Z�b�g
			for each(TEXTURE list in m_pTexture)
			{
				if(list.FileName == pMat[i].pTextureFilename)
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
		m_ModelStatus[m_MorphStatus].pMesh->DrawSubset(i);
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
CModel *CModel::Create(string fileDir, string filename)
{
	CModel *model;	// �C���X�^���X

	// �C���X�^���X����
	model = new CModel();

	// ����������
	model->Init(fileDir, filename);

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
void CModel::LoadModel(string fileDir, string filename)
{
	FILE *fp;	// �t�@�C���|�C���^
	string str = ".\\data\\MODEL\\";
	str += fileDir;
	string morphFile = str + "\\morph.txt";

	fopen_s(&fp, morphFile.c_str(), "r");

	// END_SCRIPT�܂œǂݍ���
	while(!feof(fp))
	{
		string word;
		char wordBuff[1024] = { NULL };

		// �P����擾
		fscanf(fp, "%s", wordBuff);
		word += wordBuff;

		if(word == "MORPH_COUNT")
		{// ���f������ǂݍ���

			m_ModelStatus.push_back(MODELSTATUS_MORPH());
			fscanf(fp, " = %d", &m_ModelStatus[m_ModelStatus.size() - 1].MorphTime);
		}
	}

	fclose(fp);


	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		string strbuff = str;

		strbuff += "\\";
		strbuff += to_string(i);
		strbuff += "\\";
		strbuff += filename;
		// ����3D���f���t�@�C���̃t�@�C�������Ԉ���Ă����ꍇ�A�_�~�[�̃��f����ǂݍ��ށB
		if(fopen_s(&fp, strbuff.c_str(), "r") == NULL)
		{// �t�@�C����������
			fclose(fp);
			D3DXLoadMeshFromX(strbuff.c_str(), D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
		else
		{// �w�肵���t�@�C�������݂��Ă��Ȃ�
			D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
	}

	AutomaticSetTexture();
}

//=============================================================================
//	�֐���	:AutomaticSetTexture
//	����	:����
//	�߂�l	:����
//	����	:�}�e���A������莩���Ńe�N�X�`����ǉ�����B
//=============================================================================
void CModel::AutomaticSetTexture(void)
{
	D3DXMATERIAL	*pMat = NULL;	// �}�e���A��

	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		// �}�e���A���ϊ�
		pMat = (D3DXMATERIAL *)m_ModelStatus[i].pBuffMat->GetBufferPointer();

		// �v���C���[�`��
		for(int i = 0 ; i < (int)m_ModelStatus[i].NumMat ; i++)
		{
			// �e�N�X�`���ǂݍ���
			if(pMat[i].pTextureFilename)
			{
				// �e�N�X�`���ǉ�
				AddTexture(m_pTexture, pMat[i].pTextureFilename);
			}
		}
	}
}

//=============================================================================
//	�֐���	:AddTexture
//	����	:����
//	�߂�l	:����
//	����	:�e�N�X�`����ǉ�����B
//=============================================================================
void CModel::AddTexture(vector<TEXTURE> &texture, string fileName)
{
	string optional = ".\\data\\MODEL\\";
	optional += m_FileDir;
	optional += "\\";
	optional += "TEXTURE\\";
	optional += fileName;
	TEXTURE tex = { fileName, NULL };
	texture.push_back(tex);
	const char* file = optional.c_str();
	string texname = "./data/MODEL/sample/TEXTURE/side01.png";

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(D3D_DEVICE, fileName.c_str(), &texture[texture.size() - 1].pTexture);
}

string CModel::to_string(int val)
{
	char buffer[numeric_limits<int>::digits10 + 2]; // '-' + NULL
	sprintf(buffer, "%d", val);
	return buffer;
}