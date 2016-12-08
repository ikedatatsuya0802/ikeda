#ifndef _NUMBER_H_
#define _NUMBER_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(number.cpp)
//	ファイル名	number.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/21
//
//=============================================================================
#include "sceneDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	NUMBER_TEXFILENAME000	"number000.png"	// ポリゴンのファイル名

//=============================================================================
//	マクロ定義
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CNumber : public CSceneDX
{
public:
	CNumber(bool ifListAdd = false, int priority = 2, OBJTYPE objtype = OBJTYPE_NONE);
	~CNumber();
	
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO){}
	void	Init(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO, int value = 0);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"NUMBER_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }

	static CNumber	*Create(D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO, int value = 0);
	
	void	SetNumber(int value = 0);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
	LPDIRECT3DVERTEXBUFFER9		m_pVtxBuff;		// 頂点バッファへのポインタ

	float	m_fLength;	// 対角線の長さ
	float	m_fAngle;	// 角度
};

#endif