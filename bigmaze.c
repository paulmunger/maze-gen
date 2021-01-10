#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

int lpopcnt(uint8_t *X, int n, int dx, int dy, int i0, int j0, int l);
void lfill(uint8_t *X, int n, int dx, int dy, int i0, int j0, int l, int v);
void init_image(uint8_t *X, int n, uint32_t seed);
void write_image(uint8_t *X, int n, int Xmax);
void rand_clk();
void organize(uint8_t *X, int n, int l);

uint32_t sr;
uint32_t ntap;
uint32_t mtap;
int Dx[8] = {-1, 0, 1, -1, 1, -1, 0, 1};
int Dy[8] = {1, 1, 1, 0, 0, -1, -1, -1};

int main(int argc, char *argv[]) {
    int n = atoi(argv[1]); // image size
    int K = atoi(argv[2]); // # iterations
    int seed = atoi(argv[3]); // random seed
    ntap = 1 << atoi(argv[4]);
    mtap = 1 << atoi(argv[5]); // taps for random number generator
    int l = atoi(argv[6]);
    int m = atoi(argv[7]);
    uint8_t *X = malloc(n * n);

    init_image(X, n, seed);
    for (int i = 0; i < K; i++) {
        organize(X, n, m + (sr % l));
        rand_clk();
    }
    write_image(X, n, 1);

    return 0;
}

int lpopcnt(uint8_t *X, int n, int dx, int dy, int i0, int j0, int l) {
    int w = 0;
    int i = i0;
    int j = j0;
    for (int k = 0; k < l; k++) {
        w += X[n*i + j];
        i += dx; j += dy;
    }
    return w;
}

void lfill(uint8_t *X, int n, int dx, int dy, int i0, int j0, int l, int v) {
    int i = i0;
    int j = j0;
    for (int k = 0; k < l; k++) {
        X[n*i + j] = v;
        i += dx; j += dy;
    }
}

void organize(uint8_t *X, int n, int l) {
    int k, m, dx, dy, a, b, t, c;
    for (int i = l; i < n - l; i++){
        rand_clk();
        k = sr % 8;
        dx = Dx[k];
        dy = Dy[k];
        for (int j = l; j < n - l; j++){
            c = lpopcnt(X, n, dx, dy, i, j, l);
            if (X[n*i + j]){
                lfill(X, n, dx, dy, i, j, c, 1);
                lfill(X, n, dx, dy, i + c*dx, j + c*dy, l - c, 0);
            } else {
                lfill(X, n, dx, dy, i, j, l - c, 0);
                lfill(X, n, dx, dy, i + (l-c)*dx, j + (l-c)*dy, c, 1);
            }
        }
    }
}

void rand_clk() {
    int t = ((sr & mtap) > 0) ^ ((sr & ntap) > 0);
    sr = sr << 1;
    sr += t;
}

void init_image(uint8_t *X, int n, uint32_t seed) {
    sr = seed;
    for (int i = 0; i < n; i++){
        for (int j = 0; j < n; j++){
            X[n*i + j] = sr % 2;
            rand_clk();
        }
    }
}

void write_image(uint8_t *X, int n, int Xmax) {
    printf("P2 %d %d %d\n", n, n, Xmax);
        for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", X[n*i + j]);
        } printf("\n");
    }
}


