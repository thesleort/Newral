#ifndef __CL_BACKPROP_HPP
#define __CL_BACKPROP_HPP

#include <CL/cl.hpp>

#include "net/netconfig.hpp"

class cl_backprop {
	public:
	cl_backprop();
	void load(cl::Program &program);
	void compute_fully(Layer &this_layer, float *targets);

	private:
	cl::Program m_program;
	cl::Context m_context;
  std::vector<cl::Device> m_devices;
  cl::Device m_device;
  cl::CommandQueue m_queue;

  cl::Buffer m_neuron_buffer_1; // Originally input buffer
  cl::Buffer m_neuron_buffer_2; // Originally output buffer
	cl::Buffer m_target_buffer;
  cl::Buffer *p_output_neurons;
};

#endif