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

	SetCollisionAttribute(CollisionConfig::kCollisionAttributePlayer);
	SetCollisionMask(~CollisionConfig::kCollisionAttributePlayer);
}

void Player::Update() {
	Attack();
	Move();

	worldTransformBase_.UpdateMatrix();
	ImGui::Begin("Player");
	ImGui::DragFloat3("Pos", worldTransformBase_.translation_.num, 0.1f);
	ImGui::Text("Life : %d", life_);
	ImGui::Text("Attack Push::P");
	ImGui::End();

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
	worldTransformBase_.translation_.num[0] += 0.1f;
	if (worldTransformBase_.translation_.num[0] > 20.0f) {
		worldTransformBase_.translation_.num[0] = -40.0f;
	}
	//俯瞰視点移動
	if (input_->TriggerKey(DIK_D)) {
		worldTransformBase_.translation_.num[2] -= 15.0f;
	}else if (input_->TriggerKey(DIK_A)) {
		worldTransformBase_.translation_.num[2] += 15.0f;
	}
	if (worldTransformBase_.translation_.num[2] <= -15.0f) {
		worldTransformBase_.translation_.num[2] = -15.0f;
	}else if (worldTransformBase_.translation_.num[2] >= 15.0f) {
		worldTransformBase_.translation_.num[2] = 15.0f;
	}




}

void Player::Attack() {
	if (input_->PressKey(DIK_P) ) {
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

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる関数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.num[0] = worldTransformBase_.matWorld_.m[3][0];
	worldPos.num[1] = worldTransformBase_.matWorld_.m[3][1];
	worldPos.num[2] = worldTransformBase_.matWorld_.m[3][2];
	return worldPos;
}

void Player::OnCollision() {
	isHit = true;
	life_--;
}