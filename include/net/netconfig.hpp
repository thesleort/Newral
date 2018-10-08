#ifndef __NETCONFIG_HPP
#define __NETCONFIG_HPP

#include <vector>

typedef struct neuron_filter;
typedef struct filter;
typedef struct layer_config;
class cl_compute;

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

    filter *m_filter;
    // For neuron
    layer *m_layer;

    // Has all weights been initialized for this filter
    bool initialized = false;
} filter_config;

typedef struct layer_config {
    // type layer_type;
    // unsigned width;
    // unsigned height;
    // unsigned depth;     // Based on amount of filters in previous layer.

    // // For neuron
    // layer *layer_this;
    // layer *layer_prev;
    // layer *layer_next;

    // float *neurons;    // "Dot" product of all filters. PSEUDO data, can be freed from memory after layer is done
	// filter *filters;

    // // Filters
    // unsigned num_filters;
    // filter_config *filter_configs;
} layer_config;

typedef struct net_config {
    unsigned input_width;
    unsigned input_height;
    unsigned input_depth; // Typically color for images
    unsigned num_layers;
    layer *layer_num; // The topology consists of n layers
    std::vector<layer> m_layers;
} net_config;

typedef struct filter {
    filter_config *m_filter_config;
    float *filter_weight; // To make NxNxDepth feature map
	float *filter_delta_weight;
} filter;

typedef struct layer {
	net_config *m_net_config;
    type layer_type;
    unsigned width;
    unsigned height;
    unsigned depth;     // Based on amount of filters in previous layer.

    // For neuron
    layer *layer_this;
    layer *layer_prev;
    layer *layer_next;

    float *neurons;    // "Dot" product of all filters. PSEUDO data, can be freed from memory after layer is done
	filter *filters;

    // Filters
    unsigned num_filters;
    filter_config *filter_configs;
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

#endif