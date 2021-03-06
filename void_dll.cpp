// void_dll.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"


#include <opencv2/opencv.hpp>
using namespace cv;
#pragma comment(lib, "opencv_world331.lib")

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <mutex>
using namespace std;


#define DLLEXPORT extern "C" __declspec(dllexport)


map<size_t, char*> ids;
mutex rw_ids;

DLLEXPORT int get_img_from_char_array(char *c, int *width, int *height)
{
	Mat frame_content(*height, *width, CV_8UC1, c);

	return 0;
}


DLLEXPORT char *alloc_img(const char *file_name, int *width, int *height, size_t *image_id)
{
	rw_ids.lock();

	Mat img = imread(file_name, IMREAD_GRAYSCALE);

	if (img.empty())
	{
		*width = 0;
		*height = 0;
		return nullptr;
	}

	*width = img.cols;
	*height = img.rows;

	size_t num_pixels = (*width)*(*height);

	char *p = new char[num_pixels];
	memcpy(p, img.data, num_pixels*sizeof(char));

	ids[ids.size()] = p;
	*image_id = ids.size() - 1;

	cout << "Created img id " << *image_id << endl;

	rw_ids.unlock();

	return p;
}

DLLEXPORT int free_img(size_t *image_id)
{
	rw_ids.lock();

	for (map<size_t, char*>::const_iterator ci = ids.begin(); ci != ids.end(); ci++)
	{
		if (ci->first == *image_id)
		{
			cout << "Deleted img id " << *image_id << endl;

			if (nullptr != ci->second)
				delete[] ci->second;

			ids.erase(ci);

			rw_ids.unlock();
			return 0;
		}
	}

	rw_ids.unlock();

	return 1;
}
