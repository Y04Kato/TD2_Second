#pragma once
#include <cstdint>

namespace CollisionConfig {
	const uint32_t kCollisionAttributePlayer               = 0b0000001;
	const uint32_t kCollisionAttributeEnemy                = 0b0000010;
	const uint32_t kCollisionAttributeBullet               = 0b0000100;
	const uint32_t kCollisionAttributeObstacleNone         = 0b0001000;
	const uint32_t kCollisionAttributeObstacleAcceleration = 0b0010000;
	const uint32_t kCollisionAttributeObstacleDeceleration = 0b0100000;
	const uint32_t kCollisionAttributeObstacleHealLife     = 0b1000000;

	const uint32_t kCollisionMaskPlayer               = 0b1111110;
	const uint32_t kCollisionMaskEnemy                = 0b0000101;
	const uint32_t kCollisionMaskBullet               = 0b0001010;
	const uint32_t kCollisionMaskObstacleNone         = 0b0000101;
	const uint32_t kCollisionMaskObstacleAcceleration = 0b0000001;
	const uint32_t kCollisionMaskObstacleDeceleration = 0b0000001;
	const uint32_t kCollisionMaskObstacleHealLife     = 0b0000001;
};