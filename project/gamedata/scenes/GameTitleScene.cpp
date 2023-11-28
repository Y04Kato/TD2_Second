#include"GameTitleScene.h"

void GameTitleScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	title_ = textureManager_->Load("project/gamedata/resources/Start.png");
	tutorial_ = textureManager_->Load("project/gamedata/resources/tutorial.png");

	//Audio
	audio_ = Audio::GetInstance();
	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/gamestart.wav");

	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	spriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	sprite_[0] = std::make_unique <CreateSprite>();
	sprite_[0]->Initialize(Vector2{100.0f,100.0f}, title_, false, false);
	sprite_[0]->SetTextureInitialSize();
	
	sprite_[1] = std::make_unique <CreateSprite>();
	sprite_[1]->Initialize(Vector2{100.0f,100.0f}, tutorial_, false, false);
	sprite_[1]->SetTextureInitialSize();

	count = 0;
}

void GameTitleScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		count++;
	}
	if (count == 1) {
		if (isPlaySE1 == false) {
			audio_->SoundPlayWave(soundData1_, 0.5f, false);
			isPlaySE1 = true;
		}
	}
	if (count == 2) {
		fade_->FadeInFlagSet(true);
	}

	if (count > 2) {
		count = 2;
	}

	if (fade_->GetColor(0) > 1.0f) {
		sceneNo = GAME_SCENE;
		isPlaySE1 = false;
		count = 0;
	}

	fade_->FadeInUpdate();
}

void GameTitleScene::Draw() {
#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();
	if (count == 0) {
		sprite_[0]->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);
	}
	if (count == 1) {
		sprite_[1]->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);
	}
	fade_->FadeInDraw();
	
#pragma endregion
}

void GameTitleScene::Finalize() {
	audio_->SoundUnload(&soundData1_);
}