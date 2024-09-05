

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
// void find_smallest_channel(data_t data_in[G_NR_OF_INPUTS], ch_t channel_idx_in[G_NR_OF_INPUTS], data_t *min_data_out, ch_t *min_index_out) {
void find_smallest_channel(data_t data[G_ROW_START_BUFFER_LENGTH], ch_t channel_idx[G_ROW_START_BUFFER_LENGTH], data_t *min_data_out, ch_t *min_index_out) {
	#pragma HLS PIPELINE
	#pragma HLS ARRAY_PARTITION variable=data complete dim=1
    #pragma HLS ARRAY_PARTITION variable=channel_idx complete dim=1
    // data_t data[G_ROW_START_BUFFER_LENGTH];
    // ch_t channel_idx[G_ROW_START_BUFFER_LENGTH];
    // #pragma HLS ARRAY_PARTITION variable=data complete
    // #pragma HLS ARRAY_PARTITION variable=channel_idx complete

    // LOOP_POPULATE : for(int i = 0; i < G_NR_OF_INPUTS; i++) {
    // 	#pragma HLS UNROLL
    //     data[i] = data_in[i];
    //     channel_idx[i] = channel_idx_in[i];
    // }

	#pragma HLS DEPENDENCE variable=data type=inter distance=3 true
	#pragma HLS DEPENDENCE variable=channel_idx type=inter distance=3 true
    #pragma HLS DEPENDENCE variable=data type=intra false
    #pragma HLS DEPENDENCE variable=channel_idx type=intra false
    LOOP_STAGES : for(int stage = 0; stage < LOG2_CEIL(G_PPIXEL) - 1; stage++) {
        // #pragma HLS PIPELINE II=1

        int stage_offset = G_PPIXEL - G_PPIXEL/(1 << stage);
        int next_stage_offset = G_PPIXEL - G_PPIXEL/(1 << (stage + 1));
        int nr_of_pairs = G_PPIXEL/(1 << (stage + 2));

#ifndef __SYNTHESIS__
        std::cout << std::endl;
        std::cout << "Stage: " << stage << " | Nr of pairs: " << nr_of_pairs << std::endl;
#endif

        #pragma HLS DEPENDENCE variable=data type=intra false // intra false signifies there is no dependence on data inside the loop
        #pragma HLS DEPENDENCE variable=data type=inter distance=3 false // intra false signifies there is no dependence on data from one loop to the next
        #pragma HLS DEPENDENCE variable=channel_idx type=intra false
        #pragma HLS DEPENDENCE variable=channel_idx type=inter distance=3 false
        LOOP_PAIRS : for(int i = 0; i < nr_of_pairs; i++) {
            #pragma HLS UNROLL
            //#pragma HLS PIPELINE II=1

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

/**
 * This one achieves PIPELINED II=1 with Ts= 3.33 ns with all three #pragmas. Latency is 4 cycles.
 */
void find_smallest_channel_c_like(data_t data[G_NR_OF_INPUTS], ch_t channel_idx[G_NR_OF_INPUTS], data_t *min_data_out, ch_t *min_index_out){

#pragma HLS ARRAY_PARTITION variable=data complete dim=1
#pragma HLS ARRAY_PARTITION variable=channel_idx complete dim=1

#pragma HLS PIPELINE II=1

    *min_data_out = data[0];
    *min_index_out = channel_idx[0];

    for (int i = 1; i < G_NR_OF_INPUTS; i++) {
		//#pragma HLS UNROLL
        if (data[i] < *min_data_out) {
            *min_data_out = data[i];  // Update smallest if a smaller value is found
            *min_index_out = channel_idx[i];;  // Update smallest_idx with the index of the smaller value
        }
    }
}

/**
 * 
 */
void find_smallest_channel_unrolled_16(data_t data[G_ROW_START_BUFFER_LENGTH], ch_t channel_idx[G_ROW_START_BUFFER_LENGTH], data_t *min_data_out, ch_t *min_index_out) {
    #pragma HLS ARRAY_PARTITION variable=data complete dim=1
    #pragma HLS ARRAY_PARTITION variable=channel_idx complete dim=1
    
    #pragma HLS PIPELINE II=3

    int stage;
    int stage_offset;
    int next_stage_offset;
    int nr_of_pairs;
    
    
    // stage 0
    stage = 0;
    stage_offset = G_PPIXEL - G_PPIXEL/(1 << stage);
    next_stage_offset = G_PPIXEL - G_PPIXEL/(1 << (stage + 1));
    nr_of_pairs = G_PPIXEL/(1 << (stage + 2));

#ifndef __SYNTHESIS__
        std::cout << std::endl;
        std::cout << "Stage: " << stage << " | Nr of pairs: " << nr_of_pairs << std::endl;
#endif

    { // for i = 0 to 3
        int i;
        int idx_a;
        int idx_b;
        int idx_o;
    
        // pair 0
        i = 0;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

        // pair 1
        i = 1;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

        // pair 2
        i = 2;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

        // pair 3
        i = 3;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

    }

    // stage 1
    stage = 1;
    stage_offset = G_PPIXEL - G_PPIXEL/(1 << stage);
    next_stage_offset = G_PPIXEL - G_PPIXEL/(1 << (stage + 1));
    nr_of_pairs = G_PPIXEL/(1 << (stage + 2));

#ifndef __SYNTHESIS__
        std::cout << std::endl;
        std::cout << "Stage: " << stage << " | Nr of pairs: " << nr_of_pairs << std::endl;
#endif

    { // for i = 0 to 1
        int i;
        int idx_a;
        int idx_b;
        int idx_o;
    
        // pair 0
        i = 0;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

        // pair 1
        i = 1;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

    }

    // stage 2
    stage = 2;
    stage_offset = G_PPIXEL - G_PPIXEL/(1 << stage);
    next_stage_offset = G_PPIXEL - G_PPIXEL/(1 << (stage + 1));
    nr_of_pairs = G_PPIXEL/(1 << (stage + 2));

#ifndef __SYNTHESIS__
        std::cout << std::endl;
        std::cout << "Stage: " << stage << " | Nr of pairs: " << nr_of_pairs << std::endl;
#endif

    { // for i = 0 to 0
        int i;
        int idx_a;
        int idx_b;
        int idx_o;
    
        // pair 0
        i = 0;
        idx_a = 2*i + stage_offset;
        idx_b = 2*i + stage_offset + 1;
        idx_o = next_stage_offset + i;
        find_min(data[idx_a], channel_idx[idx_a], data[idx_b], channel_idx[idx_b], &data[idx_o], &channel_idx[idx_o]);

    }

    *min_data_out = data[G_ROW_START_BUFFER_LENGTH-1];
    *min_index_out = channel_idx[G_ROW_START_BUFFER_LENGTH-1];


}
