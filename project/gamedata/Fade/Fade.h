#pragma once

#include "CreateSprite.h"
#include "TextureManager.h"

class Fade {
public:
	~Fade();

	void Initialize();
	
	void FadeInUpdate();
	void FadeOutUpdate();

	void FadeInDraw();
	void FadeOutDraw();

	void Finalize();

	bool FadeInFlagSet(bool flag) { return isCount_[0] = flag; }
	bool FadeOutFlagSet(bool flag) { return isCount_[1] = flag; }

	float GetColor(int num) { return fadeColor_[num]; }

	void SetFadeInFlag(bool flag) { isCount_[0] = flag; }
	void SetFadeOutFlag(bool flag) { isCount_[1] = flag; }
	void SetColor(int num,float num2) { fadeColor_[num] = num2; }

private:
	std::unique_ptr<CreateSprite> sprite_[2];
	uint32_t textureHandle_;

	Transform spriteTransform_[2];
	Transform spriteUvTransform_[2];
	Vector4 spriteMaterial_[2];

	TextureManager* textureManager_;

	float fadeColor_[2];
	bool isCount_[2];
};