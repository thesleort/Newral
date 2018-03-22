/**
 * @brief 
 * 
 * @file layer.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-22
 */

#ifndef __LAYER_HPP
#define __LAYER_HPP

typedef feature_map;


void convolution(feature_map &map, unsigned kernel, unsigned stride, int padding = 0);

#endif