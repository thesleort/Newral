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

		cl::Buffer m_neuron_buffer_1; // Originally input buffer
		cl::Buffer m_neuron_buffer_2; // Originally output buffer
		cl::Buffer *p_output_neurons;

};

#endif