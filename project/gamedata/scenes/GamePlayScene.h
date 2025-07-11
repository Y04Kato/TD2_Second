#pragma once
#include "components/manager/Iscene.h"
#include "components/audio/Audio.h"
#include "components/input/Input.h"
#include "components/3d/WorldTransform.h"
#include "components/3d/ViewProjection.h"
#include "components/manager/TextureManager.h"
#include "components/2d/CreateTriangle.h"
#include "components/2d/CreateSprite.h"
#include "components/3d/CreateSphere.h"
#include "components/3d/Model.h"
#include "components/debugcamera/DebugCamera.h"
#include "components/utilities/collisionManager/CollisionManager.h"
#include "components/utilities/collisionManager/CollisionConfig.h"
#include "../Obstacle/ObstacleManager.h"
#include "gamedata/Fade/Fade.h"

#pragma region ゲームオブジェクト
#include "Ground/GroundManager.h"
#include "Player/Player.h"
#include "Enemy/EnemyManager.h"
#include "Numbers/Numbers.h"
#pragma endregion

class GamePlayScene :public Iscene {
public:
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
	void Reset();

	void ApplyGlobalVariables();

private:
	CitrusJunosEngine* CJEngine_;
	DirectXCommon* dxCommon_;
	ViewProjection viewProjection_;
	TextureManager* textureManager_;

	uint32_t uvResourceNum_;
	

	Audio* audio_;
	SoundData soundData1_;

	Input* input_;

	DebugCamera* debugCamera_;

	CollisionManager* collisionManager_;

	std::unique_ptr <CreateSprite> sprite_;
	Transform spriteTransform_;
	Transform SpriteuvTransform_;
	Vector4 spriteMaterial_;

	uint32_t ui_;

	//障害物
	std::unique_ptr<ObstacleManager> obstacleManager_ = nullptr;
	std::unique_ptr<GroundManager> groundManager_;
	float groundCounter = 0;
	bool isGroundSet = false;
	bool isGroundSet2 = false;

	bool isSideScroll_ = true;
	float distance;
	float distance_ = 100.0f;
 
	/*----------------プレイヤー---------------*/

	std::unique_ptr<Player> player_;
	std::unique_ptr<Model> playerModel_;
	uint32_t smokeParticleTexture_;

	/*-------------------敵------------------*/
	std::unique_ptr<EnemyManager> enemyManager_ = nullptr;

	// フェード
	std::unique_ptr<Fade> fade_;
	bool isGameStart_;

	//数字
	std::unique_ptr<Numbers> numbers_;
	Transform transformNumbers_;

	std::unique_ptr<Numbers> numbers2_;
	Transform transformNumbers2_;
};
