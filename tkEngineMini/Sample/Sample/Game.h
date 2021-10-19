#pragma once
class Player_new;
class SkinModelRender;
class SpriteRender;
class BG;
class Direction;
class DirectionLight;
class PointLight;
class SpotLight;

class Game : public IGameObject
{
public:
	Game(){}
	~Game();
	
	/// @brief 
	/// @return 
	bool Start() override;

	/// @brief ���t���[���Ă΂�鏈��
	void Update() override;

	/// @brief ����������
	/// @param renderingEngine �����_�����O�G���W��
	void Init(RenderingEngine& renderingEngine);

private:
	Player_new* m_player = nullptr;					//�v���C���[

	//Player_new* m_player2 = nullptr;					//�v���C���[


	BG* m_bg = nullptr;								//�w�i
	//Direction* m_direction = nullptr;
	DirectionLight* m_directionLight = nullptr;		//�f�B���N�V�������C�g
	PointLight* m_pointLight = nullptr;				//�|�C���g���C�g
	SpotLight* m_spotLight = nullptr;				//�X�|�b�g���C�g
	
	////�e�����p�̃��C�g�J����
	//Camera m_lightCamera;

	//�e�X�g
	SpriteRender* m_sprite = nullptr;				//�e�X�g�\���X�v���C�g
};

