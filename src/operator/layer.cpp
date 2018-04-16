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

void layer_op::max_pool(filter &filter_map, feature_map_config &config) {

    // TODO: Could return and reuse this pool.
    double **pool = (double **)malloc(sizeof(double *) * config.size);
    for (unsigned i = 0; i < config.size; ++i) {
        pool[i] = (double *)malloc(sizeof(double) * config.size);
    }

    for (int x = 0 - config.padding; x < filter_map.width + config.padding; x = x + config.stride) {
        for (int y = 0 - config.padding; y < filter_map.height + config.padding; y = y + config.stride) {
            for (unsigned pool_x = 0; pool_x < config.size; ++pool_x) {
                for (unsigned pool_y = 0; pool_y < config.size; ++pool_y) {
                    if ((x < 0 || x > filter_map.width) || (y < 0 || y > filter_map.height)) {
                        // If out of bounds
                        pool[x][y] = 0;
                    } else {
                        // If not out of bounds
                        pool[x][y] = filter_map.neurons[x][y].get_output_val();
                    }
                }
            }
        }
    }
    // Max pool

    for (unsigned i = 0; i < config.size; ++i) {
        free(pool[i]);
    }
    free(pool);
    // Allocate matrix with size of kernel x kernel.
    // double **pool = (double **)malloc(sizeof(double) * (kernel));
    // for (unsigned i = 0; i < kernel; ++i) {
    //     double *pool = (double *)malloc(sizeof(double) * kernel);
    // }

    // double max = 0;

    // for (unsigned offset_x = 0; offset_x < sizeof(fmap_in.map); ++offset_x) {
    //     for (unsigned offset_y = 0; offset_y < sizeof(fmap_in.map); ++offset_y) {
    // 		max = 0;
    // 		for(unsigned x = 0 + offset_x; x < kernel + offset_x; ++x) {
    // 			for(unsigned y = 0 + offset_y; y < kernel + offset_y; ++y) {
    // 				if (max < fmap_in.map[x][y]) {
    // 					max = fmap_in.map[x][y];
    // 				}
    // 			}
    // 		}
    // 		fmap_out.map[offset_x][offset_y] = max;
    //     }
    // }
}

void layer_op::convolution(feature_map &map, unsigned kernel, unsigned stride, int padding = 0) {
}

void layer_op::fully_connect(feature_map &map) {
}