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
#include "d3dx9.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	PRIORITY_NUM	(4)

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
protected:
//	static CSceneDX *m_pTop[PRIORITY_NUM];	// ���X�g�̐擪�|�C���^
//	static CSceneDX *m_pCur[PRIORITY_NUM];	// ���X�g�̏I�[
	static CSceneDX *m_pTop;	// ���X�g�̐擪�|�C���^
	static CSceneDX *m_pCur;	// ���X�g�̏I�[�|�C���^

	OBJTYPE		m_ObjType;

	D3DXVECTOR3 m_Pos;		// �ʒu
	D3DXVECTOR3 m_Rot;		// ��]�p
	
	CSceneDX(int priority = 1, OBJTYPE objType = OBJTYPE_NONE);
	~CSceneDX();
	
	CSceneDX *m_pPrev;		// �O�Q�Ɛ�|�C���^
	CSceneDX *m_pNext;		// ��Q�Ɛ�|�C���^

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

//	static CSceneDX	*GetList(int priority)	{ return m_pTop[priority]; }
//	CSceneDX		*GetNext(void)			{ return m_pNext; }
//	OBJTYPE			GetObjType(void)		{ return m_ObjType; }
};

#endif