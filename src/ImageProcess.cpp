#include <opencv2/opencv.hpp>

#include "ImageProcess.h"
#include "ImageProcess.hpp"

#ifdef ANDROID

IMAGE_PROCESS_API jstring Java_com_crowxr_app_MainActivity_stringFromJNI(JNIEnv* env, jobject)
{
    auto hello = std::string("Hello from C++");
    return env->NewStringUTF(hello.c_str());
}

#else

IMAGE_PROCESS_API void crop(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height) { CrowXR::crop(pSrcBitmap, pDstBitmap, input_width, input_height, roi_x, roi_y, roi_width, roi_height); }
IMAGE_PROCESS_API void rescale(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height) { CrowXR::rescale(pSrcBitmap, pDstBitmap, input_width, input_height, output_width, output_height); }
IMAGE_PROCESS_API void hconcate(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height) { CrowXR::hconcate(pSrcBitmapL, pSrcBitmapR, pDstBitmap, left_width, right_width, input_height); }

#endif

namespace CrowXR {
    void crop(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height) {
        auto srcFrame = cv::Mat(input_height, input_width, CV_8UC4, pSrcBitmap);
        auto dstFrame = cv::Mat(roi_height, roi_width, CV_8UC4, pDstBitmap);
        srcFrame(cv::Rect(roi_x, roi_y, roi_width, roi_height)).copyTo(dstFrame);
    }

    void rescale(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height) {
        auto srcFrame = cv::Mat(input_height, input_width, CV_8UC4, pSrcBitmap);
        auto dstFrame = cv::Mat(output_height, output_width, CV_8UC4, pDstBitmap);
        cv::resize(srcFrame, dstFrame, dstFrame.size(), 0, 0, cv::INTER_NEAREST_EXACT);
    }

    void hconcate(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height) {
        auto srcFrameL = cv::Mat(input_height, left_width, CV_8UC4, pSrcBitmapL);
        auto srcFrameR = cv::Mat(input_height, right_width, CV_8UC4, pSrcBitmapR);
        auto dstFrame = cv::Mat(input_height, left_width + right_width, CV_8UC4, pDstBitmap);
        auto tmpFrame = cv::Mat(dstFrame.size(), CV_8UC4);
        cv::hconcat(srcFrameL, srcFrameR, dstFrame);
    }
}
