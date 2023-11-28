#include "ObstacleManager.h"
#include "components/math/MatrixCalculation.h"

void ObstacleManager::Initialize() {
	noneModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/none", "None.obj"));
	accelerationModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/acceleration", "Acceleration.obj"));
	decelerationModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/deceleration", "Deceleration.obj"));
	healLifeModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/healLife", "HealLife.obj"));

	std::random_device seedGenerator;
	randomEngine_ = std::mt19937(seedGenerator());

	InitializeObstacles();
}

void ObstacleManager::Update() {

	//if (--obstacleSpawnTimer_ <= 0) {
	//	obstacleSpawnTimer_ = kObstacleSpawnInterval;
	//	Obstacle* obstacle = new Obstacle();
	//	Vector3 position = Add(playerPos_, obstacleSpawnPosition_);
	//	position.num[1] = 0.0f;
	//	int lane = GetRandomInt(0, 2);
	//	int mode = GetRandomInt(0, 2);
	//	obstacle->Initialize(position, lane, mode);
	//	AddObstacle(obstacle);
	//}

	for (std::unique_ptr<Obstacle>& obstacle : obstacles_) {
		//障害物のレーンを取得
		int obstacleLane = obstacle->GetLane();
		//横スクロールの時
		if (isSideScroll_) {
			////プレイヤーのレーンに合わせて障害物を移動させる
			//switch (lane_) {
			//case Obstacle::Lane::Left:
			//	if (obstacleLane == Obstacle::Lane::Middle || obstacleLane == Obstacle::Lane::Right) {
			//		obstacle->SetPositionZ(lanePosition_[0]);
			//	}
			//	break;
			//case Obstacle::Lane::Middle:
			//	if (obstacleLane == Obstacle::Lane::Left || obstacleLane == Obstacle::Lane::Right) {
			//		obstacle->SetPositionZ(lanePosition_[1]);
			//	}
			//	break;
			//case Obstacle::Lane::Right:
			//	if (obstacleLane == Obstacle::Lane::Left || obstacleLane == Obstacle::Lane::Middle) {
			//		obstacle->SetPositionZ(lanePosition_[2]);
			//	}
			//	break;
			//}
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
		//横スクロールの時はプレイヤーのいるレーンだけ描画する
		if (isSideScroll_) {
			if (lane_ == obstacle->GetLane()) {
				if (obstacle->GetMode() == Obstacle::Mode::None) {
					noneModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
				}
				if (obstacle->GetMode() == Obstacle::Mode::Acceleration) {
					accelerationModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
				}
				if (obstacle->GetMode() == Obstacle::Mode::Deceleration) {
					decelerationModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
				}
				if (obstacle->GetMode() == Obstacle::Mode::HealLife) {
					healLifeModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
				}
			}
		}
		//縦スクロールの時はすべてを描画
		else {
			if (obstacle->GetMode() == Obstacle::Mode::None) {
				noneModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
			}
			if (obstacle->GetMode() == Obstacle::Mode::Acceleration) {
				accelerationModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
			}
			if (obstacle->GetMode() == Obstacle::Mode::Deceleration) {
				decelerationModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
			}
			if (obstacle->GetMode() == Obstacle::Mode::HealLife) {
				healLifeModel_->Draw(obstacle->GetWorldTransform(), viewProjection, Vector4{ 1.0f,1.0f,1.0f,1.0f });
			}
		}
	}
}

void ObstacleManager::AddObstacle(Obstacle* obstacle) {
	//障害物をリストに追加
	obstacles_.push_back(std::unique_ptr<Obstacle>(obstacle));
}

void ObstacleManager::AddObstacle(const Vector3& position, Obstacle::Lane lane, Obstacle::Mode mode) {
	Obstacle* newObstacle = new Obstacle();
	newObstacle->Initialize(position, lane, mode);
	obstacles_.push_back(std::unique_ptr<Obstacle>(newObstacle));
}

int ObstacleManager::GetRandomInt(int min, int max) {
	std::uniform_int_distribution<int> distribution(min, max);
	return distribution(randomEngine_);
}

void ObstacleManager::Reset() {
	obstacles_.clear();
	InitializeObstacles();
}

void ObstacleManager::InitializeObstacles() {
	AddObstacle({ 55.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 80.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 100.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 125.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 135.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 150.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 160.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 185.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Deceleration);
	AddObstacle({ 195.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 210.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 215.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 230.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 255.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 255.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 275.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 275.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 305.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 325.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 330.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Deceleration);
	AddObstacle({ 335.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 345.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 365.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 375.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 380.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 400.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 435.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 440.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 460.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 465.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 480.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 485.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 500.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 505.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 515.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 525.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 525.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 550.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 555.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 575.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 590.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 595.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Deceleration);
	AddObstacle({ 600.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 625.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 625.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 640.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 640.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 665.0f, 0.0f, 0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 665.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Acceleration);
	AddObstacle({ 690.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 690.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 700.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 700.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 725.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 725.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 750.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 750.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 750.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 760.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 770.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 780.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 790.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 800.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 810.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 830.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 830.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 850.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 850.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 870.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 870.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
}