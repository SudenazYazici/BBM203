#include "ImageProcessor.h"

ImageProcessor::ImageProcessor() {

}

ImageProcessor::~ImageProcessor() {

}


std::string ImageProcessor::decodeHiddenMessage(const ImageMatrix &img) {


    ImageSharpening sharpening;
    ImageMatrix sharpenedImg = sharpening.sharpen(img, 2);

    EdgeDetector edgeDetector;
    std::vector<std::pair<int, int>> edges = edgeDetector.detectEdges(sharpenedImg);

    DecodeMessage decodeMessage;
    std::string message = decodeMessage.decodeFromImage(sharpenedImg, edges);

    return message;

}

ImageMatrix ImageProcessor::encodeHiddenMessage(const ImageMatrix &img, const std::string &message) {

    EncodeMessage encodeMessage;

    return encodeMessage.encodeMessageToImage(img, message, {});

}
