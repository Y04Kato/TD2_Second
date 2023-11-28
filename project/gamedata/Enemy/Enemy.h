#pragma once

#include "Model.h"
#include "WorldTransform.h"
#include "ViewProjection.h"
#include "components/utilities/collisionmanager/Collider.h"
#include <memory>

#include "components/audio/Audio.h"

class Enemy : public Collider {
public:
	void Initialize(const std::vector<Model*>& models, const std::vector<Model*>& models2);
	void Update();
	void Draw(const ViewProjection& viewProjection);

	WorldTransform GetWorldTransform()override { return worldTransformBase_; }
	void SetPositionX(float pos) { worldTransformBase_.translation_.num[0] = pos; };
	//当たり判定

	void OnCollision(const Collider* collider) override;

	void SetSideScroll(bool isSideScroll) { isSideScroll_ = isSideScroll; }

	int GetLife() { return life_; };

	void SetLife(int life) { life_ = life; };

	~Enemy();

private:
	std::vector<Model*> models_;
	std::vector<Model*> models2_;
	std::vector<Model*> models3_;
	WorldTransform worldTransformBase_;
	WorldTransform worldTransformTire_;
	WorldTransform worldTransformTire2_;
	bool isDead_ = false;
	bool isSideScroll_;
	int life_ = 3;

	Audio* audio_;
	SoundData soundData1_;
};