#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"
#include <memory>

class Enemy : public Collider {
public:
	void Initialize(const std::vector<Model*>& models);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	WorldTransform GetWorldTransform() override { return worldTransformBase_; }
	void SetPositionX(float pos) { worldTransformBase_.translation_.num[0] = pos; };
	//当たり判定
	void OnCollision(const Collider* collider)override;

private:
	std::vector<Model*> models_;
	WorldTransform worldTransformBase_;
	bool isDead_ = false;
	int life_ = 3;
};