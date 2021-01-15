#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void init_image(uint8_t *X, int n, uint32_t seed);
void write_image(uint8_t *X, int n, int Xmax);
void rand_clk();
void organize(uint8_t *X, int n, int c, int d);
int mod(int x, int m);

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
    int c = atoi(argv[6]);
    int d = atoi(argv[7]); // x and y strides
    uint8_t *X = malloc(n * n);

    init_image(X, n, seed);
    for (int i = 0; i < K; i++){
        organize(X, n, c, d);
    }
    write_image(X, n, 1);
    return 0;
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

void organize(uint8_t *X, int n, int c, int d) {
    int i1, i2, j1, j2, k, m, dx, dy, a, b, t;
    for (int i = 0; i < n; i++){
        rand_clk();
        k = sr % 8;
        dx = Dx[k];
        dy = Dy[k];
        for (int j = 0; j < n; j++){
            m = n*i + j;
	    i1 = mod(i + dx, n);
	    i2 = mod(i + d*dx, n);
	    j1 = mod(j + dy, n);
	    j2 = mod(j + c*dy, n);
            a = n*i1 + j1;
            b = n*i2 + j2;

            if(X[a] != X[m] && X[b] == X[m]) {
                t = X[a];
                X[a] = X[b];
                X[b] = t;
            }
        }
    }
}

int mod(int x, int m) {
	int y = x % m;
	return y >= 0 ? y : m + y;
}
