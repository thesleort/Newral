#ifndef __CL_COMPUTE_HPP
#define __CL_COMPUTE_HPP

#include <CL/cl.hpp>

#include "net/netconfig.hpp"

struct layer;

class cl_compute {
	public:
		cl_compute();
		void load(cl::Program &program);
		void compute_convolution(Layer &this_layer);
		void compute_maxpool(Layer &this_layer);
		void output(Layer &this_layer);
	protected:
	private:
		cl::Program m_program;
		cl::Context m_context;
		std::vector<cl::Device> m_devices;
		cl::Device m_device;
		cl::CommandQueue m_queue;

		cl::Buffer m_input_neurons;
		cl::Buffer m_output_neurons;
};

#endif