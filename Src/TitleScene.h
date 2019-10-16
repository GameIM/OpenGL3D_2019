/**
* @file TitleScene.h
*/
#ifndef _TITLESCENE_H_INCLUDED
#define _TITLESCENE_H_INCLUDED
#include "Scene.h"
#include "Sprite.h"
#include "Font.h"
#include "Audio/Audio.h"
#include <vector>

class TitleScene : public Scene
{
public:
	TitleScene() : Scene("TitleScene"){}
	virtual ~TitleScene() = default;

	virtual bool Initialize() override;
	virtual void ProcessInput() override;
	virtual void Update(float) override;
	virtual void Render() override;
	virtual void Finalize() override {};

private:
	bool flag = false;
	std::vector<Sprite> sprites;
	SpriteRenderer spriteRenderer;
	FontRenderer fontRenderer;
	Audio::SoundPtr bgm;
	float timer = 0;

};
#endif//TITLESCENE_H_INCLUDED
