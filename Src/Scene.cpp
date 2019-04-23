#include <iostream>
#include "pch.h"
#include "Scene.h"

/**
* コンストラクタ
*
* @param name シーン名
*/
Scene::Scene(const char* name) : name(name)
{
	std::cout << "Scene コンストラクタ:" << name << "\n";
}

/**
* デストラクタ
*/
Scene::~Scene()
{
	Finalize();
	std::cout << "Scene デストラクタ:" << "\n";
}

/**
* シーンを活動状態にする
*/
void Scene::Play()
{
	isActive = true;
	std::cout << "Scene Play:" << "\n";
}

/**
* シーンを停止状態にする
*/
void Scene::Stop()
{
	isActive = false;
	std::cout << "Scene Stop:" << "\n";
}

/**
* シーンを表示する
*/
void Scene::Show()
{
	isVisible = true;
	std::cout << "Scene Show:" << "\n";
}

/**
* シーンを非表示にする
*/
void Scene::Hide()
{
	isVisible = false;
	std::cout << "Scene Hide:" << "\n";
}

/**
* シーン名を取得する
*
* @return シーン名
*/
const std::string& Scene::Name() const
{
	return name;
}

/**
* シーンの活動状態を調べる
*
* @retval true 活動している
* @retval false 停止している
*/
bool Scene::IsActive() const
{
	return isActive;
}

/**
* シーンの表示状態を調べる
*
* @retval true 表示状態
* @retval false 非表示状態
*/
bool Scene::IsVisible() const
{
	return isVisible;
}