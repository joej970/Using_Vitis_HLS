

#include "find_smallest_channel.h"

// Function to find the index of the smaller of two elements
void find_min(data_t data_a, ch_t ch_a, data_t data_b, ch_t ch_b, data_t *min_data, ch_t *min_channel) {
    if(data_a < data_b){
        *min_data = data_a;
        *min_channel = ch_a;
        #ifndef __SYNTHESIS__
        // std::cout << "data_a: " << data_a << " ch_a: " << ch_a << " data_b: " << data_b << " ch_b: " << ch_b << " a is smaller." << std::endl;
        std::cout << "smaller is A" << std::endl;
        #endif
    } else {
        *min_data = data_b;
        *min_channel = ch_b;
        #ifndef __SYNTHESIS__
        std::cout << "smaller is B" << std::endl;
        // std::cout << "data_a: " << data_a << " ch_a: " << ch_a << " data_b: " << data_b << " ch_b: " << ch_b << " b is smaller." << std::endl;
        #endif
    }
}

// EXAMPLE FOR P_PIXELS = 16: indices of tree like data structures
// ___________________________________________________________________________________________________________________
//  stage |   idx start    |                 indices	   	     | last/valid(in/out) |nr of pairs|       pairs
//    0   | 16-16/2^0 =  0 | | 0 1 |  | 2 3 |  | 4 5 |  | 6 7 |  |     (0,1)          |     4     | 0-1, 2-3, 4-5, 6-7                      
//        |                |     \        /        \     /       |                    |           |                    
//    1   | 16-16/2^1 =  8 |     |8    9|         |10   11|      |     (1,2)          |     2     | 8-9, 10-11                    
//        |                |         \               /           |                    |           |                
//    2   | 16-16/2^2 = 12 |         |12           13|           |     (2,3)          |     1     | 12-13                
//        |                |                  |                  |                    |           |              
//   N/A  | 16-16/2^3 = 14 |                | 14|                |     (3,)           |           |    

// EXAMPLE FOR P_PIXELS = 8: indices of tree like data structures
// ______________________________________________________________________________________________
//  stage |   idx start   |      indices      | last/valid(in/out) |nr of pairs|    pairs
//    0   | 8-8/2^0 =  0  |  | 0 1 |  | 2 3 | |     (0,1)          |     2     |   0-1, 2-3
//        |               |      \      /     |                    |           |                    
//    1   | 8-8/2^1 =  4  |      |4    5|     |     (1,2)          |     1     |     4-5
//        |               |          |        |                    |           |                
//    N/A | 8-8/2^2 =  6  |        | 6 |      |     (2, )          |           | 


// Function to find the index of the smallest channel in an array
void find_smallest_channel(data_t data_in[G_NR_OF_INPUTS], ch_t channel_idx_in[G_NR_OF_INPUTS], data_t *min_data_out, ch_t *min_index_out) {
    
    data_t data[G_ROW_START_BUFFER_LENGTH];
    ch_t channel_idx[G_ROW_START_BUFFER_LENGTH];

    LOOP_POPULATE : for(int i = 0; i < G_NR_OF_INPUTS; i++) {
        data[i] = data_in[i];
        channel_idx[i] = channel_idx_in[i];
    }

    LOOP_STAGES : for(int stage = 0; stage < LOG2_CEIL(G_PPIXEL) - 1; stage++) {


        int stage_offset = G_PPIXEL - G_PPIXEL/(1 << stage);
        int next_stage_offset = G_PPIXEL - G_PPIXEL/(1 << (stage + 1));

        int nr_of_pairs = G_PPIXEL/(1 << (stage + 2));

#ifndef __SYNTHESIS__
        std::cout << std::endl;
        std::cout << "Stage: " << stage << " | Nr of pairs: " << nr_of_pairs << std::endl;
#endif

        LOOP_PAIRS : for(int i = 0; i < nr_of_pairs; i++) {
            // #pragma HLS PIPELINE II=1
            // #pragma HLS UNROLL

            int idx_a = 2*i + stage_offset;
            int idx_b = 2*i + stage_offset + 1;
            int idx_o = next_stage_offset + i;
            find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);
#ifndef __SYNTHESIS__
        std::cout << "data a/b: " << data[idx_a] << "/" << data[idx_b] << " | " << "idx a/b: " << idx_a << "/" << idx_b << " | idx out:" << idx_o << " | channel out: "<< channel_idx[idx_o] <<" | result out: "<< data[idx_o] << std::endl; 
#endif
        }
    }

    *min_data_out = data[G_ROW_START_BUFFER_LENGTH-1];
    *min_index_out = channel_idx[G_ROW_START_BUFFER_LENGTH-1];
    
}
