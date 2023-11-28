#include "Obstacle.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Obstacle::Initialize(const Vector3& position, int lane, int mode) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 3.0f,3.0f,4.0f };
	worldTransform_.UpdateMatrix();
	lane_ = lane;
	mode_ = mode;
	switch (mode_) {
	case Mode::None:
		SetCollisionAttribute(CollisionConfig::kCollisionAttributeObstacleNone);
		SetCollisionMask(CollisionConfig::kCollisionMaskObstacleNone);
		break;
	case Mode::Acceleration:
		SetCollisionAttribute(CollisionConfig::kCollisionAttributeObstacleAcceleration);
		SetCollisionMask(CollisionConfig::kCollisionMaskObstacleAcceleration);
		break;
	case Mode::Deceleration:
		SetCollisionAttribute(CollisionConfig::kCollisionAttributeObstacleDeceleration);
		SetCollisionMask(CollisionConfig::kCollisionMaskObstacleDeceleration);
		break;
	case Mode::HealLife:
		SetCollisionAttribute(CollisionConfig::kCollisionAttributeObstacleHealLife);
		SetCollisionMask(CollisionConfig::kCollisionMaskObstacleHealLife);
		break;
	}
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
}

void Obstacle::OnCollision(const Collider* collider) {
	//衝突相手の属性を取得
	uint32_t collisionAttribute = collider->GetCollisionAttribute();

	//衝突相手がプレイヤーの時
	if (collisionAttribute & CollisionConfig::kCollisionAttributePlayer) {
		isDead_ = true;
	}
}