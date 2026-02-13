#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define PI 3.14159265358979323846
#define CK_T 60
#define CK_R 50
#define TTDR 15
#define MH_R (CK_R * 11 / TTDR)
#define HH_R (CK_R * 6  / TTDR)
#define HT_R (CK_R * 3  / TTDR)
#define MT_R (CK_R * 1  / TTDR)
#define CK_O CK_R
#define CK_SIZE ((CK_R << 1) | 1)
#define CK_CH '*'
#define MH_CH 'O'
#define HH_CH '#'

float hand[CK_T];
char face[CK_SIZE][CK_SIZE + 1];

void draw(float rad, int sr, int er, char ch) {
    for(int r = sr; r <= er; ++r) {
        int x = CK_O + round(r * cos(rad));
        int y = CK_O + round(r * sin(rad));
        assert(0 <= x && x < CK_SIZE && 0 <= y && x < CK_SIZE);
        face[x][y] = ch;
    }
}

void init() {
    // hand
    for(int i = 0; i < CK_T; ++i) hand[(CK_T + CK_T / 2 - i) % CK_T] = 2 * PI * i / CK_T;

    // face
    for(int i = 0; i < CK_SIZE; ++i) for(int j = 0; j < CK_SIZE; ++j) face[i][j] = ' ';
    for(int i = 0; i < CK_SIZE; ++i) face[i][CK_SIZE] = '\0';
    for(int i = 0; i < 8 * CK_R; ++i) draw(2 * PI * i / (8 * CK_R), CK_R, CK_R, CK_CH);
    for(int i = 0; i < CK_T; ++i) draw(hand[i], CK_R - MT_R, CK_R, CK_CH);
    for(int i = 0; i < 12; ++i) draw(2 * PI * i / 12, CK_R - HT_R, CK_R, CK_CH);
    face[CK_O][CK_O] = CK_CH;
}

void print() {
    for(int i = 0; i < CK_SIZE; ++i) {
        printf("%s\n", face[i]);
    }
}

// t in [0, CK_T)
void draw_mhand(int t) { draw(hand[t], 1, MH_R, MH_CH); }
void draw_hhand(int t) { draw(hand[t], 1, HH_R, HH_CH); }

// h in [0, 12) and m in [0, 60)
void draw_ckhands(int h, int m) {
    draw_mhand(m);
    draw_hhand((h % CK_T * 5 % CK_T) + (m / 12));
}


int main() {
    init();

    time_t _now; time(&_now);
    struct tm *now = localtime(&_now);
    draw_ckhands(now->tm_hour, now->tm_min);

    print();
}
