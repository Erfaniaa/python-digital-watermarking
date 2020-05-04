#include "watermarker.h"

void shiftDFT(cv::Mat mag) {

    mag = mag(cv::Rect(0, 0, mag.cols & (-2), mag.rows & (-2)));

    int cx = mag.cols / 2;
    int cy = mag.rows / 2;

    cv::Mat q0 = cv::Mat(mag, cv::Rect(0, 0, cx, cy));
    cv::Mat q1 = cv::Mat(mag,  cv::Rect(cx, 0, cx, cy));
    cv::Mat q2 =  cv::Mat(mag,  cv::Rect(0, cy, cx, cy));
    cv::Mat q3 =  cv::Mat(mag,  cv::Rect(cx, cy, cx, cy));

    cv::Mat tmp =  cv::Mat();
    q0.copyTo(tmp);
    q3.copyTo(q0);
    tmp.copyTo(q3);

    q1.copyTo(tmp);
    q2.copyTo(q1);
    tmp.copyTo(q2);
}

cv::Mat getBlueChannel(cv::Mat image)
{
    cv::Mat nextImg = image;
    std::vector<cv::Mat> channel;
    split(nextImg,channel);
    return channel[0];
}

cv::Mat getDftMat(cv::Mat padded)
{
    std::vector<cv::Mat> planes;
    planes.push_back(padded);
    planes.push_back(cv::Mat::zeros(padded.size(), CV_32F));
    cv::Mat comImg;
    merge(planes,comImg);
    cv::dft(comImg, comImg);
    return comImg;
}

void addTextByMat(cv::Mat comImg,cv::String watermarkText,cv::Point point,double fontSize)
{
    cv::putText(comImg, watermarkText, point, cv::FONT_HERSHEY_DUPLEX, fontSize, cv::Scalar::all(0),2);
    cv::flip(comImg, comImg, -1);
    putText(comImg, watermarkText, point, cv::FONT_HERSHEY_DUPLEX, fontSize, cv::Scalar::all(0),2);
    flip(comImg, comImg, -1);
}

cv::Mat transFormMatWithText(cv::Mat srcImg, cv::String watermarkText,double fontSize) {
        cv::Mat padded=getBlueChannel(srcImg);
        padded.convertTo(padded, CV_32F);
        cv::Mat comImg = getDftMat(padded);
        // add text
        cv::Point center(padded.cols/2, padded.rows/2);
        addTextByMat(comImg,watermarkText,center,fontSize);
        cv::Point outer(45, 45);
        addTextByMat(comImg,watermarkText,outer,fontSize);
        //back image
        cv::Mat invDFT;
        idft(comImg, invDFT, cv::DFT_SCALE | cv::DFT_REAL_OUTPUT, 0);
        cv::Mat restoredImage;
        invDFT.convertTo(restoredImage, CV_8U);
        std::vector<cv::Mat> backPlanes;
        split(srcImg, backPlanes);
        backPlanes.erase(backPlanes.begin());
        backPlanes.insert(backPlanes.begin(), restoredImage);
        cv::Mat backImage;
        cv::merge(backPlanes,backImage);
        return backImage;
}

cv::Mat getTextFormMat(cv::Mat backImage) {
        cv::Mat padded=getBlueChannel(backImage);
        padded.convertTo(padded, CV_32F);
        cv::Mat comImg = getDftMat(padded);
        std::vector<cv::Mat> backPlanes;
        // split the comples image in two backPlanes
        cv::split(comImg, backPlanes);
        cv::Mat mag;
        // compute the magnitude
        cv::magnitude(backPlanes[0], backPlanes[1], mag);
        // move to a logarithmic scale
        cv::add(cv::Mat::ones(mag.size(), CV_32F), mag, mag);
        cv::log(mag, mag);
        shiftDFT(mag);
        mag.convertTo(mag, CV_8UC1);
        normalize(mag, mag, 0, 255, cv::NORM_MINMAX, CV_8UC1);
        return mag;
}