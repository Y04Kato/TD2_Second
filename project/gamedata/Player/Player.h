#pragma once
#include "WorldTransform.h"
#include "Model.h"
#include "Input.h"
#include "ViewProjection.h"


class Player {
public:
	void Initialize(const std::vector<Model*>& models);

	void Update();

	void Draw(const ViewProjection& view);

	const WorldTransform& GetWorldTransform() { return worldTransformBase_; }

	//ワールド座標
	Vector3 GetWorldPosition();
	//当たり判定
	void OnCollision();

private:
	WorldTransform worldTransformBase_;
	Input* input_ = nullptr;

	std::vector<Model*> models_;
	Vector4 modelMaterial_;

	bool isHit;
};