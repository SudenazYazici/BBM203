cmake_minimum_required(VERSION 3.26)
project(Assignment1)

set(CMAKE_CXX_STANDARD 11)

add_executable(Assignment1 main.cpp
        ImageMatrix.cpp
        ImageLoader.cpp
        ImageProcessor.cpp
        ImageSharpening.cpp
        EncodeMessage.cpp
        EdgeDetector.cpp
        DecodeMessage.cpp
        Convolution.cpp)
