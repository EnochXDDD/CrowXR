#include "ImageProcess.h"
#include "ImageProcess.hpp"

#ifdef ANDROID

IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_crop(JNIEnv* env, jobject, jbyteArray srcBitmap, jbyteArray dstBitmap, jint input_width, jint input_height, jint roi_x, jint roi_y, jint roi_width, jint roi_height) {
    auto pSrcBitmap = env->GetByteArrayElements(srcBitmap, nullptr);
    auto pDstBitmap = env->GetByteArrayElements(dstBitmap, nullptr);
    CrowXR::crop(reinterpret_cast<unsigned char*>(pSrcBitmap), reinterpret_cast<unsigned char*>(pDstBitmap), input_width, input_height, roi_x, roi_y, roi_width, roi_height);
    env->ReleaseByteArrayElements(srcBitmap, reinterpret_cast<jbyte*>(pSrcBitmap), 0);
    env->ReleaseByteArrayElements(dstBitmap, reinterpret_cast<jbyte*>(pDstBitmap), 0);
}

IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_rescale(JNIEnv* env, jobject, jbyteArray srcBitmap, jbyteArray dstBitmap, jint input_width, jint input_height, jint output_width, jint output_height) {
    auto pSrcBitmap = env->GetByteArrayElements(srcBitmap, nullptr);
    auto pDstBitmap = env->GetByteArrayElements(dstBitmap, nullptr);
    CrowXR::rescale(reinterpret_cast<unsigned char*>(pSrcBitmap), reinterpret_cast<unsigned char*>(pDstBitmap), input_width, input_height, output_width, output_height);
    env->ReleaseByteArrayElements(srcBitmap, reinterpret_cast<jbyte*>(pSrcBitmap), 0);
    env->ReleaseByteArrayElements(dstBitmap, reinterpret_cast<jbyte*>(pDstBitmap), 0);
}

IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_hconcate(JNIEnv* env, jobject, jbyteArray srcBitmapL, jbyteArray srcBitmapR, jbyteArray dstBitmap, jint left_width, jint right_width, jint input_height) {
    auto pSrcBitmapL = env->GetByteArrayElements(srcBitmapL, nullptr);
    auto pSrcBitmapR = env->GetByteArrayElements(srcBitmapR, nullptr);
    auto pDstBitmap = env->GetByteArrayElements(dstBitmap, nullptr);
    CrowXR::hconcate(reinterpret_cast<unsigned char*>(pSrcBitmapL), reinterpret_cast<unsigned char*>(pSrcBitmapR), reinterpret_cast<unsigned char*>(pDstBitmap), left_width, right_width, input_height);
    env->ReleaseByteArrayElements(srcBitmapL, reinterpret_cast<jbyte*>(pSrcBitmapL), 0);
    env->ReleaseByteArrayElements(srcBitmapR, reinterpret_cast<jbyte*>(pSrcBitmapR), 0);
    env->ReleaseByteArrayElements(dstBitmap, reinterpret_cast<jbyte*>(pDstBitmap), 0);
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
