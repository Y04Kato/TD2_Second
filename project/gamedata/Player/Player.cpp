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

	model_.reset(Model::CreateModelFromObj("project/gamedata/resources/bullet", "Screw.obj"));

	//Audio
	audio_ = Audio::GetInstance();
	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/jump.wav");
	soundData2_ = audio_->SoundLoadWave("project/gamedata/resources/up.wav");
	soundData3_ = audio_->SoundLoadWave("project/gamedata/resources/down.wav");
	soundData4_ = audio_->SoundLoadWave("project/gamedata/resources/damage.wav");
	soundData5_ = audio_->SoundLoadWave("project/gamedata/resources/Attack.wav");
	soundData6_ = audio_->SoundLoadWave("project/gamedata/resources/heal.wav");
	soundData7_ = audio_->SoundLoadWave("project/gamedata/resources/change.wav");

	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(CollisionConfig::kCollisionMaskPlayer);

	//回復パーティクルの初期化
	healParticleTextureHandle_ = TextureManager::GetInstance()->Load("project/gamedata/resources/Heart.png");
	healParticle_.particle = std::make_unique<CreateParticle>();
	Emitter emitter{
		.transform{.scale{1.0f,1.0f,1.0f},.rotate{0.0f,0.0f,0.0f},.translate{0.0f,0.0f,0.0f}},
		.count = 0,
		.frequency = 10000,
		.frequencyTime = 10000,
	};
	AccelerationField accelerationField{
		.acceleration{0.0f,15.0f,0.0f},
		.area{.min{-1.0f,-1.0f,-1.0f},.max{1.0f,1.0f,1.0f}},
	};
	healParticle_.particle->Initialize(1000, emitter, accelerationField, healParticleTextureHandle_);
  
	//パーティクルの設定
	emitter.transform.translate = GetWorldPosition();
	emitter.transform.translate.num[1] = emitter.transform.translate.num[1] - 3.0f;
	emitter.transform.scale = { 5.0f,5.0f,5.0f };
	emitter.count = 1;
	emitter.frequency = 0.05f;
	emitter.frequencyTime = 0.0f;


	//フィールド設定

	field.acceleration = { -5,10,0.0f };
	field.area.min = { -1.0f,-1.0f,-20.0f };
	field.area.max = { 10000.0f,10.0f,20.0f };
	particle_ = std::make_unique<CreateParticle>();
	particle_->Initialize(200, emitter, field,textureHandle_);
}

void Player::Update() {	


	Move();
	if (jump_) {
		Jump();
	}

	if (input_->TriggerKey(DIK_W)) {
		audio_->SoundPlayWave(soundData1_, 0.8f, false);
		if (!jump_) {
			velocity_.num[1] = kJumpFirstSpeed;

			jump_ = true;
		}
	}


	bulletParticle();
	smokeParticle();
	worldTransformBase_.UpdateMatrix();
#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::Text("Life : %d", life_);
	ImGui::Text("Attack Push::P");

	ImGui::DragFloat("FirstSpeed", &kJumpFirstSpeed, 0.1f);
	ImGui::DragFloat("Gravity", &kGravity, 0.01f);
	ImGui::DragFloat("Width", &jumpWidth_, 0.1f);
	ImGui::DragFloat("CameraPosY", &cameraPosY, 0.1f);
	ImGui::DragFloat("CameraRotX", &cameraRotX, 0.01f);
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
#endif

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

	Attack();

	if (input_->TriggerKey(DIK_SPACE)) {
		audio_->SoundPlayWave(soundData7_, 0.6f, false);
		if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
			debugCamera_->MovingCamera(Vector3{ -50.0f + worldTransformBase_.translation_.num[0],22.7f,0.0f }, Vector3{ 0.0f,1.6f,-0.3f }, 0.05f);
			isSideScroll_ = false;
		}
		else {
			//debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.05f);
			debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0],cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f }, 0.05f);
			isSideScroll_ = true;
		}
	}

	if (isAccelerationFlag_ == true) {//加速した時
		accelerationTimer_++;
		if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
			/*debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0] + 10.0f,2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);*/
			debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0] + 10.0f,cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f }, 0.06f);
			if (accelerationTimer_ >= 120) {
				/*debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);*/
				debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0],cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f }, 0.06f);
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
			//debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0] - 10.0f,2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);
			debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0] - 10.0f,cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f }, 0.06f);
			if (decelerationTimer_ >= 120) {
				/*debugCamera_->MovingCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 0.06f);*/
				debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0],cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f }, 0.06f);
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
			/*debugCamera_->SetCamera(Vector3{ 20.0f + worldTransformBase_.translation_.num[0],2.7f,-50.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f });*/
			debugCamera_->SetCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0],cameraPosY,-60.0f + cameraDistance_ }, Vector3{ cameraRotX,0.0f,0.0f });
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

	//回復パーティクルの更新
	if (healParticle_.isActive) {
		if (--healParticle_.activeTimer <= 0) {
			healParticle_.isActive = false;
		}
		healParticle_.particle->Update();
	}
}

void Player::Draw(const ViewProjection& view) {
	models_[0]->Draw(worldTransformBase_, view, modelMaterial_);

	//弾の描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(view);
	}
    for (PlayerParticle* particle : bulletParticle_) {
		particle->Draw(view);
	}
}
void Player::ParticleDraw(const ViewProjection& view) {
	particle_->Draw(view);
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
			audio_->SoundPlayWave(soundData1_, 0.5f, false);
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
			audio_->SoundPlayWave(soundData1_, 0.5f, false);
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
	worldTransformBase_.rotation_.num[2] -= velocity_.num[0];

}

void Player::Attack() {
	if (input_->PressKey(DIK_P) && !jump_ && isSideScroll_ == true) {
		--fireTimer_;
	}
	else {
		fireTimer_ = 1;
	}

	if (fireTimer_ == 0) {
		audio_->SoundPlayWave(soundData5_, 0.8f, false);
		// 弾の速度
		const float kBulletSpeed = 1.0f + moveSpeed_;
		Vector3 velocity = { kBulletSpeed, 0, 0 };


		// 弾を生成、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(GetWorldPosition(), velocity,model_.get());
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
	if (collider->GetCollisionAttribute() & CollisionConfig::kCollisionAttributeObstacleNone) {
		life_--;
		isDamageFlag_ = true;
		shakeTimer_ = 0;
		audio_->SoundPlayWave(soundData4_, 0.8f, false);

	}
	if (collider->GetCollisionAttribute() & CollisionConfig::kCollisionAttributeObstacleAcceleration) {
		moveSpeed_ += 0.2f;
		isAccelerationFlag_ = true;
		accelerationTimer_ = 0;
		audio_->SoundPlayWave(soundData2_, 0.8f, false);
	}
	if (collider->GetCollisionAttribute() & CollisionConfig::kCollisionAttributeObstacleDeceleration) {
		moveSpeed_ -= 0.2f;
		if (moveSpeed_ <= 0.0f) {//速度が0になった時、これ以上下がらないようにする
			moveSpeed_ = 0.1f;
		}
		isDecelerationFlag_ = true;
		decelerationTimer_ = 0;
		audio_->SoundPlayWave(soundData3_, 0.8f, false);
	}
	if (collider->GetCollisionAttribute() & CollisionConfig::kCollisionAttributeObstacleHealLife) {
		life_++;
		audio_->SoundPlayWave(soundData6_, 0.8f, false);

		//エミッターを設定
		Vector3 pos = GetWorldPosition();
		Emitter emitter{
			.transform{.scale{1.0f,1.0f,1.0f},.rotate{0.0f,0.0f,0.0f},.translate{pos}},
			.count = 100,
			.frequency = 10000,
			.frequencyTime = 10000,
		};
		AccelerationField accelerationField{
			.acceleration{0.0f,30.0f,0.0f},
			.area{.min{pos.num[0] - 2.0f,pos.num[1] - 2.0f,pos.num[2] - 2.0f},.max{pos.num[0] + 2.0f,pos.num[1] + 2.0f,pos.num[2] + 2.0f}},
		};
		healParticle_.particle->SetEmitter(emitter);
		healParticle_.particle->SetAccelerationField(accelerationField);
		healParticle_.isActive = true;
		healParticle_.activeTimer = kHealParticleActiveTime;
	}
}


void Player::bulletParticle() {
	for (PlayerBullet* bullet : bullets_) {
		if (bullet->IsDead()) {
			for (int i = 0; i < 15; ++i) {
				Vector3 velocity = { 0, 0, 0 };
				float numberX = (rand() % 11 - 5) / 5.0f;
				float numberY = (rand() % 11 - 5) / 5.0f;
				float numberZ = (rand() % 11 - 5) / 5.0f;
				velocity = { numberX, numberY, numberZ };
				//初期化
				PlayerParticle* newParticles = new PlayerParticle();
				newParticles->Initialize(model_.get(), bullet->GetWorldPosition(), { 0.5f, 0.5f, 0.5f }, velocity, velocity);

				bulletParticle_.push_back(newParticles);
			}

		}
	}

	for (PlayerParticle* particle : bulletParticle_) {
		particle->Update();

	}

	// デスフラグが立った弾を削除
	bulletParticle_.remove_if([](PlayerParticle* particle) {
		if (particle->IsDead()) {

			delete particle;
			return true;
		}
		return false;
		});
}


void Player::smokeParticle() {


	particle_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	particle_->SetTranslate(worldTransformBase_.translation_);
	particle_->SetFrequency(0.05f / velocity_.num[0]);
	particle_->SetCount(1 + int(velocity_.num[0] / 2));
	particle_->Update();

}


void Player::Reset() {
	worldTransformBase_.translation_ = { 0.0f,0.0f,0.0f };
	debugCamera_->MovingCamera(Vector3{ 30.0f + worldTransformBase_.translation_.num[0],2.7f,-60.0f + cameraDistance_ }, Vector3{ 0.0f,0.0f,0.0f }, 1.0f);

	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
	bullets_.clear();

	life_ = 5;

	moveSpeed_ = 0.4f;

	shakeTimer_ = 0;
	isDamageFlag_ = false;

	accelerationTimer_ = 0;
	isAccelerationFlag_ = false;

	decelerationTimer_ = 0;
	isDecelerationFlag_ = false;

	cameraDistance_ = 0.0f;

	lane_ = Obstacle::Lane::Middle;
}

Player::~Player() {
	audio_->SoundUnload(&soundData1_);
	audio_->SoundUnload(&soundData2_);
	audio_->SoundUnload(&soundData3_);
	audio_->SoundUnload(&soundData4_);
	audio_->SoundUnload(&soundData5_);
	audio_->SoundUnload(&soundData6_);
	audio_->SoundUnload(&soundData7_);
}

void Player::DrawParticle(const ViewProjection& viewProjection) {
	//回復パーティクルの描画
	if (healParticle_.isActive) {
		healParticle_.particle->Draw(viewProjection);
	}
}