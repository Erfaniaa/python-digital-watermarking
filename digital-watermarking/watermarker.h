#ifndef _WATERMARKER_H
#define _WATERMARKER_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

void shiftDFT(cv::Mat);
cv::Mat getBlueChannel(cv::Mat);
cv::Mat getDftMat(cv::Mat);
void addTextByMat(cv::Mat, cv::String, cv::Point, double);
extern "C" cv::Mat transFormMatWithText(cv::Mat, cv::String, double);
extern "C" cv::Mat getTextFormMat(cv::Mat);

#endif