#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"

#include "Obstacle/Obstacle.h"

class Player {
public:
	void Initialize(const std::vector<Model*>& models);

	void Update();

	void Draw(const ViewProjection& view);

	void Move();

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	//ワールド座標
	Vector3 GetWorldPosition();
	//当たり判定
	void OnCollision();

	void SetObstacleMode(int obstacleMode) { obstacleMode_ = obstacleMode; };

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	Vector4 modelMaterial_;

	float moveSpeed_;

	bool isHit;

	int obstacleMode_;
};