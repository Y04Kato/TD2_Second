#include "GamePlayScene.h"
#include "components/utilities/globalVariables/GlobalVariables.h"

void GamePlayScene::Initialize() {
	CJEngine_ = CitrusJunosEngine::GetInstance();
	dxCommon_ = DirectXCommon::GetInstance();
	textureManager_ = TextureManager::GetInstance();

	// フェード
	fade_ = std::make_unique<Fade>();
	fade_->Initialize();
	isGameStart_ = false;

	//テクスチャ
	uvResourceNum_ = textureManager_->Load("project/gamedata/resources/uvChecker.png");

	//Input
	input_ = Input::GetInstance();

	//Audio
	audio_ = Audio::GetInstance();
	soundData1_ = audio_->SoundLoadWave("project/gamedata/resources/cyber14.wav");
	//音声再生
	audio_->SoundPlayWave(soundData1_, 0.3f, true);

	// デバッグカメラの初期化
	debugCamera_ = DebugCamera::GetInstance();
	debugCamera_->initialize();

	viewProjection_.Initialize();

	//CollisionManager
	collisionManager_ = CollisionManager::GetInstance();

	GlobalVariables* globalVariables{};
	globalVariables = GlobalVariables::GetInstance();

	const char* groupName = "GamePlayScene";
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "Test", 90);

	ui_ = textureManager_->Load("project/gamedata/resources/ui.png");

	spriteMaterial_ = { 1.0f,1.0f,1.0f,1.0f };
	spriteTransform_ = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	SpriteuvTransform_ = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	sprite_ = std::make_unique <CreateSprite>();
	sprite_->Initialize(Vector2{ 100.0f,100.0f }, ui_, false, false);
	sprite_->SetTextureInitialSize();

	//障害物
	obstacleManager_ = std::make_unique<ObstacleManager>();
	obstacleManager_->Initialize();

	// グラウンド
	groundManager_ = make_unique<GroundManager>();
	groundManager_->Initialize();

	//プレイヤー
	player_ = std::make_unique <Player>();
	playerModel_.reset(Model::CreateModelFromObj("project/gamedata/resources/player", "Player.obj"));
	std::vector<Model*>playerModels = { playerModel_.get() };
	player_->Initialize(playerModels, uvResourceNum_);

	//敵
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize();

}

void GamePlayScene::Update() {
	ApplyGlobalVariables();

	// フェード
	fade_->FadeInUpdate();

	if (isGameStart_ == false) {
		fade_->FadeOutFlagSet(true);
		fade_->FadeOutUpdate();
	}

	if (fade_->GetColor(1) < -1.0f) {
		isGameStart_ = true;
	}

	// フェードが終わるまで処理待機
	if (isGameStart_ == true) {
		debugCamera_->Update();

		if (player_->GetLife() <= 0) {
			fade_->FadeInFlagSet(true);
		}

		if (enemyManager_->GetEnemys()->GetLife() <= 0) {
			fade_->FadeInFlagSet(true);
		}

		if (fade_->GetColor(0) > 1.0f && player_->GetLife() <= 0) {
			sceneNo = GAMEOVER_SCENE;
		}

		if (fade_->GetColor(0) > 1.0f && enemyManager_->GetEnemys()->GetLife() <= 0) {
			sceneNo = CLEAR_SCENE;
		}

		viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
		viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
		viewProjection_.UpdateMatrix();

		groundManager_->Update();
		groundManager_->SetLane(player_->GetLane());
		player_->Update();
		player_->SetIsSideScroll(isSideScroll_);
		//敵の更新処理
		enemyManager_->SetPlayerPosition(player_->GetWorldPosition());
		enemyManager_->SetIsSideScroll(isSideScroll_);
		enemyManager_->Update();
		//障害物の更新処理
		obstacleManager_->Update();
		obstacleManager_->SetIsSideScroll(isSideScroll_);
		obstacleManager_->SetPlayerPosition(player_->GetWorldPosition());
		obstacleManager_->SetLane(player_->GetLane());
		obstacleManager_->SetCameraPosition(debugCamera_->GetViewProjection()->translation_);

		if (input_->TriggerKey(DIK_SPACE)) {
			if (isSideScroll_ == true) {//横スクロールから縦スクロールへ
				isSideScroll_ = false;
				groundManager_->SetFlag(true);
				groundManager_->SetScroll(false);
			}
			else {
				isSideScroll_ = true;
				groundManager_->SetFlag(false);
				groundManager_->SetScroll(true);
			}
		}

		groundCounter += player_->GetMoveSpeed();
		if (groundCounter >= groundManager_->GetSideGround() && isGroundSet == false) {
			groundManager_->SetSideGround();
			isGroundSet = true;
		}
		if (groundCounter >= groundManager_->GetSideGround2()) {
			groundManager_->SetSideGround2();
			isGroundSet = false;
		}
		if (groundCounter >= groundManager_->GetSideGround3() && isGroundSet2 == false) {
			groundManager_->SetSideGround3();
			isGroundSet2 = true;
		}
		if (groundCounter >= groundManager_->GetSideGround4()) {
			groundManager_->SetSideGround4();
			isGroundSet2 = false;
		}

		collisionManager_->ClearColliders();
		collisionManager_->AddCollider(player_.get());
		//敵
		const std::unique_ptr<Enemy>& enemys = enemyManager_->GetEnemys();
		collisionManager_->AddCollider(enemys.get());
		const std::list<std::unique_ptr<Obstacle>>& obstacles = obstacleManager_->GetObstacles();
		for (const std::unique_ptr<Obstacle>& obstacle : obstacles) {
			collisionManager_->AddCollider(obstacle.get());
			distance = obstacle->GetWorldTransform().translation_.num[0] - player_->GetWorldPosition().num[0];
			if (distance <= distance_ && player_->GetLane() == obstacle->GetLane()) {
				distance_ = distance;
				player_->SetObstacleMode(obstacle->GetMode());
			}
			if (distance_ <= 2.0f) {
				distance_ = 100.0f;
			}
		}
		//弾
		const std::list<PlayerBullet*> bullets = player_->GetPlayerBullet();
		for (PlayerBullet* bullet : bullets) {
			collisionManager_->AddCollider(bullet);
			bullet->SetSideScroll(isSideScroll_);
		}

		collisionManager_->CheckAllCollision();

		//ゲームオーバー処理
		if (player_->GetLife() <= 0 || player_->GetMoveSpeed() <= 0.0f || Input::GetInstance()->TriggerKey(DIK_1)) {
			sceneNo = GAMEOVER_SCENE;
			Reset();
			debugCamera_->Update();
			viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
			viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
			viewProjection_.UpdateMatrix();
		}

		//ゲームクリア処理
		if (enemyManager_->GetEnemyLife() <= 0 || Input::GetInstance()->TriggerKey(DIK_2)) {
			sceneNo = CLEAR_SCENE;
			Reset();
			debugCamera_->Update();
			viewProjection_.translation_ = debugCamera_->GetViewProjection()->translation_;
			viewProjection_.rotation_ = debugCamera_->GetViewProjection()->rotation_;
			viewProjection_.UpdateMatrix();
		}

		ImGui::Begin("debug");
		ImGui::Text("GamePlayScene");
		ImGui::Text("FPS %f", ImGui::GetIO().Framerate);
		ImGui::Text("Distance %f", distance_);
		ImGui::Text("Lane %d", player_->GetLane());
		ImGui::Text("1 : GAMEOVER_SCENE");
		ImGui::Text("2 : CLEAR_SCENE");
		ImGui::End();
	}
}

void GamePlayScene::Draw() {
#pragma region 3Dオブジェクト描画
	CJEngine_->PreDraw3D();

	if (isGameStart_ == true) {

		player_->Draw(viewProjection_);
		enemyManager_->Draw(viewProjection_);

		//障害物の描画
		obstacleManager_->Draw(viewProjection_);


		groundManager_->Draw(viewProjection_);
	}

#pragma endregion

#pragma region パーティクル描画
	CJEngine_->PreDrawParticle();

#pragma endregion

#pragma region 前景スプライト描画
	CJEngine_->PreDraw2D();

	if (isGameStart_ == true) {
		sprite_->Draw(spriteTransform_, SpriteuvTransform_, spriteMaterial_);
	}

	fade_->FadeInDraw();
	fade_->FadeOutDraw();

#pragma endregion
}

void GamePlayScene::Finalize() {
	audio_->SoundUnload(&soundData1_);
}

void GamePlayScene::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "GamePlayScene";
}

void GamePlayScene::Reset() {
	isSideScroll_ = true;
	distance = 0.0f;
	distance_ = 100.0f;
	groundCounter = 0.0f;
	player_->Reset();
	enemyManager_->Reset();
	groundManager_->Reset();
	obstacleManager_->Reset();
}