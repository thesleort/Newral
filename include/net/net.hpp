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


enum type { CONVOLUTION,
            MAXPOOL,
            FULLY,
            INPUT };

typedef struct layer;
typedef struct neuron;

typedef struct feature_map_config {
    type layer_type;
    unsigned filters = 1;
    unsigned stride = 1;
    unsigned size;
    int padding;

    // For neuron
    layer *layer_prev;
    layer *layer_next;
} feature_map_config;

typedef struct net_config {
    unsigned width;
    unsigned height;
    unsigned filters; // Typically color for images
    std::vector<feature_map_config> layer_feature_map;
} net_config;

typedef struct class_object {
    double object; // Class of object defines as a number
    double x;      // top left coordinates of object on image
    double y;
    double width; // height and width of object
    double height;
} class_object;

typedef struct feature_map { // Filter / feature map
    neuron *map;             // To make NxN feature map
    type map_type;
} feature_map;

typedef struct filter {
    unsigned width;
    unsigned height;
    neuron **neurons; // To make NxN feature map
} filter;

typedef struct layer {
    feature_map_config *fmc;
    filter *filters; // Every layer contains n feature maps
} layer;

typedef struct Topology {
    unsigned num_layers;
    layer *layers; // The topology constists of m layers
} Topology;

/**
 * @brief 
 * 
 */
class net {
public:
    net(net_config nc);
    void add_layer(feature_map_config config);
    layer get_layer(unsigned index);
    void feed_forward();
    void backpropagate(std::vector<class_object> objects); // TODO: Rewrite to use pointer instead at some point

private:
    Topology m_topology;
    net_config m_net_config;
};

unsigned filter_size_calc(unsigned *length, unsigned filter, int padding, unsigned stride);

unsigned filter_size(unsigned *length, feature_map_config *fmc);

#endif

// Fast User-friendly Classification