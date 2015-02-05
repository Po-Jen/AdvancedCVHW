#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>

//#define VISUALIZE

struct block
{
	cv::Mat subImg;
	int x;
	int y;	
};

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
	cv::Mat fatTruckb;
	int searchRange = 51; //Now can only support odd serach range
	int top, bottom, left, right;
	top = bottom = left = right = (searchRange-1)/2;
	copyMakeBorder( truckb, fatTruckb, top, bottom, left, right, cv::BORDER_CONSTANT, 0);

#ifdef  VISUALIZE
	cv::imshow("fat truck b", fatTruckb);
	cv::waitKey();
#endif 	

	/* Compute the motion vector by 16x16 Blocks*/
    //Extract all blocks from trucka and save the position of its center
 	int row = trucka.rows;
 	int col = trucka.cols;
	int blockSize = 31;
	int rowBlockNum = row/blockSize;
	int colBlockNum = col/blockSize;
	
	std::vector<block> blocks;
	block b;
	cv::Mat tmpImg = cv::Mat::zeros(blockSize, blockSize, CV_8U);

	for(int i=0; i<rowBlockNum; i++)
	{
		for(int j=0; j<colBlockNum; j++)
		{
			for(int blocki=i*blockSize; blocki<(i*blockSize)+blockSize; blocki++)
			{
				for(int blockj=j*blockSize; blockj<(j*blockSize)+blockSize; blockj++)
				{
					tmpImg.at<uchar>(blocki-(i*blockSize),blockj-(j*blockSize)) = trucka.at<uchar>(blocki, blockj); 	
				}
			}
			b.subImg = tmpImg.clone();
			b.x = i*blockSize + blockSize/2;
			b.y = j*blockSize + blockSize/2;
			blocks.push_back(b);
		}
	}

#ifdef VISUALIZE
	std::string s="block";
	std::stringstream num;
	for(int i=0; i<blocks.size(); i++)
	{	
		std::cout << blocks[i].x << " " << blocks[i].y << std::endl;
		num << i;
		cv::imshow(s+num.str(), blocks[i].subImg);
	}
	cv::waitKey();
#endif

    //Match the location for all blocks in truckb
 	   
    //Calculate motion vector for all blocks
      
    //Draw motion vector field

	return 0;
}
