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

void max_pool(feature_map &fmap_in, feature_map &fmap_out, unsigned kernel, unsigned stride) {

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

void convolution(feature_map &map, unsigned kernel, unsigned stride, int padding = 0) {
}

void fully_connect(feature_map &map) {
}