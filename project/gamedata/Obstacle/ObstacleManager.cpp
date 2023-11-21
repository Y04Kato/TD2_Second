#include "ObstacleManager.h"
#include "components/math/MatrixCalculation.h"

void ObstacleManager::Initialize(CreateSphere* sphere, uint32_t textureHandle) {
	assert(sphere);
	sphere_ = sphere;
	textureHandle_ = textureHandle;
	std::random_device seedGenerator;
	randomEngine_ = std::mt19937(seedGenerator());
	Obstacle* obstacles[3]{};
	for (int i = 0; i < 3; i++) {
		obstacles[i] = new Obstacle();
		switch (i) {
		case Obstacle::Lane::Left:
			obstacles[i]->Initialize({ 10 + 0.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
			break;
		case Obstacle::Lane::Middle:
			obstacles[i]->Initialize({ 10 + 10.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
			break;
		case Obstacle::Lane::Right:
			obstacles[i]->Initialize({ 10 + 0.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
			break;
		}
		AddObstacle(obstacles[i]);
	}
}

void ObstacleManager::Update() {

	//if (--obstacleSpawnTimer_ <= 0) {
	//	obstacleSpawnTimer_ = kObstacleSpawnInterval;
	//	Obstacle* obstacle = new Obstacle();
	//	int lane = GetRandomInt(0, 2);
	//	switch (lane) {
	//	case Obstacle::Lane::Left:
	//		obstacle->Initialize(Add(playerPos_, obstacleSpawnPosition_), Obstacle::Lane::Left);
	//		break;
	//	case Obstacle::Lane::Middle:
	//		obstacle->Initialize(Add(playerPos_, obstacleSpawnPosition_), Obstacle::Lane::Middle);
	//		break;
	//	case Obstacle::Lane::Right:
	//		obstacle->Initialize(Add(playerPos_, obstacleSpawnPosition_), Obstacle::Lane::Right);
	//		break;
	//	}
	//	AddObstacle(obstacle);
	//}

	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		//障害物のレーンを取得
		int obstacleLane = obstacle->GetLane();
		//横スクロールの時
		if (isSideScroll_) {
			//プレイヤーのレーンに合わせて障害物を移動させる
			switch (lane_) {
			case Obstacle::Lane::Left:
				if (obstacleLane == Obstacle::Lane::Middle || obstacleLane == Obstacle::Lane::Right) {
					obstacle->SetPositionZ(lanePosition_[0]);
				}
				break;
			case Obstacle::Lane::Middle:
				if (obstacleLane == Obstacle::Lane::Left || obstacleLane == Obstacle::Lane::Right) {
					obstacle->SetPositionZ(lanePosition_[1]);
				}
				break;
			case Obstacle::Lane::Right:
				if (obstacleLane == Obstacle::Lane::Left || obstacleLane == Obstacle::Lane::Middle) {
					obstacle->SetPositionZ(lanePosition_[2]);
				}
				break;
			}
		}
		//俯瞰カメラの時
		else {
			switch (obstacleLane) {
			case Obstacle::Lane::Left:
				obstacle->SetPositionZ(lanePosition_[0]);
				break;
			case Obstacle::Lane::Middle:
				obstacle->SetPositionZ(lanePosition_[1]);
				break;
			case Obstacle::Lane::Right:
				obstacle->SetPositionZ(lanePosition_[2]);
				break;
			}
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
		obstacle->GetCameraPosition(cameraPos_);
	}
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