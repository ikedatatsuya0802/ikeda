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
#define	DEBUGPROC_CNT	(120)	// �f�o�b�O���̕\������

//=============================================================================
//	�\���́E�񋓑�
//=============================================================================
typedef enum {
	PRIORITY_TOP = 0,
	PRIORITY_2D = 2,
	PRIORITY_3D,
	PRIORITY_3D_NOFOG,
	PRIORITY_MAX
} PRIORITY;

typedef enum{
	OBJTYPE_NONE = 0,
	OBJTYPE_2D,
	OBJTYPE_3D,
	OBJTYPE_PLAYER,
	OBJTYPE_ENEMY,
	OBJTYPE_NUMBER,
	OBJTYPE_MAX
} OBJTYPE;

//=============================================================================
//	�N���X��`
//=============================================================================
class CSceneDX
{
public:
	CSceneDX(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objType = OBJTYPE_NONE);
	~CSceneDX();

	virtual void	Init() {};
	virtual void	Uninit(void)	= 0;
	virtual void	Update(void)	= 0;
	virtual void	Draw(void)		= 0;

	static void	UpdateAll(void);
	static void	DrawAll(void);
	static void	DeleteAll(void);
	void		Release(void);
	void		UnlinkList(void);

	D3DXVECTOR3	GetPos(void)		{ return m_Pos; }
	D3DXVECTOR3	GetRot(void)		{ return m_Rot; }
	cbool		GetDrawFrag(void)	{ return m_flgDraw; }

	static list<CSceneDX*> GetList(void) { return m_List[0]; }
	static list<CSceneDX*> GetList(int priority) { return (priority < PRIORITY_MAX) ? m_List[priority] : m_List[0]; }
	
	void		AddPos(D3DXVECTOR3 pos)				{ m_Pos += pos; }
	void		AddPos(float x, float y, float z)	{ m_Pos += D3DXVECTOR3(x, y, z); }
	void		AddRot(D3DXVECTOR3 rot)				{ m_Rot += rot; }
	void		AddRot(float x, float y, float z)	{ m_Rot += D3DXVECTOR3(x, y, z); }
	void		SetPos(D3DXVECTOR3 pos)				{ m_Pos = pos; }
	void		SetPos(float x, float y, float z)	{ m_Pos = D3DXVECTOR3(x, y, z); }
	void		SetRot(D3DXVECTOR3 rot)				{ m_Rot = rot; }
	void		SetRot(float x, float y, float z)	{ m_Rot = D3DXVECTOR3(x, y, z); }

	void		SetDrawFrag(cbool flag) { m_flgDraw = flag; }
	void		ChangeDrawFrag(void)	{ m_flgDraw = m_flgDraw ? false : true; }
		
protected:
	void AddVtxBuffer(vector<LPDIRECT3DVERTEXBUFFER9> &pVtxBuff, const int size);

	static list<CSceneDX*>	m_List[PRIORITY_MAX];	// ���X�g

	OBJTYPE	m_ObjType;		// �I�u�W�F�N�g�^�C�v

	D3DXVECTOR3 m_Pos;		// �ʒu
	D3DXVECTOR3 m_Rot;		// ��]�p

	bool	m_flgDraw;		// �`��t���O
	int		m_DebugProcCnt;	// �f�o�b�O���̕\���J�E���^
};

#endif