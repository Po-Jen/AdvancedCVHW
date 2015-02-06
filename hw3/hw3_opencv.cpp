#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"
#include <vector>

int main(int argc, char* argv[])
{
	/*Load two images*/
    cv::Mat trucka = cv::imread("trucka.bmp", CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat truckb = cv::imread("truckb.bmp", CV_LOAD_IMAGE_GRAYSCALE);

	/* Track feature points by calculating optical flow */
	//void calcOpticalFlowPyrLK(InputArray prevImg, InputArray nextImg, InputArray prevPts, InputOutputArray nextPts, 
    //                          OutputArray status, OutputArray err, Size winSize=Size(21,21), int maxLevel=3, 
    //                          TermCriteria criteria=TermCriteria(TermCriteria::COUNT+TermCriteria::EPS, 30, 0.01), 
    //                          int flags=0, double minEigThreshold=1e-4 )
	//TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS,20,0.03);
	std::vector<cv::Point2f> points[2];
	std::vector<uchar> status;
    std::vector<float> err;
	cv::goodFeaturesToTrack(trucka, points[0], 500, 0.01, 10, cv::Mat(), 3, 0, 0.04);
	cv::calcOpticalFlowPyrLK(trucka, truckb, points[0], points[1], status, err);

	/* Draw the flow on the result image */	
	cv::Mat result = truckb.clone();	
    for(int  i=0; i<points[1].size(); i++ )
    {
#if 0
        cv::circle( result, points[1][i], 3, cv::Scalar(0,255,0), -1, 8);
        cv::circle( result, points[0][i], 3, cv::Scalar(255,255,255), -1, 8);
#endif
		cv::line(result, points[0][i], points[1][i], cv::Scalar(255,255,255), 2, CV_AA, 0);    
	}
	
	cv::imshow("Optical Flow", result);
	cv::waitKey();

	return 0;
}
