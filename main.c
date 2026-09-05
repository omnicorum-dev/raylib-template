#define OMNI_IMPLEMENTATION
#include "omni.h"
#include <raylib.h>

#define BGColor GetColor(0x181818FF)

int main() {
    InitWindow(1280, 720, "Vipper - (c) Omnicorum");
    InitAudioDevice();

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BGColor);

        EndDrawing();
    }

    return 0;
}
