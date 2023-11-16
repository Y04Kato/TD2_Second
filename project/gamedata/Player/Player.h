#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Bullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"

#include "components/debugcamera/DebugCamera.h"

#include "Obstacle/Obstacle.h"

class Player : public Collider {
public:
	void Initialize(const std::vector<Model*>& models, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& view);

	void Move();

	void Attack();

	void Jump();

	WorldTransform GetWorldTransform() override { return worldTransformBase_; }

	//ワールド座標
	Vector3 GetWorldPosition();
	//当たり判定
	void OnCollision() override;

	void SetObstacleMode(int mode) { mode_ = mode; };

	float GetMoveSpeed() { return moveSpeed_; }

	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	Vector4 modelMaterial_;

	//
	Vector3 velocity_;
	//ジャンプ初速
	float kJumpFirstSpeed = 0.8f;
	// 重力加速度
	float kGravity = 0.05f;
	//ジャンプの幅
	float jumpWidth_ = 0.46f;
	//ジャンプフラグ
	bool jump_ = false;

	bool isHit;

	int life_ = 3;

	//弾
	std::list<PlayerBullet*> bullets_;
	uint32_t textureHandle_ = 0u;

	//攻撃時間
	int fireTimer_ = 1;

	int mode_;

	float moveSpeed_ = 0.1f;

	int shakeTimer_ = 0;
	bool isDamageFlag_ = false;

	int accelerationTimer_ = 0;
	bool isAccelerationFlag_ = false;

	int decelerationTimer_ = 0;
	bool isDecelerationFlag_ = false;

	bool isSideScroll_ = true;

	float cameraDistance_ = 0.0f;

	DebugCamera* debugCamera_;
};