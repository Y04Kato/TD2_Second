#pragma once
#include "components/manager/Iscene.h"
#include "components/input/Input.h"
#include "components/manager/TextureManager.h"
#include "components/2d/CreateSprite.h"
#include "gamedata/Fade/Fade.h"

class GameClearScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	CitrusJunosEngine* CJEngine_;

	Input* input_ = nullptr;

	TextureManager* textureManager_;

	std::unique_ptr <CreateSprite> sprite_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;
	Vector4 spriteMaterial_;

	uint32_t clear_;

	std::unique_ptr<Fade> fade_;
	bool isFade_;
};