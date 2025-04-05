#pragma once

#ifdef ANDROID
    #include <jni.h>
    #define IMAGE_PROCESS_API extern "C" JNIEXPORT JNICALL
#elif WIN32
    #include <windows.h>
    #define IMAGE_PROCESS_API extern "C" __declspec(dllexport)
#else
    #define IMAGE_PROCESS_API extern "C"
#endif

#ifdef ANDROID
    IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_crop(JNIEnv* env, jobject, jbyteArray srcBitmap, jbyteArray dstBitmap, jint input_width, jint input_height, jint roi_x, jint roi_y, jint roi_width, jint roi_height);
    IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_rescale(JNIEnv* env, jobject, jbyteArray srcBitmap, jbyteArray dstBitmap, jint input_width, jint input_height, jint output_width, jint output_height);
    IMAGE_PROCESS_API void Java_com_crowxr_app_MainActivity_hconcate(JNIEnv* env, jobject, jbyteArray srcBitmapL, jbyteArray srcBitmapR, jbyteArray dstBitmap, jint left_width, jint right_width, jint input_height);
#else
    IMAGE_PROCESS_API void crop(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height);
    IMAGE_PROCESS_API void rescale(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height);
    IMAGE_PROCESS_API void hconcate(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height);
#endif
