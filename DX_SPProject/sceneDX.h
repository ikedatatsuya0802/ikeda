#ifndef _SCENEDX_H_
#define _SCENEDX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene.cpp)
//	�t�@�C����	scene.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/19
//
//=============================================================================
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	PRIORITY_NUM	(4)
#define	DEBUGPROC_CNT	(120)	// �f�o�b�O���̕\������

//=============================================================================
//	�\���́E�񋓑�
//=============================================================================
typedef enum{
	OBJTYPE_NONE = 0,
	OBJTYPE_PLAYER,
	OBJTYPE_ENEMY,
	OBJTYPE_MAX
} OBJTYPE;

//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneDX
{
public:
	virtual void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f))		= 0;
	virtual void	Uninit(void)	= 0;
	virtual void	Update(void)	= 0;
	virtual void	Draw(void)		= 0;

	static void	UpdateAll(void);
	static void	DrawAll(void);
	static void	DeleteAll(void);
	void		Release(void);
	void		UnlinkList(void);
	
	void		AddPos(D3DXVECTOR3 pos)	{ m_Pos += pos; }
	void		AddRot(D3DXVECTOR3 rot)	{ m_Rot += rot; }
	void		SetPos(D3DXVECTOR3 pos)	{ m_Pos = pos; }
	void		SetRot(D3DXVECTOR3 rot)	{ m_Rot = rot; }
	D3DXVECTOR3	GetPos(void)			{ return m_Pos; }
	D3DXVECTOR3	GetRot(void)			{ return m_Rot; }
	void	ChangeDrawFrag(void) { m_flgDraw = m_flgDraw ? false : true; }
		
protected:
	CSceneDX(int priority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSceneDX();

	OBJTYPE		m_ObjType;	// �I�u�W�F�N�g�^�C�v

	static CSceneDX *m_pTop;	// ���X�g�̐擪�|�C���^
	static CSceneDX *m_pCur;	// ���X�g�̏I�[�|�C���^

	CSceneDX *m_pPrev;		// �O�Q�Ɛ�|�C���^
	CSceneDX *m_pNext;		// ��Q�Ɛ�|�C���^

	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// ���_�o�b�t�@�ւ̃|�C���^

	D3DXVECTOR3 m_Pos;		// �ʒu
	D3DXVECTOR3 m_Rot;		// ��]�p

	bool	m_flgDraw;		// �`��t���O
	int		m_DebugProcCnt;	// �f�o�b�O���̕\���J�E���^
};

#endif