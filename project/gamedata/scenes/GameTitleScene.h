#pragma once
#include "components/manager/Iscene.h"
#include "CJEngine.h"
#include "components/input/Input.h"
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
#include "components/audio/Audio.h"
#include "Fade/Fade.h"

class GameTitleScene :public Iscene{
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;

	TextureManager* textureManager_;

	std::unique_ptr <CreateSprite> sprite_[2];
	Transform spriteTransform_;
	Transform SpriteuvTransform_;
	Vector4 spriteMaterial_;

	uint32_t title_;
	uint32_t tutorial_;

	int count = 0;

	Audio* audio_;
	SoundData soundData1_;
	bool isPlaySE1 = false;

	
	std::unique_ptr<Fade> fade_;
};
