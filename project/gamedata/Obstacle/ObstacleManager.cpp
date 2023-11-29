#include "ObstacleManager.h"
#include "components/math/MatrixCalculation.h"

void ObstacleManager::Initialize() {
	noneModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/none", "None.obj"));
	accelerationModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/acceleration", "Acceleration.obj"));
	decelerationModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/deceleration", "Deceleration.obj"));
	healLifeModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/obstacle/healLife", "HealLife.obj"));

	std::random_device seedGenerator;
	randomEngine_ = std::mt19937(seedGenerator());

	InitializeObstacles0();
	InitializeObstacles1();
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

	if (playerPos_.num[0] >= 1000 && isSetObstacles2 == false) {
		InitializeObstacles2();
		isSetObstacles2 = true;
	}
	if (playerPos_.num[0] >= 2000 && isSetObstacles3 == false) {
		InitializeObstacles3();
		isSetObstacles3 = true;
	}
	if (playerPos_.num[0] >= 3000 && isSetObstacles4 == false) {
		InitializeObstacles4();
		isSetObstacles4 = true;
	}
	if (playerPos_.num[0] >= 4000 && isSetObstacles5 == false) {
		InitializeObstacles5();
		isSetObstacles5 = true;
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
	InitializeObstacles0();
	InitializeObstacles1();

	isSetObstacles1 = false;
	isSetObstacles2 = false;
	isSetObstacles3 = false;
	isSetObstacles4 = false;
	isSetObstacles5 = false;
}

void ObstacleManager::InitializeObstacles0() {
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
	AddObstacle({ 890.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 890.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 890.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 910.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 920.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 950.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 960.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 970.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 990.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
}

void ObstacleManager::InitializeObstacles1() {
	AddObstacle({ 1020.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1020.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1020.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 1050.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::HealLife);
	AddObstacle({ 1050.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 1070.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 1090.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1090.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1120.0f, 0.0f, 0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1120.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1130.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 1140.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1140.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1150.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 1160.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1160.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1170.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 1190.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 1250.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1250.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1270.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1270.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 1290.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 1290.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1320.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1320.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1350.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1350.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1400.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1400.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1450.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1450.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1500.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1500.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1470.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 1530.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1560.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1560.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1590.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1620.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1620.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1580.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::HealLife);
	AddObstacle({ 1580.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 1610.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1610.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1650.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1650.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1690.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1690.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1630.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 1730.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1730.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1680.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 1750.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1770.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 1790.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1830.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 1860.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1860.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1890.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 1920.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 1940.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 1940.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 1970.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
}

void ObstacleManager::InitializeObstacles2() {
	AddObstacle({ 2000.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2000.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2040.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2060.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 2060.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 2090.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 2120.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2120.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2150.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2150.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 2180.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2210.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 2260.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2260.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2290.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 2330.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2360.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 2360.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2380.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 2410.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2450.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 2480.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2480.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2530.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2550.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2580.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2620.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2650.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2650.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2680.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2700.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 2730.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2760.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2760.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 2790.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2830.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2830.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2860.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2890.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 2920.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2920.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 2950.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 2970.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 2990.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
}

void ObstacleManager::InitializeObstacles3() {
	//X3000以降
	AddObstacle({ 3020.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3030.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3055.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 3080.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3100.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 3125.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3135.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 3150.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Deceleration);
	AddObstacle({ 3160.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3185.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3195.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 3210.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 3215.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3230.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3255.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3255.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 3275.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3275.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 3305.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3325.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3330.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3335.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3345.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 3365.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3375.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 3380.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3400.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 3435.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3440.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3460.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3465.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3480.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3485.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3500.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3505.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3515.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 3525.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3525.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3550.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3555.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3575.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 3590.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 3595.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3600.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 3625.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3625.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3640.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Deceleration);
	AddObstacle({ 3640.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 3665.0f, 0.0f, 0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3665.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Acceleration);
	AddObstacle({ 3690.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3690.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 3700.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3700.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3725.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3725.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3750.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::Acceleration);
	AddObstacle({ 3750.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3750.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 3760.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Acceleration);
	AddObstacle({ 3770.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3780.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::Deceleration);
	AddObstacle({ 3790.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3800.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3810.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3830.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3830.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3850.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3850.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3870.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3870.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3890.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 3890.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3890.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3910.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3920.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3950.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 3960.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 3970.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 3990.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
}

void ObstacleManager::InitializeObstacles4() {
	AddObstacle({ 4020.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4020.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4020.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::HealLife);
	AddObstacle({ 4050.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::HealLife);
	AddObstacle({ 4050.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 4070.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 4090.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4090.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4120.0f, 0.0f, 0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4120.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4130.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 4140.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4140.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4150.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 4160.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4160.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4170.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Deceleration);
	AddObstacle({ 4190.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::Acceleration);
	AddObstacle({ 4250.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4250.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	AddObstacle({ 4270.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4270.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 4290.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::HealLife);
	AddObstacle({ 4290.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4320.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4320.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4350.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4350.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4400.0f,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
	AddObstacle({ 4400.0f,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	AddObstacle({ 4400.0f,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	for (int i = 0; i < 15; i++) {
		AddObstacle({ 4500.0f + i * 30,0.0f,0.0f }, Obstacle::Lane::Left, Obstacle::Mode::None);
		AddObstacle({ 4500.0f + i * 30,0.0f,0.0f }, Obstacle::Lane::Right, Obstacle::Mode::None);
	}
}

void ObstacleManager::InitializeObstacles5() {
	for (int i = 0; i < 30; i++) {
		AddObstacle({ 5100.0f + i * 30,0.0f,0.0f }, Obstacle::Lane::Middle, Obstacle::Mode::None);
	}
}