
// TODO: Change floats to vectors for hardware optimization
__kernel void convolution(
    __global float *input_layer,
    int layer_width,
    int layer_height,
    int layer_depth,
    __constant float *filter,
    int filter_width,
    int filter_height,
    int filter_depth,
    __global float *output_layer,
    int filter_padding,
    int filter_stride,
    int filter_num,
    float bias) {
    int column = get_local_id(0);
    int row = get_local_id(1);
    int depth = get_local_id(2);

    float sum = 0;

    int filter_id = 0;

    int half_width = (int)(filter_width / 2);
    int half_height = (int)(filter_height / 2);

    int output_column;
    int output_row;
    int output_depth;

    int output_height;
    int output_width;

    int3 coord;

    bool padX;
    bool padY;

    for (int z = 0; z < layer_depth; ++z) {
        coord.z = z;

        for (int y = -half_height; y <= half_height; ++y) {
            coord.y = -filter_padding + (row * filter_stride) + y + half_height;

            for (int x = -half_width; x <= half_width; ++x) {
                coord.x = -filter_padding + (column * filter_stride) + x + half_width;

                padX = false;
                padY = false;

                padX = (coord.x >= 0 && coord.x < layer_width) ? false : true;
                padY = (coord.y >= 0 && coord.y < layer_height) ? false : true;

                if (!padX && !padY) {
                    sum +=
                        input_layer[coord.x +
                                    coord.y *
                                        layer_width +
                                    coord.z *
                                        layer_width *
                                        layer_height] *
                        filter[filter_id];
                }
                filter_id++;
            }
        }
    }
    output_column = column;
    output_row = row;
    output_depth = filter_num;

    output_width = (layer_width - filter_width + 2 * filter_padding) / filter_stride + 1;
    output_height = (layer_height - filter_height + 2 * filter_padding) / filter_stride + 1;

    output_layer[output_column + output_row * output_width + filter_num * output_width * output_height] = sum + bias;
}

__kernel void maxpool(
    __global float *input_layer,
    int layer_width,
    int layer_height,
    int layer_depth,
    __global float *output_layer,
    int pool_width,
    int pool_height,
    // int pool_slice,		// Depth where maxpool takes place
    int pool_stride
    // int pool_padding
) {
    int column = get_local_id(0);
    int row = get_local_id(1);
    int depth = get_local_id(2);

    float sum = 0;

    int filter_id = 0;

    int half_width = (int)(pool_width / 2);
    int half_height = (int)(pool_height / 2);

    int output_column;
    int output_row;
    int output_depth;

    int output_height;
    int output_width;

    int3 coord;

    float max_value = 0.0f;

    float current_value;

    bool padX;
    bool padY;

    for (int y = -half_height; y <= half_height; ++y) {
        coord.y = (row * pool_stride) + y + half_height;

        for (int x = -half_width; x <= half_width; ++x) {
            coord.x = (column * pool_stride) + x + half_width;

            padX = false;
            padY = false;

            padX = (coord.x >= 0 && coord.x < layer_width) ? false : true;
            padY = (coord.y >= 0 && coord.y < layer_height) ? false : true;

            if (!padX && !padY) {
                current_value = input_layer[coord.x +
                                            coord.y *
                                                layer_width +
                                            depth *
                                                layer_width *
                                                layer_height];
                if (current_value > max_value) {
                    max_value = current_value;
                }
                // max_value = 2;
            }
        }
    }

    output_column = column;
    output_row = row;
    output_depth = depth;

    // output_width = ceil(( (float) layer_width - (float) pool_width ) / (float) pool_stride + 1);
    // output_height = ceil(( (float) layer_height - (float) pool_height ) / (float) pool_stride + 1);
    output_width = (layer_width - pool_width) / pool_stride + 1;
    output_height = (layer_height - pool_height) / pool_stride + 1;

    output_layer[output_column + output_row * output_width + depth * output_width * output_height] = max_value;
}

__kernel void fully_connected_relu(
    __global float *input_layer,
    __constant float *neuron_weights,
    __local float *local_neurons,
    __local float *local_weights,
    __global float *output_layer,
    int input_size,
    float bias,
    int position) {
    unsigned global_size = get_global_size(0);
    unsigned global_id = get_global_id(0);
    unsigned local_size = get_local_size(0);
    unsigned local_id = get_local_id(0);
    float sum = 0;

    // Copy data into workgroup
    for (unsigned i = 0; i < input_size; i += local_size) {
        for (unsigned j = 0; j < local_size; ++j) {
            if (local_id * i + j < input_size) {
                local_neurons[local_id + j] = input_layer[i + j];
                local_weights[local_id + j] = neuron_weights[i + j];
            }
        }

        for (unsigned j = 0; j < local_size; ++j) {
            if (i + j < input_size) {
                // sum += local_neurons[i + j] * local_weights[i + j];
                sum = fma(local_neurons[i + j], local_weights[i + j], sum);
                // if(local_neurons[i + j ] > local_weights[i + j]) {
                // }
                // TODO: Try FMA and vector types
            }
        }
        barrier(CLK_LOCAL_MEM_FENCE);
    }
    barrier(CLK_LOCAL_MEM_FENCE);

    // Add bias neuron
    sum += bias;

    // sum = sum / input_size;
    // output_layer[global_id] = 1;

    // ReLU function
    if (sum > 0) {
        output_layer[position] = sum;
    } else {
        output_layer[position] = 0;
    }
}

__kernel void gpu_convolution(
    __global float *input_layer,
    int layer_width,
    int layer_height,
    int layer_depth,
    __constant float *filter,
    int filter_width,
    int filter_height,
    int filter_depth,
    __global float *output_layer,
    int filter_padding,
    int filter_stride,
    int filter_num,
    float bias) {
}

__kernel void simple(
    __global float *output_layer) {
    output_layer[0] = 1.0f;
}