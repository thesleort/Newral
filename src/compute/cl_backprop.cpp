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

}