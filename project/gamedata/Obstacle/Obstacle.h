#pragma once
#include "components/3d/WorldTransform.h"
#include "components/utilities/collisionmanager/Collider.h"

class Obstacle : public Collider {
public:
	enum class Lane {
		Left,
		Middle,
		Right,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Initialize(const Vector3& position, Lane lane);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 死亡フラグを取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() { return isDead_; };

	/// <summary>
	/// レーンの情報を取得
	/// </summary>
	/// <returns></returns>
	Lane GetLane() { return lane_; };

	/// <summary>
	/// Z座標を設定
	/// </summary>
	/// <param name="pos"></param>
	void SetPositionZ(float pos) { worldTransform_.translation_.num[2] = pos; };

	/// <summary>
	/// 衝突判定
	/// </summary>
	void OnCollision() override;

	/// <summary>
	/// ワールドトランスフォームを取得
	/// </summary>
	/// <returns></returns>
	WorldTransform GetWorldTransform() override { return worldTransform_; };

private:
	WorldTransform worldTransform_;
	Lane lane_ = Lane::Middle;
	bool isDead_ = false;
};

