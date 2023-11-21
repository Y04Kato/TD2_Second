#include "EnemyManager.h"

void EnemyManager::Initialize() {
	enemy_ = std::make_unique <Enemy>();
	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/player", "cube.obj"));
	std::vector<Model*>enemyModels = { model_.get() };
	enemy_->Initialize(enemyModels);
}

void EnemyManager::Update() {

	enemy_->SetSideScroll(isSideScroll_);
	enemy_->Update();

}

void EnemyManager::Draw(const ViewProjection& viewProjection) {
	if (isSideScroll_) {
		enemy_->Draw(viewProjection);
	}
}
	
//ワールド座標
Vector3 EnemyManager::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.num[0] = enemy_->GetWorldTransform().matWorld_.m[3][0];
	worldPos.num[1] = enemy_->GetWorldTransform().matWorld_.m[3][1];
	worldPos.num[2] = enemy_->GetWorldTransform().matWorld_.m[3][2];
	return worldPos;
}

void EnemyManager::SetPlayerPosition(const Vector3& position) {
	enemy_->SetPositionX(position.num[0] +30);
}