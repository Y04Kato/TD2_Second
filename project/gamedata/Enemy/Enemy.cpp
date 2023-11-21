#include "Enemy.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Enemy::Initialize(const std::vector<Model*>& models) {
	worldTransformBase_.Initialize();
	worldTransformBase_.scale_ = { 3.0f,3.0f,3.0f };
	models_ = models;

	SetCollisionAttribute(CollisionConfig::kCollisionAttributeEnemy);
	SetCollisionMask(CollisionConfig::kCollisionMaskEnemy);
}

void Enemy::Update() {

	worldTransformBase_.UpdateMatrix();
	if (life_ == 0) {
		isDead_ = true;
	}

	ImGui::Begin("Enemy");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::Text("Life : %d", life_);
	ImGui::End();
}

void Enemy::Draw(const ViewProjection& viewProjection) {
	if (!isDead_) {
		models_[0]->Draw(worldTransformBase_, viewProjection, { 1.0f,1.0f,1.0f,1.0f });;
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