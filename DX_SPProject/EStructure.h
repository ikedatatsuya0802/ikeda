#ifndef _ESTRUCTURE_H_
#define _ESTRUCTURE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	EStructure.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	ESTRUCTURE_WIDTH		(15.0f)				// ����
#define	ESTRUCTURE_MARGIN		(50.0f)				// ���[���̊Ԋu
#define	ESTRUCTURE_SET		(20)				// ���[���̕�����
#define	ESTRUCTURE_VECTOR		(4)					// ���[���̃x�N�g����
#define	ESTRUCTURE_VERTEX		(ESTRUCTURE_SET * 2 + 2)	// ���[���̒��_��
#define	ESTRUCTURE_TEXFILENAME000	"EStructure000.png"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CEStructure : public CScene3DDX
{
public:
	void	Init(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CEStructure	*Create(int line = 0, D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CEStructure(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CEStructure();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^

	int			m_EStructureLine;				// ���[���̔z�u
	SPLINE*		m_Spline;
};

#endif