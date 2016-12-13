#ifndef _RENDERERDX_H_
#define _RENDERERDX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(renderer.cpp)
//	�t�@�C����	renderer.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/13
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "main.h"
#include "d3dx9.h"

//=============================================================================
//	�����N
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

//=============================================================================
//	�}�N����`
//=============================================================================
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// ���_���
#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// ���_���

#define	VERTEX_NUM			(4)							// ���_��
#define	PRIMITIVE_NUM		(2)							// �v���~�e�B�u��
#define	D3D_DEVICE			CRendererDX::m_pD3DDevice	// 3D�f�o�C�X

#define	VEC2_ZERO			(D3DXVECTOR2(0.0f, 0.0f))		// 2D�x�N�g����0
#define	VEC3_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 3D�x�N�g����0

#define	TEX_FILEPASS		".\\data\\TEXTURE\\"	// �e�N�X�`���ۑ��f�B���N�g��
#define	WHITE				1.0f, 1.0f, 1.0f		// ������

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	D3DXVECTOR3	Pos;	// ���_���W(2D)
	float		rhw;	// ���Z��
	D3DCOLOR	col;	// ���_�F
	D3DXVECTOR2	tex;	// �e�N�X�`�����W
}VERTEX_2D;

typedef struct {
	D3DXVECTOR3 Pos;	// ���_���W(3D)
	D3DXVECTOR3 Nor;	// �@��
	D3DCOLOR	col;	// ���_�F
	D3DXVECTOR2 tex;	// �e�N�X�`�����W
}VERTEX_3D;

//=============================================================================
//	�O���錾
//============================================================================

//=============================================================================
//	�N���X��`
//=============================================================================
class CRendererDX
{
public:
	static HRESULT	Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
	static void	Uninit(void);
	static void	Update(void);
	static void	Begin(void);
	static void	End(void);

	static char* FileName(const char *filename, const char *directory = "./data/TEXTURE/");
	static LPDIRECT3DVERTEXBUFFER9 SetFullScreenVtx(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff);
	static void SetMatrix(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot = VEC3_ZERO, D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	static void SetMatrixBB(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot = VEC3_ZERO, D3DXVECTOR3 scl = D3DXVECTOR3(1.0f, 1.0f, 1.0f));
	static void EnableZTest(void) {
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, TRUE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESS);
		D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}
	static void DisableZTest(void) {
		D3D_DEVICE->SetRenderState(D3DRS_ZENABLE, FALSE);
		D3D_DEVICE->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
		D3D_DEVICE->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	}

	static LPDIRECT3DDEVICE9		m_pD3DDevice;		// 3D�f�o�C�X�ւ̃|�C���^

protected:
	static void DrawPrimitiveForTarget(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPDIRECT3DSURFACE9 target, LPDIRECT3DTEXTURE9 texture, bool alphafalse = false);

	static LPDIRECT3D9				m_pD3D;				// 3D�I�u�W�F�N�g

	static LPDIRECT3DTEXTURE9		m_BlurTex1;			// �u���[�G�t�F�N�g�p�e�N�X�`��1
	static LPDIRECT3DSURFACE9		m_BlurSurf1;		// �u���[�G�t�F�N�g�p�T�[�t�F�X1
	static LPDIRECT3DTEXTURE9		m_BlurTex2;			// �u���[�G�t�F�N�g�p�e�N�X�`��2
	static LPDIRECT3DSURFACE9		m_BlurSurf2;		// �u���[�G�t�F�N�g�p�T�[�t�F�X2
	static LPDIRECT3DSURFACE9		m_BackBufferSurf;	// �o�b�N�o�b�t�@�ޔ�p�T�[�t�F�X

	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBlur;
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffAll;

	static bool m_flgBlur;
	static bool m_flgBlurBigger;
};

#endif