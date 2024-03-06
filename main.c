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

void print_binary_r(uint32_t value) {
    for (int i = 31; i >= 0; i--) {
        printf("%d", (value >> i) & 1);
    }
}

void print_binary_data(const uint32_t* m, int size) {
    for (int i = 0; i < 16; i++) {
        print_binary_r(m[i]);
        if (i % 2 != 0) {
            printf("\n");
        } else {
            printf(" ");
        }
    }
}

uint32_t* convert_to_binary(const char string[]) {
    int message_len = strlen(string);
    int message_bit_len = message_len * BYTE_SIZE;

    // Вычисление K - наименьшего неотрицательного числа, такого, что (L + 1 +
    // K) mod 512 = 448
    int K = 0;
    while ((message_bit_len + 1 + K) % 512 != 448) {
        K++;
    }

    // Вычисление общей длины преобразованного сообщения (в битах)
    int total_bit_len = message_bit_len + 1 + K + 64;

    // Вычисление размера массива m в 32-битных словах
    int m_size = total_bit_len / 32 + (total_bit_len % 32 != 0);

    uint32_t* binary_data = (uint32_t*)malloc(m_size * sizeof(uint32_t));
    int bit_index = 0;
    // Преобразование каждого символа входного сообщения в биты
    for (int i = 0; i < message_len; i++) {
        char ch = string[i];
        for (int j = BYTE_SIZE - 1; j >= 0; j--) {
            // Запись очередного бита в массив m
            binary_data[bit_index / 32] |= ((ch >> j) & 1)
                                           << (31 - (bit_index % 32));
            bit_index++;
        }
    }

    // Добавление единичного бита в конец преобразованного сообщения
    binary_data[bit_index / 32] |= 1 << (31 - (bit_index % 32));
    bit_index++;

    // Добавление нулевых битов до достижения необходимой длины
    for (; bit_index < total_bit_len - 64; bit_index++) {
        binary_data[bit_index / 32] |= 0 << (31 - (bit_index % 32));
    }

    // Добавление длины исходного сообщения в виде 64-битного числа
    uint64_t message_length = (uint64_t)message_bit_len;
    for (int i = 0; i < 8; i++) {
        binary_data[(total_bit_len - 64 + i) / 32] |=
            ((message_length >> (56 - i * 8)) & 0xFF)
            << (24 + (i * 8) - ((total_bit_len - 64 + i) % 32));
    }

    print_binary_data(binary_data, m_size);
    return binary_data;
}

int main(void) {
    const char str[] = "hello world";
    uint32_t* binary_data = convert_to_binary(str);
    uint32_t* hashes = generate_hashes();
    uint32_t* constants = generate_constants();

    print_hashes(hashes, 8);
    printf("\n");
    print_hashes(constants, 64);

    free(binary_data);
    free(hashes);
    free(constants);
    return 0;
}
