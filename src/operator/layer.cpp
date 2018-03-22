/**
 * @brief 
 * 
 * @file layer.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */
#include <stdlib.h>

#include "operator/layer.hpp"

void max_pool(feature_map &map, unsigned kernel, unsigned stride) {
	for(unsigned i; i < map; ++i) {
		double *pool = (double *) malloc(sizeof(void)*(kernel));
	}
}

void convolution(feature_map &map, unsigned kernel, unsigned stride, int padding = 0) {

}