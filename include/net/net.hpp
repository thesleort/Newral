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


enum type { CONVOLUTION,
            MAXPOOL,
            FULLY,
            INPUT,
			OUTPUT };

typedef struct layer;
typedef struct Topology;
class neuron;

typedef struct filter_config {

    unsigned filters = 1;
    unsigned stride = 1;
    int padding;

    // Filter size;
    unsigned width;
    unsigned height;
    unsigned depth;     // Same as layer depth.

    filter *filter;
    // For neuron
    layer_config *layer_config;
} feature_map_config;

typedef struct layer_config {
    type layer_type;
    unsigned width;
    unsigned height;
    unsigned depth;     // Based on amount of filters in previous layer.

    // For neuron
    layer *layer_this;
    layer *layer_prev;
    layer *layer_next;

    // Filters
    unsigned num_filters;
    filter_config *filter_configs;
} layer_config;

typedef struct net_config {
    unsigned input_width;
    unsigned input_height;
    unsigned input_depth; // Typically color for images
    unsigned num_layers;
    layer *layer_num; // The topology constists of n layers
    std::vector<layer_config> layer_config;
} net_config;

typedef struct filter {
    filter_config *filter_config;
    neuron ***neurons; // To make NxNxDepth feature map
} filter;

typedef struct layer {
    layer_config layer_config;
    neuron ***neurons;    // "Dot" product of all filters. PSEUDO data, can be freed from memory after layer is done
} layer;

// typedef struct Topology {
// } Topology;

typedef struct class_object {
    double object; // Class of object defines as a number
    double x;      // top left coordinates of object on image
    double y;
    double width; // height and width of object
    double height;
} class_object;

/**
 * @brief 
 * 
 */
class net {
public:
    net(net_config nc);
    layer get_layer(unsigned index);
    void feed_forward(std::vector<std::vector<std::vector<float>>> &input);
    void backpropagate(std::vector<class_object> objects); // TODO: Rewrite to use pointer instead at some point

private:
    void add_layer(layer_config &config, enum type type);
    void add_filter(filter_config &config);
    void connect_neurons(layer_config &lc, filter_config &fc);
    //Topology m_topology;
    net_config m_net_config;
};

unsigned filter_size_calc(unsigned *length, unsigned filter, int padding, unsigned stride);

unsigned filter_size(unsigned &length, feature_map_config *fmc);

#endif

// Fast User-friendly Classification