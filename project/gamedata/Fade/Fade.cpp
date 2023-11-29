#include "Fade.h"

Fade::~Fade() {

}

void Fade::Initialize() {
	textureManager_ = TextureManager::GetInstance();

	textureHandle_ = textureManager_->Load("project/gamedata/resources/fade.png");

	sprite_[0] = std::make_unique<CreateSprite>();
	sprite_[0]->Initialize(Vector2{ 100.0f,100.0f }, textureHandle_, false, false);
	sprite_[0]->SetTextureInitialSize();

	sprite_[1] = std::make_unique<CreateSprite>();
	sprite_[1]->Initialize(Vector2{ 100.0f,100.0f }, textureHandle_, false, false);
	sprite_[1]->SetTextureInitialSize();

	spriteMaterial_[0] = {1.0f,1.0f,1.0f,0.0f};
	spriteTransform_[0] = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	spriteUvTransform_[0] = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	spriteMaterial_[1] = {1.0f,1.0f,1.0f,1.0f};
	spriteTransform_[1] = {{1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f}};
	spriteUvTransform_[1] = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	fadeColor_[0] = 0.0f;
	fadeColor_[1] = 1.0f;

	isCount_[0] = false;
	isCount_[1] = false;
}

void Fade::FadeInUpdate() {
	if (isCount_[0] == true) {
		fadeColor_[0] += 0.02f;
	}

	if (fadeColor_[0] > 1.2f) {
		isCount_[0] = false;
		fadeColor_[0] = 0.0f;
	}
}

void Fade::FadeOutUpdate() {
	if (isCount_[1] == true) {
		fadeColor_[1] -= 0.02f;
	}

	if (fadeColor_[1] < -1.2f) {
		isCount_[1] = false;
		fadeColor_[1] = 1.0f;
	}
}

void Fade::FadeInDraw() {
	//sprite_[0]->SetColor({ 1.0f,1.0f,1.0f,fadeColor_[0] });
	sprite_[0]->Draw(spriteTransform_[0],spriteUvTransform_[0],{1.0f,1.0f,1.0f,fadeColor_[0]});
}

void Fade::FadeOutDraw() {
	//sprite_[1]->SetColor({ 1.0f,1.0f,1.0f,fadeColor_[1] });
	sprite_[1]->Draw(spriteTransform_[1],spriteUvTransform_[1],{1.0f,1.0f,1.0f,fadeColor_[1]});
}

void Fade::Finalize() {
	
}