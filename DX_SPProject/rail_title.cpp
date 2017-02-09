//=============================================================================
//
//	タイトル	レールファイル
//	ファイル名	rail.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/06/29
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "rail_title.h"
#include "rendererDX.h"
#include "meshfield.h"

//=============================================================================
//	関数名	:Init
//	引数	:無し
//	戻り値	:無し
//	説明	:初期化処理を行う。
//=============================================================================
void CRail_Title::Init(int line, D3DXVECTOR3 pos)
{	
	char		* str	= NULL;	// ファイル内容格納配列
	unsigned int	offset	= 0;	// 文字列指定子

	// 各種初期化処理
	SetPos(D3DXVECTOR3(pos.x, pos.y, pos.z));
	SetRot(VEC3_ZERO);

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_SQUARE), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);

	// テクスチャのロード
	D3DXCreateTextureFromFile(D3D_DEVICE, CRendererDX::FileName(RAIL_TEXFILENAME000), &m_pTexture);
	
	// レール情報セット
	SetVtxBuff();

	Load();

	m_flgDraw = true;
}

//=============================================================================
//	関数名	:SetRail_TitleData
//	引数	:無し
//	戻り値	:無し
//	説明	:レールの情報をセットする。
//=============================================================================
void CRail_Title::SetVtxBuff(void)
{
	VERTEX_3D* pVtx;	// 3D頂点情報


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos.x = -(RAIL_WIDTH * 0.5f);
	pVtx[0].Pos.y = 0.1f;
	pVtx[0].Pos.z = (MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[1].Pos.x = (RAIL_WIDTH * 0.5f);
	pVtx[1].Pos.y = 0.1f;
	pVtx[1].Pos.z = (MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[2].Pos.x = -(RAIL_WIDTH * 0.5f);
	pVtx[2].Pos.y = 0.1f;
	pVtx[2].Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);

	pVtx[3].Pos.x = (RAIL_WIDTH * 0.5f);
	pVtx[3].Pos.y = 0.1f;
	pVtx[3].Pos.z = -(MESHFIELD_TOTALHEIGHT * 0.5f);

	for(int nCntSet = 0 ; nCntSet < VERTEX_SQUARE ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, MESHFIELD_TOTALHEIGHT * 0.02f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, MESHFIELD_TOTALHEIGHT * 0.02f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CRail_Title::Update(void)
{

}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CRail_Title::Draw(void)
{
	if(m_flgDraw)
	{
		// マトリックス設定
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);

		// ライティング設定をオフに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

		// 描画処理
		D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
		D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
		D3D_DEVICE->SetTexture(0, m_pTexture);								// テクスチャの設定
		D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_SQUARE);	// 描画

		// ライティング設定をオンに
		D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	}
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CRail_Title *CRail_Title::Create(int line, D3DXVECTOR3 pos)
{
	CRail_Title *instance;

	instance = new CRail_Title;

	instance->Init(line, pos);

	return instance;
}