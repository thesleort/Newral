#include <iostream>

#include "compute/cl_backprop.hpp"

cl_backprop::cl_backprop() {
}

void cl_backprop::load(cl::Program &program) {
  m_program = program;
  m_context = program.getInfo<CL_PROGRAM_CONTEXT>();
  m_devices = m_context.getInfo<CL_CONTEXT_DEVICES>();
  m_device = m_devices.front();
  m_queue = cl::CommandQueue(m_context, m_device);
}

void cl_backprop::compute_fully(Layer &this_layer, float *targets) {
	std::cout << "BACKPROP FULLY CONNECTED\n";
  int layer_front_width = this_layer.layer_front->width;
  int layer_front_height = this_layer.layer_front->height;
  int layer_front_depth = this_layer.layer_front->depth;

  int layer_width = this_layer.width;
  int layer_height = this_layer.height;
  int layer_depth = this_layer.depth;

  int input_size = layer_front_width * layer_front_height * layer_front_depth;
  int output_size = this_layer.fully_config->size;

  std::vector<cl::Buffer> filter_buffers;

	int err;

	m_neuron_buffer_1 = cl::Buffer(
		m_context,
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
		layer_front_height * layer_front_width * layer_front_depth * sizeof(float),
		this_layer.layer_front->neurons,
		&err
	);

	m_neuron_buffer_2 = cl::Buffer(
		m_context,
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
		output_size * sizeof(float),
		NULL,
		&err
	);

	m_target_buffer = cl::Buffer(
		m_context,
		CL_MEM_HOST_WRITE_ONLY | CL_MEM_COPY_HOST_PTR,
		layer_front_height * layer_front_width * layer_front_depth * sizeof(float),
		targets,
		&err
	);

	switch(this_layer.fully_config->activation_function) {
		case RELU:
		std::cout << "RELU\n";
		cl::Kernel kernel(m_program, "fully_connected_relu");
		unsigned workgroup_size = 
			kernel.getWorkGroupInfo<CL_KERNEL_WORK_GROUP_SIZE>(m_device, NULL);

		std::cout << "Workgroup size: " << workgroup_size << "\n";
		
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
			kernel.setArg(2, m_target_buffer);
			kernel.setArg(3, workgroup_size * sizeof(float), NULL);
			kernel.setArg(4, workgroup_size * sizeof(float), NULL);
			kernel.setArg(5, workgroup_size * sizeof(float), NULL);
			kernel.setArg(6, input_size);
			
			m_queue.enqueueNDRangeKernel(
				kernel,
				cl::NullRange,
				cl::NDRange(output_size),
				cl::NDRange(workgroup_size)
			);
		}
		break;
	}
}