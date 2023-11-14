#pragma once
#include "components/3d/WorldTransform.h"
#include "components/utilities/collisionmanager/Collider.h"

class Obstacle : public Collider {
public:
	enum Lane {
		Left,
		Middle,
		Right,
	};

	enum Mode {
		None,//効果なし
		Acceleration,//加速
		Deceleration,//減速
		Recuperation,//プレイヤー回復
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position"></param>
	void Initialize(const Vector3& position, int lane, int mode);

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
	int GetLane() { return lane_; };

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

	/// <summary>
	/// カメラ座標を取得
	/// </summary>
	/// <param name="position"></param>
	void GetCameraPosition(const Vector3& position) { cameraPos_ = position; };

	/// <summary>
	/// モードを取得
	/// </summary>
	int GetObstacleMode() { return mode_; }

private:
	WorldTransform worldTransform_;
	int lane_ = Middle;
	int mode_ = None;
	bool isDead_ = false;
	Vector3 cameraPos_{ 0.0f,0.0f,0.0f };
};

