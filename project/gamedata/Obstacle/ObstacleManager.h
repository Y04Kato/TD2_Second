#pragma once
#include "components/3d/CreateSphere.h"
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
	void Initialize(CreateSphere* sphere, uint32_t textureHandle);

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
	void SetIsSideScroll(bool isSideScroll) { cameraChenge_ = isSideScroll; };

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

private:
	//モデル
	CreateSphere* sphere_ = nullptr;
	//モデルの色
	Vector4 sphereMaterial_{ 1.0f,1.0f,1.0f,1.0f };
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//障害物のリスト
	std::list<std::unique_ptr<Obstacle>> obstacles_{};
	//カメラモード
	int cameraMode_ = Horizontal;
	//障害物のスポーンタイマー
	int obstacleSpawnTimer_ = 0;
	//2Dモードの生成位置
	Vector3 obstacleSpawnPosition_{ 60.0f,0.0f,0.0f };
	//レーンごとの座標
	float lanePosition_[3] = { 15.0f,0.0f,-15.0f };
	//ランダムエンジン
	std::mt19937 randomEngine_;
	//カメラ切り替えのフラグ
	bool cameraChenge_ = false;
	//プレイヤーの座標
	Vector3 playerPos_{ 0.0f,0.0f,0.0f };
	//カメラの座標
	Vector3 cameraPos_{ 0.0f,0.0f,0.0f };
};

