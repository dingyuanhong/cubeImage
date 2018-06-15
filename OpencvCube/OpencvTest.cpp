#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>

#ifdef _WIN32
#include <Windows.h>//为了在一个窗口显示多幅图像而加的头文件
#pragma comment(lib,"opencv_world341d.lib")
#endif

#include "split.h"

using namespace cv;
using namespace std;

//https://www.imooc.com/article/14196


#define ArgBegin() for (int i = 1; i < argc; i++){
#define ArgGet(CMD,VALUE)	if (strcmp(argv[i],(CMD)) == 0){if(i < argc-1){ (VALUE) = argv[++i];continue;}}
#define ArgGetInt(CMD,VALUE) if (strcmp(argv[i],(CMD)) == 0){if(i < argc-1){(VALUE) = atoi(argv[++i]);continue;}}
#define ArgGetBool(CMD,VALUE) if (strcmp(argv[i],(CMD)) == 0){(VALUE) = true;continue;}
#define ArgElse() else
#define ArgEnd() }

#define LOGE printf
#define LOGD printf
#define LOGI printf

int main(int argc, char **argv)
{
	char* filename = "";
	int index = -1;
	int width = 0;
	int height = 0;
	char * outPath = "";
	char * outName = "";
	char * outExt = "";
	bool help = false;

	ArgBegin()
		ArgGet("-f", filename)
		ArgGetInt("-i", index)
		ArgGetInt("-w", width)
		ArgGetInt("-h", height)
		ArgGet("-op", outPath)
		ArgGet("-on", outName)
		ArgGet("-oe", outExt)
		ArgGetBool("-h", help)
		ArgGetBool("--help", help)
		ArgGetBool("help", help)
	ArgEnd();

	if (help)
	{
		LOGI("Usage:\n");
		LOGI("	help 帮助\n");
		LOGI("	--help 帮助\n");
		LOGI("	-f 文件全路径\n");
		LOGI("	-i 索引(0-5)\n");
		LOGI("	-w 输出图片宽\n");
		LOGI("	-h 输出图片高\n");
		LOGI("	-op 输出文件目录\n");
		LOGI("	-on 输出文件名\n");
		LOGI("	-oe 输出文件后缀(.png/.jpg/.bmp)\n");
		return 0;
	}

	Mat srcImg = imread(filename, CV_LOAD_IMAGE_COLOR);//输入图像
	if (srcImg.empty()) {
		LOGE("input error.(\"%s\")\n",filename);
		return -1;
	}
	
	if (width <= 0 || height <= 0)
	{
		LOGE("output w/h error.(w:%d h:%d)\n", width,height);
		return -1;
	}

	char * list[6] = {
		"front",
		"right",
		"back",
		"left",
		"top",
		"bottom"
	};

	
	for (int i=0; i<6; i++)
	{
		if (index != -1 && i != index) continue;
		Mat dstImg;
		createCubeMapFace(srcImg, dstImg, i, width, height);
		
		string outFile;
		outFile += outPath;
		if (outFile.length() > 0)
		{
			outFile += "/";
		}
		outFile += outName;
		outFile += ".";
		outFile += list[i];
		outFile += outExt;

		imwrite(outFile,dstImg);
	}

	return 0;
}