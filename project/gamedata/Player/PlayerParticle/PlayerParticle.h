#pragma once
#include "Input.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "components/utilities/collisionmanager/Collider.h"
#include <Model.h>

class PlayerParticle {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, const Vector3& position, const Vector3& size, const Vector3& rotation, const Vector3& velo);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const ViewProjection& viewprojection);

	bool IsDead() { return isDead_; }

private:
	WorldTransform worldTransform_;

	Model* model_;

	// 速度
	Vector3 velocity_;

	// 寿命
	static const int32_t kLifeTime = 60 * 1;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
