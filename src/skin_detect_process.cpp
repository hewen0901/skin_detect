#include "skin_detect_process.h"

#include <algorithm>
#include <iostream>
#include <string>

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>


#define INPUT_IMG_WIDTH  1920
#define INPUT_IMG_HEIGHT 1080
#define BRIGHTNESS_DARK_THRESH  128
#define BRIGHTNESS_LIGHT_THRESH 150

//#define PC_DEBUG

int skinWhite(const char* path, const char* savePath, const float alpha, const float beta, const float satur)
{
    int iret = 0;
    if(nullptr == path || nullptr == savePath)
    {
        return 1;
    }
    char input_path[256], save_path[256];
    memset(input_path, 0, sizeof(input_path));
    memset(save_path, 0, sizeof(save_path));
    sprintf(input_path, "%s", path);
    sprintf(save_path, "%s.jpg", savePath);

    cv::Mat Img = cv::imread(input_path);
    if(!Img.data)
        return 2;
    cv::Mat temp = Img.clone();
    float temp_alpha = alpha;
    float temp_beta = beta;
    float temp_satur = satur;
    int brightness = 0;
    iret = beauty_img(temp, &Img, &brightness, temp_alpha, temp_beta, temp_satur);
    if (iret == 0)
    {
        cv::imwrite(save_path, Img);
    }
    return iret;
}

static int img_check_color_(const cv::Mat& image)
{
    int cnt= 0;
    if(!image.data)
        return -1;

    int row = image.rows;
    int col = image.cols;
    for(int i = 0; i < row;i++)
    {
        for(int j = 0; j<col;j++)
        {
            if(image.channels() ==3)
            {
                int c1 = (int)(image.at<cv::Vec3b>(i,j)[0]);
                int c2 = (int)(image.at<cv::Vec3b>(i,j)[1]);
                int c3 = (int)(image.at<cv::Vec3b>(i,j)[2]);
                if(c1 != c2 && c2 != c3
                        && c1 != 0 &&c1 != 255
                        && c2 != 0 &&c2 != 255)
                {
                    cnt++;
                }
            }
            else if(image.channels() ==1)
            {
                int c = (int)(image.at<uchar>(i,j));
                if(c != 0 &&  c != 255)
                {
                    cnt++;
                }
            }
        }
    }
    if(cnt < 15000 && cnt >= 0)
        return 0;
    else if(cnt > 100000)
        return 1;
}


static int saturat_img(cv::Mat& src, cv::Mat *dst, const float satur)
{
    int iret = 0;
    if(src.empty() == true || dst == nullptr || satur == 0.0)
    {
        printf("input param error\n");
        return -1;
    }
    cv::Mat Img_out;
    src.convertTo(Img_out, CV_32FC3);
    cv::Mat Img_in(src.size(), CV_32FC3);
    src.convertTo(Img_in, CV_32FC3);
    // define the iterator of the input image
    cv::MatIterator_<cv::Vec3f> inp_begin, inp_end;
    inp_begin=Img_in.begin<cv::Vec3f>();
    inp_end =Img_in.end<cv::Vec3f>();

    // define the iterator of the output image
    cv::MatIterator_<cv::Vec3f> out_begin, out_end;
    out_begin=Img_out.begin<cv::Vec3f>();
    out_end =Img_out.end<cv::Vec3f>();

    // increment (-100.0, 100.0)
    float Increment = satur/100.0;

    float delta=0;
    float minVal, maxVal;
    float t1, t2, t3;
    float L,S;
    float alpha;

    for(; inp_begin != inp_end; inp_begin++, out_begin++)
    {
        t1=(*inp_begin)[0];
        t2=(*inp_begin)[1];
        t3=(*inp_begin)[2];

        minVal=std::min(std::min(t1,t2),t3);
        maxVal=std::max(std::max(t1,t2),t3);
        delta=(maxVal-minVal)/255.0;
        L=0.5*(maxVal+minVal)/255.0;
        S=std::max(0.5*delta/L, 0.5*delta/(1-L));
        if (Increment>0)
        {
            alpha = std::max(S, 1-Increment);
            alpha=1.0/alpha-1;
            (*out_begin)[0] = cv::saturate_cast<float>((*inp_begin)[0]+((*inp_begin)[0]-L*255.0)*alpha);
            (*out_begin)[1] = cv::saturate_cast<float>((*inp_begin)[1]+((*inp_begin)[1]-L*255.0)*alpha);
            (*out_begin)[2] = cv::saturate_cast<float>((*inp_begin)[2]+((*inp_begin)[2]-L*255.0)*alpha);
        }
        else
        {
            alpha=Increment;
            (*out_begin)[0] = cv::saturate_cast<float>(L*255.0+((*inp_begin)[0]-L*255.0)*(1+alpha));
            (*out_begin)[1] = cv::saturate_cast<float>(L*255.0+((*inp_begin)[1]-L*255.0)*(1+alpha));
            (*out_begin)[2] = cv::saturate_cast<float>(L*255.0+((*inp_begin)[2]-L*255.0)*(1+alpha));

        }
    }
    //*Img_out *= 255;
    Img_out.convertTo(*dst, CV_8UC3);
#ifdef PC_DEBUG
    cv::imwrite("out.jpg", *Img_out);
#endif
    return iret;
}


static int img_brightness_contrast(cv::Mat& img, float alpha, float beta)//alpha contrast param, beta brightness param
{
    int iret = 0;
    if(img.empty() == true)
    {
        printf("img is null\n");
        return -1;
    }
    for (int y = 0; y < img.rows; y++)
    {
        for (int x = 0; x < img.cols; x++)
        {
            for (int c = 0; c < 3; c++)
            {
                img.at<cv::Vec3b>(y, x)[c] = cv::saturate_cast<uchar>(alpha*(img.at<cv::Vec3b>(y, x)[c]) + beta);
            }
        }
    }
    return iret;
}


static int enhance_img(cv::Mat &img, int* pbrightness, const float alpha, const float beta)
{
    int iret = 0;
    if(img.empty() == true || pbrightness == nullptr)
    {
        return -1;
    }
    cv::Mat dst;
    int iBrightness = 0;
    float temp_alpha = alpha;
    float temp_beta = beta;
    int iflag = 1, icount = 1;
    while(iflag)
    {
        iBrightness = get_brightness(img);
        *pbrightness = iBrightness;
        if(iBrightness < BRIGHTNESS_DARK_THRESH)
        {
            int ibias = BRIGHTNESS_DARK_THRESH - iBrightness;
            if(ibias > 30)
            {
                temp_alpha = temp_alpha + icount*0.2;
                temp_beta = temp_beta + icount*5.0;
            }
            iret = img_brightness_contrast(img, temp_alpha + icount*0.1, temp_beta + icount*3.0);
#ifdef PC_DEBUG
            cv::imwrite("./img_brightness_contrast.jpg", img);
#endif
            if(img.type() == CV_8UC3)
            {
                cv::bilateralFilter(img, dst, 15, 50, 12.5);
                dst.copyTo(img);
#ifdef PC_DEBUG
                cv::imwrite("./bilateralFilter.jpg", dst);
#endif
            }
        }
        else/* if(iBrightness > BRIGHTNESS_LIGHT_THRESH || icount > 3)*/
        {
            break;
        }
        icount++;
    }
    return iret;
}


static int beauty_img(cv::Mat &src, cv::Mat *dst, int *pBrightness, const float alpha, const float beta, const float satur)
{
    int iret = 0;
    if(src.empty() == true || dst == nullptr || src.cols != INPUT_IMG_WIDTH || src.rows != INPUT_IMG_HEIGHT)
    {
        printf("input img is null\n");
        return 1;
    }
    cv::Mat img(src.size(), CV_8UC3);
#ifdef IOS
    cv::cvtColor(src, img, CV_BGRA2BGR);
#else
    src.copyTo(img);
#endif
    int icolor = img_check_color_(img);
    if(icolor == 0)
    {
        return 3;
    }
    int** iBrightness = &pBrightness;;
    if(enhance_img(img, *iBrightness, alpha, beta) != 0)
    {
        printf("enhance_img error\n");
        return 4;
    }
    if(saturat_img(img, dst, satur) != 0)
    {
         printf("saturat_img error\n");
        return 5;
    }
#ifdef PC_DEBUG
    cv::imwrite("./0saturat_img.jpg", *dst);
#endif
    return iret;
}

static int paint_img(cv::Mat& mat_roi, const cv::Rect roi, cv::Mat* dst)
{
    int iret = 0;
    if(mat_roi.empty() == true || dst == nullptr)
    {
        printf("input is null\n");
        return -1;
    }
    cv::Rect rect;
    rect.x = roi.x;
    rect.y = roi.y;
    rect.width = roi.width;
    rect.height = roi.height;
    for (int i = 0; i < mat_roi.rows; i++)
    {
        uchar* pRoiData = mat_roi.ptr<uchar>(i);
        uchar* pDstData = dst->ptr<uchar>(i + rect.y);
        for (int j = 0; j < mat_roi.cols; j++)
        {
            pDstData[3 * (j + rect.x) + 0] = pRoiData[3 * j + 0];
            pDstData[3 * (j + rect.x) + 1] = pRoiData[3 * j + 1];
            pDstData[3 * (j + rect.x) + 2] = pRoiData[3 * j + 2];
        }
    }
    return iret;
}

static int get_brightness(const cv::Mat img)
{
    if(img.empty() == true)
        return -1;
    int ibrightness = 0;
    cv::Mat hsv;
    cv::cvtColor(img, hsv, CV_BGR2HSV);
    cv::Scalar cs = cv::mean(img);
#if 0
    double max1 = max(cs.val[0],cs.val[1]), min1=min(cs.val[0],cs.val[1]);
    ibrightness = (max(max1,cs.val[2]) + min(min1,cs.val[2]))/2;
#else
    ibrightness = cs[2];
#endif
    return ibrightness;
}
