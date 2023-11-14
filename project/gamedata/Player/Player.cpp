#include "Player.h"

void Player::Initialize(const std::vector<Model*>& models) {
	worldTransformBase_.Initialize();
	//worldTransformBase_.translation_.num[1] = -2.0f;
	worldTransformBase_.scale_ = { 3.0f,3.0f,3.0f };
	models_ = models;
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	input_ = Input::GetInstance();

	moveSpeed_ = 0.1f;
}

void Player::Update() {
	
	Move();

	worldTransformBase_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::End();

}

void Player::Draw(const ViewProjection& view) {
	models_[0]->Draw(worldTransformBase_, view, modelMaterial_);
}

void Player::Move() {
	//横スクロール視点移動
	worldTransformBase_.translation_.num[0] += moveSpeed_;
	if (worldTransformBase_.translation_.num[0] > 20.0f) {
		worldTransformBase_.translation_.num[0] = -20.0f;
	}
	//俯瞰視点移動
	if (input_->TriggerKey(DIK_D)) {
		worldTransformBase_.translation_.num[2] -= 20.0f;
	}else if (input_->TriggerKey(DIK_A)) {
		worldTransformBase_.translation_.num[2] += 20.0f;
	}
	if (worldTransformBase_.translation_.num[2] <= -20.0f) {
		worldTransformBase_.translation_.num[2] = -20.0f;
	}else if (worldTransformBase_.translation_.num[2] >= 20.0f) {
		worldTransformBase_.translation_.num[2] = 20.0f;
	}

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
	if (obstacleMode_ == Obstacle::Mode::None) {

	}
	if (obstacleMode_ == Obstacle::Mode::Acceleration) {
		moveSpeed_ = 0.5f;
	}
}