// Copyright 2022 Alexey Kutepov <reximkut@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef OLIVE_C_
#define OLIVE_C_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef OLIVECDEF
#ifdef __cplusplus
#define OLIVECDEF extern "C" inline
#else
#define OLIVECDEF static inline
#endif
#endif

#ifndef OLIVEC_AA_RES
#define OLIVEC_AA_RES 2
#endif

#define OLIVEC_SWAP(T, a, b) \
    do {                     \
        T t = a;             \
        a = b;               \
        b = t;               \
    } while(0)
#define OLIVEC_SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define OLIVEC_ABS(T, x)  (OLIVEC_SIGN(T, x) * (x))

typedef struct {
    size_t width, height;
    const char* glyphs;
} Olivec_Font;

#define OLIVEC_DEFAULT_FONT_HEIGHT 6
#define OLIVEC_DEFAULT_FONT_WIDTH  6
// TODO: allocate proper descender and acender areas for the default font
static char olivec_default_glyphs[128][OLIVEC_DEFAULT_FONT_HEIGHT][OLIVEC_DEFAULT_FONT_WIDTH] = {
    //     hex | dec | oct | 'c'h
    // 0x00 | 0 | 0 | 'N'UL (null)
    {0},
    // 0x01 | 1 | 1 | 'S'OH (start of header)
    {0},
    // 0x02 | 2 | 2 | 'S'TX (start of text)
    {0},
    // 0x03 | 3 | 3 | 'E'TX (end of text)
    {0},
    // 0x04 | 4 | 4 | 'E'OT (end of transmission)
    {0},
    // 0x05 | 5 | 5 | 'E'NQ (enquiry)
    {0},
    // 0x06 | 6 | 6 | 'A'CK (acknowledge)
    {0},
    // 0x07 | 7 | 7 | 'B'EL (bell)
    {0},
    // 0x08 | 8 | 10 | 'B'S (backspace)
    {0},
    // 0x09 | 9 | 11 | 'H'T (horizontal tab)
    {0},
    // 0x0A | 10 | 12 | 'L'F (line feed - new line)
    {0},
    // 0x0B | 11 | 13 | 'V'T (vertical tab)
    {0},
    // 0x0C | 12 | 14 | 'F'F (form feed - new page)
    {0},
    // 0x0D | 13 | 15 | 'C'R (carriage return)
    {0},
    // 0x0E | 14 | 16 | 'S'O (shift out)
    {0},
    // 0x0F | 15 | 17 | 'S'I (shift in)
    {0},
    // 0x10 | 16 | 20 | 'D'LE (data link escape)
    {0},
    // 0x11 | 17 | 21 | 'D'C1 (device control 1)
    {0},
    // 0x12 | 18 | 22 | 'D'C2 (device control 2)
    {0},
    // 0x13 | 19 | 23 | 'D'C3 (device control 3)
    {0},
    // 0x14 | 20 | 24 | 'D'C4 (device control 4)
    {0},
    // 0x15 | 21 | 25 | 'N'AK (negative acknowledge)
    {0},
    // 0x16 | 22 | 26 | 'S'YN (synchronous idle)
    {0},
    // 0x17 | 23 | 27 | 'E'TB (end of transmission block)
    {0},
    // 0x18 | 24 | 30 | 'C'AN (cancel)
    {0},
    // 0x19 | 25 | 31 | 'E'M (end of medium)
    {0},
    // 0x1A | 26 | 32 | 'S'UB (substitute)
    {0},
    // 0x1B | 27 | 33 | 'E'SC (escape)
    {0},
    // 0x1C | 28 | 34 | 'F'S (file separator)
    {0},
    // 0x1D | 29 | 35 | 'G'S (group separator)
    {0},
    // 0x1E | 30 | 36 | 'R'S (record separator)
    {0},
    // 0x1F | 31 | 37 | 'U'S (unit separator)
    {0},
    // 0x20 | 32 | 40 | ' '
    {0},
    // 0x21 | 33 | 41 | '!'
    {0},
    // 0x22 | 34 | 42 | '"'
    {0},
    // 0x23 | 35 | 43 | '#'
    {0},
    // 0x24 | 36 | 44 | '$'
    {0},
    // 0x25 | 37 | 45 | '%'
    {0},
    // 0x26 | 38 | 46 | '&'
    {0},
    // 0x27 | 39 | 47 | '''
    {0},
    // 0x28 | 40 | 50 | '('
    {0},
    // 0x29 | 41 | 51 | ')'
    {0},
    // 0x2a | 42 | 52 | '*'
    {0},
    // 0x2b | 43 | 53 | '+'
    {0},
    // 0x2C | 44 | 54 | ','
    {
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 1, 0},
     {0, 0, 1, 0, 0},
     },
    // 0x2D | 45 | 55 | '-'
    {
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {1, 1, 1, 1, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     },
    // 0x2E | 46 | 56 | '.'
    {
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     },
    // 0x2f | 47 | 57 | '/'
    {0},
    // 0x30 | 48 | 60 | '0'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x31 | 49 | 61 | '1'
    {
     {0, 0, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     },
    // 0x32 | 50 | 62 | '2'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {0, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 1, 0},
     },
    // 0x33 | 51 | 63 | '3'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x34 | 52 | 64 | '4'
    {
     {0, 0, 1, 1, 0},
     {0, 1, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 1, 1, 1, 1},
     {0, 0, 0, 1, 0},
     {0, 0, 0, 1, 0},
     },
    // 0x35 | 53 | 65 | '5'
    {
     {1, 1, 1, 0, 0},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 0, 0},
     {0, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x36 | 54 | 66 | '6'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 0, 0},
     {1, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x37 | 55 | 67 | '7'
    {
     {1, 1, 1, 1, 0},
     {0, 0, 0, 1, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     },
    // 0x38 | 56 | 70 | '8'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x39 | 57 | 71 | '9'
    {
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     {0, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x3A | 58 | 72 | ':'
    {0},
    // 0x3B | 59 | 73 | ';'
    {0},
    // 0x3C | 60 | 74 | '<'
    {0},
    // 0x3D | 61 | 75 | '='
    {0},
    // 0x3E | 62 | 76 | '>'
    {0},
    // 0x3F | 63 | 77 | '?'
    {0},
    // 0x40 | 64 | 100 | '@'
    {0},
    // 0x41 | 65 | 101 | 'A'
    {0},
    // 0x42 | 66 | 102 | 'B'
    {0},
    // 0x43 | 67 | 103 | 'C'
    {0},
    // 0x44 | 68 | 104 | 'D'
    {0},
    // 0x45 | 69 | 105 | 'E'
    {0},
    // 0x46 | 70 | 106 | 'F'
    {0},
    // 0x47 | 71 | 107 | 'G'
    {0},
    // 0x48 | 72 | 110 | 'H'
    {0},
    // 0x49 | 73 | 111 | 'I'
    {0},
    // 0x4A | 74 | 112 | 'J'
    {0},
    // 0x4B | 75 | 113 | 'K'
    {0},
    // 0x4C | 76 | 114 | 'L'
    {0},
    // 0x4D | 77 | 115 | 'M'
    {0},
    // 0x4E | 78 | 116 | 'N'
    {0},
    // 0x4F | 79 | 117 | 'O'
    {0},
    // 0x50 | 80 | 120 | 'P'
    {0},
    // 0x51 | 81 | 121 | 'Q'
    {0},
    // 0x52 | 82 | 122 | 'R'
    {0},
    // 0x53 | 83 | 123 | 'S'
    {0},
    // 0x54 | 84 | 124 | 'T'
    {0},
    // 0x55 | 85 | 125 | 'U'
    {0},
    // 0x56 | 86 | 126 | 'V'
    {0},
    // 0x57 | 87 | 127 | 'W'
    {0},
    // 0x58 | 88 | 130 | 'X'
    {0},
    // 0x59 | 89 | 131 | 'Y'
    {0},
    // 0x5A | 90 | 132 | 'Z'
    {0},
    // 0x5B | 91 | 133 | '['
    {0},
    // 0x5C | 92 | 134 | '\'
    {0},
    // 0x5D | 93 | 135 | ']'
    {0},
    // 0x5E | 94 | 136 | '^'
    {0},
    // 0x5F | 95 | 137 | '_'
    {0},
    // 0x60 | 96 | 140 | '`'
    {0},
    // 0x61 | 97 | 141 | 'a'
    {
     {0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     },
    // 0x62 | 98 | 142 | 'b'
    {
     {1, 0, 0, 0, 0},
     {1, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 1, 1, 0, 0},
     },
    // 0x63 | 99 | 143 | 'c'
    {
     {0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 0, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x64 | 100 | 144 | 'd'
    {
     {0, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 1, 0},
     },
    // 0x65 | 101 | 145 | 'e'
    {
     {0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 1, 1, 1, 0},
     {1, 0, 0, 0, 0},
     {0, 1, 1, 1, 0},
     },
    // 0x66 | 102 | 146 | 'f'
    {
     {0, 0, 1, 1, 0},
     {0, 1, 0, 0, 0},
     {1, 1, 1, 1, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     },
    // 0x67 | 103 | 147 | 'g'
    {0},
    // 0x68 | 104 | 150 | 'h'
    {
     {1, 0, 0, 0, 0},
     {1, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     },
    // 0x69 | 105 | 151 | 'i'
    {
     {0, 0, 1, 0, 0},
     {0, 0, 0, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     },
    // 0x6A | 106 | 152 | 'j'
    {0},
    // 0x6B | 107 | 153 | 'k'
    {
     {0, 1, 0, 0, 0},
     {0, 1, 0, 0, 0},
     {0, 1, 0, 1, 0},
     {0, 1, 1, 0, 0},
     {0, 1, 1, 0, 0},
     {0, 1, 0, 1, 0},
     },
    // 0x6C | 108 | 154 | 'l'
    {
     {0, 1, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 0, 1, 0, 0},
     {0, 1, 1, 1, 0},
     },
    // 0x6D | 109 | 155 | 'm'
    {0},
    // 0x6E | 110 | 156 | 'n'
    {0},
    // 0x6F | 111 | 157 | 'o'
    {
     {0, 0, 0, 0, 0},
     {0, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {0, 1, 1, 0, 0},
     },
    // 0x70 | 112 | 160 | 'p'
    {
     {1, 1, 1, 0, 0},
     {1, 0, 0, 1, 0},
     {1, 0, 0, 1, 0},
     {1, 1, 1, 0, 0},
     {1, 0, 0, 0, 0},
     {1, 0, 0, 0, 0},
     },
    // 0x71 | 113 | 161 | 'q'
    {0},
    // 0x72 | 114 | 162 | 'r'
    {
     {0, 0, 0, 0, 0},
     {1, 0, 1, 1, 0},
     {1, 1, 0, 0, 1},
     {1, 0, 0, 0, 0},
     {1, 0, 0, 0, 0},
     {1, 0, 0, 0, 0},
     },
    // 0x73 | 115 | 163 | 's'
    {0},
    // 0x74 | 116 | 164 | 't'
    {0},
    // 0x75 | 117 | 165 | 'u'
    {0},
    // 0x76 | 118 | 166 | 'v'
    {0},
    // 0x77 | 119 | 167 | 'w'
    {
     {0, 0, 0, 0, 0},
     {1, 0, 0, 0, 1},
     {1, 0, 1, 0, 1},
     {1, 0, 1, 0, 1},
     {1, 0, 1, 0, 1},
     {0, 1, 1, 1, 1},
     },
    // 0x78 | 120 | 170 | 'x'
    {0},
    // 0x79 | 121 | 171 | 'y'
    {0},
    // 0x7A | 122 | 172 | 'z'
    {0},
    // 0x7B | 123 | 173 | '{'
    {0},
    // 0x7C | 124 | 174 | '|'
    {0},
    // 0x7D | 125 | 175 | '}'
    {0},
    // 0x7E | 126 | 176 | '~'
    {0},
    // 0x7F | 127 | 177 | DEL (delete)
    {0},
};

static Olivec_Font olivec_default_font = {
    .width = OLIVEC_DEFAULT_FONT_WIDTH,
    .height = OLIVEC_DEFAULT_FONT_HEIGHT,
    .glyphs = &olivec_default_glyphs[0][0][0],
};

typedef struct {
    uint32_t* pixels;
    size_t width;
    size_t height;
    size_t stride;
} Olivec_Canvas;

#define OLIVEC_CANVAS_NULL     ((Olivec_Canvas){0})
#define OLIVEC_PIXEL(oc, x, y) (oc).pixels[(y) * (oc).stride + (x)]

OLIVECDEF Olivec_Canvas olivec_canvas(uint32_t* pixels, size_t width, size_t height, size_t stride);
OLIVECDEF Olivec_Canvas olivec_subcanvas(Olivec_Canvas oc, int x, int y, int w, int h);
OLIVECDEF bool olivec_in_bounds(Olivec_Canvas oc, int x, int y);
OLIVECDEF void olivec_blend_color(uint32_t* c1, uint32_t c2);
OLIVECDEF void olivec_fill(Olivec_Canvas oc, uint32_t color);
OLIVECDEF void olivec_rect(Olivec_Canvas oc, int x, int y, int w, int h, uint32_t color);
OLIVECDEF void olivec_frame(Olivec_Canvas oc, int x, int y, int w, int h, size_t thiccness, uint32_t color);
OLIVECDEF void olivec_circle(Olivec_Canvas oc, int cx, int cy, int r, uint32_t color);
OLIVECDEF void olivec_ellipse(Olivec_Canvas oc, int cx, int cy, int rx, int ry, uint32_t color);
// TODO: lines with different thiccness
OLIVECDEF void olivec_line(Olivec_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color);
OLIVECDEF bool olivec_normalize_triangle(size_t width, size_t height, int x1, int y1, int x2, int y2, int x3, int y3, int* lx, int* hx, int* ly, int* hy);
OLIVECDEF bool olivec_barycentric(int x1, int y1, int x2, int y2, int x3, int y3, int xp, int yp, int* u1, int* u2, int* det);
OLIVECDEF void olivec_triangle(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color);
OLIVECDEF void olivec_triangle3c(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t c1, uint32_t c2, uint32_t c3);
OLIVECDEF void olivec_triangle3z(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3);
OLIVECDEF void olivec_triangle3uv(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture);
OLIVECDEF void olivec_triangle3uv_bilinear(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture);
OLIVECDEF void olivec_text(Olivec_Canvas oc, const char* text, int x, int y, Olivec_Font font, size_t size, uint32_t color);
OLIVECDEF void olivec_sprite_blend(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF void olivec_sprite_copy(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF void olivec_sprite_copy_bilinear(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite);
OLIVECDEF uint32_t olivec_pixel_bilinear(Olivec_Canvas sprite, int nx, int ny, int w, int h);

typedef struct {
    // Safe ranges to iterate over.
    int x1, x2;
    int y1, y2;

    // Original uncut ranges some parts of which may be outside of the canvas boundaries.
    int ox1, ox2;
    int oy1, oy2;
} Olivec_Normalized_Rect;

// The point of this function is to produce two ranges x1..x2 and y1..y2 that are guaranteed to be safe to iterate over the canvas of size pixels_width by pixels_height without any boundary checks.
//
// Olivec_Normalized_Rect nr = {0};
// if (olivec_normalize_rect(x, y, w, h, WIDTH, HEIGHT, &nr)) {
//     for (int x = nr.x1; x <= nr.x2; ++x) {
//         for (int y = nr.y1; y <= nr.y2; ++y) {
//             OLIVEC_PIXEL(oc, x, y) = 0x69696969;
//         }
//     }
// } else {
//     // Rectangle is invisible cause it's completely out-of-bounds
// }
OLIVECDEF bool olivec_normalize_rect(int x, int y, int w, int h,
    size_t canvas_width, size_t canvas_height,
    Olivec_Normalized_Rect* nr);

#endif // OLIVE_C_

#ifdef OLIVEC_IMPLEMENTATION

OLIVECDEF Olivec_Canvas olivec_canvas(uint32_t* pixels, size_t width, size_t height, size_t stride) {
    Olivec_Canvas oc = {
        .pixels = pixels,
        .width = width,
        .height = height,
        .stride = stride,
    };
    return oc;
}

OLIVECDEF bool olivec_normalize_rect(int x, int y, int w, int h,
    size_t canvas_width, size_t canvas_height,
    Olivec_Normalized_Rect* nr) {
    // No need to render empty rectangle
    if(w == 0) return false;
    if(h == 0) return false;

    nr->ox1 = x;
    nr->oy1 = y;

    // Convert the rectangle to 2-points representation
    nr->ox2 = nr->ox1 + OLIVEC_SIGN(int, w) * (OLIVEC_ABS(int, w) - 1);
    if(nr->ox1 > nr->ox2) OLIVEC_SWAP(int, nr->ox1, nr->ox2);
    nr->oy2 = nr->oy1 + OLIVEC_SIGN(int, h) * (OLIVEC_ABS(int, h) - 1);
    if(nr->oy1 > nr->oy2) OLIVEC_SWAP(int, nr->oy1, nr->oy2);

    // Cull out invisible rectangle
    if(nr->ox1 >= (int)canvas_width) return false;
    if(nr->ox2 < 0) return false;
    if(nr->oy1 >= (int)canvas_height) return false;
    if(nr->oy2 < 0) return false;

    nr->x1 = nr->ox1;
    nr->y1 = nr->oy1;
    nr->x2 = nr->ox2;
    nr->y2 = nr->oy2;

    // Clamp the rectangle to the boundaries
    if(nr->x1 < 0) nr->x1 = 0;
    if(nr->x2 >= (int)canvas_width) nr->x2 = (int)canvas_width - 1;
    if(nr->y1 < 0) nr->y1 = 0;
    if(nr->y2 >= (int)canvas_height) nr->y2 = (int)canvas_height - 1;

    return true;
}

OLIVECDEF Olivec_Canvas olivec_subcanvas(Olivec_Canvas oc, int x, int y, int w, int h) {
    Olivec_Normalized_Rect nr = {0};
    if(!olivec_normalize_rect(x, y, w, h, oc.width, oc.height, &nr)) return OLIVEC_CANVAS_NULL;
    oc.pixels = &OLIVEC_PIXEL(oc, nr.x1, nr.y1);
    oc.width = nr.x2 - nr.x1 + 1;
    oc.height = nr.y2 - nr.y1 + 1;
    return oc;
}

// TODO: custom pixel formats
// Maybe we can store pixel format info in Olivec_Canvas
#define OLIVEC_RED(color)       (((color) & 0x000000FF) >> (8 * 0))
#define OLIVEC_GREEN(color)     (((color) & 0x0000FF00) >> (8 * 1))
#define OLIVEC_BLUE(color)      (((color) & 0x00FF0000) >> (8 * 2))
#define OLIVEC_ALPHA(color)     (((color) & 0xFF000000) >> (8 * 3))
#define OLIVEC_RGBA(r, g, b, a) ((((r) & 0xFF) << (8 * 0)) | (((g) & 0xFF) << (8 * 1)) | (((b) & 0xFF) << (8 * 2)) | (((a) & 0xFF) << (8 * 3)))

OLIVECDEF void olivec_blend_color(uint32_t* c1, uint32_t c2) {
    uint32_t r1 = OLIVEC_RED(*c1);
    uint32_t g1 = OLIVEC_GREEN(*c1);
    uint32_t b1 = OLIVEC_BLUE(*c1);
    uint32_t a1 = OLIVEC_ALPHA(*c1);

    uint32_t r2 = OLIVEC_RED(c2);
    uint32_t g2 = OLIVEC_GREEN(c2);
    uint32_t b2 = OLIVEC_BLUE(c2);
    uint32_t a2 = OLIVEC_ALPHA(c2);

    r1 = (r1 * (255 - a2) + r2 * a2) / 255;
    if(r1 > 255) r1 = 255;
    g1 = (g1 * (255 - a2) + g2 * a2) / 255;
    if(g1 > 255) g1 = 255;
    b1 = (b1 * (255 - a2) + b2 * a2) / 255;
    if(b1 > 255) b1 = 255;

    *c1 = OLIVEC_RGBA(r1, g1, b1, a1);
}

OLIVECDEF void olivec_fill(Olivec_Canvas oc, uint32_t color) {
    for(size_t y = 0; y < oc.height; ++y)
        for(size_t x = 0; x < oc.width; ++x)
            OLIVEC_PIXEL(oc, x, y) = color;
}

OLIVECDEF void olivec_rect(Olivec_Canvas oc, int x, int y, int w, int h, uint32_t color) {
    Olivec_Normalized_Rect nr = {0};
    if(!olivec_normalize_rect(x, y, w, h, oc.width, oc.height, &nr)) return;
    for(int x = nr.x1; x <= nr.x2; ++x)
        for(int y = nr.y1; y <= nr.y2; ++y)
            olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), color);
}

OLIVECDEF void olivec_frame(Olivec_Canvas oc, int x, int y, int w, int h, size_t t, uint32_t color) {
    if(t == 0) return; // Nothing to render

    // Convert the rectangle to 2-points representation
    int x1 = x;
    int y1 = y;
    int x2 = x1 + OLIVEC_SIGN(int, w) * (OLIVEC_ABS(int, w) - 1);
    if(x1 > x2) OLIVEC_SWAP(int, x1, x2);
    int y2 = y1 + OLIVEC_SIGN(int, h) * (OLIVEC_ABS(int, h) - 1);
    if(y1 > y2) OLIVEC_SWAP(int, y1, y2);

    olivec_rect(oc, x1 - t / 2, y1 - t / 2, (x2 - x1 + 1) + t / 2 * 2, t, color);  // Top
    olivec_rect(oc, x1 - t / 2, y1 - t / 2, t, (y2 - y1 + 1) + t / 2 * 2, color);  // Left
    olivec_rect(oc, x1 - t / 2, y2 + t / 2, (x2 - x1 + 1) + t / 2 * 2, -t, color); // Bottom
    olivec_rect(oc, x2 + t / 2, y1 - t / 2, -t, (y2 - y1 + 1) + t / 2 * 2, color); // Right
}

OLIVECDEF void olivec_ellipse(Olivec_Canvas oc, int cx, int cy, int rx, int ry, uint32_t color) {
    Olivec_Normalized_Rect nr = {0};
    int rx1 = rx + OLIVEC_SIGN(int, rx);
    int ry1 = ry + OLIVEC_SIGN(int, ry);
    if(!olivec_normalize_rect(cx - rx1, cy - ry1, 2 * rx1, 2 * ry1, oc.width, oc.height, &nr)) return;

    for(int y = nr.y1; y <= nr.y2; ++y) {
        for(int x = nr.x1; x <= nr.x2; ++x) {
            float nx = (x + 0.5 - nr.x1) / (2.0f * rx1);
            float ny = (y + 0.5 - nr.y1) / (2.0f * ry1);
            float dx = nx - 0.5;
            float dy = ny - 0.5;
            if(dx * dx + dy * dy <= 0.5 * 0.5)
                OLIVEC_PIXEL(oc, x, y) = color;
        }
    }
}

OLIVECDEF void olivec_circle(Olivec_Canvas oc, int cx, int cy, int r, uint32_t color) {
    Olivec_Normalized_Rect nr = {0};
    int r1 = r + OLIVEC_SIGN(int, r);
    if(!olivec_normalize_rect(cx - r1, cy - r1, 2 * r1, 2 * r1, oc.width, oc.height, &nr)) return;

    for(int y = nr.y1; y <= nr.y2; ++y) {
        for(int x = nr.x1; x <= nr.x2; ++x) {
            int count = 0;
            for(int sox = 0; sox < OLIVEC_AA_RES; ++sox) {
                for(int soy = 0; soy < OLIVEC_AA_RES; ++soy) {
                    // TODO: switch to 64 bits to make the overflow less likely
                    // Also research the probability of overflow
                    int res1 = (OLIVEC_AA_RES + 1);
                    int dx = (x * res1 * 2 + 2 + sox * 2 - res1 * cx * 2 - res1);
                    int dy = (y * res1 * 2 + 2 + soy * 2 - res1 * cy * 2 - res1);
                    if(dx * dx + dy * dy <= res1 * res1 * r * r * 2 * 2) count += 1;
                }
            }
            uint32_t alpha = ((color & 0xFF000000) >> (3 * 8)) * count / OLIVEC_AA_RES / OLIVEC_AA_RES;
            uint32_t updated_color = (color & 0x00FFFFFF) | (alpha << (3 * 8));
            olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), updated_color);
        }
    }
}

OLIVECDEF bool olivec_in_bounds(Olivec_Canvas oc, int x, int y) {
    return 0 <= x && x < (int)oc.width && 0 <= y && y < (int)oc.height;
}

// TODO: AA for line
OLIVECDEF void olivec_line(Olivec_Canvas oc, int x1, int y1, int x2, int y2, uint32_t color) {
    int dx = x2 - x1;
    int dy = y2 - y1;

    // If both of the differences are 0 there will be a division by 0 below.
    if(dx == 0 && dy == 0) {
        if(olivec_in_bounds(oc, x1, y1))
            olivec_blend_color(&OLIVEC_PIXEL(oc, x1, y1), color);
        return;
    }

    if(OLIVEC_ABS(int, dx) > OLIVEC_ABS(int, dy)) {
        if(x1 > x2) {
            OLIVEC_SWAP(int, x1, x2);
            OLIVEC_SWAP(int, y1, y2);
        }

        for(int x = x1; x <= x2; ++x) {
            int y = dy * (x - x1) / dx + y1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if(olivec_in_bounds(oc, x, y))
                olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), color);
        }
    } else {
        if(y1 > y2) {
            OLIVEC_SWAP(int, x1, x2);
            OLIVEC_SWAP(int, y1, y2);
        }

        for(int y = y1; y <= y2; ++y) {
            int x = dx * (y - y1) / dy + x1;
            // TODO: move boundary checks out side of the loops in olivec_draw_line
            if(olivec_in_bounds(oc, x, y))
                olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), color);
        }
    }
}

OLIVECDEF uint32_t mix_colors2(uint32_t c1, uint32_t c2, int u1, int det) {
    // TODO: estimate how much overflows are an issue in integer only environment
    int64_t r1 = OLIVEC_RED(c1);
    int64_t g1 = OLIVEC_GREEN(c1);
    int64_t b1 = OLIVEC_BLUE(c1);
    int64_t a1 = OLIVEC_ALPHA(c1);

    int64_t r2 = OLIVEC_RED(c2);
    int64_t g2 = OLIVEC_GREEN(c2);
    int64_t b2 = OLIVEC_BLUE(c2);
    int64_t a2 = OLIVEC_ALPHA(c2);

    if(det != 0) {
        int u2 = det - u1;
        int64_t r4 = (r1 * u2 + r2 * u1) / det;
        int64_t g4 = (g1 * u2 + g2 * u1) / det;
        int64_t b4 = (b1 * u2 + b2 * u1) / det;
        int64_t a4 = (a1 * u2 + a2 * u1) / det;

        return OLIVEC_RGBA(r4, g4, b4, a4);
    }

    return 0;
}

OLIVECDEF uint32_t mix_colors3(uint32_t c1, uint32_t c2, uint32_t c3, int u1, int u2, int det) {
    // TODO: estimate how much overflows are an issue in integer only environment
    int64_t r1 = OLIVEC_RED(c1);
    int64_t g1 = OLIVEC_GREEN(c1);
    int64_t b1 = OLIVEC_BLUE(c1);
    int64_t a1 = OLIVEC_ALPHA(c1);

    int64_t r2 = OLIVEC_RED(c2);
    int64_t g2 = OLIVEC_GREEN(c2);
    int64_t b2 = OLIVEC_BLUE(c2);
    int64_t a2 = OLIVEC_ALPHA(c2);

    int64_t r3 = OLIVEC_RED(c3);
    int64_t g3 = OLIVEC_GREEN(c3);
    int64_t b3 = OLIVEC_BLUE(c3);
    int64_t a3 = OLIVEC_ALPHA(c3);

    if(det != 0) {
        int u3 = det - u1 - u2;
        int64_t r4 = (r1 * u1 + r2 * u2 + r3 * u3) / det;
        int64_t g4 = (g1 * u1 + g2 * u2 + g3 * u3) / det;
        int64_t b4 = (b1 * u1 + b2 * u2 + b3 * u3) / det;
        int64_t a4 = (a1 * u1 + a2 * u2 + a3 * u3) / det;

        return OLIVEC_RGBA(r4, g4, b4, a4);
    }

    return 0;
}

// NOTE: we imply u3 = det - u1 - u2
OLIVECDEF bool olivec_barycentric(int x1, int y1, int x2, int y2, int x3, int y3, int xp, int yp, int* u1, int* u2, int* det) {
    *det = ((x1 - x3) * (y2 - y3) - (x2 - x3) * (y1 - y3));
    *u1 = ((y2 - y3) * (xp - x3) + (x3 - x2) * (yp - y3));
    *u2 = ((y3 - y1) * (xp - x3) + (x1 - x3) * (yp - y3));
    int u3 = *det - *u1 - *u2;
    return (
        (OLIVEC_SIGN(int, *u1) == OLIVEC_SIGN(int, *det) || *u1 == 0) && (OLIVEC_SIGN(int, *u2) == OLIVEC_SIGN(int, *det) || *u2 == 0) && (OLIVEC_SIGN(int, u3) == OLIVEC_SIGN(int, *det) || u3 == 0));
}

OLIVECDEF bool olivec_normalize_triangle(size_t width, size_t height, int x1, int y1, int x2, int y2, int x3, int y3, int* lx, int* hx, int* ly, int* hy) {
    *lx = x1;
    *hx = x1;
    if(*lx > x2) *lx = x2;
    if(*lx > x3) *lx = x3;
    if(*hx < x2) *hx = x2;
    if(*hx < x3) *hx = x3;
    if(*lx < 0) *lx = 0;
    if((size_t)*lx >= width) return false;
    ;
    if(*hx < 0) return false;
    ;
    if((size_t)*hx >= width) *hx = width - 1;

    *ly = y1;
    *hy = y1;
    if(*ly > y2) *ly = y2;
    if(*ly > y3) *ly = y3;
    if(*hy < y2) *hy = y2;
    if(*hy < y3) *hy = y3;
    if(*ly < 0) *ly = 0;
    if((size_t)*ly >= height) return false;
    ;
    if(*hy < 0) return false;
    ;
    if((size_t)*hy >= height) *hy = height - 1;

    return true;
}

OLIVECDEF void olivec_triangle3c(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3,
    uint32_t c1, uint32_t c2, uint32_t c3) {
    int lx, hx, ly, hy;
    if(olivec_normalize_triangle(oc.width, oc.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy)) {
        for(int y = ly; y <= hy; ++y) {
            for(int x = lx; x <= hx; ++x) {
                int u1, u2, det;
                if(olivec_barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det))
                    olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), mix_colors3(c1, c2, c3, u1, u2, det));
            }
        }
    }
}

OLIVECDEF void olivec_triangle3z(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float z1, float z2, float z3) {
    int lx, hx, ly, hy;
    if(olivec_normalize_triangle(oc.width, oc.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy)) {
        for(int y = ly; y <= hy; ++y) {
            for(int x = lx; x <= hx; ++x) {
                int u1, u2, det;
                if(olivec_barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det)) {
                    float z = z1 * u1 / det + z2 * u2 / det + z3 * (det - u1 - u2) / det;
                    OLIVEC_PIXEL(oc, x, y) = *(uint32_t*)&z;
                }
            }
        }
    }
}

OLIVECDEF void olivec_triangle3uv(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture) {
    int lx, hx, ly, hy;
    if(olivec_normalize_triangle(oc.width, oc.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy)) {
        for(int y = ly; y <= hy; ++y) {
            for(int x = lx; x <= hx; ++x) {
                int u1, u2, det;
                if(olivec_barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det)) {
                    int u3 = det - u1 - u2;
                    float z = z1 * u1 / det + z2 * u2 / det + z3 * (det - u1 - u2) / det;
                    float tx = tx1 * u1 / det + tx2 * u2 / det + tx3 * u3 / det;
                    float ty = ty1 * u1 / det + ty2 * u2 / det + ty3 * u3 / det;

                    int texture_x = tx / z * texture.width;
                    if(texture_x < 0) texture_x = 0;
                    if((size_t)texture_x >= texture.width) texture_x = texture.width - 1;

                    int texture_y = ty / z * texture.height;
                    if(texture_y < 0) texture_y = 0;
                    if((size_t)texture_y >= texture.height) texture_y = texture.height - 1;
                    OLIVEC_PIXEL(oc, x, y) = OLIVEC_PIXEL(texture, (int)texture_x, (int)texture_y);
                }
            }
        }
    }
}

OLIVECDEF void olivec_triangle3uv_bilinear(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, float tx1, float ty1, float tx2, float ty2, float tx3, float ty3, float z1, float z2, float z3, Olivec_Canvas texture) {
    int lx, hx, ly, hy;
    if(olivec_normalize_triangle(oc.width, oc.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy)) {
        for(int y = ly; y <= hy; ++y) {
            for(int x = lx; x <= hx; ++x) {
                int u1, u2, det;
                if(olivec_barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det)) {
                    int u3 = det - u1 - u2;
                    float z = z1 * u1 / det + z2 * u2 / det + z3 * (det - u1 - u2) / det;
                    float tx = tx1 * u1 / det + tx2 * u2 / det + tx3 * u3 / det;
                    float ty = ty1 * u1 / det + ty2 * u2 / det + ty3 * u3 / det;

                    float texture_x = tx / z * texture.width;
                    if(texture_x < 0) texture_x = 0;
                    if(texture_x >= (float)texture.width) texture_x = texture.width - 1;

                    float texture_y = ty / z * texture.height;
                    if(texture_y < 0) texture_y = 0;
                    if(texture_y >= (float)texture.height) texture_y = texture.height - 1;

                    int precision = 100;
                    OLIVEC_PIXEL(oc, x, y) = olivec_pixel_bilinear(
                        texture,
                        texture_x * precision, texture_y * precision,
                        precision, precision);
                }
            }
        }
    }
}

// TODO: AA for triangle
OLIVECDEF void olivec_triangle(Olivec_Canvas oc, int x1, int y1, int x2, int y2, int x3, int y3, uint32_t color) {
    int lx, hx, ly, hy;
    if(olivec_normalize_triangle(oc.width, oc.height, x1, y1, x2, y2, x3, y3, &lx, &hx, &ly, &hy)) {
        for(int y = ly; y <= hy; ++y) {
            for(int x = lx; x <= hx; ++x) {
                int u1, u2, det;
                if(olivec_barycentric(x1, y1, x2, y2, x3, y3, x, y, &u1, &u2, &det))
                    olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), color);
            }
        }
    }
}

OLIVECDEF void olivec_text(Olivec_Canvas oc, const char* text, int tx, int ty, Olivec_Font font, size_t glyph_size, uint32_t color) {
    for(size_t i = 0; *text; ++i, ++text) {
        int gx = tx + i * font.width * glyph_size;
        int gy = ty;
        const char* glyph = &font.glyphs[(*text) * sizeof(char) * font.width * font.height];
        for(int dy = 0; (size_t)dy < font.height; ++dy) {
            for(int dx = 0; (size_t)dx < font.width; ++dx) {
                int px = gx + dx * glyph_size;
                int py = gy + dy * glyph_size;
                if(0 <= px && px < (int)oc.width && 0 <= py && py < (int)oc.height) {
                    if(glyph[dy * font.width + dx])
                        olivec_rect(oc, px, py, glyph_size, glyph_size, color);
                }
            }
        }
    }
}

OLIVECDEF void olivec_sprite_blend(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite) {
    if(sprite.width == 0) return;
    if(sprite.height == 0) return;

    Olivec_Normalized_Rect nr = {0};
    if(!olivec_normalize_rect(x, y, w, h, oc.width, oc.height, &nr)) return;

    int xa = nr.ox1;
    if(w < 0) xa = nr.ox2;
    int ya = nr.oy1;
    if(h < 0) ya = nr.oy2;
    for(int y = nr.y1; y <= nr.y2; ++y) {
        for(int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - xa) * ((int)sprite.width) / w;
            size_t ny = (y - ya) * ((int)sprite.height) / h;
            olivec_blend_color(&OLIVEC_PIXEL(oc, x, y), OLIVEC_PIXEL(sprite, nx, ny));
        }
    }
}

OLIVECDEF void olivec_sprite_copy(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite) {
    if(sprite.width == 0) return;
    if(sprite.height == 0) return;

    // TODO: consider introducing flip parameter instead of relying on negative width and height
    // Similar to how SDL_RenderCopyEx does that
    Olivec_Normalized_Rect nr = {0};
    if(!olivec_normalize_rect(x, y, w, h, oc.width, oc.height, &nr)) return;

    int xa = nr.ox1;
    if(w < 0) xa = nr.ox2;
    int ya = nr.oy1;
    if(h < 0) ya = nr.oy2;
    for(int y = nr.y1; y <= nr.y2; ++y) {
        for(int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - xa) * ((int)sprite.width) / w;
            size_t ny = (y - ya) * ((int)sprite.height) / h;
            OLIVEC_PIXEL(oc, x, y) = OLIVEC_PIXEL(sprite, nx, ny);
        }
    }
}

// TODO: olivec_pixel_bilinear does not check for out-of-bounds
// But maybe it shouldn't. Maybe it's a responsibility of the caller of the function.
OLIVECDEF uint32_t olivec_pixel_bilinear(Olivec_Canvas sprite, int nx, int ny, int w, int h) {
    int px = nx % w;
    int py = ny % h;

    int x1 = nx / w, x2 = nx / w;
    int y1 = ny / h, y2 = ny / h;
    if(px < w / 2) {
        // left
        px += w / 2;
        x1 -= 1;
        if(x1 < 0) x1 = 0;
    } else {
        // right
        px -= w / 2;
        x2 += 1;
        if((size_t)x2 >= sprite.width) x2 = sprite.width - 1;
    }

    if(py < h / 2) {
        // top
        py += h / 2;
        y1 -= 1;
        if(y1 < 0) y1 = 0;
    } else {
        // bottom
        py -= h / 2;
        y2 += 1;
        if((size_t)y2 >= sprite.height) y2 = sprite.height - 1;
    }

    return mix_colors2(mix_colors2(OLIVEC_PIXEL(sprite, x1, y1),
                           OLIVEC_PIXEL(sprite, x2, y1),
                           px, w),
        mix_colors2(OLIVEC_PIXEL(sprite, x1, y2),
            OLIVEC_PIXEL(sprite, x2, y2),
            px, w),
        py, h);
}

OLIVECDEF void olivec_sprite_copy_bilinear(Olivec_Canvas oc, int x, int y, int w, int h, Olivec_Canvas sprite) {
    // TODO: support negative size in olivec_sprite_copy_bilinear()
    if(w <= 0) return;
    if(h <= 0) return;

    Olivec_Normalized_Rect nr = {0};
    if(!olivec_normalize_rect(x, y, w, h, oc.width, oc.height, &nr)) return;

    for(int y = nr.y1; y <= nr.y2; ++y) {
        for(int x = nr.x1; x <= nr.x2; ++x) {
            size_t nx = (x - nr.ox1) * sprite.width;
            size_t ny = (y - nr.oy1) * sprite.height;
            OLIVEC_PIXEL(oc, x, y) = olivec_pixel_bilinear(sprite, nx, ny, w, h);
        }
    }
}

#endif // OLIVEC_IMPLEMENTATION

// TODO: Benchmarking
// TODO: SIMD implementations
// TODO: bezier curves
// TODO: olivec_ring
// TODO: fuzzer
// TODO: Stencil
