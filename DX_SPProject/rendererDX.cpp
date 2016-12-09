//=============================================================================
//
//	�^�C�g��	�����_���t�@�C��
//	�t�@�C����	renderer.cpp
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/13
//
//=============================================================================
//=============================================================================
//	�C���N���[�h
//=============================================================================
#include "rendererDX.h"
#include "debugproc.h"

//=============================================================================
//	�ÓI�����o�ϐ�
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
//	�֐���	:Init
//	����	:HINSTANCE hInstance
//			:HWND hWnd
//			:BOOL bWindow
//	�߂�l	:HRESULT
//	����	:�������������s���B
//=============================================================================
HRESULT CRendererDX::Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS	d3dpp;
	D3DDISPLAYMODE			d3ddm;

	// DirectX�̃o�[�W������`
	m_pD3D = Direct3DCreate9(D3D_SDK_VERSION);

	if(m_pD3D == NULL)
	{
		MessageBox(hWnd, "m_pD3D�̏������Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
		return E_FAIL;
	}
	if(FAILED(m_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		MessageBox(hWnd, "m_pD3D->GetAdapterDisplayMode�Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
		return E_FAIL;
	}

	// �o�b�N�o�b�t�@����`
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

	// 3D�f�o�C�X����
	if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_HARDWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
	{
		if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
		{
			if(FAILED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_REF, hWnd, D3DCREATE_SOFTWARE_VERTEXPROCESSING, &d3dpp, &m_pD3DDevice)))
			{
				MessageBox(hWnd, "m_pD3D->CreateDevice�Ɏ��s���܂���", "E_FAIL���Ԃ���܂���", MB_OK);
				return E_FAIL;
			}
		}
	}

	// �����_�[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);				// CULL���J�����O�ACCW�����ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);					// CULL���J�����O�ACCW�����ʂ��J�����O
	m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);					// �A���t�@�l(���ߒl)�ݒ�ON
	m_pD3DDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);				// �����_�[�X�e�[�g�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);			// �����_�[�X�e�[�g�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);		// �����_�[�X�e�[�g�ݒ�
	m_pD3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);							// ���C�e�B���O�ݒ�(�I���E�I�t)

																				// �T���v���[�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���k�����̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);		// �e�N�X�`���g�厞�̕�Ԑݒ�
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);		// �J��Ԃ��ݒ�(U�l�T�C�h���J��Ԃ�)
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);		// �J��Ԃ��ݒ�(V�l�T�C�h���J��Ԃ�)

																				// �e�N�X�`���X�e�[�W�X�e�[�g�̐ݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);		// ���ߐݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);		// ���ߐݒ�
	m_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);		// ���ߐݒ�


																				// �e�N�X�`������
	D3DXCreateTexture(m_pD3DDevice, (UINT)SCREEN_WIDTH, (UINT)SCREEN_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_BlurTex1);
	m_BlurTex1->GetSurfaceLevel(0, &m_BlurSurf1);
	D3DXCreateTexture(m_pD3DDevice, (UINT)SCREEN_WIDTH, (UINT)SCREEN_HEIGHT, 1,
		D3DUSAGE_RENDERTARGET, D3DFMT_A8B8G8R8, D3DPOOL_DEFAULT, &m_BlurTex2);
	m_BlurTex2->GetSurfaceLevel(0, &m_BlurSurf2);

	m_pD3DDevice->GetRenderTarget(0, &m_BackBufferSurf);

	// ���_�o�b�t�@����	
	CRendererDX::SetFullScreenVtx(&m_pVtxBuffBlur);
	CRendererDX::SetFullScreenVtx(&m_pVtxBuffAll);

	m_flgBlur = false;
	m_flgBlurBigger = true;

	return S_OK;
}

//=============================================================================
//	�֐���	:Uninit
//	����	:����
//	�߂�l	:����
//	����	:�I���������s���B
//=============================================================================
void CRendererDX::Uninit(void)
{
	// 3D�f�o�C�X�J��
	SafetyRelease(m_pD3DDevice);
	SafetyRelease(m_pD3D);

	// �e��J��
	SafetyRelease(m_BlurSurf1);
	SafetyRelease(m_BlurSurf2);
	SafetyRelease(m_BlurTex1);
	SafetyRelease(m_BlurTex2);
	SafetyRelease(m_BackBufferSurf);
}

//=============================================================================
//	�֐���	:Update
//	����	:����
//	�߂�l	:����
//	����	:�X�V�������s���B
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
//	�֐���	:Begin
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����J�n����B
//=============================================================================
void CRendererDX::Begin(void)
{
	// ��ʃN���A
	m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_COLORVALUE(0.3f, 0.6f, 1.0f, 0.0f), 1.0f, 0);

	// �`�揈���J�n
	m_pD3DDevice->BeginScene();
}

//=============================================================================
//	�֐���	:End
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����I������B
//=============================================================================
void CRendererDX::End(void)
{
	// �`�揈���I��
	m_pD3DDevice->EndScene();

	// �`��m��
	m_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}

//=============================================================================
//	�֐���	:FileName
//	����	:����
//	�߂�l	:����
//	����	:�f�B���N�g���ƃt�@�C�������������ĕԂ��B
//=============================================================================
char* CRendererDX::FileName(char* filename, char* directory)
{
	char str[1024] = { NULL };

	strcat(str, directory);
	strcat(str, filename);

	return str;
}

//=============================================================================
//	�֐���	:SetFullScreenVtx
//	����	:����
//	�߂�l	:����
//	����	:�`�揈�����I������B
//=============================================================================
LPDIRECT3DVERTEXBUFFER9 CRendererDX::SetFullScreenVtx(LPDIRECT3DVERTEXBUFFER9 *pVtxBuff)
{
	VERTEX_2D *pVtx;

	D3D_DEVICE->CreateVertexBuffer((sizeof(VERTEX_2D) * VERTEX_NUM), D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, pVtxBuff, NULL);


	(*pVtxBuff)->Lock(0, 0, (void**)&pVtx, 0);

	// �`����W�ݒ�
	pVtx[0].Pos = VEC3_ZERO;
	pVtx[1].Pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].Pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].Pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);

	for(int i = 0 ; i < VERTEX_NUM ; i++)
	{
		// ���Z�W���ݒ�
		pVtx[i].rhw = 1.0f;

		// �J���[�ݒ�
		pVtx[i].col = D3DCOLOR_COLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	}

	// �e�N�X�`���\�t���W�ݒ�
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	(*pVtxBuff)->Unlock();

	return *pVtxBuff;
}

//=============================================================================
//	�֐���	:DrawPrimitiveForTarget
//	����	:����
//	�߂�l	:����
//	����	:�B
//=============================================================================
void CRendererDX::DrawPrimitiveForTarget(LPDIRECT3DVERTEXBUFFER9 pVtxBuff, LPDIRECT3DSURFACE9 target, LPDIRECT3DTEXTURE9 texture, bool alphafalse)
{
	// �����_�[�^�[�Q�b�g�ݒ�
	m_pD3DDevice->SetRenderTarget(0, target);

	// ��ʃN���A
	if(alphafalse) m_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL), D3DCOLOR_COLORVALUE(0.3f, 0.6f, 1.0f, 0.0f), 1.0f, 0);

	// �`�揈���J�n
	m_pD3DDevice->BeginScene();

	// �e�N�X�`���Z�b�g
	m_pD3DDevice->SetTexture(0, texture);

	// �A���t�@�u�����h������
	if(alphafalse) m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);

	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetStreamSource(0, pVtxBuff, 0, sizeof(VERTEX_2D));
	// ���_�t�H�[�}�b�g�̐ݒ�
	m_pD3DDevice->SetFVF(FVF_VERTEX_2D);

	// �S��ʃ|���S���`��
	m_pD3DDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, PRIMITIVE_NUM);

	// �A���t�@�u�����h�L����
	if(alphafalse) m_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	// �`�揈���I��
	m_pD3DDevice->EndScene();
}

//=============================================================================
//	�֐���	:DrawPrimitiveForTarget
//	����	:D3DXMATRIX *mtxWorld	->	���[���h�}�g���b�N�X�̃|�C���^
//			:D3DXMATRIX pos			->	�ݒ肷����W
//			:D3DXMATRIX rot			->	�ݒ肷���](�f�t�H���g�őS��0.0f)
//			:D3DXMATRIX scl			->	�ݒ肷��X�P�[��(�f�t�H���g�őS��1.0f)
//	�߂�l	:����
//	����	:�}�g���b�N�X��ݒ肷��B
//=============================================================================
void CRendererDX::SetMatrix(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// �}�g���b�N�X


	// �}�g���b�N�X������
	D3DXMatrixIdentity(mtxWorld);

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_WORLD, mtxWorld);	
}

//=============================================================================
//	�֐���	:DrawPrimitiveForTarget
//	����	:D3DXMATRIX *mtxWorld	->	���[���h�}�g���b�N�X�̃|�C���^
//			:D3DXMATRIX pos			->	�ݒ肷����W
//			:D3DXMATRIX rot			->	�ݒ肷���](�f�t�H���g�őS��0.0f)
//			:D3DXMATRIX scl			->	�ݒ肷��X�P�[��(�f�t�H���g�őS��1.0f)
//	�߂�l	:����
//	����	:�r���{�[�h���[�h�Ń}�g���b�N�X��ݒ肷��B
//=============================================================================
void CRendererDX::SetMatrixBB(D3DXMATRIX *mtxWorld, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR3 scl)
{
	D3DXMATRIX mtxView, mtxScl, mtxRot, mtxTrans;			// �}�g���b�N�X


	// �}�g���b�N�X������
	D3DXMatrixIdentity(mtxWorld);

	// �r���{�[�h�ݒ�
	D3D_DEVICE->GetTransform(D3DTS_VIEW, &mtxView);
	D3DXMatrixInverse(mtxWorld, NULL, &mtxView);
	mtxWorld->_41 = 0.0f;
	mtxWorld->_42 = 0.0f;
	mtxWorld->_43 = 0.0f;

	// �X�P�[���ݒ�
	D3DXMatrixScaling(&mtxScl, scl.x, scl.y, scl.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxScl);

	// ��]�ݒ�
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxRot);

	// ���W�ݒ�
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(mtxWorld, mtxWorld, &mtxTrans);

	// ���[���h�}�g���b�N�X�̐ݒ�
	D3D_DEVICE->SetTransform(D3DTS_WORLD, mtxWorld);
}