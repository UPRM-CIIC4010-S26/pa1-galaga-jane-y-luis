#include "raylib.h"

#include "resource_dir.h"
#include "Program.hpp"

extern int score; // Añade la variable de Program.cpp

int main ()
{
	SearchAndSetResourceDir("resources");
    InitAudioDevice();
	SetTargetFPS(60);

	raylib::Window window(1000, 1000, "Galaga", FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);
	Program Galaga;
	ImageManager::Load();
	SoundManager::Load();

	if (!IsWindowReady()) {
    TraceLog(LOG_ERROR, "Window failed to initialize!");
    return 1;
}

	while (!WindowShouldClose()) {
		BeginDrawing();
		ClearBackground(BLACK);

		Galaga.Update();
		Galaga.Draw();
		Galaga.KeyInputs();

		DrawText(TextFormat("SCORE: %010i", score), 20, 20, 30, WHITE);// Añade Score.
		EndDrawing();
	}

	ImageManager::Unload();
	SoundManager::Unload();
	CloseAudioDevice();
	window.Close();
	return 0;
}
