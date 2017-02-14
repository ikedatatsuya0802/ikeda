//=============================================================================
//
//	�^�C�g��	���b�V���t�B�[���h
//	�t�@�C����	skydome.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/05/10
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "skydome.h"
#include "cameraDX.h"
#include "manager.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CSkydome::m_pTexture;

//=============================================================================
//	�֐���	:CSkydome()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSkydome::CSkydome(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CSkydome()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSkydome::~CSkydome()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CSkydome::Init(bool ifLight, cVec3 pos, cVec3 rot)
{
	// �e�평��������
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(D3DXVECTOR3(rot.x, rot.y, rot.z));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer(((sizeof(VERTEX_3D) * SKYDOME_VERTEX_NUM)), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	
	// ���_�o�b�t�@�̐ݒ�
	SetVtxBuff(ifLight);

	// �C���f�b�N�X�Z�b�g
	SetMeshIndex(&m_pIdxBuff, SKYDOME_HORIZONTAL, SKYDOME_VERTICAL);

	Load();
}

//=============================================================================
//	�֐���	:SetVtxBuff
//	����	:����
//	�߂�l	:����
//	����	:���_�o�b�t�@�Ƀf�[�^���Z�b�g����B
//=============================================================================
void CSkydome::SetVtxBuff(bool ifLight)
{
	VERTEX_3D* pVtx;

	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	{
		float radius = 0.0f;
		float radianMax = atan2f((SKYDOME_HEIGHT * SKYDOME_VERTICAL), SKYDOME_RADIUS);

		// ���W�ݒ�
		for(int y = 0 ; y <= SKYDOME_VERTICAL ; y++)
		{
			radius = cosf(radianMax / SKYDOME_VERTICAL * (SKYDOME_VERTICAL - y)) * SKYDOME_RADIUS;

			for(int x = 0 ; x <= SKYDOME_HORIZONTAL ; x++)
			{
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.x
					= sinf(D3DX_PI * 2.0f / SKYDOME_HORIZONTAL * x) * radius;

				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.y
					= ((SKYDOME_HEIGHT * 1.0f * SKYDOME_VERTICAL)) - (SKYDOME_HEIGHT * y);

				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].Pos.z
					= cosf(D3DX_PI * 2.0f / SKYDOME_HORIZONTAL * x) * radius;
			}
		}

		// �@���ݒ�
		for(int i = 0 ; i < SKYDOME_VERTEX_NUM ; i++)
		{
			pVtx[i].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

			// �F�ݒ�
			pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}

		// �e�N�X�`���\�t���W�ݒ�
		for(int y = 0 ; y <= SKYDOME_VERTICAL ; y++)
		{
			for(int x = 0 ; x <= SKYDOME_HORIZONTAL ; x++)
			{
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].tex.x = (1.0f / SKYDOME_HORIZONTAL * x);
				pVtx[(y * (SKYDOME_HORIZONTAL + 1) + x)].tex.y = (1.0f / SKYDOME_VERTICAL * y);
			}
		}
	}
	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CSkydome::Uninit(void)
{
	SafetyRelease(m_pVtxBuff);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CSkydome::Update(void)
{
	// �J�������W�ɒǏ]
	m_Pos.x = DX_CAMERA->GetCameraPosV().x;
	m_Pos.z = DX_CAMERA->GetCameraPosV().z;

	m_Rot.y -= SKYDOME_MOVEROT;
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CSkydome::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

	// ���C�e�B���O�ݒ���I�t��
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Z�e�X�g�J�n
	CRendererDX::EnableZTest();

	// �`�揈��(�~����)
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetIndices(m_pIdxBuff);									// �C���f�b�N�X�o�b�t�@���o�C���h
	D3D_DEVICE->SetTexture(0, m_pTexture);								// �e�N�X�`���̐ݒ�

	// �h�[��(�~����)�`��
	D3D_DEVICE->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP, 0, 0, SKYDOME_VERTEX_NUM, 0, SKYDOME_POLYGON_NUM);

	// �`�揈��(�V�䕔)
	D3D_DEVICE->SetTexture(0, NULL);														// �e�N�X�`���̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLEFAN, SKYDOME_VERTEX_NUM, SKYDOME_HORIZONTAL);	// �`��

	// Z�e�X�g�I��
	CRendererDX::DisableZTest();

	// ���C�e�B���O�ݒ���I����
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
}

//=============================================================================
//	�֐���	:Create
//	����	:D3DXVECTOR3 pos(�����ʒu)
//	�߂�l	:����
//	����	:�C���X�^���X�������s���B
//=============================================================================
CSkydome *CSkydome::Create(bool ifLight, cVec3 pos, cVec3 rot)
{
	CSkydome *instance;

	instance = new CSkydome();

	instance->Init(ifLight, pos, rot);

	return instance;
}