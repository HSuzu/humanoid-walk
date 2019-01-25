#ifndef HELPERS_H
#define  HELPERS_H

extern "C" {
  #include "extApi.h"
}

#include <iostream>

#define PI 3.14159265
const int step_ms = 25;
// #define DEBUG //comment to disable debug

inline void log_response(int code) {
  if (code == 0) return;

  std::cout << "Error Code: ";
  if ( code & simx_return_novalue_flag) {
    std::cout << "simx_return_novalue_flag";
  }

  if ( code & simx_return_timeout_flag) {
    std::cout << "simx_return_timeout_flag";
  }

  if ( code & simx_return_illegal_opmode_flag) {
    std::cout << "simx_return_illegal_opmode_flag";
  }

  if ( code & simx_return_local_error_flag) {
    std::cout << "simx_return_local_error_flag";
  }

  if ( code & simx_return_remote_error_flag) {
    std::cout << "simx_return_remote_error_flag";
  }

  if ( code & simx_return_split_progress_flag) {
    std::cout << "simx_return_split_progress_flag";
  }

  if ( code & simx_return_initialize_error_flag) {
    std::cout << "simx_return_initialize_error_flag";
  }
  std::cout << std::endl;
}
#endif
