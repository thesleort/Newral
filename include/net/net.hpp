/**
 * @brief 
 * 
 * @file net.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __NET_HPP
#define __NET_HPP

#include <vector>

#include "neuron/neuron.hpp"

typedef struct feature_map {
	double **map;

} feature_map;

// typedef struct layer {
// 	feature_map *map;
// } layer;

typedef std::vector<feature_map*> layer;

typedef struct topology {
	std::vector<layer> layers;
} topology;

class net {
public:
	net();

private:
	std::vector<layer> m_layers;
};

#endif