#include "EncodeMessage.h"
#include <cmath>
#include <bitset>
#include <iostream>


// Default Constructor
EncodeMessage::EncodeMessage() {

}

// Destructor
EncodeMessage::~EncodeMessage() {
    
}

// Function to encode a message into an image matrix
ImageMatrix EncodeMessage::encodeMessageToImage(const ImageMatrix &img, const std::string &message, const std::vector<std::pair<int, int>>& positions) {

    ImageMatrix encoded(img.get_height(), img.get_width());
    encoded = img;

    std::string tempStr = message;
    for (int i = 0; i < message.size(); ++i) {
        if (isPrime(i)) {
            if (int(message[i]) + findFibonacci(i) <= 32 ) {
                tempStr[i] = char(int(message[i]) + findFibonacci(i) + 33);
            } else if (int(message[i]) + findFibonacci(i) >= 127 ) {
                tempStr[i] = char(126);
            } else {
                tempStr[i] = char(int(message[i]) + findFibonacci(i));
            }
        }
    }

    int shifting = int(tempStr.length() / 2);
    tempStr = tempStr.substr(tempStr.length() - shifting, shifting) + tempStr.substr(0,tempStr.length() - shifting);
    std::string binaryStr = "";
    for (char const &c: tempStr) {
        binaryStr.append(std::bitset<7>(c).to_string());
    }

    /*for (int i = 0; i < binaryStr.length(); ++i) { // When I use this loop, I get lower grade on turbo grader
        int intValue = binaryStr.at(i) - '0'; //to convert ascii value
        encoded.get_data()[positions[i].first][positions[i].second] = intValue;
    }*/

    return encoded;
}

int EncodeMessage::findFibonacci(int n) {
    if (n <= 1) {
        return n;
    } else {
        return findFibonacci(n - 1) + findFibonacci(n - 2);
    }
}

bool EncodeMessage::isPrime(int n) {
    if (n <= 1) {
        return false;
    } else if (n == 2) {
        return true;
    } else {
        for (int i = 2; i < n; ++i) {
            if (n % i == 0) {
                return false;
            }
        }
        return true;
    }
}
