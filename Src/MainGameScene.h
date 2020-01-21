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
* タイトル画面
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
	int jizoId = -1;//現在戦闘中のお地蔵様のID
	bool achivements[4] = { false, false, false, false };//敵討伐状態
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

		//画面パラメータ
		float width = 1280;//画面の幅(ピクセル数)
		float height = 720;//画面」の高さ(ピクセル数)
		float near = 1;//最小z値(メートル)
		float far = 500;//最大z値z(メートル)

		//カメラパラメータ
		float fnumber = 1.4f;//エフナンバー = カメラのF値
		float fov = glm::radians(60.0f);//フィールドオブビュー = カメラの視野角(ラジアン)
		float sensorSize = 36.0f;//センサーサイズ = カメラのセンサ―の横幅(ミリ)

		//update関数で計算するパラメータ
		float focalLength = 50.0f;//フォーカルレングス = 焦点距離(ミリ)
		float aperture = 20.0f;//アパーチャ = 開口(ミリ)
		float focalPlane = 10000.0f;//フォーカルプレ-ン = ピントの合う距離

		void Update(const glm::mat4& matView);
	};
	Camera camera;
};
#endif //MAINGAMESCENE_H_INCLUDED

