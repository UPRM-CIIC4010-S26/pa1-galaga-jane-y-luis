#include "Program.hpp"

int score = 0;

Program::Program() {

    //here we will put the score since it is a game variable, like the lives, player, etc.
    //here we will also put the scores of enemies whenever they die


    Background::sideWalls = std::pair<HitBox, HitBox>{ 
        HitBox(0, 0, 10, GetScreenHeight()), 
        HitBox(GetScreenWidth() - 10, 0, 10, GetScreenHeight())
    };

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{350, 150}, 
            new SpEnemy(350, 150)
        });

    Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{600, 150}, 
            new SpEnemy(600, 150)
        });

    for (int i = 0; i < 30; i++) {
        float x = 250 + 50 * (i%10); // Se le añadió la lógica matemática para
        float y = 200 + 50 * (i/10); // arreglar la formacion de los enemigos.

        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{x, y}, 
            new StdEnemy(x, y)
        });
    }
}

void Program::Update() {

    //here we will put the 500 points add up.
    
    for (Animation& a : Animation::animations) a.update();
    for (int i = 0; i < Animation::animations.size(); i++) {
        if (Animation::animations[i].done) Animation::animations.erase(Animation::animations.begin() + i);
    }
    pauseFrames = std::max(pauseFrames - 1, 0);

    //aqui se ve como el juego esta haciendo ss funciones, osea, que esta corriendo.
    if (!startup && !paused && !gameOver && pauseFrames <= 0) {
        Enemy::ManageEnemies(player->hitBox);
        StdEnemy::attackReset();
        ManageEnemyRespawns();
        player->update();
    }

    //EDITE AQUI
    currentScore = score/1000;

    if(currentScore > previousScore){
     if(lives < 5)
        lives++
        previousScore = currentScore;
    }
    //HASTA AQUI
}

        for (std::pair<std::pair<float, float>, Enemy*> p : Enemy::enemies) {
            if (p.second && HitBox::Collision(player->hitBox, p.second->hitBox)) {
                Animation::animations.push_back(
                    Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
                );

                PlaySound(SoundManager::gameOver);
                Projectile::projectiles.clear();
                player->position.first = GetScreenWidth() / 2 - 15;
                p.second->health = 0;
                pauseFrames = 120;
                lives--;
            }
        

        for (Projectile& p : Projectile::projectiles) { 
            p.update(); 
            if (p.ID != 0){     //Verifica si el projectile es del enemigo.
                // Verifica si el hitbox del projectile está en el hitbox del jugador.
                if(HitBox::Collision(p.getHitBox(), player->hitBox)){
                    PlayerReset(); // Mata al jugador y lo devuelve en la posición inicial.
                }
            }

        }

        if (lives <= 0 && pauseFrames <= 0) gameOver = true;
        Projectile::CleanProjectiles();
        Projectile::ProjectileCollision();
    }



void Program::Draw() {

    //Here is where it will SHOW the score, since in here there are animations and images

    background.Draw();
    if (pauseFrames <= 0 && !gameOver) player->draw();
    for (Animation& a : Animation::animations) a.draw();

    for (int i = 0; i < lives; i++) {
         DrawTexturePro(ImageManager::SpriteSheet, Rectangle{0, 0, 17, 18}, 
                   Rectangle{10.0f + i * 30, GetScreenHeight() - 30.0f, 20, 20}, 
                   Vector2{0, 0}, 0, WHITE);
    }


    for (Projectile p : Projectile::projectiles) p.draw();
    for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) if (p.second) p.second->draw();

    if (startup) DrawStartup();
    if (paused) DrawPauseScreen();
    if (gameOver) DrawGameOver();
}

void Program::ManageEnemyRespawns() {

    //here we will put how the enemies respawn quicker as the score increases.

    //"Take into consideration that depending on how you implement this the re-spawn counter can reduce too quickly having enemies spawn at an uncontrollable pace."

    delay = std::max(delay - 1, 0);

    int difficulty = score / 1000; //EDITADO

    respawnCooldown -= 1;
    if (respawnCooldown <= 0) {
         respawnCooldown = std::max(1080 - difficulty * 150, 400); // EDITADO

        for (std::pair<std::pair<float, float>, Enemy*>& p : Enemy::enemies) {
            if (!p.second && p.first.second != 150) {
                int eType = GetRandomValue(1, 3);

                if (eType == 1) {
                    p.second = new StEnemy(GetScreenWidth() / 2 - 15, 0, true);
                    respawnCooldown /= 2;
                } else {
                    p.second = new StdEnemy(GetScreenWidth() / 2 - 15, 0, true);
                }

                respawns++;
                break;
            } else if (!p.second && p.first.second == 150) {
                p.second = new SpEnemy(GetScreenWidth() / 2 - 15, 0, true);
                respawns++;
                break;
            }
        }
    }

    if(respawns >= 4) {
        count = 4;
        respawns = 0;
    }

    if (count > 0 && delay <= 0) {
        Enemy::enemies.push_back(std::pair<std::pair<float, float>, Enemy*> {
            std::pair<float, float>{0, 0}, 
            new DyEnemy(GetScreenWidth(), 300)
        });

        count--;
        delay = 20;
    }
}

void Program::DrawStartup() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Galaga", (GetScreenWidth() / 2 - 237), 75, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawPauseScreen() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Paused", (GetScreenWidth() / 2) - 85, GetScreenHeight() / 2 - 60, 48, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::DrawGameOver() {
    DrawRectangle(0, 0, (float)GetScreenWidth(), (float)GetScreenHeight(), Color{0, 0, 0, 125});
    DrawText("Game Over", (GetScreenWidth() / 2) - 380, 50, 144, WHITE);
    DrawText("Press Enter", (GetScreenWidth() / 2) - 75, GetScreenHeight() / 2, 24, GRAY);
}

void Program::KeyInputs() {

    //here we will put the k adding 500 points, since the K is a keyboard input.
    if(IsKeyPressed('K')) score+= 500;
        

    if ((!gameOver && !startup && IsKeyPressed('P')) || (paused && IsKeyPressed(KEY_ENTER))) paused = !paused;
    if (!paused && !startup && IsKeyPressed('O')) gameOver = !gameOver;
    if (!gameOver && !paused && IsKeyPressed('I')) startup = !startup;
    if (IsKeyPressed('H')) HitBox::drawHitbox = !HitBox::drawHitbox;
    if (IsKeyPressed('K')) score += 500; // Suma 500 puntos si se aprieta la 'K'.
    
    if (gameOver && IsKeyPressed(KEY_ENTER)) {
        gameOver = false;
        Reset();
    }

    if (startup && IsKeyPressed(KEY_ENTER)) {
        startup = false;
    }

    if (!startup && !paused && !gameOver && pauseFrames <= 0) player->keyInputs();
   
}

void Program::PlayerReset() {
    Animation::animations.push_back(
        Animation(player->position.first, player->position.second, 16, 0, 33, 34, 30 ,30, 3, ImageManager::SpriteSheet)
    );

    PlaySound(SoundManager::gameOver);
    Projectile::projectiles.clear();
    player->position.first = GetScreenWidth() / 2 - 15;
    pauseFrames = 120;
    lives--;
}

void Program::Reset() {

    Enemy::enemies.clear();
    Program(); // Hace lo mismo pero sin copiar el bloque de código de antes.
    StdEnemy::attackInProgress = false;
    player = new Player((GetScreenWidth() / 2) - 15, GetScreenHeight() * 0.75f);
    respawnCooldown = 1080;
    respawns = 0;
    count = 0;
    delay = 0;
    lives = 3;
    previousScore = 0; //THIS WAS ADDED
    score = 0; //THIS WAS ADDED
}

