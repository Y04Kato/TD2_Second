#include "Obstacle.h"

void Obstacle::Initialize(const Vector3& position, Lane lane) {
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	lane_ = lane;
}

void Obstacle::Update() {
	//移動処理
	const float kSpeed = 0.06f;
	worldTransform_.translation_.num[0] -= kSpeed;

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//画面外に出たら死亡フラグを立てる
	if (worldTransform_.translation_.num[0] <= -30.0f) {
		isDead_ = true;
	}
}

void Obstacle::OnCollision() {

}