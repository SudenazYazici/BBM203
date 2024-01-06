// EdgeDetector.cpp

#include "EdgeDetector.h"
#include <cmath>

// Default constructor
EdgeDetector::EdgeDetector() {

    Gx = new double*[3]; // initializing Gx and Gy
    for (int i = 0; i < 3; ++i) {
        Gx[i] = new double[3];
    }

    Gy = new double*[3];
    for (int i = 0; i < 3; ++i) {
        Gy[i] = new double[3];
    }

    Gx[0][1] = Gx[1][1] = Gx [2][1] = 0;
    Gx[0][0] = Gx [2][0] = -1;
    Gx[0][2] = Gx [2][2] = 1;
    Gx[1][0] = -2;
    Gx[1][2] = 2;

    Gy[1][0] = Gy[1][1] = Gy [1][2] = 0;
    Gy[0][0] = Gy [0][2] = -1;
    Gy[2][0] = Gy [2][2] = 1;
    Gy[0][1] = -2;
    Gy[2][1] = 2;

}

// Destructor
EdgeDetector::~EdgeDetector() {

    if (Gx != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] Gx[i];
        }
        delete[] Gx;
    }

    if (Gy != nullptr) {
        for (int i = 0; i < 3; ++i) {
            delete[] Gy[i];
        }
        delete[] Gy;
    }

}

// Detect Edges using the given algorithm
std::vector<std::pair<int, int>> EdgeDetector::detectEdges(const ImageMatrix& input_image) {

    Convolution conv_x(const_cast<double **>(Gx), 3, 3, 1, true );
    ImageMatrix image_x = conv_x.convolve(input_image);

    Convolution conv_y(const_cast<double **>(Gy), 3, 3, 1, true );
    ImageMatrix image_y = conv_y.convolve(input_image);

    ImageMatrix magnitude(image_x.get_height(), image_x.get_width());
    double total = 0;
    for (int i = 0; i < image_x.get_height(); ++i) {
        for (int j = 0; j < image_x.get_width(); j++) {
            magnitude.get_data()[i][j] = sqrt(image_x.get_data(i,j)*image_x.get_data(i,j) + image_y.get_data(i,j)*image_y.get_data(i,j));
            total += magnitude.get_data()[i][j];
        }
    }

    double threshold = total / (magnitude.get_height() * magnitude.get_width());
    std::vector<std::pair<int, int>> edgePixels;
    for (int i = 0; i < magnitude.get_height(); ++i) {
        for (int j = 0; j < magnitude.get_width(); j++) {
            if(magnitude.get_data(i,j) > threshold) {
                edgePixels.push_back({i,j});
            }
        }
    }

    return edgePixels;
}

