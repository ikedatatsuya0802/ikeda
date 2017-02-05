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
#include "manager.h"
#include "cameraDX.h"

//=============================================================================
//	�ÓI�����o�ϐ�
//=============================================================================
list<CSceneDX*>	CSceneDX::m_List[PRIORITY_MAX];

//=============================================================================
//	�֐���	:CSceneDX()
//	����	:����
//	�߂�l	:����
//	����	:�R���X�g���N�^�B
//=============================================================================
CSceneDX::CSceneDX(bool ifListAdd, int priority, OBJTYPE objType)
{
	// ���X�g�ǉ��t���O���I���̏ꍇ�A���X�g�Ɏ��g��ǉ�
	if(ifListAdd) m_List[priority].push_back(this);

	// �I�u�W�F�N�g�^�C�v��������
	m_ObjType = objType;

	// ���W�E��]�̏�����
	m_Pos = VEC3_ZERO;
	m_Rot = VEC3_ZERO;

	m_flgDraw		= true;
	m_DebugProcCnt	= 0;
}

//=============================================================================
//	�֐���	:~CSceneDX()
//	����	:����
//	�߂�l	:����
//	����	:�f�X�g���N�^�B
//=============================================================================
CSceneDX::~CSceneDX()
{

}

//=============================================================================
//	�֐���	:UpdateAll
//	����	:����
//	�߂�l	:����
//	����	:���X�g�ɒǉ�����Ă���S�Ă̑Ώۂ��X�V����B
//=============================================================================
void CSceneDX::UpdateAll(void)
{
	// �S���X�g������
	for(int i = (PRIORITY_MAX - 1) ; i >= 0 ; i--)
	{
		// ���X�g�ɓo�^����Ă���S�Ă̗v�f�ɍX�V�������s��
		for each(CSceneDX* list in m_List[i])
		{
			// �X�V����
			list->Update();
		}
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
	// �S���X�g������
	for(int i = (PRIORITY_MAX - 1) ; i >= 0 ; i--)
	{
		// ���X�g�ɓo�^����Ă���S�Ă̗v�f�ɕ`�揈�����s��
		for each(CSceneDX* list in m_List[i])
		{
			// �`��t���O���I���̏ꍇ�̂ݕ`��
			if(list->GetDrawFrag())
			{
				if(i == PRIORITY_3D)
				{
					if(!CManager::GetEdhitMode())
					{
						// �t�H�O��L���ɂ���
						D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, TRUE);
					}
					else
					{
						// �t�H�O�𖳌��ɂ���
						D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, FALSE);
					}
				}

				// �`�揈��
				list->Draw();

				// �t�H�O�𖳌��ɂ���
				D3D_DEVICE->SetRenderState(D3DRS_FOGENABLE, FALSE);
			}
		}
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
	list<CSceneDX*>::iterator itr;	// ���X�g�̃C�e���[�^

	// �S���X�g������
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// ���X�g�ɓo�^����Ă���S�Ă̗v�f���폜����
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; )
		{
			// �C���X�^���X�����݂��Ă���ꍇ�̂ݏ���
			if(*itr)
			{
				// �I������
				(*itr)->Uninit();

				// �C���X�^���X�폜
				delete (*itr);

				(*itr) = NULL;
			}

			// ���X�g����폜
			itr = m_List[i].erase(itr);
		}
	}
}

//=============================================================================
//	�֐���	:Release
//	����	:����
//	�߂�l	:����
//	����	:�Ώۂ��폜����B
//=============================================================================
void CSceneDX::Release(void)
{
	list<CSceneDX*>::iterator itr;	// ���X�g�̃C�e���[�^

	// �S���X�g������
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// ���X�g���玩�g�̃C���X�^���X��T������
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; itr++)
		{
			// ���g�̃C���X�^���X�������A���X�g����폜
			if(*itr == this)
			{
				if(*itr)
				{
					// �I������
					(*itr)->Uninit();

					// �C���X�^���X�폜
					delete (*itr);
				}

				// ���X�g�폜
				itr = m_List[i].erase(itr);

				// �����I��
				break;
			}
		}
	}
}

//=============================================================================
//	�֐���	:UnlinkList
//	����	:����
//	�߂�l	:����
//	����	:�Ώۂ����X�g����O���B
//=============================================================================
void CSceneDX::UnlinkList(void)
{
	list<CSceneDX*>::iterator itr;	// ���X�g�̃C�e���[�^

	// �S���X�g������
	for(int i = 0 ; i < PRIORITY_MAX ; i++)
	{
		// ���X�g���玩�g�̃C���X�^���X��T������
		for(itr = m_List[i].begin() ; itr != m_List[i].end() ; itr++)
		{
			// ���g�̃C���X�^���X�������A���X�g����폜
			if(*itr == this)
			{
				// ���X�g�폜
				itr = m_List[i].erase(itr);

				// �����I��
				break;
			}
		}
	}
}

//=============================================================================
//	�֐���	:AddVtxBuffer
//	����	:vector<LPDIRECT3DVERTEXBUFFER9> &pVtxBuff	->	���_�o�b�t�@
//			:int size	->	�o�b�t�@�T�C�Y
//	�߂�l	:����
//	����	:���_�o�b�t�@��ǉ�����B
//=============================================================================
void CSceneDX::AddVtxBuffer(vector<LPDIRECT3DVERTEXBUFFER9> &pVtxBuff, const int size)
{
	// �T�C�Y�ύX
	pVtxBuff.resize(pVtxBuff.size() + 1);

	// �o�b�t�@����
	D3D_DEVICE->CreateVertexBuffer(size,
		D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED,
		&pVtxBuff[pVtxBuff.size() - 1],
		NULL);
}