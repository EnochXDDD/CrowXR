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
    IMAGE_PROCESS_API jstring Java_com_enochxr_streamer_MainActivity_stringFromJNI(JNIEnv* env, jobject);
#else
    IMAGE_PROCESS_API void crop(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height);
    IMAGE_PROCESS_API void rescale(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height);
    IMAGE_PROCESS_API void hconcate(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height);
#endif
