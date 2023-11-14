#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"

class Player : public Collider {
public:
	void Initialize(const std::vector<Model*>& models);

	void Update();

	void Draw(const ViewProjection& view);

	void Move();

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

	bool isHit;

	int life_ = 3;
};