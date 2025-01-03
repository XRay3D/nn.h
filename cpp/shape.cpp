#include <stdio.h>
#include <time.h>

#define OLIVEC_AA_RES 1
#define OLIVEC_IMPLEMENTATION
#include "olive.c"

#define NN_BACKPROP_TRADITIONAL
#define NN_ACT ACT_SIG

#include "gym.hpp"
#include "nn.hpp"

#define WIDTH  28
#define HEIGHT WIDTH
enum {
    SHAPE_CIRCLE,
    SHAPE_RECT,
    SHAPES,
};
#define TRAINING_SAMPLES_PER_SHAPE     2000
#define VERIFICATION_SAMPLES_PER_SHAPE (TRAINING_SAMPLES_PER_SHAPE / 2)
#define BACKGROUND_COLOR               0xFF000000
#define FOREGROUND_COLOR               0xFFFFFFFF

size_t arch[] = {WIDTH * HEIGHT, 14, 7, 5, SHAPES};
size_t batch_size = 20;
size_t batches_per_frame = 20;
float rate = 0.1f;
bool paused = true;

void random_boundary(size_t width, size_t height, int* x1, int* y1, int* w, int* h) {
    int x2, y2, i = 0;
    do {
        *x1 = rand() % width;
        *y1 = rand() % height;
        x2 = rand() % width;
        y2 = rand() % height;
        if(*x1 > x2) OLIVEC_SWAP(int, *x1, x2);
        if(*y1 > y2) OLIVEC_SWAP(int, *y1, y2);
        *w = x2 - *x1;
        *h = y2 - *y1;
    } while((*w < 4 || *h < 4) && i++ < 100);
    assert(*w >= 4 && *h >= 4);
}

void random_circle(Olivec_Canvas oc) {
    int x, y, w, h;
    random_boundary(oc.width, oc.height, &x, &y, &w, &h);
    olivec_fill(oc, BACKGROUND_COLOR);
    int r = (w < h ? w : h) / 2;
    olivec_circle(oc, x + w / 2, y + h / 2, r, FOREGROUND_COLOR);
}

void random_rect(Olivec_Canvas oc) {
    int x, y, w, h;
    random_boundary(oc.width, oc.height, &x, &y, &w, &h);
    olivec_fill(oc, BACKGROUND_COLOR);
    olivec_rect(oc, x, y, w, h, FOREGROUND_COLOR);
}

void canvas_to_row(Row row, Olivec_Canvas oc) {
    NN_ASSERT(oc.width * oc.height == row.cols);
    for(size_t y = 0; y < oc.height; ++y)
        for(size_t x = 0; x < oc.width; ++x)
            row[y * oc.width + x] = (float)(OLIVEC_PIXEL(oc, x, y) & 0xFF) / 255.f;
}

Mat generate_samples(Region* r, size_t samples) {
    size_t input_size = WIDTH * HEIGHT;
    size_t output_size = SHAPES;
    Mat t = Mat::alloc(r, samples * SHAPES, input_size + output_size);
    size_t s = r->save();
    Olivec_Canvas oc{};
    oc.pixels = (decltype(oc.pixels))Region::alloc(r, WIDTH * HEIGHT * sizeof(*oc.pixels));
    oc.width = WIDTH;
    oc.height = HEIGHT;
    oc.stride = WIDTH;
    for(size_t i = 0; i < samples; ++i) {
        int x, y, w, h;
        random_boundary(oc.width, oc.height, &x, &y, &w, &h);
        int r = (w < h ? w : h) / 2;
        for(size_t j = 0; j < SHAPES; ++j) {
            Row row = Mat::row(t, i * 2 + j);
            Row in = row.slice(0, input_size);
            Row out = row.slice(input_size, output_size);
            olivec_fill(oc, BACKGROUND_COLOR);
            switch(j) {
            case SHAPE_CIRCLE: olivec_circle(oc, x + w / 2, y + h / 2, r, FOREGROUND_COLOR); break;
            case SHAPE_RECT: olivec_rect(oc, x, y, w, h, FOREGROUND_COLOR); break;
            default: assert(0 && "unreachable");
            }
            canvas_to_row(in, oc);
            out.fill(0);
            out[j] = 1.0f;
        }
    }
    r->rewind(s);
    return t;
}

void gym_drawable_canvas(Olivec_Canvas oc, Gym_Rect r) {
    NN_ASSERT(oc.width == oc.height && "We support only square canvases");
    r = gym_fit_square(r);
    float cw = r.w / oc.width;
    float ch = r.h / oc.height;

    Vector2 mouse = GetMousePosition();
    Rectangle boundary = {r.x, r.y, r.w, r.h};
    if(CheckCollisionPointRec(mouse, boundary)) {
        size_t x = (mouse.x - r.x) / cw;
        size_t y = (mouse.y - r.y) / ch;
        if(IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            OLIVEC_PIXEL(oc, x, y) = FOREGROUND_COLOR;
        if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT))
            OLIVEC_PIXEL(oc, x, y) = BACKGROUND_COLOR;
    }

    for(size_t y = 0; y < oc.height; ++y)
        for(size_t x = 0; x < oc.width; ++x)
            DrawRectangle(
                ceilf(r.x + x * cw),
                ceilf(r.y + y * ch),
                ceilf(cw),
                ceilf(ch),
                *(Color*)&OLIVEC_PIXEL(oc, x, y));
}

void display_training_data(Mat t) {
    for(size_t i = 0; i < t.rows; ++i) {
        Row row = Mat::row(t, i);
        Row in = row.slice(0, WIDTH * HEIGHT);
        Row out = row.slice(WIDTH * HEIGHT, SHAPES);
        for(size_t y = 0; y < HEIGHT; ++y) {
            for(size_t x = 0; x < WIDTH; ++x)
                if(in[y * WIDTH + x] > 1e-6f)
                    printf("##");
                else
                    printf("  ");
            printf("\n");
        }
        MAT_PRINT(out.as_mat());
    }
}

int main(void) {
    srand(time(0));

    Region temp(256 * 1024 * 1024);
    Region main(256 * 1024 * 1024);

    NN nn = NN::alloc(&main, arch);
    nn.rand(-1, 1);
    Mat t = generate_samples(&main, TRAINING_SAMPLES_PER_SHAPE);
    Mat v = generate_samples(&main, VERIFICATION_SAMPLES_PER_SHAPE);

    Gym_Plot tplot = {0};
    Gym_Plot vplot = {0};
    Batch batch = {0};

    int factor = 80;
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(16 * factor, 9 * factor, "Shape");
    SetTargetFPS(60);

    Gym_Plot plot = {0};
    Font font = LoadFontEx(JetBrains, 72, NULL, 0);
    SetTextureFilter(font.texture, TEXTURE_FILTER_BILINEAR);

    Olivec_Canvas canvas = {0};
    canvas.pixels = (decltype(canvas.pixels))Region::alloc(&main, WIDTH * HEIGHT * sizeof(*canvas.pixels));
    canvas.width = WIDTH;
    canvas.height = HEIGHT;
    canvas.stride = WIDTH;
    olivec_fill(canvas, BACKGROUND_COLOR);

    while(!WindowShouldClose()) {
        if(IsKeyPressed(KEY_SPACE))
            paused = !paused;
        if(IsKeyPressed(KEY_R)) {
            nn.rand(-1, 1);
            tplot.count = 0;
            vplot.count = 0;
        }
        if(IsKeyPressed(KEY_C))
            olivec_fill(canvas, BACKGROUND_COLOR);
        if(IsKeyPressed(KEY_Q))
            random_circle(canvas);
        if(IsKeyPressed(KEY_W))
            random_rect(canvas);

        for(size_t i = 0; i < batches_per_frame && !paused; ++i) {
            size_t s = temp.save();
            batch.process(&temp, batch_size, nn, t, rate);
            if(batch.finished) {
                da_append(&tplot, batch.cost);
                t.shuffle_rows();
                da_append(&vplot, nn.cost(v));
            }
            temp.rewind(s);
        }

        BeginDrawing();
        ClearBackground(GYM_BACKGROUND);
        gym_layout_begin(GLO_HORZ, gym_root(), 2, 10);
        gym_layout_begin(GLO_VERT, gym_layout_slot(), 2, 10);
        gym_plot(font, tplot, gym_layout_slot(), RED);
        gym_plot(font, vplot, gym_layout_slot(), GREEN);
        gym_layout_end();
        gym_layout_begin(GLO_VERT, gym_layout_slot(), 2, 10);
        gym_drawable_canvas(canvas, gym_layout_slot());
        canvas_to_row(nn.input(), canvas);
        nn.forward();
        {
            Gym_Rect slot = gym_layout_slot();
            gym_render_mat_as_heatmap(nn.output().as_mat(), slot, nn.output().cols);
            if(nn.output()[0] > nn.output()[1])
                DrawText("circle", slot.x, slot.y, slot.h * 0.08, WHITE);
            else if(nn.output()[0] < nn.output()[1])
                DrawText("rectangle", slot.x, slot.y, slot.h * 0.08, WHITE);
        }
        gym_layout_end();
        gym_layout_end();
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
