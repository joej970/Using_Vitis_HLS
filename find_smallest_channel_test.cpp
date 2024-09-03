#include <iostream>
#include "find_smallest_channel.h"
// #include "find_smallest_channel.cpp"



int main() {

#if G_PPIXEL == 64

    data_t data[G_NR_OF_INPUTS] = {20,45,1,25,0,15,99,85,63,32,88,75,32,17,33,19,58,45,22,14,0,75,98,63,98,01,8,9,11,12,14};
    ch_t channels[G_NR_OF_INPUTS] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31};

#elif G_PPIXEL == 32

    data_t data[G_NR_OF_INPUTS] = {20,45,1,25,0,15,99,85,63,32,88,75,32,17,33,19};
    ch_t channels[G_NR_OF_INPUTS] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

#elif G_PPIXEL == 16

    data_t data[G_NR_OF_INPUTS] = {20, 34, 50, 1, 15, 5, 30, 4};
    ch_t channels[G_NR_OF_INPUTS] = {0, 1, 2, 3, 4, 5, 6, 7};

#elif G_PPIXEL == 8
    data_t data[G_NR_OF_INPUTS] = {15,99,85,63};
    ch_t channels[G_NR_OF_INPUTS] = {0, 1, 2, 3};
#else
    std::cout << "Invalid G_PPIXEL value" << std::endl;
    return 1;
#endif

    data_t min_data;
    ch_t min_channel;

    find_smallest_channel(data, channels, &min_data, &min_channel);

    // int index = min_channel.to_int();
    // int value = min_data.to_int();

    std::cout << "Array: ";
    for(int i = 0; i < G_NR_OF_INPUTS; i++) {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
    std::cout << std::endl;

    std::cout << "The index of the smallest channel is: " << min_channel << " with value: "<< min_data << std::endl;
    std::cout << std::endl;

    return 0;
}
