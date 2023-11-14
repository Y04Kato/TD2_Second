#pragma once
#include "components/3d/CreateSphere.h"
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class  PlayerBullet {
public:

	~PlayerBullet();
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& position, const Vector3& velocity, uint32_t textureHandle);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewprojection);

	bool IsDead() const { return isDead_; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

private:
	WorldTransform worldTransform_;
	std::unique_ptr <CreateSphere> sphere_;
	uint32_t textureHandle_ = 0u;
	Vector4 sphereMaterial_{ 1.0f,1.0f,1.0f,1.0f };

	//速度
	Vector3 velocity_;

	//寿命
	static const int32_t kLifeTime = 60 * 5;
	//デスタイマー
	int32_t deathTimer_ = kLifeTime;
	//デスフラグ
	bool isDead_ = false;
};