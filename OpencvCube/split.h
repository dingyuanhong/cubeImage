#ifndef _SPLIT_H_
#define _SPLIT_H_

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
using namespace cv;


void createCubeMapFace(const Mat &in, Mat &face, int faceId = 0, const int width = -1, const int height = -1);




#endif