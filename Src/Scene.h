/**
* @file scene.h
*/
#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED
#include <memory>
#include <string>

class SceneStack;//���ݎQ��

/**
* �V�[���̊��N���X
*/
class Scene
{
public:
	Scene(const char* name);//�R���X�g���N�^
	Scene(const Scene&) = delete;//�R�s�[�R���X�g���N�^
	Scene& operator = (const Scene&) = delete;
	virtual ~Scene();//�f�X�g���N�^

	//�������z�֐�
	virtual bool Initialize() = 0{}
	virtual void ProcessInput() = 0{}
	virtual void Update(float) = 0{}
	virtual void Render() = 0{}
	virtual void Finalize() = 0{}

	//���z�֐�
	virtual void Play();
	virtual void Stop();
	virtual void Show();
	virtual void Hide();
	const std::string& Name() const;
	bool IsActive() const;
	bool IsVisible() const;

private:
	std::string name;
	bool isVisible = true;
	bool isActive = true;
};
using ScenePtr = std::shared_ptr<Scene>;
#endif //SCENE_H_INCLUDED
