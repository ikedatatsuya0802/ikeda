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
#include "scene2DDX.h"
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
class CNumber : public CScene2DDX
{
public:
	CNumber(int value = 0);
	~CNumber();
	
	void	Init(int value, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);

	static CNumber	*Create(int value = 0, D3DXVECTOR3 pos = VEC3_ZERO, D3DXVECTOR2 size = VEC2_ZERO);
	
	// リソースのロード
	static void	Load(void) { D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\"NUMBER_TEXFILENAME000, &m_pTexture); }
	// リソースのアンロード
	static void	Unload(void) { SafetyRelease(m_pTexture); }
	
	void	SetNumber(int value = 0);

protected:
	static LPDIRECT3DTEXTURE9	m_pTexture;		// テクスチャへのポインタ
};

#endif