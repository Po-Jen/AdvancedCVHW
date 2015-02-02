#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

//#define VISUALIZE

int main(int argc, char *argv[])
{
	/*Load two images*/
	cv::Mat trucka = cv::imread("trucka.bmp", CV_LOAD_IMAGE_GRAYSCALE);
	cv::Mat truckb = cv::imread("truckb.bmp", CV_LOAD_IMAGE_GRAYSCALE);

#ifdef  VISUALIZE
	cv::imshow("truck a", trucka);
	cv::imshow("truck b", truckb);
	cv::waitKey();
#endif
	/* Add black srroundings to truckb */
	//search range = 50

	/* Compute the motion vector by 16x16 Blocks*/
        //Extract all blocks from trucka and save the position of its center
       
        //Match the location for all blocks in truckb
       
      	//Calculate motion vector for all blocks
      
        //Draw motion vector field

	return 0;
}
