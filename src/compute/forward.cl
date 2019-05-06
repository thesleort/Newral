
// TODO: Change floats to vectors for hardware optimization
__kernel void convolution(
		__global float *input_layer, 
		int layer_height,
		int layer_width,
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

	// output_layer = input_layer;

	float sum = 0;

	int filter_id = 0;

	int half_width = (int)(filter_width / 2);
	int half_height = (int)(filter_height / 2);

	int output_column;
	int output_row;
	int output_depth;

	int output_height;
	int output_width;

	// int3 coords; 
	int coordX;
	int coordY;
	int coordZ;

				
	bool padX;
	bool padY;

	for(int z = 0; z < layer_depth; ++z) {

		for(int y = -half_height; y <= half_height; ++y) {
			coordY = -filter_padding + (row * filter_stride) + y + half_height;

			for(int x = -half_width; x <= half_width; ++x) {
				coordX = -filter_padding + (column * filter_stride) + x + half_width;

				padX = false;
				padY = false;

				padX = (coordX >= 0 && coordX < layer_width) ? false : true;
				padY = (coordY >= 0 && coordY < layer_height) ? false : true;

				if(!padX && !padY) {
					sum += 
					input_layer[
						coordX + 
						coordY * 
						layer_width + 
						z * 
						layer_width * 
						layer_height] * 
						filter[filter_id];
				}
				filter_id++;
			}
		}
	}
	// barrier(CLK_GLOBAL_MEM_FENCE);
	output_column = column;
	output_row = row;
	output_depth = filter_num;

	// output_column = 1;
	// output_row = 1;

	output_width = (layer_width - filter_width + 2 * filter_padding) / filter_stride + 1;
	output_height = (layer_height - filter_height + 2 * filter_padding) / filter_stride + 1;

	output_layer[output_column + output_row * output_width + filter_num * output_width * output_height] = sum + bias;
}

// __kernel void maxpool(
// 	__global float *input_layer, 
// 	__constant int layer_height,
// 	__constant int layer_width,
// 	__constant int layer_depth,
// 	__constant float *filter, 
// 	__constant int filter_width,
// 	__constant int filter_height,
// 	__constant int filter_depth,
// 	__global float *output_layer,
// 	__constant int filter_padding,
// 	__constant int filter_stride,
// 	__constant int filter_num
// )


__kernel void simple(
	__global float *output_layer
) {
	output_layer[0] = 1.0f;
}