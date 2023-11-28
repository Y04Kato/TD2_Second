#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Bullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"

#include "components/debugcamera/DebugCamera.h"

#include "Obstacle/Obstacle.h"

#include "components/audio/Audio.h"

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
	void OnCollision(const Collider* collider) override;

	void SetObstacleMode(int mode) { mode_ = mode; };

	float GetMoveSpeed() { return moveSpeed_; }

	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };

	int GetLane() { return lane_; };

	int GetLife() { return life_; }

	const std::list<PlayerBullet*> GetPlayerBullet() const { return bullets_; };

	void Reset();

	int GetLife() { return life_; };

	~Player();

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	std::unique_ptr<Model> model_;
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

	float moveSpeed_ = 0.4f;

	int shakeTimer_ = 0;
	bool isDamageFlag_ = false;

	int accelerationTimer_ = 0;
	bool isAccelerationFlag_ = false;

	int decelerationTimer_ = 0;
	bool isDecelerationFlag_ = false;

	bool isSideScroll_ = true;

	float cameraDistance_ = 0.0f;

	int lane_ = Obstacle::Lane::Middle;

	DebugCamera* debugCamera_;

	Audio* audio_;
	SoundData soundData1_;
	SoundData soundData2_;
	SoundData soundData3_;
	SoundData soundData4_;
	SoundData soundData5_;
	SoundData soundData6_;
	SoundData soundData7_;
};