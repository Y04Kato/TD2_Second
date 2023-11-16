#pragma once
#include <cstdint>

namespace CollisionConfig {
	const uint32_t kCollisionAttributePlayer = 0b1;
	const uint32_t kCollisionAttributeEnemy = 0b1 << 1;
	const uint32_t kCollisionAttributeBullet = 0b1 << 2;
	const uint32_t kCollisionAttributeObstacle = 0b1 << 3;

	const uint32_t kCollisionMaskPlayer = 0b1110;
	const uint32_t kCollisionMaskEnemy = 0b0101;
	const uint32_t kCollisionMaskBullet = 0b1010;
	const uint32_t kCollisionMaskObstacle = 0b0101;
};