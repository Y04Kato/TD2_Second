#include "Player.h"
#include "components/utilities/collisionManager/CollisionConfig.h"

void Player::Initialize(const std::vector<Model*>& models, uint32_t textureHandle) {
	worldTransformBase_.Initialize();
	//worldTransformBase_.translation_.num[1] = -2.0f;
	worldTransformBase_.scale_ = { 3.0f,3.0f,3.0f };
	models_ = models;
	modelMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	input_ = Input::GetInstance();
	textureHandle_ = textureHandle;

	debugCamera_ = DebugCamera::GetInstance();

	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(CollisionConfig::kCollisionMaskPlayer);
}

void Player::Update() {
	
	Move();
	if (jump_) {
		Jump();
	}
	Attack();

	if (input_->TriggerKey(DIK_W)) {
		velocity_.num[1] = kJumpFirstSpeed;
		
		jump_ = true;
	}

	worldTransformBase_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::Text("Life : %d", life_);
	ImGui::Text("Attack Push::P");
	
	ImGui::DragFloat("FirstSpeed",& kJumpFirstSpeed, 0.1f);
	ImGui::DragFloat("Gravity",& kGravity, 0.01f);
	ImGui::DragFloat("Width",& jumpWidth_, 0.1f);
	switch (lane_) {
	case Obstacle::Lane::Left:
		ImGui::Text("Left");
		break;
	case Obstacle::Lane::Middle:
		ImGui::Text("Middle");
		break;
	case Obstacle::Lane::Right:
		ImGui::Text("Right");
		break;
	}
	ImGui::End();

	debugCamera_->SetMovingSpeed(Vector3{ moveSpeed_,0.0f,0.0f });

	if (isDamageFlag_ == true) {
		shakeTimer_++;
		if (shakeTimer_ <= 60) {
			debugCamera_->ShakeCamera(4, 6);
		}
		else {
			isDamageFlag_ = false;
		}
	}

	if (input_->TriggerKey(DIK_SPACE)) {
		if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
			debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0],22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.05f);
			isSideScroll_ = false;
		}
		else {
			debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.05f);
			isSideScroll_ = true;
		}
	}

	if (isAccelerationFlag_ == true) {//加速した時
		accelerationTimer_++;
		if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
			debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0] + 10.0f,2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);
			if (accelerationTimer_ >= 120) {
				debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);
				isAccelerationFlag_ = false;
			}
		}
		else {
			debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0] - 30.0f,22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.06f);
			if (accelerationTimer_ >= 120) {
				debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0],22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.06f);
				isAccelerationFlag_ = false;
			}
		}
	}

	if (isDecelerationFlag_ == true) {//減速した時
		decelerationTimer_++;
		if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
			debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0] - 10.0f,2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);
			if (decelerationTimer_ >= 120) {
				debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);
				isDecelerationFlag_ = false;
			}
		}
		else {
			debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0] + 30.0f,22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.06f);
			if (decelerationTimer_ >= 120) {
				debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0],22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.06f);
				isDecelerationFlag_ = false;
			}
		}
	}

	if (isDamageFlag_ == false) {//ダメージを受けていない時
		if (isSideScroll_ == true) {//横スクロール中
			debugCamera_->SetCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f });
		}
		else {
			debugCamera_->SetCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0],22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f });
			if (input_->TriggerKey(DIK_D)) {
				cameraDistance_ -= 15.0f;
			}
			else if (input_->TriggerKey(DIK_A)) {
				cameraDistance_ += 15.0f;
			}
			if (cameraDistance_ <= -15.0f) {
				cameraDistance_ = -15.0f;
			}
			else if (cameraDistance_ >= 15.0f) {
				cameraDistance_ = 15.0f;
			}
		}
	}

}

void Player::Draw(const ViewProjection& view) {
	models_[0]->Draw(worldTransformBase_, view, modelMaterial_);

	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(view);
	}
}

void Player::Move() {
	//横スクロール視点移動
	velocity_.num[0] = moveSpeed_;
	//if (worldTransformBase_.translation_.num[0] > 20.0f) {
	//	worldTransformBase_.translation_.num[0] = -40.0f;
	//}

	//俯瞰視点移動
	if (!jump_) {
		if (input_->TriggerKey(DIK_D) && worldTransformBase_.translation_.num[2] >= -14.5f && isSideScroll_ == false) {
			velocity_.num[1] = kJumpFirstSpeed;
			velocity_.num[2] = -jumpWidth_;
			jump_ = true;
			if (lane_ == Obstacle::Lane::Middle) {
				lane_ = Obstacle::Lane::Right;
			}
			if (lane_ == Obstacle::Lane::Left) {
				lane_ = Obstacle::Lane::Middle;
			}
		}
		else if (input_->TriggerKey(DIK_A) && worldTransformBase_.translation_.num[2] <= 14.5f && isSideScroll_ == false) {
			velocity_.num[1] = kJumpFirstSpeed;
			velocity_.num[2] = jumpWidth_;
			jump_ = true;
			if (lane_ == Obstacle::Lane::Middle) {
				lane_ = Obstacle::Lane::Left;
			}
			if (lane_ == Obstacle::Lane::Right) {
				lane_ = Obstacle::Lane::Middle;
			}
		}
	}



	// 移動
	worldTransformBase_.translation_ += velocity_;


}

void Player::Attack() {
	if (input_->PressKey(DIK_P)&& !jump_ ) {
		--fireTimer_;
	}
	else {
		fireTimer_ = 1;
	}

	if (fireTimer_ == 0) {
		// 弾の速度
		const float kBulletSpeed = 1.0f;
		Vector3 velocity = { kBulletSpeed, 0, 0 };


		// 弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity, textureHandle_);
		newBullet->SetObstacleMode(mode_);

		bullets_.push_back(newBullet);

		fireTimer_ = 20;
	}

	// デスフラグが立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	for (PlayerBullet* bullet : bullets_) {

		bullet->Update();
	}

}

void Player::Jump() {
	// 加速ベクトル
	Vector3 accelerationVector = { 0, -kGravity, 0 };
	// 加速
	velocity_ += accelerationVector;

	if (worldTransformBase_.translation_.num[1] <= 0.0f) {
		worldTransformBase_.translation_.num[1] = 0.0f;
		velocity_.num[1] = 0.0f;
		velocity_.num[2] = 0.0f;
		jump_ = false;
	}
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.num[0] = worldTransformBase_.matWorld_.m[3][0];
	worldPos.num[1] = worldTransformBase_.matWorld_.m[3][1];
	worldPos.num[2] = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision(const Collider* collider) {
	isHit = true;
	if (mode_ == Obstacle::Mode::None) {
		life_--;
		isDamageFlag_ = true;
		shakeTimer_ = 0;
	}
	if (mode_ == Obstacle::Mode::Acceleration) {
		moveSpeed_ += 0.2f;
		isAccelerationFlag_ = true;
		accelerationTimer_ = 0;
	}
	if (mode_ == Obstacle::Mode::Deceleration) {
		moveSpeed_ -= 0.2f;
		if (moveSpeed_ <= 0.0f) {//速度が0になった時、これ以上下がらないようにする
			moveSpeed_ = 0.1f;
		}
		isDecelerationFlag_ = true;
		decelerationTimer_ = 0;
	}
	if (mode_ == Obstacle::Mode::HealLife) {
		life_++;
	}
}