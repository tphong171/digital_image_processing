#include "Converter.h"
#include <iostream>
using namespace std;
Converter::Converter()
{
	// Constructor //
}

Converter::~Converter()
{
	// Destructor //
}

int Converter::RGB2GrayScale(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty() || sourceImage.channels() != 3) return 1; // Kiểm tra ảnh có đọc được hay không, có phải ảnh màu?
	int height = sourceImage.rows; // Chiều cao của ảnh.
	int width = sourceImage.cols; // Chiều rộng của ảnh.
	int nChannels = sourceImage.channels(); // Số kênh màu của ảnh.
	int widthStep = sourceImage.step; // Số byte giữa 2 hàng trong ma trận điểm ảnh.
	uchar* pData = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh.

	destinationImage = Mat(height, width, CV_8UC1); // Khởi tạo ma trận điểm ảnh kết quả có 1 kênh màu, cùng kích thước ảnh gốc.
	int ws = destinationImage.step; // Số byte giữa 2 hàng trong ma trận điểm ảnh kết quả.
	uchar* pd = (uchar*)destinationImage.data; // Con trỏ đến ma trận điểm ảnh kết quả.

	for (int i = 0; i < height; i++) // Duyệt từ phần tử trong ma trận điểm ảnh.
	{
		uchar* pRow = pData; // Con trỏ đến hàng đang duyệt hiện tại trong ảnh gốc. 
		uchar* pr = pd;	// Con trỏ đến hàng đang duyệt hiện tại trong ảnh kết quả.
		for (int j = 0; j < width; j++)
		{
			pr[0] = 0.3 * pRow[2] + 0.59 * pRow[1] + 0.11 * pRow[0]; // Công thức chuyển ảnh màu sang ảnh xám theo xác suất từng kênh.
			//pr[0] = (pRow[2] + pRow[1] + pRow[0])/3; // Công thức chuyển ảnh màu sang ảnh xám: g = (R+G+B)/3.
			pRow += nChannels; // Dịch chuyển qua phần tử kế tiếp trong hàng đang duyệt của ảnh gốc (3 byte)
			pr += 1; // Dịch chuyển qua phần từ kế tiếp trong hàng đang duyệt của ảnh kết quả (1 byte)
		}
		pData += widthStep;	// Di chuyển con trỏ đến hàng tiếp theo trong ảnh gốc.
		pd += ws; // Di chuyển con trỏ đến hàng tiếp theo trong ảnh kết quả.
	}
	return 0; // Trả về 0 nếu chuyển đổi thành công.
}

// Hàm trả về giá trị lớn nhất trong 2 số thực //
double getMax(double a, double b)
{
	if (a > b) return a;
	else return b;
}

// Hàm trả về giá trị nhỏ nhất trong 2 số thực //
double getMin(double a, double b)
{
	if (a < b) return a;
	else return b;
}

int Converter::RGB2HSV(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty() || sourceImage.channels() != 3) return 1; // Trả về 1 nếu không đọc được ảnh input, không phải ảnh màu.
	int height = sourceImage.rows; // Chiều cao của ảnh input (số hàng trong ma trận điểm ảnh input).
	int width = sourceImage.cols; // Chiều rộng của ảnh input (số cột trong ma trận điểm ảnh input).
	int nChannels = sourceImage.channels(); // Số kênh màu của ảnh input.
	int widthStep = sourceImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh input.
	uchar* pData = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh của ảnh input.

	destinationImage = Mat(height, width, CV_8UC3); // Khởi tạo ảnh kết quả có 3 kênh màu, cùng kích thước ảnh input.
	int ws = destinationImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh kết quả.
	uchar* pd = (uchar*)destinationImage.data; // Con trỏ đến ma trận điểm ảnh của ảnh kết quả.

	// Lần lượt duyệt từng phần tử trong ma trận điểm ảnh input. //
	for (int i = 0; i < height; i++) 
	{
		uchar* pRow = pData; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh input.
		uchar* pr = pd; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh kết quả.
		for (int j = 0; j < width; j++)
		{

			double R = pRow[2]/255.0; // Quy đổi kênh đỏ R về 1.
			double G = pRow[1]/255.0; // Quy đổi kênh xanh lá G về 1.
			double B = pRow[0]/255.0; // Quy đổi kênh xanh dương B về 1.

			double H = 0; // Khởi tạo giá trị Hue.
			double S = 0; // Khởi tạo giá trị Saturation.
			double V = getMax(getMax(R,G),B); // Tính giá trị cường độ sáng của điểm ảnh Value.
			double temp = getMin(getMin(R, G), B); // temp dùng để tính delta = V - temp.

			if (V <= 0.0) S = 0;	// Nếu V<=0.0 đồng nghĩa V=0 thì S=0 (theo mô hình màu HSV).
			else S = (V - temp)*1.0 / V; // Ngược lại thì S được tính theo công thức delta/V.

			if (S <= 0.0) H = 0; // Nếu S<=0.0 đồng nghĩa S=0 thì H=0 (theo mô hình màu HSV).
			else
			{
				//double Cr = (V - R) * 1.0 / (V - temp);
				//double Cg = (V - G) * 1.0 / (V - temp);
				//double Cb = (V - B) * 1.0 / (V - temp);
				
				if (R >= V) H = (G-B)/(V-temp); // Tính giá trị cho Hue khi V=R.
				if (G >= V) H = 2 + (B - R)/(V - temp); // Tính giá trị cho Hue khi V=G.
				if (B >= V) H = 4 + (R - G)/(V - temp); // Tính giá trị cho Hue khi V=B.

				H = H*60.0; // Chuyển Hue sang độ.
				if (H < 0.0) H += 360; // Chuẩn hóa H khi bị âm.
			}

			pr[0] = H; // Giá trị kênh lam B là Hue.
			pr[1] = S*255; // Giá trị kênh lục G là S.
			pr[2] = V*255; // Giá trị kênh đỏ R là V.
			//cout << H << ", " << S << ", " << V << endl;
			//cin.get();
			pRow += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh input.
			pr += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh kết quả.
		}
		pData += widthStep; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh input.
		pd += ws; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh kết quả.
	}
	return 0; // Trả về 0 nếu chuyển đổi thành công.
}

int Converter::Convert(Mat& sourceImage, Mat& destinationImage, int type)
{
	if (type == 0)
	{
		return this->RGB2GrayScale(sourceImage, destinationImage);
	}
	else if (type == 1)
	{
		return this->RGB2HSV(sourceImage, destinationImage);
	}
	return 1;
}