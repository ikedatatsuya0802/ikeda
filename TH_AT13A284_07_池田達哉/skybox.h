#ifndef _SKYBOX_H_
#define _SKYBOX_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(scene3D.cpp)
//	�t�@�C����	scene3D.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene3DDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	SKYBOX_TEXFILENAME000	"skybox000.jpg"						// �e�N�X�`���̃t�@�C����
#define	SKYBOX_SIZE				(60000.0f)							// �h�[���̏c�u���b�N��
#define	SKYBOX_PRIMITIVE_NUM	(6)									// �v���~�e�B�u��
#define	SKYBOX_VERTEX_NUM		(VERTEX_NUM * SKYBOX_PRIMITIVE_NUM)	// ���_��

//=============================================================================
//	�\����
//=============================================================================
typedef struct {
	float r;
	float g;
	float b;
} COLOR3;

#define FLOAT_COLOR(x)	(x/255.0f)

//=============================================================================
//	�N���X��`
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