// compiler: clang-1700.3.19.1
// compile flags: -fno-common
// notice: terminal height must largger than CK_SIZE * (character height unit)
// usage:
//   clock     - display the clock and refresh per minute
//   clock now - display the clock with current time

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <assert.h>

#ifdef _WIN32
#include <windows.h>
#define wait_for(sec) Sleep(sec * 1000)
#define clear() system("cls")
#else
#include <unistd.h>
#define wait_for(sec) sleep(sec)
#define clear() system("clear")
#endif

#define PI 3.14159265358979323846
#define XY_FR 2    // ratio of char_height and char_width
#define MGN 2      // margin gap
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
char mgn[MGN + 1];
char face[CK_SIZE][CK_SIZE * XY_FR + 1];

void draw(float rad, int sr, int er, char ch) {
    for(int r = sr; r <= er; ++r) {
        int x = CK_O + round(r * cos(rad));
        int y = CK_O + round(r * sin(rad));
        assert(0 <= x && x < CK_SIZE && 0 <= y && x < CK_SIZE);
        for(int dy = 0; dy < XY_FR; ++dy)
            face[x][y * XY_FR + dy] = ch;
    }
}

void init() {
    // init clock face
    for(int i = 0; i < CK_SIZE; ++i) for(int j = 0; j < CK_SIZE * XY_FR; ++j) face[i][j] = ' ';
    for(int i = 0; i < CK_SIZE; ++i) face[i][CK_SIZE * XY_FR] = '\0';
    for(int i = 0; i < 8 * CK_R; ++i) draw(2 * PI * i / (8 * CK_R), CK_R, CK_R, CK_CH);
    for(int i = 0; i < CK_T; ++i) draw(hand[i], CK_R - MT_R, CK_R, CK_CH);
    for(int i = 0; i < 12; ++i) draw(2 * PI * i / 12, CK_R - HT_R, CK_R, CK_CH);
    for(int dy = 0; dy < XY_FR; ++dy) face[CK_O][CK_O * XY_FR + dy] = CK_CH;
}

void print() {
    for(int i = 0; i < MGN; ++i) printf("\n");
    for(int i = 0; i < CK_SIZE; ++i) {
        printf("%s", mgn);
        printf("%s\n", face[i]);
    }
}

// t in [0, CK_T)
void draw_mhand(int t) { draw(hand[t], 1, MH_R, MH_CH); }
void draw_hhand(int t) { draw(hand[t], 1, HH_R, HH_CH); }

// h in [0, 24) and m in [0, 60)
void draw_ckhands(int h, int m) {
    draw_mhand(m);
    draw_hhand((h * 5 % CK_T) + (m / 12));
}

void ck_loop() {
    time_t _now; time(&_now);
    struct tm *now = localtime(&_now);
    while(1) {
        clear();
        init();
        draw_ckhands(now->tm_hour, now->tm_min);
        print();
        wait_for(60);
        time(&_now);
        now = localtime(&_now);
    }
}


int main(int argc, char **argv) {
    // init hand
    for(int i = 0; i < CK_T; ++i) hand[(CK_T + CK_T / 2 - i) % CK_T] = 2 * PI * i / CK_T;
    // init mgn
    for(int i = 0; i < MGN; ++i) mgn[i] = ' ';
    mgn[MGN] = '\0';

    if(argc >= 2 && strcmp(argv[1], "now") == 0) {
        init();
        time_t _now; time(&_now);
        struct tm *now = localtime(&_now);
        draw_ckhands(now->tm_hour, now->tm_min);
        print();
    } else {
        ck_loop();
    }
}
