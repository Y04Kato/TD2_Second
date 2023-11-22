#include "Enemy.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Enemy::Initialize(const std::vector<Model*>& models, const std::vector<Model*>& models2) {
	worldTransformBase_.Initialize();
	worldTransformBase_.scale_ = { 3.0f,3.0f,3.0f };
	worldTransformTire_.Initialize();
	worldTransformTire_.scale_ = { 3.0f,3.0f,3.0f };
	worldTransformTire2_.Initialize();
	worldTransformTire2_.scale_ = { 3.0f,3.0f,3.0f };
	models_ = models;
	models2_ = models2;
	models3_ = models2;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(CollisionConfig::kCollisionMaskEnemy);
}

void Enemy::Update() {
	worldTransformTire_.translation_ = worldTransformBase_.translation_;
	worldTransformTire2_.translation_.num[0] = worldTransformBase_.translation_.num[0]+20.6f;
	worldTransformTire_.rotation_.num[2] -= 0.2f;
	worldTransformTire2_.rotation_.num[2] -= 0.2f;

	worldTransformBase_.UpdateMatrix();
	worldTransformTire_.UpdateMatrix();
	worldTransformTire2_.UpdateMatrix();
	if (life_ == 0) {
		isDead_ = true;
	}

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Pos", worldTransformTire2_.translation_.num, 0.1f);
	ImGui::Text("Life : %d", life_);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	if (!isDead_) {
		models_[0]->Draw(worldTransformBase_, viewProjection, { 1.0f,1.0f,1.0f,1.0f });;
		models2_[0]->Draw(worldTransformTire_, viewProjection, { 1.0f,1.0f,1.0f,1.0f });;
		models3_[0]->Draw(worldTransformTire2_, viewProjection, { 1.0f,1.0f,1.0f,1.0f });;
	}
	
}


void Enemy::OnCollision(const Collider* collider) {

	//衝突相手の属性を取得
	uint32_t collisionAttribute = collider->GetCollisionAttribute();

	//衝突相手が敵の時
	if (collisionAttribute & CollisionConfig::kCollisionAttributeBullet && isSideScroll_) {
		--life_;
		
	}

}