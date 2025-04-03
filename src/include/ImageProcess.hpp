#pragma once

namespace CrowXR {
    void crop(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height);
    void rescale(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height);
    void hconcate(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height);
}