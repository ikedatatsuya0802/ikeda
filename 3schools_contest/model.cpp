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
#include "manager.h"

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
	D3DMATERIAL9	mtrl;	// �}�e���A���I�u�W�F�N�g

	// �e�평��������
	m_FileDir = fileDir;
	m_Pos		= VEC3_ZERO;
	m_Rot		= VEC3_ZERO;

	m_MorphCountAll = 0;
	m_MorphCount	= 0;
	m_MorphStatus	= 0;

	D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);			// �y�o�b�t�@�L����
	D3D_DEVICE->SetRenderState(D3DRS_AMBIENT, 0x00303030);	// �����̐ݒ�

	// �}�e���A���̐ݒ�
	ZeroMemory(&mtrl, sizeof(mtrl));			// ��������[���ŃN���A
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0;
	mtrl.Ambient.r = mtrl.Ambient.g = mtrl.Ambient.b = mtrl.Ambient.a = 1.0;
	D3D_DEVICE->SetMaterial(&mtrl);

	// ���f���ǂݍ���
	LoadModel(m_FileDir, filename);

	// �J�E���g�������v��
	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		m_MorphCountAll += m_ModelStatus[i].MorphTime;
	}
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

	SafetyRelease(m_pTexture);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CModel::Update(void)
{
	MODELSTATUS_MORPH*		pt;	// �I�u�W�F�N�g�ւ̃|�C���^
	pt = &m_ModelStatus[0];
	int nextArray = 0;	// ���̃��[�t�B���O�z��

	VERTEX		*spt0, *spt1, *dpt;					// �e���_�f�[�^�ւ̃|�C���^
	float		ratio;								// ���Ԃ��狁�߂��ό`�̊���

	//�ό`�O�|�C���^���擾
	spt0 = pt[m_MorphStatus].pt_vertex;
	//�ό`��|�C���^���擾
	nextArray = (m_MorphStatus + 1) % m_ModelStatus.size();
	spt1 = pt[nextArray].pt_vertex;
	//���[�t�B���O���|�C���^���擾
	dpt = m_MorphVertex;

	// ���������߂�
	ratio = (float)m_MorphCount / (float)pt[nextArray].MorphTime;

	// �Q�̃f�[�^������W��ۊǂ��ă��[�t�B���O���f�[�^�Ƃ��Ċi�[
	//���ꂼ��̃|�C���^���P�����߂�
	for(int i = 0 ; i < pt[0].num_vertex ; i++, spt0++, spt1++, dpt++)
	{
		dpt->x = (spt1->x - spt0->x) * ratio + spt0->x;
		dpt->y = (spt1->y - spt0->y) * ratio + spt0->y;
		dpt->z = (spt1->z - spt0->z) * ratio + spt0->z;
	}

	m_MorphCount++;
	if(m_MorphCount >= pt[nextArray].MorphTime)
	{
		m_MorphStatus = (m_MorphStatus + 1) % m_ModelStatus.size();
		m_MorphCount = 0;
	}
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CModel::Draw(void)
{
	D3D_DEVICE->SetFVF(FVF_VERTEX);		// ���_�t�H�[�}�b�g�w��
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos);

	// �e�N�X�`���̃Z�b�g
	D3D_DEVICE->SetTexture(0, m_pTexture);

	// ���[�t�B���O���f����`��
	if(m_ModelStatus.size() > 0)
	{
		D3D_DEVICE->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST,
			0, m_ModelStatus[0].num_vertex, m_ModelStatus[0].num_face, m_ModelStatus[0].pt_index,
			D3DFMT_INDEX16, m_MorphVertex, sizeof(VERTEX));
	}

#ifdef _DEBUG
	CDebugProc::DebugProc("FRAME:%d, Key:%d\n", m_MorphCount, m_MorphStatus);
	CDebugProc::DebugProc("TIME:%.2fsec", (m_MorphCountAll / 60.0f));
#endif
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
	FILE		*fp;	// �t�@�C���|�C���^
	string str = ".\\data\\MODEL\\";
	str += fileDir;
	string morphFile = str + "morph.txt";

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

	LPDIRECT3DVERTEXBUFFER9	vertex_buffer;	// ���_�o�b�t�@
	LPDIRECT3DINDEXBUFFER9	index_buffer;	// �C���f�b�N�X�o�b�t�@
	VERTEX*		pVertices;		// ���_�f�[�^�ւ̃|�C���^
	WORD*		pIndices;		// �|���S���\���i���_�����N�j�f�[�^�ւ̃|�C���^

	for(int i = 0 ; i < (int)m_ModelStatus.size() ; i++)
	{
		string strbuff = str;

		strbuff += filename;
		strbuff += to_string(i);
		strbuff += ".x";
		// ����3D���f���t�@�C���̃t�@�C�������Ԉ���Ă����ꍇ�A�_�~�[�̃��f����ǂݍ��ށB
		if(fopen_s(&fp, strbuff.c_str(), "r") == NULL)
		{// �t�@�C����������
			fclose(fp);
			D3DXLoadMeshFromX(strbuff.c_str(), D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);

			m_ModelStatus[i].pMesh->GetVertexBuffer(&vertex_buffer);	// ���_�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
			m_ModelStatus[i].pMesh->GetIndexBuffer(&index_buffer);		// �C���f�b�N�X�o�b�t�@�I�u�W�F�N�g�ւ̃|�C���^���Q�b�g
			m_ModelStatus[i].num_vertex = m_ModelStatus[i].pMesh->GetNumVertices();	// ���_�����Q�b�g
			m_ModelStatus[i].num_face = m_ModelStatus[i].pMesh->GetNumFaces();		// �ʐ����Q�b�g

			// ���_�f�[�^�A�C���f�b�N�X�f�[�^���������ɃR�s�[
			m_ModelStatus[i].pt_vertex = new VERTEX[m_ModelStatus[i].num_vertex];	// ���_���[�N�̈���m��
			m_ModelStatus[i].pt_index = new WORD[m_ModelStatus[0].num_face * 3];	// �C���f�b�N�X���[�N�̈���m��

			vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);											// ���_�o�b�t�@�����b�N
			memcpy(m_ModelStatus[i].pt_vertex, pVertices, sizeof(VERTEX)*m_ModelStatus[0].num_vertex);	// ���_�f�[�^�����[�N�ɃR�s�[
			vertex_buffer->Unlock();																	// ���_�o�b�t�@���A�����b�N

			index_buffer->Lock(0, 0, (void**)&pIndices, 0);												// �C���f�b�N�X�o�b�t�@�����b�N
			memcpy(m_ModelStatus[i].pt_index, pIndices, sizeof(WORD)*m_ModelStatus[0].num_face * 3);	// �C���f�b�N�X�f�[�^�����[�N�ɃR�s�[
			index_buffer->Unlock();																		// �C���f�b�N�X�o�b�t�@���A�����b�N
		}
		else
		{// �w�肵���t�@�C�������݂��Ă��Ȃ�
			D3DXLoadMeshFromX(".\\data\\MODEL\\dummy.x", D3DXMESH_SYSTEMMEM, D3D_DEVICE, NULL,
				&m_ModelStatus[i].pBuffMat, NULL, &m_ModelStatus[i].NumMat, &m_ModelStatus[i].pMesh);
		}
	}

	// ���[�t�B���O�p�ɒ��_�o�b�t�@�A�C���f�b�N�X�o�b�t�@���쐬
	m_MorphVertex = new VERTEX[m_ModelStatus[0].num_vertex];		// ���[�t�B���O�p���_���[�N�̈���m��
	m_MorphIndex = new WORD[m_ModelStatus[0].num_face * 3];			// ���[�t�B���O�p�C���f�b�N�X���[�N�̈���m��

	vertex_buffer->Lock(0, 0, (void**)&pVertices, 0);								// ���_�o�b�t�@�����b�N
	memcpy(m_MorphVertex, pVertices, sizeof(VERTEX)*m_ModelStatus[0].num_vertex);	// ���_�f�[�^�����[�N�ɃR�s�[
	vertex_buffer->Unlock();														// ���_�o�b�t�@���A�����b�N

	index_buffer->Lock(0, 0, (void**)&pIndices, 0);								// �C���f�b�N�X�o�b�t�@�����b�N
	memcpy(m_MorphIndex, pIndices, sizeof(WORD)*m_ModelStatus[0].num_face * 3);	// �C���f�b�N�X�f�[�^�����[�N�ɃR�s�[
	index_buffer->Unlock();														// �C���f�b�N�X�o�b�t�@���A�����b�N

	// �e�N�X�`���ǂݍ���
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\MODEL\\tex.jpg", &m_pTexture);
}

string CModel::to_string(int val)
{
	char buffer[numeric_limits<int>::digits10 + 2]; // '-' + NULL
	sprintf(buffer, "%d", val);
	return buffer;
}