#include "Numbers.h"

void Numbers::Initialize() {
	numberTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Numbers.png");
	for (int index = 0; index < kDigits; index++) {
		Sprites_[index].reset(new CreateSprite());
		Sprites_[index]->Initialize(Vector2{ 64.0f,640.0f }, numberTextureHandle_, false, false);
		Sprites_[index]->SetTextureInitialSize();
	}
}

void Numbers::Draw() {
	int32_t drawNum;
	Transform uv = { {1.0f,1.0f / 10.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	Vector4 color{ 1.0f,1.0f,1.0f,1.0f };
	if (num_ < initialNum_ / 2) {
		color = { 0.8f,0.8f,0.0f,1.0f };
	}
	if (num_ <= 5) {
		color = { 0.9f,0.0f,0.0f,1.0f };
	}
	for (int index = 0; index < kDigits; index++) {
		drawNum = num_ / int(std::pow(10, kDigits - index - 1));
		drawNum %= 10;
		uv.translate.num[1] = 0.1f * float(drawNum);
		Sprites_[index]->Draw(transform_[index], uv, color);
	}

	ImGui::Begin("numbers");
	ImGui::DragFloat3("NumTranslate", transform_[0].translate.num);
	ImGui::DragFloat3("NumScale", transform_[0].scale.num, 0.1f);
	ImGui::Text("%d", drawNum);
	ImGui::Text("%d", num_);
	ImGui::End();
}

void Numbers::SetTransform(Transform transform) {
	for (int index = 0; index < kDigits; index++) {
		transform_[index] = transform;
		float leftTop = (float)kLeft + (index * (kSpace + kWidth));
		transform_[index].translate.num[0] += leftTop;
	}
}