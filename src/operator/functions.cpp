/**
 * @brief All basic activation functions.
 * 
 * @file functions.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include "operator/functions.hpp"


float tanh_transfer_function(float input) {
	return tanh(input);
}

float tanh_transfer_function_derivative(float input) {
	return 1.0 - input * input;
}

