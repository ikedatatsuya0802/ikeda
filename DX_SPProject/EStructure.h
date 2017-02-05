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
const float	ESTRUCTURE_WIDTH		= (RAILLINE_WIDTH * 5.0f);	// ����
const int	ESTRUCTURE_VERTICAL		= RAILLINE_SET;				// �������̃u���b�N��
const int	ESTRUCTURE_HORIZONTAL	= 10;						// ���s�����̃u���b�N��
const float	ESTRUCTURE_WALL_HEIGHT	= 10.0f;					// �O�ǂ̍���
#define	ESTRUCTURE_TEXFILENAME000	"EStructure000.png"			// �e�N�X�`���t�@�C����

const int ESTRUCTURE_POLYGON_NUM = ((ESTRUCTURE_HORIZONTAL * 2) * ESTRUCTURE_VERTICAL + ((ESTRUCTURE_VERTICAL - 1) * 2));			// �|���S����
const int ESTRUCTURE_INDEX_NUM = (((ESTRUCTURE_HORIZONTAL + 1) * 2) * ESTRUCTURE_VERTICAL + ((ESTRUCTURE_VERTICAL - 1) * 2));	// ���_�C���f�b�N�X��

const int ESTRUCTURE_VERTEX_NUM = ((ESTRUCTURE_VERTICAL + 1) * (ESTRUCTURE_HORIZONTAL + 1));	// ���_��
const int ESTRUCTURE_INDEX_STANDARD = ((ESTRUCTURE_HORIZONTAL + 1) * 2 + 2);					// �C���f�b�N�X�̊�l

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CEStructure : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"ESTRUCTURE_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CEStructure	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CEStructure(bool ifListAdd = true, int priority = PRIORITY_3D, OBJTYPE objtype = OBJTYPE_NONE);
	~CEStructure();

	void SetVtxBuff(void);
	
	static LPDIRECT3DTEXTURE9	m_pTexture;		// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9		m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_MeshPos[ESTRUCTURE_VERTEX_NUM];

	SPLINE*		m_Spline;
};

#endif