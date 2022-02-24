#pragma once

#include "HID/GamePad.h"
#include "TResourceBank.h"
#include "tkFile/TkmFile.h"
#include "Shader.h"
//#include "Texture.h"
#include <MiniEngine.h>


class GraphicsEngine;

class TkEngine {
public:
	/// <summary>
	/// �f�X�g���N�^�B
	/// </summary>
	~TkEngine();
	/// <summary>
	/// �t���[���̊J�n���ɌĂ΂�鏈���B
	/// </summary>
	void BeginFrame();
	/// <summary>
	/// �t���[���̏I�����ɌĂ΂�鏈���B
	/// </summary>
	void EndFrame();
	/// <summary>
	/// �Q�[���G���W���̏������B
	/// </summary>
	void Init(HWND hwnd, UINT frameBufferWidth, UINT frameBufferHeight);
	/// <summary>
	/// tkm�t�@�C�����o���N����擾�B
	/// </summary>
	/// <param name="filePath">�t�@�C���p�X</param>
	/// <returns>tkm�t�@�C���Bnullptr���Ԃ��Ă�����o���N�ɓo�^����Ă��Ȃ��B</returns>
	TkmFile* GetTkmFileFromBank(const char* filePath)
	{
		return m_tkmFileBank.Get(filePath);
	}
	/// <summary>
	/// tkm�t�@�C�����o���N�ɓo�^�B
	/// </summary>
	/// <param name="filePath">tkm�t�@�C���p�X</param>
	/// <param name="tkmFile"></param>
	void RegistTkmFileToBank(const char* filePath, TkmFile* tkmFile)
	{
		m_tkmFileBank.Regist(filePath, tkmFile);
	}

	/// @brief tkm�t�@�C���o���N����S����
	void ClearTkmFileBank()
	{
		m_tkmFileBank.ClearBank();
	}

	/*/// @brief �e�N�X�`�����o���N����擾
	/// @param filePath �t�@�C���p�X
	/// @return �e�N�X�`���̃|�C���^
	Texture* GetTextureFromBank(const char* filePath)
	{
		return m_textureBank.Get(filePath);
	}

	/// @brief �e�N�X�`�����o���N�ɓo�^
	/// @param filePath �t�@�C���p�X
	/// @param texture �e�N�X�`���̃|�C���^
	void RegistTextureToBank(const char* filePath, Texture* texture) 
	{
		m_textureBank.Regist(filePath, texture);
	}

	/// @brief �e�N�X�`���o���N����S����
	void ClearTextureBank()
	{
		m_textureBank.ClearBank();
	}*/

	/// <summary>
	/// �V�F�[�_�[�t�@�C���o���N����V�F�[�_�[���擾�B
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	Shader* GetShaderFromBank(const char* filePath, const char* entryPointFuncName)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		return m_shaderBank.Get(programName.c_str());
	}
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	/// <param name="shader"></param>
	void RegistShaderToBank(const char* filePath, const char* entryPointFuncName, Shader* shader)
	{
		std::string programName = filePath;
		programName += entryPointFuncName;
		m_shaderBank.Regist(programName.c_str(), shader);
	}

	/// @brief �V�F�[�_�[�o���N����S����
	void ClearShaderBank()
	{
		m_shaderBank.ClearBank();
	}

	/// @brief �e��o���N����v�f���폜
	void ClearFileBank()
	{
		ClearTkmFileBank();
		ClearShaderBank();
	}

private:
	GraphicsEngine* m_graphicsEngine = nullptr;		//�O���t�B�b�N�G���W���B
	TResourceBank<TkmFile> m_tkmFileBank;			//tkm�t�@�C���o���N�B
	TResourceBank<Shader> m_shaderBank;				//�V�F�[�_�[�o���N
	//TResourceBank<Texture> m_textureBank;
	GamePad m_pad[GamePad::CONNECT_PAD_MAX];		//�Q�[���p�b�h�B
	GameTime m_gameTime;							//�Q�[���^�C���B
	
};

extern TkEngine* g_engine;	//TK�G���W���B
extern GameTime* g_gameTime;