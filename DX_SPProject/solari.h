#ifndef _SOLARI_H_
#define _SOLARI_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SOLARI_WIDTH		(300.0f)	// �|���S���̉���
#define	SOLARI_HEIGHT		(300.0f)	// �|���S���̏c��
#define	SOLARI_TEXFILENAME000	"polygon000.jpg"	// �|���S���̃t�@�C����

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CSolari : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CSolari	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

protected:
	CSolari(int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSolari();

	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	LPDIRECT3DTEXTURE9		m_Texture;	// �e�N�X�`���ւ̃|�C���^

	D3DXMATRIX m_mtxWorld;
};

#endif