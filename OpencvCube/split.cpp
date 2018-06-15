#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <math.h>
#include "split.h"
using namespace cv;

const float _PI = 3.1415926;
// Define our six cube faces.
// 0 - 3 are side faces, clockwise order
// 4 and 5 are top and bottom, respectively
float faceTransform[6][2] =
{
    {0, 0},
    {_PI / 2, 0},
    {_PI, 0},
    {-_PI / 2, 0},
    {0, -_PI / 2},
    {0, _PI / 2}
};

// Map a part of the equirectangular panorama (in) to a cube face
// (face). The ID of the face is given by faceId. The desired
// width and height are given by width and height.
//inline void createCubeMapFace(const Mat &in, Mat &face, int faceId = 0, const int width = -1, const int height = -1)
void createCubeMapFace(const Mat &in, Mat &face, int faceId, const int width, const int height)
{

    float inWidth = in.cols;
    float inHeight = in.rows;

    // Allocate map
    Mat mapx(height, width, CV_32F);
    Mat mapy(height, width, CV_32F);

    // Calculate adjacent (ak) and opposite (an) of the
    // triangle that is spanned from the sphere center
    //to our cube face.
    const float an = sin(_PI / 4);
    const float ak = cos(_PI / 4);

    const float ftu = faceTransform[faceId][0];
    const float ftv = faceTransform[faceId][1];

    // For each point in the target image,
    // calculate the corresponding source coordinates.
    for(int y = 0; y < height; y++) {
        for(int x = 0; x < width; x++) {

            // Map face pixel coordinates to [-1, 1] on plane
            float nx = (float)y / (float)height - 0.5f;
            float ny = (float)x / (float)width - 0.5f;

            nx *= 2;
            ny *= 2;

            // Map [-1, 1] plane coords to [-an, an]
            // thats the coordinates in respect to a unit sphere
            // that contains our box.
            nx *= an;
            ny *= an;

            float u, v;

            // Project from plane to sphere surface.
            if(ftv == 0) {
                // Center faces
                u = atan2(nx, ak);
                v = atan2(ny * cos(u), ak);
                u += ftu;
            } else if(ftv > 0) {
                // Bottom face
                float d = sqrt(nx * nx + ny * ny);
                v = _PI / 2 - atan2(d, ak);
                u = atan2(ny, nx);
            } else {
                // Top face
                float d = sqrt(nx * nx + ny * ny);
                v = -_PI / 2 + atan2(d, ak);
                u = atan2(-ny, nx);
            }

            // Map from angular coordinates to [-1, 1], respectively.
            u = u / (_PI);
            v = v / (_PI / 2);

            // Warp around, if our coordinates are out of bounds.
            while (v < -1) {
                v += 2;
                u += 1;
            }
            while (v > 1) {
                v -= 2;
                u += 1;
            }

            while(u < -1) {
                u += 2;
            }
            while(u > 1) {
                u -= 2;
            }

            // Map from [-1, 1] to in texture space
            u = u / 2.0f + 0.5f;
            v = v / 2.0f + 0.5f;

            u = u * (inWidth - 1);
            v = v * (inHeight - 1);

            // Save the result for this pixel in map
            mapx.at<float>(x, y) = u;
            mapy.at<float>(x, y) = v;
        }
    }

    // Recreate output image if it has wrong size or type.
    if(face.cols != width || face.rows != height ||
        face.type() != in.type()) {
        face = Mat(width, height, in.type());
    }

    // Do actual resampling using OpenCV's remap
	if (faceId == 4)
	{
		Mat mox;
		Mat moy;
		transpose(mapx, mox);
		flip(mox, mox, 1);

		//×ªÖÃ
		transpose(mapy, moy);
		//×óÓÒµßµ¹
		flip(moy, moy, 1);

		remap(in, face, mox, moy,
			CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	}
	else if (faceId == 5)
	{
		Mat mox;
		Mat moy;
		//×ªÖÃ
		transpose(mapx, mox);
		//ÉÏÏÂµßµ¹
		flip(mox, mox, 0);

		transpose(mapy, moy);
		flip(moy, moy, 0);


		remap(in, face, mox, moy,
			CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	}
	else
	{
		// Do actual resampling using OpenCV's remap
		remap(in, face, mapx, mapy,
			CV_INTER_LINEAR, BORDER_CONSTANT, Scalar(0, 0, 0));
	}
}
