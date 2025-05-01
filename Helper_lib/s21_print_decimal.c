#include "s21_decimal.h"

/* minimal_decimal_demo.c   — 30 строк упрёка в расточительности */

#include <stdio.h>
#include <stdint.h>

/* Битовые маски и флаги для bits[3] */
#define SIGN_MASK   0x80000000
#define SCALE_MASK  0x00FF0000
#define TYPE_MASK   0x0000000F

/* Типы значений (хранятся в младших 4 битах bits[3]) */
#define S21_NORMAL_VALUE      0
#define S21_INFINITY          1
#define S21_NEGATIVE_INFINITY 2
#define S21_NAN               3

/* ---- Вспомогательные функции ---------------------------------------------*/
static __uint128_t mant96_to_u128(const s21_decimal *d) {
    return  (__uint128_t)(uint32_t)d->bits[0] |
           ((__uint128_t)(uint32_t)d->bits[1] << 32) |
           ((__uint128_t)(uint32_t)d->bits[2] << 64);
}

static int get_scale(const s21_decimal *d) {
    return (d->bits[3] & SCALE_MASK) >> 16;
}

static int is_negative(const s21_decimal *d) {
    return d->bits[3] & SIGN_MASK;
}

static int get_value_type(const s21_decimal *d) {
    return d->bits[3] & TYPE_MASK;
}

/* ---- Печать decimal ------------------------------------------------------*/
static void print_decimal(const s21_decimal *d) {
    int type = get_value_type(d);
    if (type) { puts("NAN/INF not implemented in demo"); return; }

    int scale = get_scale(d);
    __uint128_t m = mant96_to_u128(d);
    char buf[40]; size_t len = 0;

    if (!m) buf[len++] = '0';
    while (m) { buf[len++] = '0' + (m % 10); m /= 10; }

    if (is_negative(d)) putchar('-');

    if (scale >= (int)len) {
        printf("0.%0*d", scale - (int)len, 0);
        while (len) putchar(buf[--len]);
    } else {
        for (size_t i = len; i--;) {
            if (i == (size_t)scale - 1) putchar('.');
            putchar(buf[i]);
        }
    }
    putchar('\n');
}

/* ---- Демонстрация --------------------------------------------------------*/
int main(void) {
    /* Конструируем 16.00 (scale = 0) */
    s21_decimal d = {{16, 0, 0, 0}};  // bits[3] = 0 (normal, scale=0, positive)
    print_decimal(&d);   // → 16

    /* А теперь 123.45 (мантисса 12345, scale = 2) */
    s21_decimal e = {{12345, 0, 0, 2 << 16}};  // scale=2 в bits[3]
    print_decimal(&e);   // → 123.45

    /* Отрицательное число -5.25 (мантисса 525, scale=2, знак отрицательный) */
    s21_decimal f = {{525, 0, 0, (2 << 16) | SIGN_MASK}};
    print_decimal(&f);   // → -5.25

    return 0;
}