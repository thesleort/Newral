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

void cl_compute::compute_convolution(layer &this_layer) {

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

    std::vector<cl::Buffer> filter_buffers;

    m_input_neurons = cl::Buffer(m_context, CL_MEM_READ_ONLY, layer_prev_height * layer_prev_width * layer_prev_depth * sizeof(float), this_layer.layer_prev->neurons);
    m_output_neurons = cl::Buffer(m_context, CL_MEM_READ_WRITE, layer_height * layer_width * layer_depth * sizeof(float), this_layer.neurons);

    for (unsigned filter_num = 0; filter_num < this_layer.layer_prev->num_filters; ++filter_num) {

        filter_height = this_layer.filter_configs[filter_num].height;
        filter_width = this_layer.filter_configs[filter_num].width;
        filter_depth = this_layer.filter_configs[filter_num].depth;

        filter_padding = this_layer.filter_configs[filter_num].padding;
        filter_stride = this_layer.filter_configs[filter_num].stride;

        filter_buffers.push_back(cl::Buffer(m_context, CL_MEM_READ_ONLY, filter_height * filter_width * filter_depth * sizeof(float), this_layer.filters[filter_num].filter_weight));
        cl::Kernel kernel(m_program, "convolution");

        kernel.setArg(0, m_input_neurons);
        kernel.setArg(1, layer_height);
        kernel.setArg(2, layer_width);
        kernel.setArg(3, layer_depth);
        kernel.setArg(4, filter_buffers.at(filter_num));
        kernel.setArg(5, filter_height);
        kernel.setArg(6, filter_width);
        kernel.setArg(7, filter_depth);
        kernel.setArg(8, m_output_neurons);
        kernel.setArg(9, filter_padding);
        kernel.setArg(10, filter_stride);
        kernel.setArg(11, filter_num);

		// m_device.getInfo<CL_DEVICE_MAX_WORK_GROUP_SIZE>;

        m_queue.enqueueNDRangeKernel(kernel, cl::NullRange, cl::NDRange(filter_height, filter_width, filter_depth));

        // m_queue.enqueueWriteBuffer(filter_buffers[filter_num], CL_FALSE, 0, 0, )
    }

    // m_queue.enqueueWriteBuffer(filter_buffers, )
    // queue.enqueueWriteBuffer()
}

void cl_compute::compute_maxpool(layer &this_layer) {
    int layer_prev_height = this_layer.layer_prev->height;
    int layer_prev_width = this_layer.layer_prev->width;
    int layer_prev_depth = this_layer.layer_prev->depth;

    int layer_height = this_layer.height;
    int layer_width = this_layer.width;
    int layer_depth = this_layer.depth;

    
}

void cl_compute::output(layer &this_layer) {
	int total_length = this_layer.width * this_layer.height *this_layer.depth;
	float output_array[total_length];

	m_queue.enqueueReadBuffer(m_output_neurons, CL_TRUE, 0, sizeof(float) * total_length, output_array);
	for(unsigned i = 0; i < total_length; ++i) {
		std::cout << output_array[i] << " ";
	}
	std::cout << "\n";
}