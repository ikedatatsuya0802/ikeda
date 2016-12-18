//=============================================================================
//
//	タイトル	シーンファイル(3D)
//	ファイル名	scene3D.cpp
//	作成者		AT13A284_07 池田達哉
//	作成日		2016/04/20
//
//=============================================================================
//=============================================================================
//	インクルード
//=============================================================================
#include "particle.h"
#include "manager.h"
#include "main.h"

//=============================================================================
//	静的メンバ
//=============================================================================
LPDIRECT3DTEXTURE9 CParticle::m_pTexture[PARTICLE_PATTERN];

//=============================================================================
//	関数名	:CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:コンストラクタ。
//=============================================================================
CParticle::CParticle(bool ifListAdd, int priority, OBJTYPE objtype) : CScene3DDX(ifListAdd, priority, objtype)
{

}

//=============================================================================
//	関数名	:~CScene3D()
//	引数	:無し
//	戻り値	:無し
//	説明	:デストラクタ。
//=============================================================================
CParticle::~CParticle()
{

}

//=============================================================================
//	関数名	:Init
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:初期化処理を行うと共に、初期位置を設定する。
//=============================================================================
void CParticle::Init(int pattern)
{
	if((pattern < 0) || (pattern >= 4))
		pattern = 0;

	random_device rd;	// ランダムデバイス
	mt19937 mt(rd());	// 乱数生成機

	// パターン設定
	m_Pattern = pattern;

	// 初期回転は0
	m_Rot = VEC3_ZERO;

	// 情報設定
	switch(m_Pattern)
	{
	case 0:// 春
	{
		// 乱数生成機作成
		uniform_real_distribution<float> posX(-500, 500);
		uniform_real_distribution<float> posZ(-500, 500);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-10, -2);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.3f, 0.3f);
		uniform_real_distribution<float> rotY(-0.3f, 0.3f);
		uniform_real_distribution<float> rotZ(-0.3f, 0.3f);

		// 各種情報設定
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 700.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Life		= 120;
	}
	break;
	case 1:// 夏
	{
		// 乱数生成機作成
		uniform_real_distribution<float> posX(300, 800);
		uniform_real_distribution<float> posZ(-800, -300);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-10, -2);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.1f, 0.1f);
		uniform_real_distribution<float> rotY(-0.1f, 0.1f);
		uniform_real_distribution<float> rotZ(-0.1f, 0.1f);

		// 各種情報設定
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 500.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(-0.5f, 0.0f, 0.5f);
		m_Life		= 60;
	}
	break;
	case 2:// 秋
	{
		// 乱数生成機作成
		uniform_real_distribution<float> posX(300, 500);
		uniform_real_distribution<float> posZ(-500, -300);
		uniform_real_distribution<float> moveX(-10, 10);
		uniform_real_distribution<float> moveY(-5, -1);
		uniform_real_distribution<float> moveZ(-10, 10);
		uniform_real_distribution<float> rotX(-0.3f, 0.3f);
		uniform_real_distribution<float> rotY(-0.3f, 0.3f);
		uniform_real_distribution<float> rotZ(-0.3f, 0.3f);

		// 各種情報設定
		m_Size		= D3DXVECTOR2(50.0f, 50.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 500.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= D3DXVECTOR3(rotX(mt), rotY(mt), rotZ(mt));
		m_Wind		= D3DXVECTOR3(-0.1f, 0.0f, 0.1f);
		m_Life		= 120;
	}
	break;
	case 3:// 冬
	{
		// 乱数生成機作成
		uniform_real_distribution<float> posX(-500, 500);
		uniform_real_distribution<float> posZ(-500, 500);
		uniform_real_distribution<float> moveX(-5, 5);
		uniform_real_distribution<float> moveY(-3, -1);
		uniform_real_distribution<float> moveZ(-5, 5);

		// 各種情報設定
		m_Size		= D3DXVECTOR2(20.0f, 20.0f);
		m_Pos		= D3DXVECTOR3(posX(mt), 700.0f, posZ(mt));
		m_Move		= D3DXVECTOR3(moveX(mt), moveY(mt), moveZ(mt));
		m_RotMove	= VEC3_ZERO;
		m_Wind		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		m_Life		= 180;
	}
	break;
	default:
		break;
	}

	// 頂点バッファ生成
	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_3D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_3D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);	
	SetVtxBuff();
}

//=============================================================================
//	関数名	:SetVtxBuff
//	引数	:無し
//	戻り値	:無し
//	説明	:頂点バッファにデータをセットする。
//=============================================================================
void CParticle::SetVtxBuff(void)
{
	VERTEX_3D	*pVtx;	// 3D頂点情報


	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	// 頂点座標設定
	pVtx[0].Pos.x = -(m_Size.x * 0.5f);
	pVtx[0].Pos.y = (m_Size.y * 0.5f);
	pVtx[0].Pos.z = 0.0f;

	pVtx[1].Pos.x = (m_Size.x * 0.5f);
	pVtx[1].Pos.y = (m_Size.y * 0.5f);
	pVtx[1].Pos.z = 0.0f;

	pVtx[2].Pos.x = -(m_Size.x * 0.5f);
	pVtx[2].Pos.y = -(m_Size.y * 0.5f);
	pVtx[2].Pos.z = 0.0f;

	pVtx[3].Pos.x = (m_Size.x * 0.5f);
	pVtx[3].Pos.y = -(m_Size.y * 0.5f);
	pVtx[3].Pos.z = 0.0f;

	for(int nCntSet = 0 ; nCntSet < VERTEX_NUM ; nCntSet++)
	{
		// 法線設定
		pVtx[nCntSet].Nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);

		// 色設定
		switch(m_Pattern)
		{
		case 0:// 春
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(FCOLOR(255), FCOLOR(182), FCOLOR(193), 1.0f);
			break;
		case 1:// 夏
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(FCOLOR(34), FCOLOR(139), FCOLOR(34), 1.0f);
			break;
		case 2:// 秋
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 0.3f, 0.3f, 1.0f);
			break;
		case 3:// 冬
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		default:
			pVtx[nCntSet].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
			break;
		}
	}

	// テクスチャ座標設定
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	m_pVtxBuff->Unlock();
}

//=============================================================================
//	関数名	:Uninit
//	引数	:無し
//	戻り値	:無し
//	説明	:終了処理を行う。
//=============================================================================
void CParticle::Uninit(void)
{
	// インスタンス削除
	SafetyRelease(m_pVtxBuff);
}

//=============================================================================
//	関数名	:Update
//	引数	:無し
//	戻り値	:無し
//	説明	:更新処理を行う。
//=============================================================================
void CParticle::Update(void)
{
	// 各種処理
	m_Move	+= m_Wind;
	m_Pos	+= m_Move;
	m_Rot	+= m_RotMove;

	m_Life--;
	if(m_Life <= 0)
	{
		// インスタンス削除
		Release();
		return;
	}
}

//=============================================================================
//	関数名	:Draw
//	引数	:無し
//	戻り値	:無し
//	説明	:描画処理を行う。
//=============================================================================
void CParticle::Draw(void)
{
	// マトリックス設定
	if(m_Pattern < 3)
	{
		CRendererDX::SetMatrix(&m_mtxWorld, m_Pos, m_Rot);
	}
	else
	{
		CRendererDX::SetMatrixBB(&m_mtxWorld, m_Pos, m_Rot);
	}

	// ライティング設定をオフに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, FALSE);

	// カリングオフ
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	// アルファテスト開始
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 100);

	// 描画処理
	D3D_DEVICE->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));	// 頂点フォーマットの設定
	D3D_DEVICE->SetFVF(FVF_VERTEX_3D);									// 頂点フォーマットの設定
	
	// テクスチャの設定
	switch(m_Pattern)
	{
	case 0:// 春
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		break;
	case 1:// 夏
		D3D_DEVICE->SetTexture(0, m_pTexture[1]);
		break;
	case 2:// 秋
		D3D_DEVICE->SetTexture(0, m_pTexture[2]);
		break;
	case 3:// 冬
		D3D_DEVICE->SetTexture(0, m_pTexture[3]);
		break;
	default:
		D3D_DEVICE->SetTexture(0, m_pTexture[0]);
		break;
	}
	D3D_DEVICE->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);	// 描画

	// アルファテスト終了
	D3D_DEVICE->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	D3D_DEVICE->SetRenderState(D3DRS_ALPHAREF, 0);

	// ライティング設定をオンに
	D3D_DEVICE->SetRenderState(D3DRS_LIGHTING, TRUE);
	
	// カリングオン
	D3D_DEVICE->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
}

//=============================================================================
//	関数名	:Create
//	引数	:D3DXVECTOR3 pos(初期位置)
//	戻り値	:無し
//	説明	:インスタンス生成を行うと共に、初期位置を設定する。
//=============================================================================
CParticle *CParticle::Create(int pattern)
{
	CParticle *instance;	// インスタンス

	// インスタンス生成
	instance = new CParticle();

	// 初期化処理
	instance->Init(pattern);

	// インスタンスをリターン
	return instance;
}