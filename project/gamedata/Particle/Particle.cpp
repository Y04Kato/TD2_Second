#include "Particle.h"


void Particle::Initialize(Model* model,const Vector3& position) {
	float numberX = (rand() % 11 - 5) / 5.0f;
	float numberY = (rand() % 11 - 5) / 5.0f;
	float numberZ = (rand() % 11 - 5) / 5.0f;
	
	model_ = model;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.scale_ = { 0.5f,0.5f,0.5f };
	worldTransform_.rotation_ = { numberX, numberY, numberZ };
	// 引数で受け取った速度をメンバ変数に代入
	velocity_ = { numberX, numberY, numberZ };



};


void Particle::Update() {
	
	

	//時間経過でデス
	if (--deathTimer_ <= 0) {
		isDead_ = true;
	}

	worldTransform_.translation_ = Add(worldTransform_.translation_, velocity_);

	worldTransform_.UpdateMatrix();

};


void Particle::Draw(const ViewProjection& viewprojection) {
	
	if (!isDead_) {
		model_->Draw(worldTransform_, viewprojection, { 1.0f,1.0f,1.0f,1.0f });
	}
};