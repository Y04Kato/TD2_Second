#include "Obstacle.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Obstacle::Initialize(const Vector3& position, int lane) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	lane_ = lane;
	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(~CollisionConfig::kCollisionAttributeEnemy);
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

void Obstacle::OnCollision() {

}