#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models) {
	worldTransformBase_.Initialize();
	models_ = models;
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	input_ = Input::GetInstance();
}

void Player::Update() {
	if (input_->PressKey(DIK_D)) {
		worldTransformBase_.translation_.num[0] += 0.1f;
	}
	else if (input_->PressKey(DIK_A)) {
		worldTransformBase_.translation_.num[0] -= 0.1f;
	}


	worldTransformBase_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::End();

}

void Player::Draw(const ViewProjection& view) {
	models_[0]->Draw(worldTransformBase_, view, modelMaterial_);
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.num[0] = worldTransformBase_.matWorld_.m[3][0];
	worldPos.num[1] = worldTransformBase_.matWorld_.m[3][1];
	worldPos.num[2] = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {
	isHit = true;
}