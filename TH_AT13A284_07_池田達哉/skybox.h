#ifndef _SKYBOX_H_
#define _SKYBOX_H_
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
#define	SKYBOX_TEXFILENAME000	"skybox000.jpg"						// テクスチャのファイル名
#define	SKYBOX_SIZE				(60000.0f)							// ドームの縦ブロック数
#define	SKYBOX_PRIMITIVE_NUM	(6)									// プリミティブ数
#define	SKYBOX_VERTEX_NUM		(VERTEX_NUM * SKYBOX_PRIMITIVE_NUM)	// 頂点数

//=============================================================================
//	構造体
//=============================================================================
typedef struct {
	float r;
	float g;
	float b;
} COLOR3;

#define FLOAT_COLOR(x)	(x/255.0f)

//=============================================================================
//	クラス定義
//=============================================================================
class CSkybox : public CScene3DDX
{
public:
	CSkybox(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CSkybox();

	void	Init(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CSkybox	*Create(D3DXVECTOR3 pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f));

	void	SetColor(float r = 1.0f, float g = 1.0f, float b = 1.0f);
	void	ChangeColor(int time, float r = 1.0f, float g = 1.0f, float b = 1.0f);

protected:
	void	SetVtxData(void);
	
	int			m_ChangeColorTime;
	COLOR3		m_FadeColor;
	COLOR3		m_BoxColor;
};

#endif