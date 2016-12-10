#ifndef _PAUSE_H_
#define _PAUSE_H_
//=============================================================================
//
//	�^�C�g��	�w�b�_�t�@�C��(pause.cpp)
//	�t�@�C����	pause.h
//	�쐬��		AT13A284_07 �r�c�B��
//	�쐬��		2016/04/20
//
//=============================================================================
#include "scene2DDX.h"
#include "manager.h"
#include "rendererDX.h"

//=============================================================================
//	�}�N����`
//=============================================================================
#define	PAUSE_TEXTURE_NUM				(5)
#define	PAUSE_TEXFILENAME000			"pause000.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME001			"pause001.jpg"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME002			"pause002.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME003			"pause003.png"						// �|�[�Y�|���S���̃t�@�C����
#define	PAUSE_TEXFILENAME004			"pause004.png"						// �|�[�Y�|���S���̃t�@�C����

#define	PAUSE_WIDTH			(SCREEN_HEIGHT * 0.8f)		// �|�[�Y�̉���
#define	PAUSE_HEIGHT		(SCREEN_HEIGHT * 0.8f)		// �|�[�Y�̏c��
#define	PAUSE_POSX			(SCREEN_WIDTH * 0.5f)		// �|�[�Y��X�ʒu
#define	PAUSE_POSY			(SCREEN_HEIGHT * 0.5f)		// �|�[�Y��Y�ʒu

#define	PAUSE_MENU_WIDTH	(PAUSE_WIDTH * 0.54f)					// �|�[�Y���j���[�̏c��
#define	PAUSE_MENU_HEIGHT	(PAUSE_HEIGHT * 0.085f)					// �|�[�Y���j���[�̉���
#define	PAUSE_MENU_POSX		(PAUSE_POSX * 1.014f)					// �|�[�Y���j���[��X�ʒu
#define	PAUSE_MENU_POSY		(PAUSE_POSY - (PAUSE_WIDTH * 0.401f))	// �|�[�Y���j���[��Y�ʒu

#define	PAUSE_MOVE_SPEED		(1.5f)								// �|�[�Y�̈ړ����x
#define	PAUSE_STOP_SPEED		(PAUSE_MOVE_SPEED * 0.1f)			// �ړ����x�̌�����
#define	PAUSE_SCOPE_SPEED		(10.0f)								// �|�[�Y�̊g�k���x
#define	PAUSE_ROT_SPEED			(0.05f)								// �|�[�Y�̉�]���x
#define	PAUSE_ROT_STOP_SPEED	(1.0f)								// ��]���x�̌�����

typedef enum {
	PAUSE_MENU_CONTINUE = 0,	// �ĊJ
	PAUSE_MENU_RETRY,			// �Ē���
	PAUSE_MENU_QUIT,			// �I��
	PAUSE_MENU_MAX				// �|�[�Y���j���[�̍ő�l
}PAUSE_MENU;

//=============================================================================
//	�N���X��`
//=============================================================================
class CPause : public CScene2DDX
{
public:
	CPause(bool ifListAdd = true, int priority = 1, OBJTYPE objtype = OBJTYPE_NONE);
	~CPause();

	void	Init(void);
	void	Uninit(void);
	void	Update(void);
	void	Draw(void);
	static CPause	*Create(void);

	void ChangePause(void) { m_flgPause = m_flgPause ? false : true; }
	bool GetPause(void) { return m_flgPause; }

protected:
	void SetVtxBuff(LPDIRECT3DVERTEXBUFFER9 *vtxBuff);

	LPDIRECT3DTEXTURE9	m_pTexture[PAUSE_TEXTURE_NUM];
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff[3];

	bool	m_flgPause;	// �|�[�Y�����ǂ���
	int		m_PauseMenu;
};

#endif