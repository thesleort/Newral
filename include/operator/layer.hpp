/**
 * @brief 
 * 
 * @file layer.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#ifndef __LAYER_HPP
#define __LAYER_HPP

#include "net/net.hpp"

void max_pool(feature_map &map, unsigned kernel, unsigned stride);

void convolution(feature_map &map, unsigned kernel, unsigned stride, int padding);

void fully_connect(feature_map &map);

#endif