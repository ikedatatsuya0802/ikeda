//=============================================================================
//
//	タイトル	レンダラファイル
//	ファイル名	renderer.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/13
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	静的メンバ変数
//=============================================================================
LPDIRECT3D9				CRendererDX::m_pD3D;
LPDIRECT3DDEVICE9		CRendererDX::m_pD3DDevice;

LPDIRECT3DTEXTURE9		CRendererDX::m_BlurTex1;
LPDIRECT3DSURFACE9		CRendererDX::m_BlurSurf1;
LPDIRECT3DTEXTURE9		CRendererDX::m_BlurTex2;
LPDIRECT3DSURFACE9		CRendererDX::m_BlurSurf2;
LPDIRECT3DSURFACE9		CRendererDX::m_BackBufferSurf;

LPDIRECT3DVERTEXBUFFER9	CRendererDX::m_pVtxBuffBlur;
LPDIRECT3DVERTEXBUFFER9	CRendererDX::m_pVtxBuffAll;

bool CRendererDX::m_flgBlur;
bool CRendererDX::m_flgBlurBigger;


//=============================================================================
//	関数名	:Init
//	引数	:HINSTANCE hInstance
//			:HWND hWnd
//			:BOOL bWindow
//	戻り値	:HRESULT
//	説明	:初期化処理を行う。
//=============================================================================
HRESULT CRendererDX::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;

	// DirectXのバージョン定義
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(m_pD3D == NULL)
	{
		MessageBox(hWnd, "m_pD3Dの初期化に失敗しました", "E_FAILが返されました", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "m_pD3D->GetAdapterDisplayModeに失敗しました", "E_FAILが返されました", MB_OK);
		return E_FAIL;
	}

	// バックバッファ情報定義
	ZeroMemory(&d3dpp, sizeof(d3dpp));
	d3dpp.BackBufferWidth = (UINT)SCREEN_WIDTH;
	d3dpp.BackBufferHeight = (UINT)SCREEN_HEIGHT;
	d3dpp.BackBufferFormat = d3ddm.Format;
	d3dpp.BackBufferCount = 1;
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
	d3dpp.EnableAutoDepthStencil = TRUE;
	d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
	d3dpp.Windowed = bWindow;
	d3dpp.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;

	// 3Dデバイス生成
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				MessageBox(hWnd, "m_pD3D->CreateDeviceに失敗しました", "E_FAILが返されました", MB_OK);
				return E_FAIL;
			}
		}
	}

	// レンダーステートの設定
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				// CULL→カリング、CCW→裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// CULL→カリング、CCW→裏面をカリング
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);					// アルファ値(透過値)設定ON
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// レンダーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// レンダーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// レンダーステート設定
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ライティング設定(オン・オフ)

																				// サンプラーステートの設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// テクスチャ縮小時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// テクスチャ拡大時の補間設定
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// 繰り返し設定(U値サイドを繰り返し)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// 繰り返し設定(V値サイドを繰り返し)

																				// テクスチャステージステートの設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// 透過設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// 透過設定
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// 透過設定


																				// テクスチャ生成
	D3DXCreateTexture(m_pD3DDevice, (UINT)SCREEN_WIDTH, (UINT)SCREEN_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_BlurTex1);
	m_BlurTex1->GetSurfaceLevel(0, &m_BlurSurf1);
	D3DXCreateTexture(m_pD3DDevice, (UINT)SCREEN_WIDTH, (UINT)SCREEN_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_BlurTex2);
	m_BlurTex2->GetSurfaceLevel(0, &m_BlurSurf2);

	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurf);

	// 頂点バッファ生成	
	CRendererDX::SetFullScreenVtx(&m_pVtxBuffBlur);
	CRendererDX::SetFullScreenVtx(&m_pVtxBuffAll);

	m_flgBlur = false;
	m_flgBlurBigger = true;

	return S_OK;
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CRendererDX::Uninit(void)
{
	// 3Dデバイス開放
	SafetyRelease(m_pD3DDevice);
	SafetyRelease(m_pD3D);

	// 各種開放
	SafetyRelease(m_BlurSurf1);
	SafetyRelease(m_BlurSurf2);
	SafetyRelease(m_BlurTex1);
	SafetyRelease(m_BlurTex2);
	SafetyRelease(m_BackBufferSurf);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CRendererDX::Update(void)
{
	/*
	if(m_flgBlurBigger && KT_B)
	{
		m_flgBlur = true;
	}*/
}

//=============================================================================
//	関数名	:Begin
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を開始する。
//=============================================================================
void CRendererDX::Begin(void)
{
	// 画面クリア
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_COLORVALUE(0.3f, 0.6f, 1.0f, 0.0f), 1.0f, 0);

	// 描画処理開始
	m_pD3DDevice->BeginScene();
}

//=============================================================================
//	関数名	:End
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を終了する。
//=============================================================================
void CRendererDX::End(void)
{
	// 描画処理終了
	m_pD3DDevice->EndScene();

	// 描画確定
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//	関数名	:FileName
//	引数	:無し
//	戻り値	:無し
//	説明	:ディレクトリとファイル名を結合して返す。
//=============================================================================
char* CRendererDX::FileName(char* filename, char* directory)
{
	char str[1024] = { NULL };

	strcat(str, directory);
	strcat(str, filename);

	return str;
}

//=============================================================================
//	関数名	:SetFullScreenVtx
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を終了する。
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CRendererDX::SetFullScreenVtx(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	VERTEX_2D *pVtx;

	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, pVtxBuff, NULL);


	(*pVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);

	// 描画座標設定
	pVtx[0].Pos = VEC3_ZERO;
	pVtx[1].Pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].Pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0 ; i < VERTEX_NUM ; i++)
	{
		// 除算係数設定
		pVtx[i].rhw = 1.0f;

		// カラー設定
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// テクスチャ貼付座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	(*pVtxBuff)->Unlock();

	return *pVtxBuff;
}

//=============================================================================
//	関数名	:DrawPrimitiveForTarget
//	引数	:無し
//	戻り値	:無し
//	説明	:。
//=============================================================================
void CRendererDX::DrawPrimitiveForTarget(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPDIRECT3DSURFACE9 target, LPDIRECT3DTEXTURE9 texture, bool alphafalse)
{
	// レンダーターゲット設定
	m_pD3DDevice->SetRenderTarget(0, target);

	// 画面クリア
	if(alphafalse) m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_COLORVALUE(0.3f, 0.6f, 1.0f, 0.0f), 1.0f, 0);

	// 描画処理開始
	m_pD3DDevice->BeginScene();

	// テクスチャセット
	m_pD3DDevice->SetTexture(0, texture);

	// アルファブレンド無効化
	if(alphafalse) m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// 頂点フォーマットの設定
	m_pD3DDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));
	// 頂点フォーマットの設定
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// 全画面ポリゴン描画
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// アルファブレンド有効化
	if(alphafalse) m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// 描画処理終了
	m_pD3DDevice->EndScene();
}

//=============================================================================
//	関数名	:DrawPrimitiveForTarget
//	引数	:D3DXMATRIX *mtxWorld	->	ワールドマトリックスのポインタ
//			:D3DXMATRIX pos			->	設定する座標
//			:D3DXMATRIX rot			->	設定する回転(デフォルトで全て0.0f)
//			:D3DXMATRIX scl			->	設定するスケール(デフォルトで全て1.0f)
//	戻り値	:無し
//	説明	:マトリックスを設定する。
//=============================================================================
void CRendererDX::SetMatrix(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// マトリックス


	// マトリックス初期化
	D3DXMatrixIdentity(mtxWorld);

	// スケール設定
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxScl);

	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxRot);

	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	D3D_DEVICE->SetTransform(D3DTS_WORLD, mtxWorld);	
}

//=============================================================================
//	関数名	:DrawPrimitiveForTarget
//	引数	:D3DXMATRIX *mtxWorld	->	ワールドマトリックスのポインタ
//			:D3DXMATRIX pos			->	設定する座標
//			:D3DXMATRIX rot			->	設定する回転(デフォルトで全て0.0f)
//			:D3DXMATRIX scl			->	設定するスケール(デフォルトで全て1.0f)
//	戻り値	:無し
//	説明	:ビルボードモードでマトリックスを設定する。
//=============================================================================
void CRendererDX::SetMatrixBB(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// マトリックス


	// マトリックス初期化
	D3DXMatrixIdentity(mtxWorld);

	// ビルボード設定
	D3D_DEVICE->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(mtxWorld, NULL, &mtxView);
	mtxWorld->_41 = 0.0f;
	mtxWorld->_42 = 0.0f;
	mtxWorld->_43 = 0.0f;

	// スケール設定
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxScl);

	// 回転設定
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxRot);

	// 座標設定
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	// ワールドマトリックスの設定
	D3D_DEVICE->SetTransform(D3DTS_WORLD, mtxWorld);
}