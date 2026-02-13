#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#define PI 3.14159265358979323846
#define MXT 12
#define CK_R 50
#define TTDR 15
#define MH_R (CK_R * 11 / TTDR)
#define HH_R (CK_R * 6  / TTDR)
#define TT_R (CK_R * 3  / TTDR)
#define CK_O CK_R
#define CK_SIZE ((CK_R << 1) | 1)
#define CK_CH '*'
#define MH_CH '#'
#define HH_CH 'O'

float hand[MXT];
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
    for(int i = 0; i < MXT; ++i) hand[(MXT + 6 - i) % MXT] = 2 * PI * i / MXT;

    // face
    for(int i = 0; i < CK_SIZE; ++i) for(int j = 0; j < CK_SIZE; ++j) face[i][j] = ' ';
    for(int i = 0; i < CK_SIZE; ++i) face[i][CK_SIZE] = '\0';
    for(int i = 0; i < 8 * CK_R; ++i) {
        draw(2 * PI * i / (8 * CK_R), CK_R, CK_R, CK_CH);
    }
    for(int i = 0; i < MXT; ++i) {
        draw(hand[i], CK_R - TT_R, CK_R, CK_CH);
    }
    face[CK_O][CK_O] = CK_CH;
}

void draw_mhand(int t) { draw(hand[t], 1, MH_R, MH_CH); }
void draw_hhand(int t) { draw(hand[t], 1, HH_R, HH_CH); }

void print() {
    for(int i = 0; i < CK_SIZE; ++i) {
        printf("%s\n", face[i]);
    }
}


int main() {
    init();

    time_t _now; time(&_now);
    struct tm *now = localtime(&_now);
    draw_mhand(now->tm_min / 5);
    draw_hhand(now->tm_hour % MXT);

    print();
}
