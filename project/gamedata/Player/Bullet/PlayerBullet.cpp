#include "PlayerBullet.h"
#include <cassert>
#include "components/utilities/collisionManager/CollisionConfig.h"
#include "Obstacle/Obstacle.h"

PlayerBullet::~PlayerBullet() {
	
}

void PlayerBullet::Initialize(const Vector3& position, const Vector3& velocity, Model* model) {
	worldTransform_.translation_ = position;
	worldTransform_.Initialize();
	worldTransform_.rotation_.num[2] = 1.55f;

	//引数で受け取った速度をメンバ変数に代入
	velocity_ = velocity;

	model_ = model;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeBullet);
	SetCollisionMask(CollisionConfig::kCollisionMaskBullet);
};

/// <summary>
/// 毎フレーム処理
/// </summary>
void PlayerBullet::Update() {
	worldTransform_.rotation_.num[1] += 0.1f;

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);
	worldTransform_.UpdateMatrix();

	
};

/// <summary>
/// 描画
/// </summary>
void PlayerBullet::Draw(const ViewProjection& viewprojection) {
	model_->Draw(worldTransform_, viewprojection, sphereMaterial_);

};


Vector3 PlayerBullet::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.num[0] = worldTransform_.translation_.num[0];
	worldPos.num[1] = worldTransform_.translation_.num[1];
	worldPos.num[2] = worldTransform_.translation_.num[2];
	return worldPos;
}

// 衝突を検出したら呼び出されるコールバック関数
void PlayerBullet::OnCollision(const Collider* collider) {
	//衝突相手の属性を取得
	uint32_t collisionAttribute = collider->GetCollisionAttribute();

	//衝突相手が敵の時
	if (collisionAttribute & CollisionConfig::kCollisionAttributeEnemy && isSideScroll_) {
		isDead_ = true;
	}

	if (mode_ == Obstacle::Mode::None) {
		isDead_ = true;
	}
}
