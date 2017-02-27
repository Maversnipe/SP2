#ifndef ENEMY_H
#define ENEMY_H
#include "Vector3.h"
class Enemy
{
public:
	Enemy(){};
	~Enemy(){};
	Vector3 ExplosionPos = {};
	Vector3 enemyPos = {};
	float ExplosionRadius = {};
	bool enemyDead = {};
	bool enemyExplosion = {};
	float enemyRotation = {};
private:

};

#endif