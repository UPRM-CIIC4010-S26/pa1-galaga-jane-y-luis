#pragma once
#include "Enemy.hpp"

class SpEnemy : public Enemy {
    private: 
        float angle = 90;
        int delay = 0;
        int specialCooldown = GetRandomValue(1380, 3600);
        bool stop = false;

    public:
        SpEnemy(float x, float y) : Enemy(x, y) { 
            this->cooldown = GetRandomValue(300, 1380); 
            this->health = 4;
            this->eScore = 200; //special enemy tiene valor de 200 puntos.
        }

        SpEnemy(float x, float y, bool newSpawn) : Enemy(x, y){
            this->cooldown = GetRandomValue(360, 1380);
            this->health = 4;
            this->spawning = newSpawn;
        }

        void draw() override;
        void update(std::pair<float, float> pos, HitBox target) override;
        void attack(HitBox target) override;
};