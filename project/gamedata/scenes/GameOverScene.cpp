#include"GameOverScene.h"

void GameOverScene::Initialize() {
	//CJEngine
	CJEngine_ = CitrusJunosEngine::GetInstance();

	//Input
	input_ = Input::GetInstance();

	textureManager_ = TextureManager::GetInstance();

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
}

void GameOverScene::Update() {
	if (input_->TriggerKey(DIK_SPACE)) {
		sceneNo = TITLE_SCENE;
	}
}

void GameOverScene::Draw() {
#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	sprite_->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);

#pragma endregion
}

void GameOverScene::Finalize() {

}