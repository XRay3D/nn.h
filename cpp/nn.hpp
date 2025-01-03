#pragma once
// TODO: make sure nn.h/gym.h is compilable with C++ compiler
// TODO: introduce NNDEF macro for every definition of nn.h

#include "elapsed_timer.hpp"
#include <algorithm>
#include <cmath>
#include <cstdbool>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstring>
#include <memory_resource>
#include <random>
#include <ranges>

// #define NN_BACKPROP_TRADITIONAL

#ifndef NN_ACT
#define NN_ACT ACT_SIG
#endif // NN_ACT

#ifndef NN_RELU_PARAM
#define NN_RELU_PARAM 0.01f
#endif // NN_RELU_PARAM

#ifndef NN_MALLOC
#include <cstdlib>
#define NN_MALLOC malloc
#endif // NN_MALLOC

#ifndef NN_ASSERT
#include <cassert>
#define NN_ASSERT assert
#endif // NN_ASSERT

// #define ARRAY_LEN(xs) std::size(xs)
struct Act {
    enum {
        RELU,
        SIG,
        SIN,
        TANH,
    } type;

    // Dispatch to the corresponding activation function
    float (*actf)(float);

    // Derivative of the activation function based on its value
    float (*dactf)(float);
};

inline float reluf(float x) {
    return x > 0 ? x : x * NN_RELU_PARAM;
}

inline float sigmoidf(float x) {
    return 1.f / (1.f + expf(-x));
}

inline float tanhf(float x) {
    float epx = expf(+x);
    float enx = expf(-x);
    return (epx - enx) / (epx + enx);
}

inline constexpr Act ACT_RELU{
    Act::RELU,
    reluf,
    [](float y) -> float { return y >= 0 ? 1 : NN_RELU_PARAM; },
};
inline constexpr Act ACT_SIG{
    Act::SIG,
    sigmoidf,
    [](float y) -> float { return y * (1 - y); },
};
inline constexpr Act ACT_SIN{
    Act::SIN,
    sinf,
    [](float y) -> float { return cosf(asinf(y)); },
};
inline constexpr Act ACT_TANH{
    Act::TANH,
    tanhf,
    [](float y) -> float { return 1 - y * y; },
};

float rand_float(void);

class Region {
    size_t capacity;
    size_t size;
    uintptr_t* words;

public:
    Region(size_t capacity_bytes) {
        size_t word_size = sizeof(*words);
        size_t capacity_words = (capacity_bytes + word_size - 1) / word_size;
        words = (decltype(words))NN_MALLOC(capacity_words * word_size);
        NN_ASSERT(words != nullptr);
        capacity = capacity_words;
    }

    static void* alloc(Region* r, size_t size_bytes) {
        if(r == nullptr) return NN_MALLOC(size_bytes);
        size_t word_size = sizeof(*r->words);
        size_t size_words = (size_bytes + word_size - 1) / word_size;

        NN_ASSERT(r->size + size_words <= r->capacity);
        if(r->size + size_words > r->capacity) return nullptr;
        void* result = &r->words[r->size];
        r->size += size_words;
        return result;
    }

    void reset() {
        NN_ASSERT((this) != nullptr);
        size = 0;
    }
    size_t occupied_bytes() const {
        NN_ASSERT((this) != nullptr);
        return size * sizeof(*words);
    }
    size_t save() {
        NN_ASSERT((this) != nullptr);
        return size;
    }
    void rewind(size_t s) {
        NN_ASSERT((this) != nullptr);
        size = s;
    }
};

// capacity is in bytes, but it can allocate more just to keep things
// word aligned
// Region region_alloc_alloc(size_t capacity_bytes);
// void* region_alloc(Region* r, size_t size_bytes);

template <typename T>
struct Util {
    T& self() { return static_cast<T&>(*this); }

    auto span() {
        return std::span{self().data(), self().size()};
    }

    void fill(float x) {
        std::ranges::fill(span(), x);
    }

    void rand(float low, float high) {
        for(auto&& var: span())
            var = rand_float() * (high - low) + low;
    }
};

struct Mat;

struct Row : Util<Row> {
    size_t cols;
    float* elements;

    auto size() const noexcept { return cols; }
    auto data() const noexcept { return elements; }
    auto begin() const noexcept { return elements; }
    auto end() const noexcept { return elements + size(); }

    auto operator[](size_t i) const { return elements[i]; }
    auto& operator[](size_t i) { return elements[i]; }

    inline Row slice(size_t i, size_t cols_) {
        NN_ASSERT(i < cols);
        NN_ASSERT(i + cols_ <= cols);
        return {
            .cols = cols_,
            .elements = elements + i,
        };
    }
    Mat as_mat();
};

struct Mat : Util<Mat> {
    size_t rows, cols;
    float* elements;

    // Mat(size_t rows, size_t cols)
    //     : rows{rows}, cols{cols} {
    //     elements = (float*)std::pmr::get_default_resource()
    //                    ->allocate( sizeof(*elements) * rows * cols,sizeof(*elements));
    //     NN_ASSERT(elements != nullptr);
    // }

    auto size() const noexcept { return rows * cols; }
    auto data() const noexcept { return elements; }
    auto begin() const noexcept { return elements; }
    auto end() const noexcept { return elements + size(); }

    auto operator[](size_t r) const {
        return std::span{elements + r * cols, cols};
    }
    auto operator[](size_t r) {
        return std::span{elements + r * cols, cols};
    }

    static Mat alloc(Region* r, size_t rows, size_t cols) {
        Mat m;
        m.rows = rows;
        m.cols = cols;
        m.elements = (float*)Region::alloc(r, sizeof(*m.elements) * rows * cols);
        NN_ASSERT(m.elements != nullptr);
        return m;
    }

    static Row row(Mat m, size_t row) {
        return (Row){
            .cols = m.cols,
            .elements = &m[row][0],
        };
    }

    static void copy(Mat dst, Mat src) {
        NN_ASSERT(dst.rows == src.rows);
        NN_ASSERT(dst.cols == src.cols);
        for(size_t i = 0; i < dst.rows; ++i)
            for(size_t j = 0; j < dst.cols; ++j)
                dst[i][j] = src[i][j];
    }

    Mat& operator+=(Mat a) {
        NN_ASSERT(rows == a.rows);
        NN_ASSERT(cols == a.cols);
        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                (*this)[i][j] += a[i][j];
        return *this;
    }

    void act() {
        for(size_t i = 0; i < rows; ++i)
            for(size_t j = 0; j < cols; ++j)
                (*this)[i][j] = NN_ACT.actf((*this)[i][j]);
    }

    void print(const char* name, size_t padding) {
        printf("%*s%s = [\n", (int)padding, "", name);
        for(size_t i = 0; i < rows; ++i) {
            printf("%*s    ", (int)padding, "");
            for(size_t j = 0; j < cols; ++j)
                printf("%f ", (*this)[i][j]);
            printf("\n");
        }
        printf("%*s]\n", (int)padding, "");
    }

    static void dot(Mat dst, Mat a, Mat b) {
        NN_ASSERT(a.cols == b.rows);
        NN_ASSERT(dst.rows == a.rows);
        NN_ASSERT(dst.cols == b.cols);

        const size_t n = a.cols;
#if 0
    size_t startAt = n % 4;
    auto *dr = dst.begin(), *ar = a.begin();
    while(dr < dst.end()) {
        for(size_t j = 0; dr < dst.end(); ++dr, ++j) {
            *dr = 0;
            auto* ark = ar;
            size_t iterations = n / 4, k = 0;
            switch(startAt) {
                do {
                case 0: *dr += *ark++ * b[k++][j];
                // case 7: *dr += *ark++ * b[k++][j];
                // case 6: *dr += *ark++ * b[k++][j];
                // case 5: *dr += *ark++ * b[k++][j];
                // case 4: *dr += *ark++ * b[k++][j];
                case 3: *dr += *ark++ * b[k++][j];
                case 2: *dr += *ark++ * b[k++][j];
                case 1: *dr += *ark++ * b[k++][j];
                } while(iterations--);
            }
            // for(size_t k = 0; k < n;)
            //     *dr += *ark++ * b[k++][j];
        }
        ar += a.cols;
    }

#else
        // ElapsedTimer et{};
        for(size_t i = 0; i < dst.rows; ++i) {
            for(size_t j = 0; j < dst.cols; ++j) {
                dst[i][j] = 0;
                for(size_t k = 0; k < n; ++k)
                    dst[i][j] += a[i][k] * b[k][j];
            }
        }
#endif
    }

    void shuffle_rows() {
        // std::ranges::shuffle(span(), ::rand);
        for(size_t i = 0; i < rows; ++i) {
            size_t j = i + ::rand() % (rows - i);
            if(i != j)
                for(size_t k = 0; k < cols; ++k)
                    std::swap((*this)[i][k], (*this)[j][k]);
        }
    }
};

inline Mat Row::as_mat() {
    return Mat{
        .rows = 1,
        .cols = cols,
        .elements = elements,
    };
}

// #define ROW_AT(row, col) (row).elements[col]
// void Mat::dot(Mat dst, Mat a, Mat b);
// Mat row_as_mat(Row row);
#define row_alloc(r, cols) Mat::row(Mat::alloc(r, 1, cols), 0)
Row row_slice(Row row, size_t i, size_t cols);
// #define row_rand(row, low, high)      mat_rand(row_as_mat(row), low, high)
// #define row_fill(row, x)              row_as_mat(row).fill(x);
#define row_print(row, name, padding) row.as_mat().print(name, padding)
#define row_copy(dst, src)            Mat::copy(dst.as_mat(), src.as_mat())

// #define m[i][j] (m).elements[(i) * (m).cols + (j)]
// Mat mat_alloc(Region* r, size_t rows, size_t cols);
// void mat_fill(Mat m, float x);
// void mat_rand(Mat m, float low, float high);
// Row mat_row(Mat m, size_t row);
// void mat_copy(Mat dst, Mat src);
// void Mat::dot(Mat dst, Mat a, Mat b);
// void mat_sum(Mat dst, Mat a);
// void mat_act(Mat m);
// void mat_print(Mat m, const char* name, size_t padding);
// void mat_shuffle_rows(Mat m);
#define MAT_PRINT(m) m.print(#m, 0)

struct NN {
    const size_t* arch;
    size_t arch_count;
    Mat* ws; // The amount of activations is arch_count-1
    Row* bs; // The amount of activations is arch_count-1

    // TODO: maybe remove these? It would be better to allocate them in a
    // temporary region during the actual forwarding
    Row* as;

    static NN alloc(Region* r, std::span<const size_t> arch) {
        NN_ASSERT(arch.size() > 0);

        NN nn;
        nn.arch = arch.data();
        nn.arch_count = arch.size();

        nn.ws = (decltype(nn.ws))Region::alloc(r, sizeof(*nn.ws) * (nn.arch_count - 1));
        NN_ASSERT(nn.ws != nullptr);
        nn.bs = (decltype(nn.bs))Region::alloc(r, sizeof(*nn.bs) * (nn.arch_count - 1));
        NN_ASSERT(nn.bs != nullptr);
        nn.as = (decltype(nn.as))Region::alloc(r, sizeof(*nn.as) * nn.arch_count);
        NN_ASSERT(nn.as != nullptr);

        nn.as[0] = row_alloc(r, arch[0]);
        for(size_t i = 1; i < arch.size(); ++i) {
            nn.ws[i - 1] = Mat::alloc(r, nn.as[i - 1].cols, arch[i]);
            nn.bs[i - 1] = row_alloc(r, arch[i]);
            nn.as[i] = row_alloc(r, arch[i]);
        }

        return nn;
    }
    void zero() {
        for(size_t i = 0; i < arch_count - 1; ++i) {
            ws[i].fill(0);
            bs[i].fill(0);
            as[i].fill(0);
        }
        as[arch_count - 1].fill(0);
    }

    void print(const char* name) {
        char buf[256];
        printf("%s = [\n", name);
        for(size_t i = 0; i < arch_count - 1; ++i) {
            snprintf(buf, sizeof(buf), "ws%zu", i);
            ws[i].print(buf, 4);
            snprintf(buf, sizeof(buf), "bs%zu", i);
            row_print(bs[i], buf, 4);
        }
        printf("]\n");
    }

    void rand(float low, float high) {
        for(size_t i = 0; i < arch_count - 1; ++i) {
            ws[i].rand(low, high);
            bs[i].rand(low, high);
        }
    }

    auto input() {
        NN_ASSERT(arch_count > 0);
        return as[0];
    }
    auto output() {
        NN_ASSERT(arch_count > 0);
        return as[arch_count - 1];
    }

    void forward() {
        for(size_t i = 0; i < arch_count - 1; ++i) {
            Mat::dot(as[i + 1].as_mat(), as[i].as_mat(), ws[i]);
            as[i + 1].as_mat() += as[i].as_mat();
            as[i + 1].as_mat().act();
        }
    }

    float cost(Mat t) {
        NN_ASSERT(input().cols + output().cols == t.cols);
        size_t n = t.rows;

        float c = 0;
        for(size_t i = 0; i < n; ++i) {
            Row row = Mat::row(t, i);
            Row x = row.slice(0, input().cols);
            Row y = row.slice(input().cols, output().cols);

            row_copy(input(), x);
            forward();
            size_t q = y.cols;
            for(size_t j = 0; j < q; ++j) {
                float d = output()[j] - y[j];
                c += d * d;
            }
        }

        return c / n;
    }

    NN backprop(Region* r, Mat t) {
        size_t n = t.rows;
        NN_ASSERT(input().cols + output().cols == t.cols);

        NN g = NN::alloc(r, {arch, arch_count});
        g.zero();

        // i-current sample
        // l-current layer
        // j-current activation
        // k-previous activation

        for(size_t i = 0; i < n; ++i) {
            Row row = Mat::row(t, i);
            Row in = row.slice(0, input().cols);
            Row out = row.slice(input().cols, output().cols);

            row_copy(input(), in);
            forward();

            for(size_t j = 0; j < arch_count; ++j)
                g.as[j].fill(0);

            for(size_t j = 0; j < out.cols; ++j) {
#ifdef NN_BACKPROP_TRADITIONAL
                g.output()[j] = 2 * (output()[j] - out[j]);
#else
                g.output()[j] = output()[j] - out[j];
#endif // NN_BACKPROP_TRADITIONAL
            }

#ifdef NN_BACKPROP_TRADITIONAL
            float s = 1;
#else
            float s = 2;
#endif // NN_BACKPROP_TRADITIONAL

            for(size_t l = arch_count - 1; l > 0; --l) {
                for(size_t j = 0; j < as[l].cols; ++j) {
                    float a = as[l][j];
                    float da = g.as[l][j];
                    float qa = NN_ACT.dactf(a);
                    g.bs[l - 1][j] += s * da * qa;
                    for(size_t k = 0; k < as[l - 1].cols; ++k) {
                        // j-weight matrix col
                        // k-weight matrix row
                        float pa = as[l - 1][k];
                        float w = ws[l - 1][k][j];
                        g.ws[l - 1][k][j] += s * da * qa * pa;
                        g.as[l - 1][k] += s * da * qa * w;
                    }
                }
            }
        }

        for(size_t i = 0; i < g.arch_count - 1; ++i) {
            for(size_t j = 0; j < g.ws[i].rows; ++j)
                for(size_t k = 0; k < g.ws[i].cols; ++k)
                    g.ws[i][j][k] /= n;
            for(size_t k = 0; k < g.bs[i].cols; ++k)
                g.bs[i][k] /= n;
        }

        return g;
    }

    NN finite_diff(Region* r, Mat t, float eps) {
        float saved;
        float c = cost(t);

        NN g = NN::alloc(r, {arch, arch_count});

        for(size_t i = 0; i < arch_count - 1; ++i) {
            for(size_t j = 0; j < ws[i].rows; ++j) {
                for(size_t k = 0; k < ws[i].cols; ++k) {
                    saved = ws[i][j][k];
                    ws[i][j][k] += eps;
                    g.ws[i][j][k] = (cost(t) - c) / eps;
                    ws[i][j][k] = saved;
                }
            }

            for(size_t k = 0; k < bs[i].cols; ++k) {
                saved = bs[i][k];
                bs[i][k] += eps;
                g.bs[i][k] = (cost(t) - c) / eps;
                bs[i][k] = saved;
            }
        }

        return g;
    }

    void learn(NN g, float rate) {
        for(size_t i = 0; i < arch_count - 1; ++i) {
            for(size_t j = 0; j < ws[i].rows; ++j)
                for(size_t k = 0; k < ws[i].cols; ++k)
                    ws[i][j][k] -= rate * g.ws[i][j][k];

            for(size_t k = 0; k < bs[i].cols; ++k)
                bs[i][k] -= rate * g.bs[i][k];
        }
    }
};

#define NN_PRINT(nn) nn.print(#nn);

struct Batch {
    size_t begin;
    float cost;
    bool finished;
    void process(Region* r, size_t batch_size, NN nn, Mat t, float rate) {
        ElapsedTimer et{};
        if(finished) {
            finished = false;
            begin = 0;
            cost = 0;
        }

        size_t size = batch_size;
        if(begin + batch_size >= t.rows)
            size = t.rows - begin;

        // TODO: introduce similar to row_slice operation but for Mat that will give you subsequence of rows
        Mat batch_t = {
            .rows = size,
            .cols = t.cols,
            .elements = &t[begin][0],
        };

        NN g = nn.backprop(r, batch_t);
        nn.learn(g, rate);
        cost += nn.cost(batch_t);
        begin += batch_size;

        if(begin >= t.rows) {
            size_t batch_count = (t.rows + batch_size - 1) / batch_size;
            cost /= batch_count;
            finished = true;
        }
    }
};

inline float rand_float(void) {
    static std::random_device rd;
    static std::uniform_real_distribution<float> dist{0.f, 1.f};
    return dist(rd);
    // return (float)rand() / (float)RAND_MAX;
}
