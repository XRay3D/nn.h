#include "gym.hpp"
#include "nn.hpp"

size_t arch[] = {2, 2, 1};
size_t max_epoch = 100 * 1000;
size_t epochs_per_frame = 103;
float rate = 1.0f;
bool paused = true;

void verify_nn_gate(Font font, NN nn, Gym_Rect r) {
    char buffer[256];
    float s = r.h * 0.06;
    float pad = r.h * 0.03;
    for(size_t i = 0; i < 2; ++i) {
        for(size_t j = 0; j < 2; ++j) {
            nn.input()
                [0]
                = i;
            nn.input()
                [1]
                = j;
            nn.forward();
            snprintf(buffer, sizeof(buffer), "%zu @ %zu == %f", i, j, nn.output()[0]);
            DrawTextEx(font, buffer, CLITERAL(Vector2){r.x, r.y + (i * 2 + j) * (s + pad)}, s, 0, WHITE);
        }
    }
}

int main(void) {
    Region temp(256 * 1024 * 1024);

    Mat t = Mat::alloc(NULL, 4, 3);
    for(size_t i = 0; i < 2; ++i) {
        for(size_t j = 0; j < 2; ++j) {
            size_t row = i * 2 + j;
            t[row][0] = i;
            t[row][1] = j;
            t[row][2] = i ^ j;
        }
    }

    NN nn = NN::alloc(NULL, arch);
    nn.rand(-1, 1);

    size_t WINDOW_FACTOR = 80;
    size_t WINDOW_WIDTH = (16 * WINDOW_FACTOR);
    size_t WINDOW_HEIGHT = (9 * WINDOW_FACTOR);

    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "xor");
    SetTargetFPS(60);

    Font font = LoadFontEx(JetBrains, 72, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Gym_Plot plot = {0};

    size_t epoch = 0;
    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_SPACE))
            paused = !paused;
        if(IsKeyPressed(KEY_R)) {
            epoch = 0;
            nn.rand(-1, 1);
            plot.count = 0;
        }

        for(size_t i = 0; i < epochs_per_frame && !paused && epoch < max_epoch; ++i) {
            NN g = nn.backprop(&temp, t);
            nn.learn(g, rate);
            epoch += 1;
            da_append(&plot, nn.cost(t));
        }

        BeginDrawing();
        ClearBackground(GYM_BACKGROUND);
        {
            int w = GetRenderWidth();
            int h = GetRenderHeight();

            Gym_Rect r;
            r.w = w;
            r.h = h * 2 / 3;
            r.x = 0;
            r.y = h / 2 - r.h / 2;

            gym_layout_begin(GLO_HORZ, r, 3, 10);
            gym_plot(font, plot, gym_layout_slot(), RED);
            gym_render_nn(nn, gym_layout_slot());
            verify_nn_gate(font, nn, gym_layout_slot());
            gym_layout_end();

            char buffer[256];
            snprintf(buffer, sizeof(buffer), "Epoch: %zu/%zu, Rate: %f, Cost: %f, Temporary Memory: %zu bytes", epoch, max_epoch, rate, nn.cost(t), temp.occupied_bytes());
            DrawTextEx(font, buffer, CLITERAL(Vector2){}, h * 0.04, 0, WHITE);
        }
        EndDrawing();

        temp.reset();
    }

    return 0;
}
