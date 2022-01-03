#include <stdio.h>
#include <math.h>

#define PI 3.14159265

void printVals(char* title, float vals[9]) {
    printf("%s: ", title);
    for (int i = 0; i < 9; i++) {
        printf("%.2f, ", vals[i]);
    }
    printf("\n");
}

int main() {
    float sin_vals[9], cos_vals[9];

    for (int i = 0; i < 9; i++) {
        float x = i * (PI / 4);
        sin_vals[i] = sin(x);
        cos_vals[i] = cos(x);
    }

    printVals("Sine", sin_vals);
    printVals("Cosine", cos_vals);

    return 0;
}