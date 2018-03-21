/**
 * @brief All basic activation functions.
 * 
 * @file functions.cpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#include "operator/functions.hpp"


double tanh_transfer_function(double input) {
	return tanh(input);
}

double tanh_transfer_function_derivative(double input) {
	return 1.0 - input * input;
}

