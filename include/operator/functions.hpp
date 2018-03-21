/**
 * @brief 
 * 
 * @file functions.hpp
 * @author Troels Blicher Petersen
 * @date 2018-03-21
 */

#ifndef __functions_hpp
#define __functions_hpp

/*
 *               1  * * * * * * *
 *               |*
 *               * 
 * - - - - - - - 0 - - - - - - - 10
 *               *
 *              *|
 *  * * * * * * -1
 */ 
double tanh_transfer_function(double input);
double tanh_transfer_function_derivative(double input);

/*
 *               1     * * * * * * 
 *               |  *
 *               |* 
 *               *
 *              *|
 *            *  |
 * * * * * * - - 0 - - - - - - - 10
 */ 
double sigmoid_transfer_function(double input);
double sigmoid_transfer_function_derivative(double input);

/*
 *               1           *
 *               |         *
 *               |       *
 *               |     *
 *               |   *
 *               | *
 * * * * * * * * 0 - - - - - 10
 */ 
double ReLU_transfer_function(double input);
double ReLU_transfer_function_derivative(double input);

#endif