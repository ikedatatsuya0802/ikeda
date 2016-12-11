//=============================================================================
//
//	�^�C�g��	�t�F�[�h����
//	�t�@�C����	fade.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/07/12
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "input.h"
#include "scene2DDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
CMode		*CFade::m_NextMode;
FADESTATUS	CFade::m_FadeState;
FADE		CFade::m_Fade[2];		// �t�F�[�h
LPDIRECT3DVERTEXBUFFER9	CFade::m_pVtxBuff;
LPDIRECT3DTEXTURE9		CFade::m_pTexture[2];

//=============================================================================
//	�֐���	:Init
//	����	:����
//	�߂�l	:����
//	����	:�������������s���B
//=============================================================================
void CFade::Init(void)
{
	VERTEX_2D			*pVtx;	// ���_���

	
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM * 2), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\fade000.jpg", &m_pTexture[0]);
	D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\fade001.jpg", &m_pTexture[1]);

	// �t�F�[�h���̏����ݒ�
	m_Fade[0].Pos		= D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f);
	m_Fade[0].Rot		= VEC3_ZERO;
	m_Fade[0].fLength	= hypotf((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT) * 0.5f;
	m_Fade[0].fAngle	= atan2f((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT);
	
	m_Fade[1].Pos		= D3DXVECTOR3((SCREEN_WIDTH * 0.5f), (SCREEN_HEIGHT * 0.5f), 0.0f);
	m_Fade[1].Rot		= VEC3_ZERO;
	m_Fade[1].fLength	= hypotf((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT) * 0.5f;
	m_Fade[1].fAngle	= atan2f((SCREEN_WIDTH * 0.5f), SCREEN_HEIGHT);

	m_FadeState.State			= FS_FIRST;
	m_FadeState.nCntFade		= FADE_CLOSETIME;
	m_FadeState.fLength			= (SCREEN_WIDTH * 0.25f);	// �t�F�[�h���m�̋����̏����ݒ�
	m_FadeState.nCntPhaseNone	= PHASETIME_NONE;			// �����t�F�[�Y�J�E���g�̏�����

	// �t�F�[�h�̏����ݒ�
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	for(int i = 0 ; i < 2 ; i++)
	{
		// �`����W�ݒ�
		pVtx[i * VERTEX_NUM + 0].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 1].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 2].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.z = 0.0f;
	
		pVtx[i * VERTEX_NUM + 3].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.z = 0.0f;
	
		// ���Z�W���ݒ�
		for(int nCntRhw = 0 ; nCntRhw < VERTEX_NUM ; nCntRhw++)
		{
			pVtx[i * VERTEX_NUM + nCntRhw].rhw = 1.0f;
		}
	
		// �t�F�[�h�F�ݒ�
		for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
		{
			pVtx[i * VERTEX_NUM + nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
		}
	
		// �e�N�X�`�����W�ݒ�
		pVtx[i * VERTEX_NUM + 0].tex = VEC2_ZERO;
		pVtx[i * VERTEX_NUM + 1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[i * VERTEX_NUM + 2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[i * VERTEX_NUM + 3].tex = D3DXVECTOR2(1.0f, 1.0f);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CFade::Uninit(void)
{
	for(int i = 0 ; i < 2 ; i++)
	{
		if(m_pTexture[i] != NULL)
		{
			m_pTexture[i]->Release();
			m_pTexture[i] = NULL;
		}
	}
	if(m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
//=============================================================================
void CFade::Update(void)
{
	VERTEX_2D	*pVtx;	// ���_���

	if(m_FadeState.State == FS_FIRST)
	{// �����t�F�[�Y
		m_FadeState.nCntPhaseNone--;

		if(m_FadeState.nCntPhaseNone <= 0)
		{// �t�F�[�h��������
			m_FadeState.nCntPhaseNone = PHASETIME_NONE;
			m_FadeState.fLength = (SCREEN_WIDTH * 0.25f);
			m_FadeState.State = FS_IN;
		}
		else if(m_FadeState.nCntPhaseNone == FADE_CLOSETIME * 0.5f)
		{// ���[�h��ύX
			if(m_NextMode != NULL)
			{
				CManager::SetMode(m_NextMode);
				m_NextMode = NULL;
			}
		}
	}
	else if(m_FadeState.State == FS_IN)
	{// �t�F�[�h�C���̎�
		
		if(m_FadeState.nCntFade == FADE_CLOSETIME)
		{// �t�F�[�h�̊Ԋu���L����
			m_FadeState.fLength += FADE_CLOSESPEED;
			//KeyStateLock(FADE_CLOSETIME);
		}

		if(m_FadeState.fLength >= SCREEN_WIDTH / cosf(m_Fade[0].Rot.z))
		{// �Q�̃t�F�[�h�������Ȃ����ꍇ
			m_FadeState.fLength = SCREEN_WIDTH / cosf(m_Fade[0].Rot.z);
			m_FadeState.State = FS_NONE;
		}
	}
	else if(m_FadeState.State == FS_OUT)
	{// �t�F�[�h�A�E�g�̎�
		
		if(m_FadeState.nCntFade == FADE_CLOSETIME)
		{// �t�F�[�h�̊Ԋu�����߂�

			m_FadeState.fLength -= FADE_CLOSESPEED;
		}

		if(m_FadeState.fLength <= (SCREEN_WIDTH * 0.25f))
		{// �Q�̃t�F�[�h�����������ꍇ
			
			m_FadeState.nCntFade--;
			if(m_FadeState.nCntFade <= 0)
			{// �t�F�[�h��������

				//PlaySound(SOUND_LABEL_SE002);
				m_FadeState.nCntFade = FADE_CLOSETIME;
				m_FadeState.fLength = (SCREEN_WIDTH * 0.25f);
				m_FadeState.State = FS_IN;
			}
			else if(m_FadeState.nCntFade == FADE_CLOSETIME * 0.5f)
			{// ���[�h��ύX
				if(m_NextMode != NULL)
				{
					CManager::SetMode(m_NextMode);
					m_NextMode = NULL;
				}
			}
		}
	}
	
	// �t�F�[�h�̍��W�X�V
	m_Fade[0].Pos.x = (SCREEN_WIDTH * 0.5f) - m_FadeState.fLength * cosf(m_Fade[0].Rot.z);
	m_Fade[0].Pos.y = (SCREEN_HEIGHT * 0.5f) - m_FadeState.fLength * sinf(m_Fade[0].Rot.z);
	m_Fade[1].Pos.x = (SCREEN_WIDTH * 0.5f) + m_FadeState.fLength * cosf(m_Fade[1].Rot.z);
	m_Fade[1].Pos.y = (SCREEN_HEIGHT * 0.5f) + m_FadeState.fLength * sinf(m_Fade[1].Rot.z);

	// �t�F�[�h�̍��W���f
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);
	
	for(int i = 0 ; i < 2 ; i++)
	{
		pVtx[i * VERTEX_NUM + 0].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 0].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 1].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.x = m_Fade[i].Pos.x - (sinf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 2].Pos.y = m_Fade[i].Pos.y - (cosf(-m_Fade[i].fAngle - m_Fade[i].Rot.z + D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.x = m_Fade[i].Pos.x - (sinf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
		pVtx[i * VERTEX_NUM + 3].Pos.y = m_Fade[i].Pos.y - (cosf(m_Fade[i].fAngle - m_Fade[i].Rot.z - D3DX_PI) * m_Fade[i].fLength);
	}

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	�֐���	:Draw
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����s���B
//=============================================================================
void CFade::Draw(void)
{
	if(m_FadeState.State != FS_NONE)
	{
		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
		// ���_�t�H�[�}�b�g�̐ݒ�
		D3D_DEVICE->SetFVF(FVF_VERTEX_2D);
		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		// �t�F�[�h�`��
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (VERTEX_NUM * 0), PRIMITIVE_NUM);
		// �e�N�X�`���̐ݒ�
		D3D_DEVICE->SetTexture(0, m_pTexture[1]);
		// �t�F�[�h�`��
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, (VERTEX_NUM * 1), PRIMITIVE_NUM);
	}
}

//=============================================================================
//	�֐���	:Start
//	����	:����
//	�߂�l	:����
//	����	:�t�F�[�h���J�n����B
//=============================================================================
void CFade::Start(CMode *nextMode, FADE_STATE fadeState)
{
	if((nextMode != NULL) && (m_FadeState.State == FS_NONE))
	{
		m_NextMode = nextMode;
		m_FadeState.State = fadeState;
	}
}