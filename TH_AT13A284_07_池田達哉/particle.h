#ifndef _PARTICLE_H_
#define _PARTICLE_H_
//=============================================================================
//
//	タイトル	ヘッダファイル(scene3D.cpp)
//	ファイル名	scene3D.h
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	マクロ定義
//=============================================================================
#define	PARTICLE_PATTERN	(4)			// パーティクルのパターン数
#define	PARTICLE_LIFE		(120)		// パーティクルの表示時間

//=============================================================================
//	構造体
//=============================================================================

//=============================================================================
//	クラス定義
//=============================================================================
class CParticle : public CScene3DDX
{
public:
	void	Init(int pattern);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CParticle	*Create(int pattern);

	D3DXMATRIX	*GetWMatrix(void) { return &m_mtxWorld; }	// ワールドマトリックスを取得
	
	// リソースのロード
	static void	Load(void) {
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\spring.png", &m_pTexture[0]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\summer.png", &m_pTexture[1]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\autumn.png", &m_pTexture[2]);
		D3DXCreateTextureFromFile(D3D_DEVICE, ".\\data\\TEXTURE\\winter.png", &m_pTexture[3]);
	}
	// リソースのアンロード
	static void	Unload(void) {
		for(int i = 0 ; i < PARTICLE_PATTERN ; i++)
			SafetyRelease(m_pTexture[i]);
	}

protected:
	CParticle(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CParticle();

	virtual void SetVtxBuff(void);

	static LPDIRECT3DTEXTURE9 m_pTexture[PARTICLE_PATTERN];
	int	m_Pattern;

	D3DXVECTOR2 m_Size;		// サイズ
	D3DXVECTOR3	m_Move;		// 移動量
	D3DXVECTOR3	m_RotMove;	// 回転量
	D3DXVECTOR3	m_Wind;		// 風量
	int			m_Life;
};

#endif