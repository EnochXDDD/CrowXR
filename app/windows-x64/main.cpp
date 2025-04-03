#include <windows.h>

#include <functional>
#include <memory>

#include <opencv2/opencv.hpp>

class CrowXR {
  public:
    void(*CrowXRCrop)(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int roi_x, int roi_y, int roi_width, int roi_height) = nullptr;
    void(*CrowXRRescale)(unsigned char* pSrcBitmap, unsigned char* pDstBitmap, int input_width, int input_height, int output_width, int output_height) = nullptr;
    void(*CrowXRHConcate)(unsigned char* pSrcBitmapL, unsigned char* pSrcBitmapR, unsigned char* pDstBitmap, int left_width, int right_width, int input_height) = nullptr;

    CrowXR() {
      auto CrowXR = LoadLibrary(TEXT("CrowXR.dll"));
      m_phCrowXR = std::unique_ptr<HMODULE, std::function<void(HMODULE*)>>(&CrowXR, [](HMODULE* hModule){ FreeLibrary(*hModule); });
      CrowXRCrop = reinterpret_cast<decltype(CrowXRCrop)>(GetProcAddress(*m_phCrowXR, "crop"));
      CrowXRRescale = reinterpret_cast<decltype(CrowXRRescale)>(GetProcAddress(*m_phCrowXR, "rescale"));
      CrowXRHConcate = reinterpret_cast<decltype(CrowXRHConcate)>(GetProcAddress(*m_phCrowXR, "hconcate"));
    }
    
    ~CrowXR() {
      m_phCrowXR.reset();
    }

  private:
    HMODULE m_CrowXR = nullptr;
    std::unique_ptr<HMODULE, std::function<void(HMODULE*)>> m_phCrowXR = nullptr;
};

int main()
{
  int input_width = 1280;
  int input_height = 720;
  int roi_left_x = 0;
  int roi_left_y = 240;
  int roi_right_x = 640;
  int roi_right_y = 240;
  int roi_width = 640;
  int roi_height = 480;
  int rescale_width = 1024;
  int rescale_height = 768;
  
  auto crowxr = std::make_unique<CrowXR>();
  auto cap = std::make_unique<cv::VideoCapture>();
  
  cap->open(0, cv::CAP_DSHOW);
  cap->set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
  cap->set(cv::CAP_PROP_FRAME_WIDTH, input_width);
  cap->set(cv::CAP_PROP_FRAME_HEIGHT, input_height);
  cap->set(cv::CAP_PROP_FPS, 30);
  
  auto srcFrame = cv::Mat(input_height, input_width, CV_8UC3);
  auto tmpFrame = cv::Mat(input_height, input_width, CV_8UC4);
  auto cropFrame = cv::Mat(roi_height, roi_width, CV_8UC4);
  auto rescaleLeftFrame = cv::Mat(rescale_height, rescale_width, CV_8UC4);
  auto rescaleRightFrame = cv::Mat(rescale_height, rescale_width, CV_8UC4);
  auto dstFrame = cv::Mat(rescale_height, rescale_width << 1, CV_8UC4);

  while (cap->isOpened()) {
    *cap >> srcFrame;
    
    cv::cvtColor(srcFrame, tmpFrame, cv::COLOR_BGR2BGRA);
    
    crowxr->CrowXRCrop(tmpFrame.data, cropFrame.ptr(), input_width, input_height, roi_left_x, roi_left_y, roi_width, roi_height);
    crowxr->CrowXRRescale(cropFrame.data, rescaleLeftFrame.ptr(), roi_width, roi_height, rescale_width, rescale_height);

    crowxr->CrowXRCrop(tmpFrame.data, cropFrame.ptr(), input_width, input_height, roi_right_x, roi_right_y, roi_width, roi_height);
    crowxr->CrowXRRescale(cropFrame.data, rescaleRightFrame.ptr(), roi_width, roi_height, rescale_width, rescale_height);

    crowxr->CrowXRHConcate(rescaleLeftFrame.data, rescaleRightFrame.data, dstFrame.ptr(), rescale_width, rescale_width, rescale_height);

    cv::imshow("App", dstFrame);
    if (cv::waitKey(1) >= 0) {
      break;
    }
  }

  return 0;
}
