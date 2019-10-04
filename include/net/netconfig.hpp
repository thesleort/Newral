#ifndef __NETCONFIG_HPP
#define __NETCONFIG_HPP

#include <vector>

struct NeuronFilter;
struct Filter;
struct Weights;
struct Layer;

class cl_compute;

enum type { CONVOLUTION,
            MAXPOOL,
            FULLY,
            DROPOUT,
            INPUT,
            OUTPUT };

enum Activation {
  RELU,
  SIGMOID,
  STEP,
  LINEAR,
  TANH
};
// class neuron;

struct FilterConfig {

  unsigned filters = 1;
  unsigned stride = 1;
  int padding;

  // Filter size
  unsigned width;
  unsigned height;
  unsigned depth; // Same as layer depth.

  Filter *filter;
  // For neuron
  Layer *layer;

  // Has all weights been initialized for this filter
  bool initialized = false;
};

struct MaxpoolConfig {
  unsigned stride;
  int padding;

  // Pool size
  unsigned width;
  unsigned height;

  Layer *layer;

  bool initialized = false;
};

struct FullNetConfig {
  unsigned size;

  Weights *weights;

  Activation activation_function;

  Layer *layer;

  bool initializded = false;
};

struct NetConfig {
  unsigned input_width;
  unsigned input_height;
  unsigned input_depth; // Typically color for images
  unsigned num_layers;
  // layer *layer_num; // The topology consists of n layers
  std::vector<Layer> layers;
};

// For convolutional filters
struct Filter {
  FilterConfig *filter_config;
  float *filter_weights; // To make NxNxDepth feature map
  float *filter_delta_weights;
  float bias;
};

// For fully connected layers
struct Weights {
  FullNetConfig *net_config;
  float *net_weights;
  float *net_delta_weights;
  float bias;
};

struct Layer {
  NetConfig *net_config;
  type layer_type;
  std::string id;
  unsigned width;
  unsigned height;
  unsigned depth; // Based on amount of filters in previous layer.

  // For neuron
  Layer *layer_prev;
  Layer *layer_next;

  float *neurons; // "Dot" product of all filters. PSEUDO data, can be freed from memory after layer is done
  union {
    Filter *filters;
    Weights *weights;
  };

  // Filters
  unsigned num_filters;
  union {
    FilterConfig *filters_config;
    MaxpoolConfig *maxpool_config;
    FullNetConfig *fully_config;
  };
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