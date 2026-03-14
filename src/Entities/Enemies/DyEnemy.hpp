#pragma once
#include "Enemy.hpp"

class DyEnemy : public Enemy {
    private: 
        float angle = 135;
        float aimAngle = 225;
        bool loop = false;

        Rectangle enemyType; // variable para el tipo de enemigo.

    public:
        DyEnemy(float x, float y) : Enemy(x, y) { 
            this->cooldown = GetRandomValue(90, 300);
            this->health = 1; 
            this->eScore = 50; //enemy Score.
        


    if (GetRandomValue(1, 2) == 1){
    this->enemyType = Rectangle{2, 128, 13, 14};
    }
    else{
        this->enemyType = Rectangle{2, 147, 13, 13};
    }
// Lógica para escoger tipo de enemigo.
}
        void draw() override;
        void update(std::pair<float, float> pos, HitBox target) override;
        void attack(HitBox target) override;
};