#pragma once
class Bloom
{
public:
	Bloom() {};
	~Bloom() {};

	void Init(RenderTarget& mainRenderTatget);
	void Render(RenderContext& rc, RenderTarget& mainRenderTarget);

private:
	RenderTarget m_luminanceRenderTarget;	//�P�x���o�p�����_�����O�^�[�Q�b�g
	Sprite m_luminanceSprite;				//�P�x���o�e�N�X�`��
	GaussianBlur m_gaussBlur[4];			//�K�E�V�A���u���[
	Sprite m_finalSprite;					//�ŏI�I�ɍ�������X�v���C�g
};

