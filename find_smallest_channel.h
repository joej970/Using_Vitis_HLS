

#ifndef __FIND_SMALLEST_CHANNEL_H__
#define __FIND_SMALLEST_CHANNEL_H__

#include <ap_int.h>

#define LOG2_CEIL(x) (    (x) <= 1 ? 0 :    (x) <= 2 ? 1 :    (x) <= 4 ? 2 :    (x) <= 8 ? 3 :    (x) <= 16 ? 4 :    (x) <= 32 ? 5 :    (x) <= 64 ? 6 :    (x) <= 128 ? 7 :    (x) <= 256 ? 8 :    (x) <= 512 ? 9 :    (x) <= 1024 ? 10 :    (x) <= 2048 ? 11 :    (x) <= 4096 ? 12 :    (x) <= 8192 ? 13 :    (x) <= 16384 ? 14 :    (x) <= 32768 ? 15 :    (x) <= 65536 ? 16 :    (x) <= 131072 ? 17 :    (x) <= 262144 ? 18 :    (x) <= 524288 ? 19 :    (x) <= 1048576 ? 20 :    (x) <= 2097152 ? 21 :    (x) <= 4194304 ? 22 :    (x) <= 8388608 ? 23 :    (x) <= 16777216 ? 24 :    (x) <= 33554432 ? 25 :    (x) <= 67108864 ? 26 :    (x) <= 134217728 ? 27 :    (x) <= 268435456 ? 28 :    (x) <= 536870912 ? 29 :    (x) <= 1073741824 ? 30 : 31)

#define MAX_IMAGE_WIDTH (16384)
#define MAX_IMAGE_HEIGHT (16384)

#define HEIGHT_WIDTH LOG2_CEIL(MAX_IMAGE_HEIGHT)
#define WIDTH_WIDTH LOG2_CEIL(MAX_IMAGE_WIDTH)

#define COLS_WIDTH (WIDTH_WIDTH)
#define ROWS_WIDTH (HEIGHT_WIDTH)

#define G_DATA_WIDTH (COLS_WIDTH + ROWS_WIDTH)

#define G_PPIXEL (16)

// Row_start value buffer length
#define G_ROW_START_BUFFER_LENGTH (G_PPIXEL - 1)
// 0 to 14 (15 values) for 16 parallel pixels
// 0 to 6 (7 values) for    8 parallel pixels
#define G_NR_OF_INPUTS (G_PPIXEL/2)


typedef ap_int<G_DATA_WIDTH> data_t;
typedef ap_int<LOG2_CEIL(G_PPIXEL)> ch_t;

void find_smallest_channel(data_t data_in[G_NR_OF_INPUTS], ch_t channel_idx_in[G_NR_OF_INPUTS], data_t *min_data_ou, ch_t *min_index_out);
void find_min(data_t data_a, ch_t ch_a, data_t data_b, ch_t ch_b, data_t *min_data, ch_t *min_channel);

#endif // __FIND_SMALLEST_CHANNEL_H__




