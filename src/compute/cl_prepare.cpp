/**
 * @brief 
 * 
 * @file cl_prepare.cpp
 * @author Troels Blicher Petersen
 * @date 2018-08-16
 */

#include <CL/cl.hpp>
#include <iostream>
#include <fstream>

#include "error.h"
#include "compute/cl_prepare.hpp"
#include "aux.h"

cl_setup::cl_setup() {
}

/**
 * @brief 
 * 
 * @param DEVICE_TYPE 
 * @return int 
 */
int cl_setup::setup(int DEVICE_TYPE) {

  m_device_type = DEVICE_TYPE;

  cl::Platform::get(&m_platforms);
  cl::Platform platform;

  std::vector<cl::Platform> tmp_platforms;

  cl::Device device;

  for (unsigned platform_num = 0; platform_num < m_platforms.size(); ++platform_num) {
    m_platforms.at(platform_num).getDevices(DEVICE_TYPE, &m_devices);
    if (m_devices.size() > 0) {
      tmp_platforms.push_back(m_platforms.at(platform_num));
    }
  }

  if (m_devices.size() < 1) {
    std::cout << COLOR_ERROR << "ERROR: Couldn't find any CL platforms!" << COLOR_NORMAL_N;
    return ERR_CL_NO_PLATFORM;
  } else if (tmp_platforms.size() > 1) {
    std::cout << "Which platform is preferred?\n";
    unsigned pref;
    for (unsigned platform_num = 0; platform_num < tmp_platforms.size(); ++platform_num) {
      std::cout << "(" << platform_num << "): " << tmp_platforms.at(platform_num).getInfo<CL_PLATFORM_NAME>() << "\n";
    }
    std::cout << "Please choose a platform: ";
    std::cin >> pref;
    if (pref > tmp_platforms.size() - 1) {
      std::cout << "This platform does not exist.\n";
      return ERR_INVALID_INPUT;
    }

    platform = tmp_platforms.at(pref);
  } else {
    std::cout << "(0): " << tmp_platforms.at(0).getInfo<CL_PLATFORM_NAME>() << "\n";
    platform = m_platforms.front();
  }

  // Alpha version currently only supports one device
  device = m_devices.at(0);
  m_setup = true;
  return 0;
}

/**
 * @brief 
 * 
 * @param file 
 * @param VERSION 
 */
void cl_setup::build(const std::string &file, const char *version) {
  if (m_setup != true) {
    std::cerr << "OpenCL has not been setup yet" << std::endl;
    exit(0);
  }
  // cl_int err;

  std::ifstream cl_file(file);
  std::string src(std::istreambuf_iterator<char>(cl_file), (std::istreambuf_iterator<char>()));

  cl::Program::Sources sources(1, std::make_pair(src.c_str(), src.length() + 1));

  for (unsigned device_num = 0; device_num < m_devices.size(); ++device_num) {
    cl::Context context(m_devices.at(0));
    cl::Program program(context, sources);

    program.build(version);

    cl::Device device = m_devices.at(device_num);
    cl_build_status status = program.getBuildInfo<CL_PROGRAM_BUILD_STATUS>(device);
    if (status == CL_BUILD_ERROR) {
      // Get the build log
      std::string name = device.getInfo<CL_DEVICE_NAME>();
      std::string buildlog = program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device);
      std::cerr << "Build log for " << name << ":" << std::endl
                << buildlog << std::endl;
    }
    m_programs.push_back(program);
  }
}

/**
 * @brief 
 * 
 * @return std::vector<cl::Program>* 
 */
std::vector<cl::Program> *cl_setup::get_programs() {
  return &m_programs;
}

/**
 * @brief Returns a pointer to the available devices
 * 
 * 
 * @return std::vector<cl::Device>* 
 */
std::vector<cl::Device> *cl_setup::get_devices() {
  return &m_devices;
}

/**
 * @brief Returns the first available device that matches the argument.
 * 
 * @param DEVICE_TYPE if not specified, then the first device will be returned.
 * @return cl::Device* 
 */
cl::Device *cl_setup::get_device(const int DEVICE_TYPE = CL_DEVICE_TYPE_ALL) {
  switch (DEVICE_TYPE) {
  case CL_DEVICE_TYPE_ALL:
    return &m_devices.front();
    break;
  default:
    for (unsigned i = 0; i < m_devices.size(); ++i) {
      if (DEVICE_TYPE == (int)m_devices.at(i).getInfo<CL_DEVICE_TYPE>()) {
        return &m_devices.at(i);
      }
    }
    break;
  }
  return NULL;
}