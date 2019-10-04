/**
 * @brief 
 * 
 * @file cl_prepare.hpp
 * @author Troels Blicher Petersen
 * @date 2018-08-16
 */

#ifndef __CL_PREPARE
#define __CL_PREPARE

#include <CL/cl.hpp>
// #include

class cl_setup {
  public:
  cl_setup();
  int setup(int DEVICE_TYPE);
  void build(const std::string &file, const char *version);
  std::vector<cl::Program> *get_programs();
  std::vector<cl::Device> *get_devices();
  cl::Device *get_device(const int DEVICE_TYPE);

  private:
  bool m_setup = false;
  int m_device_type;
  std::vector<cl::Program> m_programs;
  std::vector<cl::Platform> m_platforms; // For future, now only one platform supported at a time.
  std::vector<cl::Device> m_devices;
};

#endif