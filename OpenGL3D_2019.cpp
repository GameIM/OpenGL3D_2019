// OpenGL3D_2019.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include "pch.h"
#include <iostream>
#include "Src/GLFWEW.h"
#include "Src/TitleScene.h"
#include "Src/SkeletalMesh.h"
#include "Src/Audio/Audio.h"
#include <Windows.h>


int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 720, u8"アクションゲーム");

	//音声再生プログラムを初期化する
	Audio::Engine& audioEngine = Audio::Engine::Instance();
	if (!audioEngine.Initialize())
	{
		return 1;
	}

	//スケルタルアニメーションを利用可能する
	Mesh::SkeletalAnimation::Initialize();

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());

	while (!window.ShouldClose())
	{
		//ESCキーが押されたら終了ウィンドウを表示
		if (window.IsKeyPressed(GLFW_KEY_ESCAPE))
		{
			if (MessageBox(nullptr, "ゲームを終了しますか?", "終了", MB_OKCANCEL) == IDOK)
			{
				break;
			}
		}
		const float deltaTime = window.DeltaTime();
		window.UpdateTimer();

		//スケルタルアニメーション用データの作成準備
		Mesh::SkeletalAnimation::ResetUniformData();

		sceneStack.Update(deltaTime);

		//スケルタルアニメーション用データをGPUメモリに転送
		Mesh::SkeletalAnimation::UploadUniformData();

		//音声再生プログラムを更新する
		audioEngine.Update();

		//バックバッファを消去する
		glClearColor(0.8f, 0.2f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//GLコンテキストのパラメータを設定
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);

		sceneStack.Render();
		window.SwapBuffers();
	}

	//スケルタルアニメーションの利用を終了する
	Mesh::SkeletalAnimation::Finalize();

	//音声再生プログラムを終了する
	audioEngine.Finalize();
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

