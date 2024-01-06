// DecodeMessage.cpp

#include "DecodeMessage.h"
#include <cmath>

// Default constructor
DecodeMessage::DecodeMessage() {
    // Nothing specific to initialize here
}

// Destructor
DecodeMessage::~DecodeMessage() {
    // Nothing specific to clean up
}


std::string DecodeMessage::decodeFromImage(const ImageMatrix& image, const std::vector<std::pair<int, int>>& edgePixels) {

    std::vector<std::string> lsbValues;

    for (const auto& pixel : edgePixels) {

        int pixelValue = int(image.get_data(pixel.first, pixel.second));
        int lsb = (pixelValue & 1);

        lsbValues.push_back(std::to_string(lsb));
    }

    if (lsbValues.size() % 7 != 0) {
        int remainder = lsbValues.size() % 7;
        int zeros = 7 - remainder;
        for (int i = 0; i < zeros ; i++){
            lsbValues.insert(lsbValues.begin(),"0");
        }
    }

    std::vector<char> ascii;
    std::string stringLsb;

    for (const auto & lsbValue : lsbValues) {
        stringLsb += lsbValue;
    }

    for (int i = 0; i < lsbValues.size(); i+=7) {
        std::string tempStr = stringLsb.substr(i, 7);
        int temp = 0;

        for (int j = 0; j < 7; ++j) {
            temp += stoi(tempStr.substr(j,1))* pow(2, 6-j);
        }
        if(temp <= 32){
            ascii.push_back(char(temp+33));
        }else if (temp > 126) {
            ascii.push_back(char(126));
        }else {
            ascii.push_back(char(temp));
        }

    }

    std::string message = "";
    for (int i = 0; i < ascii.size(); ++i) {
        message += ascii[i] ;
    }

    return message;

}

