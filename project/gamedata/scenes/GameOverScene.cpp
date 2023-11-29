#include"GameOverScene.h"

void GameOverScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	textureManager_ = TextureManager::GetInstance();

	// フェード
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();

	over_ = textureManager_->Load("project/gamedata/resources/Over.png");

	spriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	sprite_ = std::make_unique <CreateSprite>();
	sprite_->Initialize(Vector2{ 100.0f,100.0f }, over_, false, false);
	sprite_->SetTextureInitialSize();
	fade_->SetColor(1, 1);
	isFade_ = true;
}

void GameOverScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		fade_->FadeInFlagSet(true);
	}

	if (fade_->GetColor(0) > 1.0f) {
		fade_->SetFadeInFlag(false);
		fade_->SetColor(1, 1);
		fade_->SetColor(0, 0);
		isFade_ = true;
		sceneNo = TITLE_SCENE;
	}

	if (isFade_ == true) {
		fade_->FadeOutFlagSet(true);
	}
	if (fade_->GetColor(1) < 0.0f) {
		isFade_ = false;
		fade_->FadeOutFlagSet(false);
	}

	fade_->FadeInUpdate();
	fade_->FadeOutUpdate();
}

void GameOverScene::Draw() {
#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	sprite_->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);
	fade_->FadeOutDraw();
	fade_->FadeInDraw();

#pragma endregion
}

void GameOverScene::Finalize() {

}