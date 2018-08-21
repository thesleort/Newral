/**
 * @brief These functions are used to encapsule the work in specific
 * areas of the feature map.
 * 
 * @file layer.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */
#include <stdlib.h>
#include <iostream>

#include "operator/layer.hpp"

// void layer_op::max_pool(filter &filter_map, feature_map_config &config) {

//     // TODO: Could return and reuse this pool.
//     // double **pool = (double **)malloc(sizeof(double *) * config.size);
//     // for (unsigned i = 0; i < config.size; ++i) {
//     //     pool[i] = (double *)malloc(sizeof(double) * config.size);
//     // }

//     for (int x = 0 - config.padding; x < filter_map.width + config.padding; x = x + config.stride) {
//         for (int y = 0 - config.padding; y < filter_map.height + config.padding; y = y + config.stride) {
//             float max = 0;
//             for (unsigned pool_x = x; pool_x < config.size + x; ++pool_x) {
//                 for (unsigned pool_y = y; pool_y < config.size + y; ++pool_y) {
//                     if ((x < 0 || x > filter_map.width) || (y < 0 || y > filter_map.height)) {
//                         // If out of bounds
//                         if (filter_map.neurons[pool_x][pool_y].get_output_val() > max) {
//                             max = filter_map.neurons[pool_x][pool_y].get_output_val();
//                         }
//                     } else {
//                         // If not out of bounds
//                         if (filter_map.neurons[pool_x][pool_y].get_output_val() > max) {
//                             max = filter_map.neurons[pool_x][pool_y].get_output_val();
//                         }
//                     }
//                 }
//             }
//             // output

// 			// TODO: How to set the values for all output neurons?
//             // for (unsigned filter_num = 0; filter_num < filter_map.neurons[x][y].get_output_weights(); ++filter_num) {
				
//             // }
//         }
//     }
// }

// void layer_op::convolution(feature_map &map, unsigned kernel, unsigned stride, int padding = 0) {
// }

// void layer_op::fully_connect(feature_map &map) {
// }