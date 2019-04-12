#include <iostream>

#include "compute/cl_compute.hpp"

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
    int num_filters = this_layer.num_filters;

    int layer_prev_height = this_layer.layer_prev->height;
    int layer_prev_width = this_layer.layer_prev->width;
    int layer_prev_depth = this_layer.layer_prev->depth;

    int layer_height = this_layer.height;
    int layer_width = this_layer.width;
    int layer_depth = this_layer.depth;

    int filter_height;
    int filter_width;
    int filter_depth;
    int filter_padding;
    int filter_stride;

    int layersize = (layer_prev_height * layer_prev_width * layer_prev_depth);

    std::vector<cl::Buffer> filter_buffers;
    std::cout << this_layer.layer_prev->neurons << "\n";

    m_input_neurons = cl::Buffer(m_context, CL_MEM_READ_ONLY | CL_MEM_HOST_NO_ACCESS | CL_MEM_COPY_HOST_PTR, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons);
    m_output_neurons = cl::Buffer(m_context, CL_MEM_WRITE_ONLY | CL_MEM_HOST_READ_ONLY, layer_height * layer_width * layer_depth * sizeof(float), this_layer.neurons);

    std::cout << "This layer\n" << layersize << "\n";
    for(unsigned i = 0; i < layersize; ++i) {
        this_layer.layer_prev->neurons[i] = 2;
		std::cout << this_layer.layer_prev->neurons[i] << " ";
	}
	std::cout << "\n";

    for (unsigned filter_num = 0; filter_num < this_layer.layer_prev->num_filters; ++filter_num) {

        filter_height = this_layer.filters_config->height;
        filter_width = this_layer.filters_config->width;
        filter_depth = this_layer.filters_config->depth;

        filter_padding = this_layer.filters_config->padding;
        filter_stride = this_layer.filters_config->stride;

        filter_buffers.push_back(cl::Buffer(m_context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, filter_height * filter_width * filter_depth * sizeof(float), this_layer.filters[filter_num].filter_weights));
        cl::Kernel kernel(m_program, "convolution");

        kernel.setArg(0, m_input_neurons);					// input layer
        kernel.setArg(1, layer_height);						// layer height
        kernel.setArg(2, layer_width);						// layer width
        kernel.setArg(3, layer_depth);						// layer depth
        kernel.setArg(4, filter_buffers.at(filter_num));	// filter
        kernel.setArg(5, filter_height);					// filter width
        kernel.setArg(6, filter_width);						// filter height
        kernel.setArg(7, filter_depth);						// filter depth
        kernel.setArg(8, m_output_neurons);					// output layer
        kernel.setArg(9, filter_padding);					// filter padding
        kernel.setArg(10, filter_stride);					// filter stride
        kernel.setArg(11, filter_num);						// filter num

		// m_device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>;

        m_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(filter_height, filter_width, filter_depth));

        // m_queue.enqueueTask();
        // m_queue.enqueueWriteBuffer(filter_buffers[filter_num], CL_FALSE, 0, 0, )
    }

    // m_queue.enqueueWriteBuffer(filter_buffers, )
    // queue.enqueueWriteBuffer()
}

void cl_compute::compute_maxpool(Layer &this_layer) {
    int layer_prev_height = this_layer.layer_prev->height;
    int layer_prev_width = this_layer.layer_prev->width;
    int layer_prev_depth = this_layer.layer_prev->depth;

    int layer_height = this_layer.height;
    int layer_width = this_layer.width;
    int layer_depth = this_layer.depth;

    
}

void cl_compute::output(Layer &this_layer) {
	int total_length = this_layer.width * this_layer.height * this_layer.depth;
	float output_array[total_length];

	m_queue.enqueueReadBuffer(m_output_neurons, CL_TRUE, 0, sizeof(float) * total_length, output_array);
	for(unsigned i = 0; i < total_length; ++i) {
		std::cout << output_array[i] << " ";
	}
	std::cout << "\n";
	m_queue.finish();
}