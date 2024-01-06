#include "Convolution.h"

// Default constructor 
Convolution::Convolution() {
    kernelHeight = 3;
    kernelWidth = 3;
    stride = 1;
    padding = false;

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelWidth; ++i) {
        kernel[i] = new double[kernelWidth];
    }

    for (int i = 0; i < kernelHeight; ++i) {
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = 1;
        }
    }
}

// Parametrized constructor for custom kernel and other parameters
Convolution::Convolution(double** customKernel, int kh, int kw, int stride_val, bool pad) :
kernelHeight(kh), kernelWidth(kw), stride(stride_val), padding(pad){

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelWidth; ++i) {
        kernel[i] = new double[kernelWidth];
    }

    for (int i = 0; i < kernelHeight; ++i) {
        for (int j = 0; j < kernelWidth; j++) {
            kernel[i][j] = customKernel[i][j];
        }
    }
}

// Destructor
Convolution::~Convolution() {

    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

}

// Copy constructor
Convolution::Convolution(const Convolution &other){

    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;

    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

}

// Copy assignment operator
Convolution& Convolution::operator=(const Convolution &other) {

    if (this == &other) {
        return *this;
    }

    // Deallocate old memory
    if (kernel != nullptr) {
        for (int i = 0; i < kernelHeight; ++i) {
            delete[] kernel[i];
        }
        delete[] kernel;
    }

    // Copy from other
    kernelHeight = other.kernelHeight;
    kernelWidth = other.kernelWidth;
    stride = other.stride;
    padding = other.padding;
    kernel = new double*[kernelHeight];
    for (int i = 0; i < kernelHeight; ++i) {
        kernel[i] = new double[kernelWidth];
        for (int j = 0; j < kernelWidth; ++j) {
            kernel[i][j] = other.kernel[i][j];
        }
    }

    return *this;
}


// Convolve Function: Responsible for convolving the input image with a kernel and return the convolved image.
ImageMatrix Convolution::convolve(const ImageMatrix& input_image) const {
    int pad = 0;
    ImageMatrix paddedMatrix(input_image.get_height()+2, input_image.get_width()+2);
    if (padding) {
        pad = 1;

        for (int i = 1; i < input_image.get_height()+1; ++i) {
            for (int j = 1; j < input_image.get_width()+1; ++j) {
                paddedMatrix.get_data()[i][j] = input_image.get_data(i-1,j-1);
            }
        }

    }
    int convolvedHeight = ((input_image.get_height()-kernelHeight + 2*pad) / stride )+1;
    int convolvedWidth = ((input_image.get_width()-kernelWidth + 2*pad) / stride )+1;
    ImageMatrix convolvedImage(convolvedHeight, convolvedWidth);
    for (int i = 0; i < convolvedImage.get_height(); ++i) {
        for (int j = 0; j < convolvedImage.get_width(); ++j) {
            ImageMatrix tempMatrix(kernelHeight,kernelWidth);
            for (int k = 0; k < kernelHeight; ++k) { // initializing a temporary matrix
                for (int l = 0; l < kernelWidth; ++l) {
                    tempMatrix.get_data()[k][l] = 0;
                }
            }

            for (int m = i; m < i+kernelHeight; ++m) { // transferring a part of input matrix to temp
                for (int o = j; o < j+kernelWidth; ++o) {
                    if(padding) {
                        tempMatrix.get_data()[m-i][o-j] = paddedMatrix.get_data(m,o);
                    } else {
                        tempMatrix.get_data()[m-i][o-j] = input_image.get_data(m,o);
                    }

                }
            }

            double result = 0;
            for (int x = 0; x < kernelHeight; ++x) {
                for (int y = 0; y < kernelWidth; ++y) {
                    result += tempMatrix.get_data(x,y) * kernel[x][y];
                }
            }

            if(result > 255.0) { // clipping.
                convolvedImage.get_data()[i][j] = 255.0;
            }else {
                convolvedImage.get_data()[i][j] = result;
            }
        }
    }

    return convolvedImage;

}
