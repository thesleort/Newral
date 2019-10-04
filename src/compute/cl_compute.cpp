#include <iostream>

#include "compute/cl_compute.hpp"
#include <math.h>
cl_compute::cl_compute() {
}

void cl_compute::load(cl::Program &program) {
  m_program = program;
  m_context = program.getInfo<CL_PROGRAM_CONTEXT>();
  m_devices = m_context.getInfo<CL_CONTEXT_DEVICES>();
  m_device = m_devices.front();
  m_queue = cl::CommandQueue(m_context, m_device);
}

void cl_compute::compute_convolution(Layer &this_layer) {
  std::cout << "COMPUTE CONVOLUTION\n";
  unsigned num_filters = this_layer.num_filters;

  int layer_prev_width = this_layer.layer_prev->width;
  int layer_prev_height = this_layer.layer_prev->height;
  int layer_prev_depth = this_layer.layer_prev->depth;

  int layer_width = this_layer.width;
  int layer_height = this_layer.height;
  int layer_depth = this_layer.depth;

  int filter_width;
  int filter_height;
  int filter_depth;
  int filter_padding;
  int filter_stride;
  float filter_bias;

  int layersize = (layer_prev_width * layer_prev_height * layer_prev_depth);

  std::vector<cl::Buffer> filter_buffers;

  // for (int i = 0; i < layersize; i++) {
  //     this_layer.layer_prev->neurons[i] = 2;
  // }
  for (int i = 0; i < layersize; i++) {
    std::cout << this_layer.layer_prev->neurons[i] << ",";
  }
  std::cout << "\nLayer_prev: " 
            << (layer_prev_height * layer_prev_width * layer_prev_depth) 
            << " " 
            << layer_prev_width 
            << " " 
            << layer_prev_height 
            << " " 
            << layer_prev_depth << "\n";

  // m_neuron_buffer_1 = cl::Buffer(m_context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons, NULL);
  m_neuron_buffer_1 = cl::Buffer(
    m_context, 
    CL_MEM_READ_WRITE | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, 
    layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), 
    this_layer.layer_prev->neurons, 
    NULL);

  // m_neuron_buffer_2 = cl::Buffer(m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, layer_height * layer_width * layer_depth * sizeof(float));
  m_neuron_buffer_2 = cl::Buffer(
    m_context, 
    CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, 
    layer_height * layer_width * layer_depth * sizeof(float));

  for (int i = 0; i < layersize; ++i) {
    std::cout << this_layer.layer_prev->neurons[i] << ",";
  }
  std::cout << "\n";

  std::cout << "Num filters: " << this_layer.num_filters 
            << "\nNum filters prev: " << this_layer.layer_prev->num_filters 
            << "\n";

  for (unsigned filter_num = 0; filter_num < num_filters; filter_num++) {

    filter_width = this_layer.filters_config->width;
    filter_height = this_layer.filters_config->height;
    filter_depth = this_layer.filters_config->depth;
    filter_bias = this_layer.filters[filter_num].bias;

    filter_padding = this_layer.filters_config->padding;
    filter_stride = this_layer.filters_config->stride;

    filter_buffers.push_back(
      cl::Buffer(
        m_context, 
        CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
        filter_height * 
        filter_width * 
        filter_depth * 
        sizeof(float), 
        this_layer.filters[filter_num].filter_weights, 
        NULL));

    cl::Kernel kernel(m_program, "convolution");

    std::cout << "\nFilter (" << filter_num << "): \n";
    for (int i = 0; i < (filter_height * filter_width * filter_depth); i++) {
      if (i % filter_width == 0) {
        std::cout << "\n";
      }

      if (i % (filter_width * filter_height) == 0) {
        std::cout << "\n";
      }
      std::cout << this_layer.filters[filter_num].filter_weights[i] << ",";
    }
    std::cout << "\n";

    //*(T (*)[N])p = where T is the type, N is the number of elements and p is the pointer.
    // m_queue.enqueueWriteBuffer(m_input_neurons, CL_TRUE, 0, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons);
    // m_queue.enqueueWriteBuffer(filter_buffers.at(filter_num), CL_TRUE, 0, filter_height * filter_width * filter_depth * sizeof(float), this_layer.filters[filter_num].filter_weights);

    kernel.setArg(0, m_neuron_buffer_1);             // input layer
    kernel.setArg(1, layer_prev_width);              // layer width
    kernel.setArg(2, layer_prev_height);             // layer height
    kernel.setArg(3, layer_prev_depth);              // layer depth
    kernel.setArg(4, filter_buffers.at(filter_num)); // filter
    kernel.setArg(5, filter_height);                 // filter width
    kernel.setArg(6, filter_width);                  // filter height
    kernel.setArg(7, filter_depth);                  // filter depth
    kernel.setArg(8, m_neuron_buffer_2);             // output layer
    kernel.setArg(9, filter_padding);                // filter padding
    kernel.setArg(10, filter_stride);                // filter stride
    kernel.setArg(11, filter_num);                   // filter num
    kernel.setArg(12, filter_bias);

    std::cout << "Padding: " << filter_padding 
              << " - Stride: " << filter_stride 
              << " - Bias: " << this_layer.filters[filter_num].bias << "\n";

    std::cout << "Layer prev width/height: " 
              << layer_prev_width 
              << "/" 
              << layer_prev_height 
              << "/" 
              << layer_prev_depth << "\n";

    std::cout << "Layer width/height: " 
              << layer_width 
              << "/" 
              << layer_height 
              << "/" 
              << layer_depth << "\n";

    std::cout << "Filter width/height: " 
              << filter_width 
              << "/" 
              << filter_height 
              << "/" 
              << filter_depth << "\n";

    m_queue.enqueueNDRangeKernel(
      kernel, 
      cl::NullRange, 
      cl::NDRange(
        layer_width, 
        layer_height, 
        layer_depth
      ), 
      cl::NDRange(
        filter_width, 
        filter_height, 
        filter_depth
      )
    );
  }
}

void cl_compute::compute_maxpool(Layer &this_layer) {
  std::cout << "COMPUTE MAXPOOL\n";
  int layer_prev_width = this_layer.layer_prev->width;
  int layer_prev_height = this_layer.layer_prev->height;
  int layer_prev_depth = this_layer.layer_prev->depth;

  int layer_width = this_layer.width;
  int layer_height = this_layer.height;
  int layer_depth = this_layer.depth;

  int pool_width = this_layer.maxpool_config->width;
  int pool_height = this_layer.maxpool_config->height;
  int pool_stride = this_layer.maxpool_config->stride;

  // int layersize = layer_width * layer_height * layer_depth;

  std::cout << "Width: " 
            << layer_width 
            << " "
            << "| Height: " << layer_height 
            << "| Depth: " << layer_depth 
            << "| Stride: " << pool_width << "\n";

  // m_output_neurons = cl::Buffer(m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, layersize * sizeof(float));
  m_neuron_buffer_1 = cl::Buffer(
    m_context, 
    CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, 
    layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), 
    this_layer.layer_prev->neurons, 
    NULL);

  m_neuron_buffer_2 = cl::Buffer(
    m_context, 
    CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, 
    layer_height * layer_width * layer_depth * sizeof(float));

  cl::Kernel kernel(m_program, "maxpool");

  // if(this_layer.layer_prev->layer_type != INPUT) {
  //     kernel.setArg(0, m_neuron_buffer_2);
  // } else {
  //     m_neuron_buffer_1 = cl::Buffer(m_context, CL_MEM_READ_ONLY | CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons, NULL);
  //     m_neuron_buffer_2 = cl::Buffer(m_context, CL_MEM_READ_WRITE | CL_MEM_HOST_READ_ONLY, layer_height * layer_width * layer_depth * sizeof(float));
  //     kernel.setArg(0, m_neuron_buffer_1);
  // }

  kernel.setArg(0, m_neuron_buffer_1);
  kernel.setArg(1, layer_prev_width);
  kernel.setArg(2, layer_prev_height);
  kernel.setArg(3, layer_prev_depth);
  kernel.setArg(4, m_neuron_buffer_2);
  kernel.setArg(5, pool_width);
  kernel.setArg(6, pool_height);
  kernel.setArg(7, pool_stride);
  // kernel.setArg(8, )
  // kernel.setArg(5, )

  m_queue.enqueueNDRangeKernel(
    kernel, 
    cl::NullRange, 
    cl::NDRange(
      layer_width, 
      layer_height, 
      layer_depth
    ), 
    cl::NDRange(
      layer_width, 
      layer_height, 
      layer_depth
    )
  );
}

void cl_compute::compute_fully(Layer &this_layer) {
  std::cout << "COMPUTE FULLY CONNECTED\n";
  int layer_prev_width = this_layer.layer_prev->width;
  int layer_prev_height = this_layer.layer_prev->height;
  int layer_prev_depth = this_layer.layer_prev->depth;

  int layer_width = this_layer.width;
  int layer_height = this_layer.height;
  int layer_depth = this_layer.depth;

  int input_size = layer_prev_width * layer_prev_height * layer_prev_depth;
  int output_size = this_layer.fully_config->size;

  std::vector<cl::Buffer> filter_buffers;

  float bias = this_layer.fully_config->weights->bias;

  int err;

  m_neuron_buffer_1 = cl::Buffer(
    m_context, 
    CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR, 
    layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), 
    this_layer.layer_prev->neurons, 
    &err);

  m_neuron_buffer_2 = cl::Buffer(
    m_context, 
    CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, 
    output_size * sizeof(float), 
    NULL, 
    &err);

  switch (this_layer.fully_config->activation_function) {
  case RELU: {
    std::cout << "RELU\n";
    cl::Kernel kernel(m_program, "fully_connected_relu");
    unsigned workgroup_size = 
      kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(m_device, NULL);

    std::cout << "Workgroup size: " << workgroup_size << "\n";
    std::cout << "Bias: " << bias << "\n";
    
    for (unsigned neuron = 0; neuron < output_size; neuron++) {

      filter_buffers.push_back(
        cl::Buffer(
          m_context, 
          CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, 
          input_size * sizeof(float), 
          this_layer.weights[neuron].net_weights, 
          &err
        )
      );

      kernel.setArg(0, m_neuron_buffer_1);
      kernel.setArg(1, filter_buffers.at(neuron));
      kernel.setArg(2, workgroup_size * sizeof(float), NULL);
      kernel.setArg(3, workgroup_size * sizeof(float), NULL);
      kernel.setArg(4, m_neuron_buffer_2);
      kernel.setArg(5, input_size);
      kernel.setArg(6, bias);
      kernel.setArg(7, neuron);

      m_queue.enqueueNDRangeKernel(
        kernel, 
        cl::NullRange, 
        cl::NDRange(output_size), 
        cl::NDRange(workgroup_size)
      );
    }
  } break;
  default:
    break;
  }
}

void cl_compute::output(Layer &this_layer) {
  std::cout << "Width: " << this_layer.width << " "
            << "| Height: " << this_layer.height 
            << "| Depth: " << this_layer.depth << "\n";
  unsigned total_length = this_layer.width * this_layer.height * this_layer.depth;
  // float *output_array = new float[total_length];

  std::cout << "\nTotal length: " << total_length;
  std::cout << "\n";

  for (int i = 0; i < total_length; ++i) {
    this_layer.neurons[i] = 0;
  }

  m_queue.finish();
  std::cout << "Queue: " << m_queue.enqueueReadBuffer(
    m_neuron_buffer_2, 
    CL_TRUE, 
    0, 
    sizeof(float) * total_length, 
    this_layer.neurons);

  std::cout << "\nAfter read:\n";

  for (unsigned i = 0; i < total_length; ++i) {
    if (i % this_layer.width == 0) {
      std::cout << "\n";
    }

    if (i % (this_layer.width * this_layer.height) == 0) {
      std::cout << "\n";
    }
    std::cout << this_layer.neurons[i] << " ";
  }
  std::cout << "\n";
}