/**
* @file MainGameScene.h
*/
#ifndef _MAINGAMESCENE_H_INCLUDED
#define _MAINGAMESCENE_H_INCLUDED

#include "Scene.h"
#include "Sprite.h"
#include "Font.h"
#include "Mesh.h"
#include "Terrain.h"
#include "Actor.h"
#include "PlayerActor.h"
#include "JizoActor.h"
#include "Light.h"
#include "FramebufferObject.h"
#include "Particle.h"
#include <vector>
#include <random>

/**
* �^�C�g�����
*/
class MainGameScene : public Scene
{
public:
	MainGameScene() : Scene("MainGameScene"){}
	virtual ~MainGameScene() = default;

	virtual bool Initialize()override;
	virtual void ProcessInput()override;
	virtual void Update(float)override;
	virtual void Render()override;
	virtual void Finalize() override {};

	bool HandleJizoEffects(int id, const glm::vec3& pos);

private:
	void RenderMesh(Mesh::DrawType);

	bool flag = false;
	std::mt19937 rand;
	int jizoId = -1;//���ݐ퓬���̂��n���l��ID
	bool achivements[4] = { false, false, false, false };//�G�������
	std::vector<Sprite> sprites;
	SpriteRenderer spriteRenderer;
	FontRenderer fontRenderer;
	Mesh::Buffer meshBuffer;
	Terrain::HeightMap heightMap;
	PlayerActorPtr player;
	ActorList enemies;
	ActorList trees;
	ActorList objects;

	LightBuffer lightBuffer;
	ActorList lights;

	ParticleSystem particleSystem;

	FramebufferObjectPtr fboMain;
	FramebufferObjectPtr fboDepthOfField;
	FramebufferObjectPtr fboBloom[6][2];
	FramebufferObjectPtr fboShadow;

	struct Camera
	{
		glm::vec3 target = glm::vec3(100, 0, 100);
		glm::vec3 position = glm::vec3(100, 50, 150);
		glm::vec3 up = glm::vec3(0, 1, 0);
		glm::vec3 velocity = glm::vec3(0);

		//��ʃp�����[�^
		float width = 1280;//��ʂ̕�(�s�N�Z����)
		float height = 720;//��ʁv�̍���(�s�N�Z����)
		float near = 1;//�ŏ�z�l(���[�g��)
		float far = 500;//�ő�z�lz(���[�g��)

		//�J�����p�����[�^
		float fnumber = 1.4f;//�G�t�i���o�[ = �J������F�l
		float fov = glm::radians(60.0f);//�t�B�[���h�I�u�r���[ = �J�����̎���p(���W�A��)
		float sensorSize = 36.0f;//�Z���T�[�T�C�Y = �J�����̃Z���T�\�̉���(�~��)

		//update�֐��Ōv�Z����p�����[�^
		float focalLength = 50.0f;//�t�H�[�J�������O�X = �œ_����(�~��)
		float aperture = 20.0f;//�A�p�[�`�� = �J��(�~��)
		float focalPlane = 10000.0f;//�t�H�[�J���v��-�� = �s���g�̍�������

		void Update(const glm::mat4& matView);
	};
	Camera camera;
};
#endif //MAINGAMESCENE_H_INCLUDED

