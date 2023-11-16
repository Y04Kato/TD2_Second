#include "EnemyManager.h"

void EnemyManager::Initialize() {
	enemy_ = std::make_unique <Enemy>();
	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/player", "cube.obj"));
	std::vector<Model*>enemyModels = { model_.get() };
	enemy_->Initialize(enemyModels);
}

void EnemyManager::Update() {
	enemy_->Update();
}

void EnemyManager::Draw(const ViewProjection& viewProjection) {
	if (isSideScroll_) {
		enemy_->Draw(viewProjection);
	}
}
	

void EnemyManager::SetPlayerPosition(const Vector3& position) {
	enemy_->SetPositionX(position.num[0] +30);
}