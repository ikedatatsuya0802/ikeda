//=============================================================================
//
//	�^�C�g��	�V�[���t�@�C��
//	�t�@�C����	scene.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/19
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "sceneDX.h"
#include "main.h"
#include "rendererDX.h"
#include "scene2DDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
//CSceneDX *CSceneDX::m_pTop[PRIORITY_NUM] = {NULL};
//CSceneDX *CSceneDX::m_pCur[PRIORITY_NUM] = {NULL};
CSceneDX *CSceneDX::m_pTop = {NULL};
CSceneDX *CSceneDX::m_pCur = {NULL};
LPDIRECT3DTEXTURE9 CSceneDX::m_pTexture;

//=============================================================================
//	�֐���	:CSceneDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneDX::CSceneDX(int priority, OBJTYPE objType)
{
	if(m_pTop == NULL)
	{// ���X�g�ɉ����o�^����ĂȂ��ꍇ
		// ���g�̃|�C���^�����X�g�擪�ɓo�^
		m_pTop = this;

		// �擪�֓o�^
		m_pPrev = NULL;
	}
	else
	{// ���X�g�ɑ��ɒǉ�����Ă���ꍇ
		// �O���ڂ��Q�Ɛ��
		m_pPrev = m_pCur;

		// �O���ڂ̎��Q�Ɛ�����g��
		m_pPrev->m_pNext = this;
	}
		
	// ���X�g�̏I�[�|�C���^�Ɏ��g��o�^
	m_pCur = this;

	// ���̍��ڂ͖���
	m_pNext = NULL;

	// ���W�E��]�̏�����
	m_Pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_Rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	m_flgDraw = true;
}

//=============================================================================
//	�֐���	:~CSceneDX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSceneDX::~CSceneDX()
{
	CSceneDX *pScene = m_pTop;		// �Q�Ɛ�o�b�t�@
	CSceneDX *pSceneNext = NULL;	// ���Q�Ɛ�o�b�t�@
	CSceneDX *pScenePrev = NULL;	// �O�Q�Ɛ�o�b�t�@

	while(pScene)
	{
		pSceneNext = pScene->m_pNext;
		pScenePrev = pScene->m_pPrev;

		if(pScene == this)
		{
			if(pScene->m_pPrev)
			{
				pScene->m_pPrev->m_pNext = pScene->m_pNext;
			}

			if(pScene->m_pNext)
			{
				pScene->m_pNext->m_pPrev = pScene->m_pPrev;
			}

			if(pScene == m_pTop)
			{
				m_pTop = pSceneNext;
			}

			if(pScene == m_pCur)
			{
				m_pCur = pScenePrev;
			}
		}
		pScene = pSceneNext;
	}
}

//=============================================================================
//	�֐���	:UpdateAll
//	����	:����
//	�߂�l	:����
//	����	:���X�g�ɒǉ�����Ă���S�Ă̑Ώۂ��X�V����B
//=============================================================================
void CSceneDX::UpdateAll(void)
{
	CSceneDX *scene = m_pTop;	// ���X�g�̐擪�|�C���^
	CSceneDX *sceneNext;		// ���X�g�̎��Q�Ɛ�o�b�t�@

	// ���X�g�̍Ō�ɓ��B����܂ōX�V�𑱂���
	while(scene != NULL)
	{
		sceneNext = scene->m_pNext;

		// �X�V
		scene->Update();

		// ���̍��ڂ�
		scene = sceneNext;
	}
}

//=============================================================================
//	�֐���	:DrawAll
//	����	:����
//	�߂�l	:����
//	����	:���X�g�ɒǉ�����Ă���S�Ă̑Ώۂ�`�悷��B
//=============================================================================
void CSceneDX::DrawAll(void)
{
	CSceneDX *scene = m_pTop;	// ���X�g�̐擪�|�C���^
	
	// ���X�g�̍Ō�ɓ��B����܂ŕ`��𑱂���
	while(scene != NULL)
	{
		// �`��
		scene->Draw();

		// ���̍��ڂ�
		scene = scene->m_pNext;
	}
}

//=============================================================================
//	�֐���	:DeleteAll
//	����	:����
//	�߂�l	:����
//	����	:���X�g�ɒǉ�����Ă���S�Ă̑Ώۂ��폜����B
//=============================================================================
void CSceneDX::DeleteAll(void)
{
	CSceneDX *pScene = m_pTop;	// ���X�g�̐擪�|�C���^
	CSceneDX *pSceneNext;		// ���Q�Ɛ�
	
	while(pScene != NULL)
	{
		// ���Q�Ɛ��ޔ�
		pSceneNext = pScene->m_pNext;

		// �I������
		pScene->Uninit();

		// �C���X�^���X�폜
		if(pScene != NULL)
		{
			delete pScene;
		}

		// ���Q�Ɛ���Z�b�g
		pScene = pSceneNext;
	}
	m_pTop = NULL;
}

//=============================================================================
//	�֐���	:Release
//	����	:����
//	�߂�l	:����
//	����	:�Ώۂ��폜����B
//=============================================================================
void CSceneDX::Release(void)
{
	// �I������
	Uninit();

	if(this == m_pTop)
	{// ���X�g�̐擪�̏ꍇ
		// ���X�g�̐擪�����݂̎����ڂ�
		m_pTop = m_pNext;
	}
	else
	{// ���X�g�̐擪�łȂ��ꍇ
		// �O���ڂ̎��Q�Ɛ�����݂̎��Q�Ɛ��
		m_pPrev->m_pNext = m_pNext;
	}

	if(this == m_pCur)
	{// ���X�g�̏I�[�̏ꍇ
		// ���X�g�̏I�[�����݂̑O���ڂ�
		m_pCur = m_pPrev;
	}
	else
	{// ���X�g�̏I�[�łȂ��ꍇ
		// �����ڂ̑O�Q�Ɛ�����݂̑O�Q�Ɛ��
		m_pNext->m_pPrev = m_pPrev;
	}
	// �C���X�^���X�폜
	delete this;
}

//=============================================================================
//	�֐���	:UnlinkList
//	����	:����
//	�߂�l	:����
//	����	:�Ώۂ����X�g����O���B
//=============================================================================
void CSceneDX::UnlinkList(void)
{
	if(this == m_pTop)
	{// ���X�g�̐擪�̏ꍇ
		// ���X�g�̐擪�����݂̎����ڂ�
		m_pTop = m_pNext;
	}
	else
	{// ���X�g�̐擪�łȂ��ꍇ
		// �O���ڂ̎��Q�Ɛ�����݂̎��Q�Ɛ��
		m_pPrev->m_pNext = m_pNext;
	}

	if(this == m_pCur)
	{// ���X�g�̏I�[�̏ꍇ
		// ���X�g�̏I�[�����݂̑O���ڂ�
		m_pCur = m_pPrev;
	}
	else
	{// ���X�g�̏I�[�łȂ��ꍇ
		// �����ڂ̑O�Q�Ɛ�����݂̑O�Q�Ɛ��
		m_pNext->m_pPrev = m_pPrev;
	}

	// �Ώۂ̑O�㍀�ڂ�����
	this->m_pPrev = NULL;
	this->m_pNext = NULL;
}