#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Bullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"

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

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	Vector4 modelMaterial_;

	//速度
	Vector3 velocity_ = {};
	//ジャンプフラグ
	bool jump_;
	//ジャンプ初速
	float kJumpFirstSpeed = 3.45f;
	//ジャンプの幅
	float jumpWidth_ = 0.8f;
	// 重力加速度  ジャンプの長さ
	float kGravity = 0.4f;

	//ヒットフラグ
	bool isHit;
	//ライフ
	int life_ = 3;

	//弾
	std::list<PlayerBullet*> bullets_;
	uint32_t textureHandle_ = 0u;

	//攻撃時間
	int fireTimer_ = 1;
};