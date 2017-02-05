//=============================================================================
//
//	�^�C�g��	���[���t�@�C��
//	�t�@�C����	goal.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "goal.h"
#include "manager.h"
#include "main.h"
#include "rendererDX.h"
#include "game.h"
#include "player.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
LPDIRECT3DTEXTURE9	CGoal::m_pTexture;

//=============================================================================
//	�֐���	:CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CGoal::CGoal(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	�֐���	:~CScene3D()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CGoal::~CGoal()
{

}

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CGoal::Init(void)
{	
	// �e�평��������
	m_Pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
	D3DXVec3Normalize(&m_Vec, &(CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f) - m_Pos));

	// ���_�o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[0], NULL);
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff[1], NULL);

	// ���[�����Z�b�g
	SetVtxBuff();

	Load();

	CSceneXDX::Create(".\\data\\MODEL\\ekisha.x", NULL,
		D3DXVECTOR3(CGame::GetPlayer1()->GetPos().x, 0.0f, CGame::GetPlayer1()->GetPos().z + 300.0f),
		D3DXVECTOR3(0.0, D3DX_PI, 0.0f));
	CSceneXDX::Create(".\\data\\MODEL\\ekisha.x", NULL, D3DXVECTOR3(m_Pos.x, 0.0f, m_Pos.z));
}

//=============================================================================
//	�֐���	:SetGoalData
//	����	:����
//	�߂�l	:����
//	����	:���[���̏����Z�b�g����B
//=============================================================================
void CGoal::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D���_���
	SPLINE*		spline = CGame::GetRailLine()->GetSpline();		// �X�v���C�����
	D3DXVECTOR3 posFar = VEC3_ZERO;
	float rot = 0.0f;

	posFar.x = m_Pos.x + m_Vec.x * RAIL_LENGTH;
	posFar.y = m_Pos.y;
	posFar.z = m_Pos.z + m_Vec.z * RAIL_LENGTH;

	rot = atan2f(m_Vec.x, m_Vec.z);

	m_pVtxBuff[0]->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos.x = posFar.x + (sinf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[0].Pos.y = 1.0f;
	pVtx[0].Pos.z = posFar.z + (cosf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[1].Pos.x = posFar.x + (sinf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[1].Pos.y = 1.0f;
	pVtx[1].Pos.z = posFar.z + (cosf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[2].Pos.x = m_Pos.x + (sinf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[2].Pos.y = 1.0f;
	pVtx[2].Pos.z = m_Pos.z + (cosf(rot - (D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	pVtx[3].Pos.x = m_Pos.x + (sinf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));
	pVtx[3].Pos.y = 1.0f;
	pVtx[3].Pos.z = m_Pos.z + (cosf((rot + D3DX_PI / 2)) * (RAIL_WIDTH * 0.5f));

	for(int nCntSet = 0 ; nCntSet < VERTEX_SQUARE ; nCntSet++)
	{
		// �@���ݒ�
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// �F�ݒ�
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`�����W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 100.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 100.0f);

	m_pVtxBuff[0]->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CGoal::Uninit(void)
{
	SafetyRelease(m_pVtxBuff[0]);
	SafetyRelease(m_pVtxBuff[1]);

	Unload();
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CGoal::Update(void)
{
	m_Pos = CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL);
	D3DXVec3Normalize(&m_Vec, &(CGame::GetRailLine()->GetSplinePos(RAILLINE_GOAL + 0.01f) - m_Pos));

	SetVtxBuff();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CGoal::Draw(void)
{
	// �}�g���b�N�X�ݒ�
	CRendererDX::SetMatrix(&m_mtxWorld, VEC3_ZERO, m_Rot);

	// �A���t�@�EZ�e�X�g�J�n
	CRendererDX::EnableAlphaTest();
	CRendererDX::EnableZTest();

	// �`�揈��
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);										// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff[0], 0, sizeof(VERTEX_3D));	// ���_�t�H�[�}�b�g�̐ݒ�
	D3D_DEVICE->SetTexture(0, m_pTexture);									// �e�N�X�`���̐ݒ�
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);	// �`��

	// �A���t�@�EZ�e�X�g�I��
	CRendererDX::DisableAlphaTest();
	CRendererDX::DisableZTest();
}

//=============================================================================
//	�֐���	:Create
//	����	:����
//	�߂�l	:����
//	����	:�C���X�^���X�������s���Ƌ��ɁA�����ʒu��ݒ肷��B
//=============================================================================
CGoal *CGoal::Create(void)
{
	CGoal *instance;

	instance = new CGoal;
	
	instance->Init();

	return instance;
}