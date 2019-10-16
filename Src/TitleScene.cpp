/**
* @file TitleScene.cpp
*/
#include "TitleScene.h"
#include "MainGameScene.h"
#include "GLFWEW.h"

/**
* �V�[��������������
*
* @retval true ����������
* @retval false ���������s �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��
*/
bool TitleScene::Initialize()
{
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/TitleBg.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);

	//BGM���Đ�����
	bgm = Audio::Engine::Instance().Prepare("Res/Audio/she_wants_to_hear_scream.mp3");
	bgm->Play(Audio::Flag::Flag_Loop);

	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");

	return true;
}

/**
* �V�[�����X�V����
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void TitleScene::Update(float deltaTime)
{
	spriteRenderer.BeginUpdate();
	for (const Sprite& e : sprites)
	{
		spriteRenderer.AddVertices(e);
	}
	spriteRenderer.EndUpdate();

	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const float w = window.Width();
	const float h = window.Height();
	const float lineHeight = fontRenderer.LineHeight();
	fontRenderer.BeginUpdate();
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"�^�C�g�����");
	fontRenderer.AddString(glm::vec2(-128, 0), L"�A�N�V�����Q�[��");
	fontRenderer.EndUpdate();

	//�V�[���؂�ւ��҂�
	if (timer > 0)
	{
		timer -= deltaTime;
		if (timer <= 0)
		{
			bgm->Stop();
			SceneStack::Instance().Replace(std::make_shared<MainGameScene>());

			return;
		}
	}
}

/**
* �v���C���[�̓��͂���������
*/
void TitleScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown &GamePad::START)
	{
		Audio::Engine::Instance().Prepare("Res/Audio/Start.wav")->Play();
		timer = 1.0f;
	}
}

/**
* �V�[����`�悷��
*/
void TitleScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);
}