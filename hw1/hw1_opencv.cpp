#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <stdlib.h>

//#define VISUALIZE

struct block
{
	cv::Mat subImg;
	/*
	int origin_x;
	int origin_y;
	int motion_x;
	int motion_y;
	*/
	cv::Point2f original_pos;
	cv::Point2f motion_pos;
};

double calculateDistance(const cv::Mat subImg, const cv::Mat testImg, const int blockSize)
{
	double distance=0;

	for(int i=0; i<blockSize; i++)
		for(int j=0; j<blockSize; j++)
			distance += std::abs((int)subImg.at<uchar>(i,j)-(int)testImg.at<uchar>(i,j));
			
	return distance/blockSize;
}

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
	top = bottom = left = right = searchRange-1;
	copyMakeBorder( truckb, fatTruckb, top, bottom, left, right, cv::BORDER_CONSTANT, 0);

#ifdef  VISUALIZE
	cv::imshow("fat truck b", fatTruckb);
	std::cout << top << " " << left << " " << bottom << " " << right << std::endl;
	cv::waitKey();
#endif 	

	/* Compute the motion vector by NxN Blocks*/
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
			b.original_pos.x = i*blockSize + blockSize/2;
			b.original_pos.y = j*blockSize + blockSize/2;
			blocks.push_back(b);
		}
	}

#ifdef VISUALIZE
	std::string s="block";
	std::stringstream num;
	for(int i=0; i<blocks.size(); i++)
	{	
		std::cout << blocks[i].original_pos.x << " " << blocks[i].original_pos.y << std::endl;
		num << i;
		cv::imshow(s+num.str(), blocks[i].subImg);
	}
	cv::waitKey();
#endif

    //Match the location for all blocks in fatTruckb
	int fatb_x, fatb_y;
	for(int index=0; index < blocks.size(); index++)
	{
		//get the center position of subImg in truckb
		fatb_x = blocks[index].original_pos.x+searchRange-1;
		fatb_y = blocks[index].original_pos.y+searchRange-1;
		
		double distance = 1000000;
		int halfRange = (searchRange-1)/2;

		for(int searchX =fatb_x-halfRange; searchX < fatb_x+halfRange; searchX++)
		{
			for(int searchY =fatb_y-halfRange; searchY < fatb_y+halfRange; searchY++)
			{
				//Extract small area on fatTruckb for comparison
				cv::Mat testImg = cv::Mat::zeros(blockSize, blockSize, CV_8U);
				cv::Rect rect(searchX-(blockSize/2), searchY-(blockSize/2), blockSize, blockSize);
				testImg = fatTruckb(rect); //TO DEBUG

#ifdef VISUALIZE
//#if 1
				if(index == 6)
				{	
					std::string s="block";
					std::stringstream num;
					num << index;
					cv::imshow(s+num.str(), blocks[index].subImg);
					cv::imshow("testImg", testImg);
					cv::waitKey();
				}
#endif

				//Calculate the position that subImg mostly matches fatTruckb
				if(calculateDistance(blocks[index].subImg, testImg, blockSize) < distance)
				{
					blocks[index].motion_pos.x = searchX-searchRange+1;
					blocks[index].motion_pos.y = searchY-searchRange+1;
				}
			}
		}
	}	
	
    //Calculate motion vector for all blocks
	for(int i=0; i<blocks.size(); i++)
	{
		//Draw original position(black)
		cv::circle( truckb, blocks[i].original_pos, 3, cv::Scalar(0), -1, 8);
		
		//Draw Motion position(white)
		cv::circle( truckb, blocks[i].motion_pos, 3, cv::Scalar(255), -1, 8);
	}	

    //Draw motion vector field
	for(int i=0; i<blocks.size(); i++)
		cv::line( truckb, blocks[i].original_pos, blocks[i].motion_pos, cv::Scalar(255), 1, CV_AA, 0);
	
	cv::imshow("truckb with dot", truckb);
	cv::waitKey();
	
	return 0;
}
