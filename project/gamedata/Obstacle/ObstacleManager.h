#pragma once
#include "Model.h"
#include "Obstacle.h"
#include <random>

class ObstacleManager {
public:
	//障害物の間隔
	static const int kObstacleSpawnInterval = 60 * 8;

	//カメラの状態
	enum CameraMode {
		Vertical,
		Horizontal,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="textureHandle"></param>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 障害物を追加
	/// </summary>
	/// <param name="obstacle"></param>
	void AddObstacle(Obstacle* obstacle);

	/// <summary>
	/// 障害物を追加
	/// </summary>
	/// <param name="position"></param>
	/// <param name="lane"></param>
	/// <param name="mode"></param>
	void AddObstacle(const Vector3& position, Obstacle::Lane lane, Obstacle::Mode mode);

	/// <summary>
	/// 障害物のリストを取得
	/// </summary>
	/// <returns></returns>
	const std::list<std::unique_ptr<Obstacle>>& GetObstacles() const { return obstacles_; };

	/// <summary>
	/// ランダムな値を取得
	/// </summary>
	/// <param name="min"></param>
	/// <param name="max"></param>
	/// <returns></returns>
	int GetRandomInt(int min, int max);

	/// <summary>
	/// カメラの状態を取得
	/// </summary>
	/// <param name="isSideScroll"></param>
	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };

	/// <summary>
	/// プレイヤーの座標を取得
	/// </summary>
	/// <param name="position"></param>
	void SetPlayerPosition(const Vector3& position) { playerPos_ = position; };

	/// <summary>
	/// カメラの座標を取得
	/// </summary>
	/// <param name="position"></param>
	void SetCameraPosition(const Vector3& position) { cameraPos_ = position; };

	/// <summary>
	/// プレイヤーのレーンを設定
	/// </summary>
	/// <param name="lane"></param>
	void SetLane(int lane) { lane_ = lane; };

private:
	//モデル
	std::unique_ptr <Model> noneModel_;
	std::unique_ptr <Model> accelerationModel_;
	std::unique_ptr <Model> decelerationModel_;
	std::unique_ptr <Model> healLifeModel_;
	//モデルの色
	Vector4 sphereMaterial_{ 1.0f,1.0f,1.0f,1.0f };
	//障害物のリスト
	std::list<std::unique_ptr<Obstacle>> obstacles_{};
	//カメラモード
	int cameraMode_ = Horizontal;
	//障害物のスポーンタイマー
	int obstacleSpawnTimer_ = 0;
	//2Dモードの生成位置
	Vector3 obstacleSpawnPosition_{ 200.0f,0.0f,0.0f };
	//レーンごとの座標
	float lanePosition_[3] = { 15.0f,0.0f,-15.0f };
	//ランダムエンジン
	std::mt19937 randomEngine_;
	//カメラ切り替えのフラグ
	bool isSideScroll_ = false;
	//プレイヤーの座標
	Vector3 playerPos_{ 0.0f,0.0f,0.0f };
	//カメラの座標
	Vector3 cameraPos_{ 0.0f,0.0f,0.0f };
	//プレイヤーのレーン
	int lane_ = Obstacle::Lane::Middle;
};

