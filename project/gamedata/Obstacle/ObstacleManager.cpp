#include "ObstacleManager.h"

void ObstacleManager::Initialize(CreateSphere* sphere, uint32_t textureHandle) {
	assert(sphere);
	sphere_ = sphere;
	textureHandle_ = textureHandle;
	std::random_device seedGenerator;
	randomEngine_ = std::mt19937(seedGenerator());
}

void ObstacleManager::Update() {

	if (--obstacleSpawnTimer_ <= 0.0f) {
		obstacleSpawnTimer_ = kObstacleSpawnInterval;
		Obstacle* obstacle = new Obstacle();
		int lane = GetRandomInt(0, 2);
		switch (lane) {
		case 0:
			obstacle->Initialize({ 30.0f,0.0f,0.0f }, Obstacle::Lane::Left);
			break;
		case 1:
			obstacle->Initialize({ 30.0f,0.0f,0.0f }, Obstacle::Lane::Middle);
			break;
		case 2:
			obstacle->Initialize({ 30.0f,0.0f,0.0f }, Obstacle::Lane::Right);
			break;
		}
		AddObstacle(obstacle);
	}

	if (cameraChenge_) {
		cameraMode_ = CameraMode::Vertical;
	}
	else {
		cameraMode_ = CameraMode::Horizontal;
	}

	//カメラが切り替わったらレーンを変える
	if (cameraMode_ == CameraMode::Vertical) {
		for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
			switch (obstacle->GetLane()) {
			case Obstacle::Lane::Left:
				obstacle->SetPositionZ(-5.0f);
				break;
			case Obstacle::Lane::Middle:
				obstacle->SetPositionZ(0.0f);
				break;
			case Obstacle::Lane::Right:
				obstacle->SetPositionZ(5.0f);
				break;
			}
		}
	}
	else if (cameraMode_ == CameraMode::Horizontal) {
		for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
			obstacle->SetPositionZ(0.0f);
		}
	}

	//死亡フラグが立った障害物を削除
	obstacles_.remove_if([](std::unique_ptr<Obstacle>& obstacle) {
		if (obstacle->IsDead()) {
			obstacle.reset();
			return true;
		}
		return false;
	});

	//障害物の更新
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		obstacle->Update();
	}

	ImGui::Begin("Obstacle");
	ImGui::Checkbox("CameraChenge", &cameraChenge_);
	ImGui::End();
}

void ObstacleManager::Draw(const ViewProjection& viewProjection) {
	//障害物の描画
	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		sphere_->Draw(obstacle->GetWorldTransform(), viewProjection, sphereMaterial_, textureHandle_);
	}
}

void ObstacleManager::AddObstacle(Obstacle* obstacle) {
	//障害物をリストに追加
	obstacles_.push_back(std::unique_ptr<Obstacle>(obstacle));
}

int ObstacleManager::GetRandomInt(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(randomEngine_);
}