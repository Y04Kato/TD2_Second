#pragma once
#include "components/3d/CreateSphere.h"
#include "Obstacle.h"
#include <random>

class ObstacleManager {
public:
	static const int kObstacleSpawnInterval = 60 * 5;

	enum class CameraMode {
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

	int GetRandomInt(int min, int max);

private:
	CreateSphere* sphere_ = nullptr;
	Vector4 sphereMaterial_{ 1.0f,1.0f,1.0f,1.0f };
	uint32_t textureHandle_ = 0;
	std::list<std::unique_ptr<Obstacle>> obstacles_{};
	CameraMode cameraMode_ = CameraMode::Horizontal;
	int obstacleSpawnTimer_ = kObstacleSpawnInterval;
	std::mt19937 randomEngine_;
	bool cameraChenge_ = false;
};

