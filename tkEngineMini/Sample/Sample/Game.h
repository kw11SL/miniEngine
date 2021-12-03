#pragma once
#include "Level_New.h"

class Player_new;
class Enemy;
class SkinModelRender;
class SpriteRender;
class BG;
class DirectionLight;
class PointLight;
class SpotLight;
class UI;
class EnemyGenerator;
class Bullet;
class StageBackGround;

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
	RenderingEngine* m_renderingEngine = nullptr;
	
	Player_new* m_player = nullptr;					//�v���C���[
	Enemy* m_enemy[3] = { nullptr };						//�G�l�~�[
	BG* m_bg = nullptr;								//�X�e�[�W
	StageBackGround* m_stageBackGround = nullptr;				//�w�i
	DirectionLight* m_directionLight = nullptr;		//�f�B���N�V�������C�g
	PointLight* m_pointLight = nullptr;				//�|�C���g���C�g
	SpotLight* m_spotLight = nullptr;				//�X�|�b�g���C�g
	UI* m_ui = nullptr;								//UI
	EnemyGenerator* m_enemyGenerator[10] = {nullptr};		//�G�l�~�[������
	//�e�X�g
	SpriteRender* m_sprite = nullptr;				//�e�X�g�\���X�v���C�g
	
	
	Level_New m_level;

	float m_counter = 10.0f;
};

