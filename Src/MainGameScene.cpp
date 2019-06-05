/**
* @file MainGameScene.cpp
*/
#include "MainGameScene.h"
#include "StatusScene.h"
#include "GameOverscene.h"
#include "GLFWEW.h"

/**
* �V�[��������������
*
* @retval true ����������
* @retval false ���������s �Q�[���i�s�s�ɂ��A�v���O�������I�����邱��
*/
bool MainGameScene::Initialize()
{
	spriteRenderer.Init(1000, "Res/Sprite.vert", "Res/Sprite.frag");
	sprites.reserve(100);
	Sprite spr(Texture::Image2D::Create("Res/Main_result.tga"));
	spr.Scale(glm::vec2(2));
	sprites.push_back(spr);

	fontRenderer.Init(1000);
	fontRenderer.LoadFromFile("Res/font.fnt");
	return true;
}

/**
* �v���C���[�̓��͂���������
*/
void MainGameScene::ProcessInput()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	if (window.GetGamePad().buttonDown & GamePad::A)
	{
		if (!flag)
		{
			flag = true;
			//SceneStack::Instance().Push(std::make_shared<StatusScene>());
		}
		else
		{
			SceneStack::Instance().Replace(std::make_shared<GameOverScene>());
		}
		
	}
}

/**
* �V�[�����X�V����
*
* @param deltaTime �O��̍X�V����̌o�ߎ���(�b)
*/
void MainGameScene::Update(float deltaTime)
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
	fontRenderer.AddString(glm::vec2(-w * 0.5f + 32, h * 0.5f - lineHeight), L"���C���Q�[�����");
	fontRenderer.EndUpdate();
}

/**
* �V�[����`�悷��
*/
void MainGameScene::Render()
{
	const GLFWEW::Window& window = GLFWEW::Window::Instance();
	const glm::vec2 screenSize(window.Width(), window.Height());
	spriteRenderer.Draw(screenSize);
	fontRenderer.Draw(screenSize);
}

