#ifndef _RENDERERDX_H_
#define _RENDERERDX_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(renderer.cpp)
//	ファイル名	renderer.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/13
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "main.h"
#include "d3dx9.h"

//=============================================================================
//	リンク
//=============================================================================
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

//=============================================================================
//	マクロ定義
//=============================================================================
#define	FVF_VERTEX_2D		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)				// 頂点情報
#define	FVF_VERTEX_3D		(D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_DIFFUSE | D3DFVF_TEX1)	// 頂点情報

#define	VERTEX_NUM			(4)							// 頂点数
#define	PRIMITIVE_NUM		(2)							// プリミティブ数
#define	D3D_DEVICE			CRendererDX::m_pD3DDevice	// 3Dデバイス

#define	VEC2_ZERO			(D3DXVECTOR2(0.0f, 0.0f))		// 2Dベクトルの0
#define	VEC3_ZERO			(D3DXVECTOR3(0.0f, 0.0f, 0.0f))	// 3Dベクトルの0

#define	TEX_FILEPASS		".\\data\\TEXTURE\\"	// テクスチャ保存ディレクトリ
#define	WHITE				1.0f, 1.0f, 1.0f		// †白†

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	D3DXVECTOR3	Pos;	// 頂点座標(2D)
	float		rhw;	// 除算数
	D3DCOLOR	col;	// 頂点色
	D3DXVECTOR2	tex;	// テクスチャ座標
}VERTEX_2D;

typedef struct {
	D3DXVECTOR3 Pos;	// 頂点座標(3D)
	D3DXVECTOR3 Nor;	// 法線
	D3DCOLOR	col;	// 頂点色
	D3DXVECTOR2 tex;	// テクスチャ座標
}VERTEX_3D;

//=============================================================================
//	前方宣言
//============================================================================

//=============================================================================
//	クラス定義
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

	static LPDIRECT3DDEVICE9		m_pD3DDevice;		// 3Dデバイスへのポインタ

protected:
	static void DrawPrimitiveForTarget(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPDIRECT3DSURFACE9 target, LPDIRECT3DTEXTURE9 texture, bool alphafalse = false);

	static LPDIRECT3D9				m_pD3D;				// 3Dオブジェクト

	static LPDIRECT3DTEXTURE9		m_BlurTex1;			// ブラーエフェクト用テクスチャ1
	static LPDIRECT3DSURFACE9		m_BlurSurf1;		// ブラーエフェクト用サーフェス1
	static LPDIRECT3DTEXTURE9		m_BlurTex2;			// ブラーエフェクト用テクスチャ2
	static LPDIRECT3DSURFACE9		m_BlurSurf2;		// ブラーエフェクト用サーフェス2
	static LPDIRECT3DSURFACE9		m_BackBufferSurf;	// バックバッファ退避用サーフェス

	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffBlur;
	static LPDIRECT3DVERTEXBUFFER9	m_pVtxBuffAll;

	static bool m_flgBlur;
	static bool m_flgBlurBigger;
};

#endif