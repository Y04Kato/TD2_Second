#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "Bullet/PlayerBullet.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"

#include "Obstacle/Obstacle.h"

class Player : public Collider {
public:
	void Initialize(const std::vector<Model*>& models, uint32_t textureHandle);

	void Update();

	void Draw(const ViewProjection& view);

	void Move();

	void Attack();

	WorldTransform GetWorldTransform() override { return worldTransformBase_; }

	//ワールド座標
	Vector3 GetWorldPosition();
	//当たり判定
	void OnCollision() override;

	void SetObstacleMode(int mode) { mode_ = mode; };

	float GetMoveSpeed() { return moveSpeed; }

	void SetIsSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; };

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	Vector4 modelMaterial_;

	bool isHit;

	int life_ = 3;

	//弾
	std::list<PlayerBullet*> bullets_;
	uint32_t textureHandle_ = 0u;

	//攻撃時間
	int fireTimer_ = 1;

	int mode_;

	float moveSpeed = 0.1f;

	bool isSideScroll_ = false;
};