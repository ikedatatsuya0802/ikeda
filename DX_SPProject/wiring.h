#ifndef _WIRING_H_
#define _WIRING_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	wiring.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "sceneXDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	WIRING_CLEAR_BORDER	(0.01f)	// �����ɂ��鋫�E�̃X�v���C�����W��

//=============================================================================
//	�N���X��`
//=============================================================================
class CWiring : public CSceneXDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CWiring	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CWiring(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CWiring();

	void SetVtxBuff(void);
	
	vector<LPDIRECT3DVERTEXBUFFER9>	m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^

	SPLINE*		m_Spline;
};

#endif