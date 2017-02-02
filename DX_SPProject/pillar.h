#ifndef _PILLAR_H_
#define _PILLAR_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��
//	�t�@�C����	pillar.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/06/29
//
//=============================================================================
#include "scene3DDX.h"
#include "railLine.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define array_sizeof(p) ( sizeof( p) / sizeof( (p)[0] ) )
#define	PILLAR_RADIUS			(10.0f)					// ���a
#define	PILLAR_HEIGHT			(100.0f)				// �c��
#define	PILLAR_VERTICAL			(1)						// �c�u���b�N��
#define	PILLAR_HORIZONTAL		(16)					// ���u���b�N��
#define	PILLAR_TEXFILENAME000	"wall000.jpg"			// �|���S���̃t�@�C����

#define	PILLAR_POLYGON_NUM		((PILLAR_HORIZONTAL * 2) * PILLAR_VERTICAL + ((PILLAR_VERTICAL - 1) * 4))		// �|���S����
#define	PILLAR_INDEX_NUM		(((PILLAR_HORIZONTAL + 1) * 2) * PILLAR_VERTICAL + ((PILLAR_VERTICAL - 1) * 2))	// ���_�C���f�b�N�X��

#define	PILLAR_VERTEX_NUM		((PILLAR_VERTICAL + 1) * (PILLAR_HORIZONTAL + 1))	// ���_��
#define PILLAR_INDEX_STANDARD	((PILLAR_HORIZONTAL + 1) * 2 + 2)					// �C���f�b�N�X�̊�l

//=============================================================================
//	�\����
//=============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CPillar : public CScene3DDX
{
public:
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// ���\�[�X�̃��[�h
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"PILLAR_TEXFILENAME000, &m_pTexture); }
	// ���\�[�X�̃A�����[�h
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CPillar	*Create(D3DXVECTOR3 pos = VEC3_ZERO);

protected:
	CPillar(bool ifListAdd = true, int priority = PRIORITY_2D, OBJTYPE objtype = OBJTYPE_NONE);
	~CPillar();

	void SetVtxBuff(void);
	void SetMeshNor(void);
	D3DXVECTOR3 GetVecNor(D3DXVECTOR3 *vec, uint size);

	static LPDIRECT3DTEXTURE9		m_pTexture;	// �e�N�X�`���ւ̃|�C���^
	LPDIRECT3DINDEXBUFFER9			m_pIdxBuff;	// �C���f�b�N�X�o�b�t�@�ւ̃|�C���^
	
	vector<LPDIRECT3DVERTEXBUFFER9>	m_pVtxBuff;	// ���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_MeshPos[PILLAR_VERTEX_NUM];

	SPLINE*		m_Spline;
};

#endif