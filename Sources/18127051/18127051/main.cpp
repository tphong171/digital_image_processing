#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "Converter.h"
#include "ColorTransformer.h"
using namespace cv;
using namespace std;
int main(int argc, char** argv)
{
	if (argc <=1)
	{
		cout << "-> Please run by command line and follow the guide in /Document/Report.pdf";
		cin.get();
		return -1;
	}

	Converter* converter = new Converter();
	ColorTransformer* transformer = new ColorTransformer();
	
	if (strcmp(argv[1], "-rgb2gray") == 0)
	{
		Mat sourceImage;
		Mat destinationImage;
		sourceImage = imread(argv[2]);
		if (converter->Convert(sourceImage,destinationImage,0) == 0)
		{
			imwrite(argv[3], destinationImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("RGB2Gray Before", sourceImage);
			//imshow("RGB2Gray After", destinationImage);
			//waitKey(0);
		}
		else cout << "RGB2Gray Fail!";
	}
	else if (strcmp(argv[1], "-rgb2hsv") == 0)
	{
		Mat sourceImage;
		Mat destinationImage;
		sourceImage = imread(argv[2]);
		if (converter->Convert(sourceImage, destinationImage, 1) == 0)
		{
			imwrite(argv[3], destinationImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("RGB2HSV Before", sourceImage);
			//imshow("RGB2HSV After", destinationImage);
			//waitKey(0);
		}
		else cout << "RGB2HSV Fail!";
	}
	else if (strcmp(argv[1], "-bright") == 0)
	{
		Mat sourceImage;
		Mat destinationImage;
		sourceImage = imread(argv[2]);

		stringstream str;
		str << argv[4];
		short bright = 0;
		str >> bright;
		
		if (transformer->ChangeBrighness(sourceImage, destinationImage, bright) == 1)
		{
			imwrite(argv[3], destinationImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("Brightness Before", sourceImage);
			//imshow("Brightness After", destinationImage);
			//waitKey(0);
		}
		else cout << "Change Bight Fail!";
	}
	else if (strcmp(argv[1], "-contrast") == 0)
	{
		Mat sourceImage;
		Mat destinationImage;
		sourceImage = imread(argv[2]);

		stringstream str;
		str << argv[4];
		float constract = 0;
		str >> constract;

		if (transformer->ChangeContrast(sourceImage, destinationImage, constract) == 1)
		{
			imwrite(argv[3], destinationImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("Constract Before", sourceImage);
			//imshow("Constract After", destinationImage);
			//waitKey(0);
		}
		else cout << "Change Contrast Fail!";
	}
	else if (strcmp(argv[1], "-drawhist") == 0)
	{
		Mat sourceImage;
		Mat histMatrix;
		Mat histImage;
		sourceImage = imread(argv[2]);
		if (transformer->CalcHistogram(sourceImage, histMatrix) && transformer->DrawHistogram(histMatrix,histImage) == 1)
		{
			imwrite(argv[3], histImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("Image", sourceImage);
			//imshow("Histogram", histImage);
			//waitKey(0);
		}
		else cout << "Draw Hist Fail!";
	}
	else if (strcmp(argv[1], "-equalhist") == 0)
	{
		Mat sourceImage;
		Mat destinationImage;
		sourceImage = imread(argv[2]);
		if (transformer->HistogramEqualization(sourceImage,destinationImage)==1)
		{
			imwrite(argv[3], destinationImage);
			cout << "Open " << argv[3] << " to see your output." << endl;
			//imshow("Before Equalization", sourceImage);
			//imshow("After Equalization", destinationImage);
			//waitKey(0);
		}
		else cout << "Equal Histogram Fail!";
	}
	delete(transformer);
	delete(converter);
	return 0;
}