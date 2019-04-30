#ifndef __NETCONFIG_HPP
#define __NETCONFIG_HPP

#include <vector>

struct NeuronFilter;
struct Filter;
struct Layer;

class cl_compute;

enum type { CONVOLUTION,
            MAXPOOL,
            FULLY,
            INPUT,
			OUTPUT };

// class neuron;

struct FilterConfig {

    unsigned filters = 1;
    unsigned stride = 1;
    int padding;

    // Filter size;
    unsigned width;
    unsigned height;
    unsigned depth;     // Same as layer depth.

    Filter *filter;
    // For neuron
    Layer *layer;

    // Has all weights been initialized for this filter
    bool initialized = false;
};

struct NetConfig {
    unsigned input_width;
    unsigned input_height;
    unsigned input_depth; // Typically color for images
    unsigned num_layers;
    // layer *layer_num; // The topology consists of n layers
    std::vector<Layer> layers;
};

struct Filter {
    FilterConfig *filter_config;
    float *filter_weights; // To make NxNxDepth feature map
	float *filter_delta_weights;
    float bias;
};

struct Layer {
	NetConfig *net_config;
    type layer_type;
    std::string id;
    unsigned width;
    unsigned height;
    unsigned depth;     // Based on amount of filters in previous layer.

    // For neuron
    Layer *layer_prev;
    Layer *layer_next;

    float *neurons;    // "Dot" product of all filters. PSEUDO data, can be freed from memory after layer is done
	Filter *filters;

    // Filters
    unsigned num_filters;
    FilterConfig *filters_config;
};

// typedef struct Topology {
// } Topology;

struct class_object {
    double object; // Class of object defines as a number
    double x;      // top left coordinates of object on image
    double y;
    double width; // height and width of object
    double height;
};

#endif