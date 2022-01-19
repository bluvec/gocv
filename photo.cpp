#include "photo.h"

void ColorChange(Mat src, Mat mask, Mat dst, float red_mul, float green_mul, float blue_mul) {
    cv::colorChange(*src, *mask, *dst, red_mul, green_mul, blue_mul);
}

void IlluminationChange(Mat src, Mat mask, Mat dst, float alpha, float beta) {
    cv::illuminationChange(*src, *mask, *dst, alpha, beta);
}

void SeamlessClone(Mat src, Mat dst, Mat mask, Point p, Mat blend, int flags) {
    cv::Point pt(p.x, p.y);
    cv::seamlessClone(*src, *dst, *mask, pt, *blend, flags);
}

void TextureFlattening(Mat src, Mat mask, Mat dst, float low_threshold, float high_threshold, int kernel_size) {
    cv::textureFlattening(*src, *mask, *dst, low_threshold, high_threshold, kernel_size);
}


void FastNlMeansDenoisingColoredMulti(	struct Mats src, Mat dst, int imgToDenoiseIndex, int 	temporalWindowSize){
  std::vector<cv::Mat> images;
  for (int i = 0; i < src.length; ++i) {
    images.push_back(*src.mats[i]);
  }
  cv::fastNlMeansDenoisingColoredMulti( images, *dst, imgToDenoiseIndex, 	temporalWindowSize );
}

void FastNlMeansDenoisingColoredMultiWithParams( struct Mats src, Mat dst, int imgToDenoiseIndex, int 	temporalWindowSize, float 	h, float 	hColor, int 	templateWindowSize, int 	searchWindowSize ){
  std::vector<cv::Mat> images;
  for (int i = 0; i < src.length; ++i) {
    images.push_back(*src.mats[i]);
  }
  cv::fastNlMeansDenoisingColoredMulti( images, *dst, imgToDenoiseIndex, 	temporalWindowSize, h, hColor, templateWindowSize, searchWindowSize );
}

MergeMertens MergeMertens_Create() {
  return new cv::Ptr<cv::MergeMertens>(cv::createMergeMertens());
}

MergeMertens MergeMertens_CreateWithParams(float contrast_weight,
                                           float saturation_weight,
                                           float exposure_weight) {
  return new cv::Ptr<cv::MergeMertens>(cv::createMergeMertens(
      contrast_weight, saturation_weight, exposure_weight));
}

void MergeMertens_Close(MergeMertens b) {
  delete b;
}

void MergeMertens_Process(MergeMertens b, struct Mats src, Mat dst) {
  std::vector<cv::Mat> images;
  for (int i = 0; i < src.length; ++i) {
    images.push_back(*src.mats[i]);
  }
  (*b)->process(images, *dst);
}

AlignMTB AlignMTB_Create() {
  return new cv::Ptr<cv::AlignMTB>(cv::createAlignMTB(6,4,false));
}

AlignMTB AlignMTB_CreateWithParams(int max_bits, int exclude_range, bool cut) {
  return new cv::Ptr<cv::AlignMTB>(
      cv::createAlignMTB(max_bits, exclude_range, cut));
}

void AlignMTB_Close(AlignMTB b) { delete b; }

void AlignMTB_Process(AlignMTB b, struct Mats src, struct Mats *dst) {

  std::vector<cv::Mat> srcMats;
  for (int i = 0; i < src.length; ++i) {
    srcMats.push_back(*src.mats[i]);
  }

  std::vector<cv::Mat> dstMats;
  (*b)->process(srcMats, dstMats);

  dst->mats = new Mat[dstMats.size()];
  for (size_t i = 0; i < dstMats.size() ; ++i) {
	dst->mats[i] = new cv::Mat( dstMats[i] );
  }
  dst->length = (int)dstMats.size();
}
