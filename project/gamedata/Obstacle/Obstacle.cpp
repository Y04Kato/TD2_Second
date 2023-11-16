#include "Obstacle.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Obstacle::Initialize(const Vector3& position, int lane, int mode) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.UpdateMatrix();
	lane_ = lane;
	mode_ = mode;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributeObstacle);
	SetCollisionMask(CollisionConfig::kCollisionMaskObstacle);
}

void Obstacle::Update() {
	////移動処理
	//const float kSpeed = 0.1f;
	//worldTransform_.translation_.num[0] -= kSpeed;

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//画面外に出たら死亡フラグを立てる
	if (worldTransform_.translation_.num[0] <= cameraPos_.num[0] - 40.0f) {
		isDead_ = true;
	}

	ImGui::Begin("Obstacle");
	switch (mode_) {
	case Mode::Acceleration:
		ImGui::Text("Acceleration");
		break;
	case Mode::Deceleration:
		ImGui::Text("Deceleration");
		break;
	case Mode::HealLife:
		ImGui::Text("HealLife");
		break;
	case Mode::None:
		ImGui::Text("None");
		break;
	}

	switch (lane_) {
	case Lane::Left:
		ImGui::Text("Left");
		break;
	case Lane::Middle:
		ImGui::Text("Middle");
		break;
	case Lane::Right:
		ImGui::Text("Right");
		break;
	}
	ImGui::End();
}

void Obstacle::OnCollision(const Collider* collider) {
	//衝突相手の属性を取得
	uint32_t collisionAttribute = collider->GetCollisionAttribute();

	//衝突相手がプレイヤーの時
	if (collisionAttribute & CollisionConfig::kCollisionAttributePlayer) {
		isDead_ = true;
	}
}