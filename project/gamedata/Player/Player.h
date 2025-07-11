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
#include "components/2d/CreateParticle.h"
#include "PlayerParticle/PlayerParticle.h"

#include "CreateParticle.h"

class Player : public Collider {
public:
	void Initialize(const std::vector<Model*>& models,uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& view);
	void ParticleDraw(const ViewProjection& view);

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

	float GetVelocitySpeed() { return velocity_.num[0]; }

	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };

	int GetLane() { return lane_; };

	int GetLife() { return life_; }

	const std::list<PlayerBullet*> GetPlayerBullet() const { return bullets_; };

	void Reset();

	void bulletParticle();
	void smokeParticle();

	~Player();

	void DrawParticle(const ViewProjection& viewProjection);

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	
	std::list<PlayerParticle*> bulletParticle_;
	std::list<PlayerParticle*> smokeParticle_;
	std::unique_ptr <CreateParticle> particle_;
	Emitter emitter{};
	AccelerationField field;

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

	int life_ = 5;

	//弾
	std::list<PlayerBullet*> bullets_;
	uint32_t textureHandle_;

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

	//パーティクル
	struct ParticleInfo {
		std::unique_ptr<CreateParticle> particle = nullptr;
		bool isActive = false;
		int activeTimer = 0;
	};

	//回復パーティクル
	const int kHealParticleActiveTime = 60 * 5;
	ParticleInfo healParticle_{};
	uint32_t healParticleTextureHandle_ = 0;

	float cameraPosY = 12.7f;
	float cameraRotX = 0.1f;

	int time_ = 30;

	//const int kAccelerationTime = 120;
	//const int kDecelerationTime = 120;
	const int kAccelerationTime = 60;
	const int kDecelerationTime = 60;
};