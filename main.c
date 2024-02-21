#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HASH_SIZE 8
#define BYTE_SIZE 8
#define DATA_SIZE 512

uint32_t* generate_hashes() {
    uint32_t* hashes = (uint32_t*)malloc(HASH_SIZE * 8);
    if (hashes == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int numbers[] = {2, 3, 5, 7, 11, 13, 17, 19};

    for (int i = 0; i < HASH_SIZE; i++) {
        double square_root = sqrt(numbers[i]);
        double fraction_part =
            square_root - floor(square_root);  // floor - целая часть
        hashes[i] = (uint32_t)(fraction_part * UINT32_MAX);
    }

    return hashes;
}

void print_hashes(uint32_t* hashes, int size) {
    for (int i = 0; i < size; i++) {
        printf("Hash %d: %08X\n", i + 1, hashes[i]);
    }
}

uint32_t* generate_constants() {
    uint32_t* constants = (uint32_t*)malloc(HASH_SIZE * 64);
    if (constants == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }

    int numbers[64] = {2,   3,   5,   7,   11,  13,  17,  19,  23,  29,  31,
                       37,  41,  43,  47,  53,  59,  61,  67,  71,  73,  79,
                       83,  89,  97,  101, 103, 107, 109, 113, 127, 131, 137,
                       139, 149, 151, 157, 163, 167, 173, 179, 181, 191, 193,
                       197, 199, 211, 223, 227, 229, 233, 239, 241, 251, 257,
                       263, 269, 271, 277, 281, 283, 293, 307, 311};

    for (int i = 0; i < 64; i++) {
        double third_root = cbrt(numbers[i]);
        double fraction_part = third_root - floor(third_root);
        constants[i] = (uint32_t)(fraction_part * UINT32_MAX);
    }

    return constants;
}

uint8_t* string_to_binary_data(const char string[]) {
    int len = strlen(string);
    uint8_t* binary_data = (uint8_t*)malloc((DATA_SIZE) * sizeof(uint8_t));
    if (binary_data == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    int index = 0;
    for (int i = 0; i < len; i++) {
        char ch = string[i];
        for (int j = BYTE_SIZE - 1; j >= 0; j--) {
            binary_data[index] = (ch >> j) & 1;
            index++;
        }
    }
    binary_data[index] = 1;
    index = 512 - 64;
    uint64_t length = len * BYTE_SIZE;
    for (int i = 63; i >= 0; i--) {
        binary_data[index] = (length >> i) & 1;
        index++;
    }
    return binary_data;
}

void print_binary(uint8_t* binary_data, int size) {
    for (int i = 0; i < size; i++) {
        printf("%u", binary_data[i]);
        if ((i + 1) % BYTE_SIZE == 0) {
            printf(" ");
        }
        if ((i + 1) % 64 == 0) {
            printf("\n");
        }
    }
    printf("\n");
}

int main(void) {
    const char str[] = "Привет, мир";
    uint8_t* binary_data = string_to_binary_data(str);
    print_binary(binary_data, DATA_SIZE);
    free(binary_data);
    uint32_t* hashes = generate_hashes();
    uint32_t* constants = generate_constants();
    print_hashes(hashes, 8);
    printf("\n");
    print_hashes(constants, 64);
    free(hashes);
    free(constants);
    return 0;
}
