#include "ImageSharpening.h"

// Default constructor
ImageSharpening::ImageSharpening() {
    kernel_height = 3;
    kernel_width = 3;

    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
    }

    for (int i = 0; i < kernel_height; ++i) {
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = 1.0/9.0;
        }
    }
}

ImageSharpening::~ImageSharpening(){

    if (blurring_kernel != nullptr) {
        for (int i = 0; i < kernel_height; ++i) {
            delete[] blurring_kernel[i];
        }
        delete[] blurring_kernel;
    }

}

ImageMatrix ImageSharpening::sharpen(const ImageMatrix& input_image, double k) {

    kernel_height = 3;
    kernel_width = 3;

    blurring_kernel = new double*[kernel_height];
    for (int i = 0; i < kernel_height; ++i) {
        blurring_kernel[i] = new double[kernel_width];
    }

    for (int i = 0; i < kernel_height; ++i) {
        for (int j = 0; j < kernel_width; j++) {
            blurring_kernel[i][j] = 1.0/9.0;
        }
    }

    ImageMatrix SharpImg(input_image.get_height()+2, input_image.get_width()+2); // padding is true

    Convolution conv(const_cast<double **>(blurring_kernel), 3, 3, 1, true );
    ImageMatrix BlurredImg = conv.convolve(input_image);

    SharpImg = input_image + (input_image - BlurredImg)* k ;

    for (int i = 0; i < SharpImg.get_height(); ++i) {
        for (int j = 0; j < SharpImg.get_width(); j++) {
            if(SharpImg.get_data(i,j) > 255.0) { // clipping
                SharpImg.get_data()[i][j] = 255.0;
            }else if(SharpImg.get_data(i,j) < 0.0) {
                SharpImg.get_data()[i][j] = 0.0;
            }
        }
    }
    return SharpImg;
}
