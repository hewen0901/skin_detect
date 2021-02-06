#ifndef SKIN_DETECT_PROCESS_H_
#define SKIN_DETECT_PROCESS_H_

#include "export/export.h"
#include <opencv2/core.hpp>

int skinWhite(const char* path, const char* savePath, const float alpha, const float beta, const float satur);

static int img_check_color_(const cv::Mat& image);
static int beauty_img(cv::Mat &src, cv::Mat *dst, int *pBrightness, const float alpha, const float beta, const float satur);

static cv::Mat HSV_detector(cv::Mat& src);
static cv::Mat YCrCb_detect(cv::Mat& src);
static cv::Mat ellipse_detect(cv::Mat& src);
static cv::Mat RGB_detect(cv::Mat& img);

static int get_brightness(const cv::Mat img);
static int paint_img(cv::Mat& mat_roi, const cv::Rect roi, cv::Mat* dst);

static int enhance_img(cv::Mat &img, int *pbrightness, const float alpha = 1.0, const float beta = 45.0);//alpha contrast param, beta brightness param
static int saturat_img(cv::Mat& src, cv::Mat *dst, const float satur = 25.0);



#endif
