#include "ColorTransformer.h"
#include <iostream>
using namespace std;
ColorTransformer::ColorTransformer()
{
	// Constructor //
}

ColorTransformer::~ColorTransformer()
{
	// Destructor //
}

// Hàm chuẩn hóa giá trị độ xám trong đoạn [0, 255] //
int validColorCode(int value)
{
	if (value > 255) value = 255; // Nếu lớn hơn 255 thì gán = 255.
	else if (value < 0) value = 0; // Nếu âm thì gán = 0.
	return value; // Trả về giá trị chuẩn.
}

int ColorTransformer::ChangeBrighness(const Mat& sourceImage, Mat& destinationImage, short b)
{
	if (sourceImage.empty()) return 0; // Nếu không đọc được ảnh input thì trả về 0 (thất bại).
	int height = sourceImage.rows; // Chiều cao của ảnh input (số dòng trong ma trận điểm ảnh input).
	int width = sourceImage.cols; // Chiều rộng của ảnh input (số cột trong ma trận điểm ảnh input).
	int nChannels = sourceImage.channels(); // Số kênh màu của ảnh input.
	int widthStep = sourceImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh input.
	uchar* pData = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh input.
	destinationImage = Mat(height, width, sourceImage.type()); // Khởi tạo ma trận điểm ảnh kết quả có cùng thuộc tính ảnh input, CV8U3.
	int ws = destinationImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh kết quả.
	uchar* pd = (uchar*)destinationImage.data; // Con trỏ đến ma trận điểm ảnh kết quả.

	// Bắt đầu duyệt qua từng điểm ảnh trong ma trận điểm ảnh input //
	for (int i = 0; i < height; i++)
	{
		uchar* pRow = pData; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh input.
		uchar* pr = pd; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh kết quả.
		for (int j = 0; j < width; j++)
		{
			for (int c = 0; c < nChannels; c++)
			{
				pr[c] = validColorCode(pRow[c] + b); // Công thức điều chỉnh độ sáng của ảnh: g(x,y) = f(x,y) + c.
			}

			//pr[0] = validColorCode(pRow[0] + b);
			//pr[1] = validColorCode(pRow[1] + b);
			//pr[2] = validColorCode(pRow[2] + b);
			
			pRow += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh input.
			pr += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh kết quả.
		}
		pData += widthStep; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh input.
		pd += ws; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh kết quả.
	}
	return 1; // Trả về 1 nếu điều chỉnh độ sáng thành công.
}

int ColorTransformer::ChangeContrast(const Mat& sourceImage, Mat& destinationImage, float c)
{
	if (sourceImage.empty()) return 0; // Nếu không đọc được ảnh input thì trả về 0 (thất bại).
	int height = sourceImage.rows; // Chiều cao của ảnh input (số dòng trong ma trận điểm ảnh input).
	int width = sourceImage.cols; // Chiều rộng của ảnh input (số cột trong ma trận điểm ảnh input).
	int nChannels = sourceImage.channels(); // Số kênh màu của ảnh input.
	int widthStep = sourceImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh input.
	uchar* pData = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh input.

	destinationImage = Mat(height, width, sourceImage.type()); // Khởi tạo ma trận điểm ảnh kết quả có cùng thuộc tính ảnh input.
	int ws = destinationImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh kết quả.
	uchar* pd = (uchar*)destinationImage.data; // Con trỏ đến ma trận điểm ảnh kết quả.

	// Bắt đầu duyệt qua từng điểm ảnh trong ma trận điểm ảnh input //
	for (int i = 0; i < height; i++)
	{
		uchar* pRow = pData; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh input.
		uchar* pr = pd; // Con trỏ đến hàng hiện tại trong ma trận điểm ảnh kết quả.
		for (int j = 0; j < width; j++)
		{
			for (int ch = 0; ch < nChannels; ch++)
			{
				pr[ch] = validColorCode(round(pRow[ch]*c)); // Công thức điều chỉnh độ sáng của ảnh: g(x,y) = f(x,y)*c.
			}
			//pr[0] = validColorCode(pRow[0]*c);
			//pr[1] = validColorCode(pRow[1]*c);
			//pr[2] = validColorCode(pRow[2]*c);

			pRow += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh input.
			pr += nChannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh kết quả.
		}
		pData += widthStep; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh input.
		pd += ws; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh kết quả.
	}
	return 1; // Trả về 1 nếu điều chỉnh độ tương phản thành công.
}

int ColorTransformer::CalcHistogram(const Mat& sourceImage, Mat& histMatrix)
{
	if (sourceImage.empty()) return 0; // Nếu không đọc được ảnh input thì trả về 0 (thất bại).
	int height = sourceImage.rows; // Chiều cao của ảnh input (số dòng trong ma trận điểm ảnh input).
	int width = sourceImage.cols; // Chiều rộng của ảnh input (số cột trong ma trận điểm ảnh input).
	int widthStep = sourceImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh input.
	int nchannels = sourceImage.channels(); // Số kênh màu của ảnh input.
	uchar* pData = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh input.

	histMatrix = Mat(nchannels, 256, CV_32SC1); // Tạo histogram có nchannels*256 phần tử, mỗi phần tử có kiểu số nguyên.
	int* p = (int*) histMatrix.data; // Con trỏ đến vùng data của histogram.
	int ws = histMatrix.step; // Bước nhảy giữa 2 channel (2 dòng) trong ma trận (data) của histogram.

	/* Bắt đầu quá trình tính histogram của ảnh input */

	// Gán tất cả phần tử trong mảng hist[nchannels][256] bằng 0 //
	for (int i = 0; i < nchannels; i++)
	{
		for (int j = 0; j < 256; j++)
		{
			*(p + j) = 0; // Gán phần tử hiện tại bằng 0.
		}
		p += (ws / 4); // Di chuyển đến channel kế tiếp (do ws là số byte để thực hiện phép cộng con trỏ kiểu int ta cần chia cho sizeof(int).
	}

	p = (int*)histMatrix.data; // Dịch chuyển con trỏ về vị trí phần tử đầu tiên trong kênh màu đầu tiên của histogram.
	// Bắt đầu tích histogram //
	// Duyệt qua toàn bộ ảnh input //
	for (int i = 0; i < height; i++)
	{
		uchar* pRow = pData; // Con trỏ đến dòng hiện tại (kênh màu hiện tại) trong histogram.
		for (int j = 0; j < width; j++)
		{
			// RGB - BGR
			// Duyệt qua từng kênh màu của điểm ảnh đang xét để tích histogram cho kênh màu tương ứng. //
			for (int c = 0; c < nchannels; c++) 
			{		
				// Vì OpenCV mỗi điểm ảnh 3 kênh có thứ tự là BGR nên cần chuyển đổi về RGB khi tính toán //
				// nchannels - c - 1 là cách chuyển đổi BGR sang RGB //
				// ws là số byte của 1 dòng nên khi cộng con trỏ ta cần chia lại cho sizeof(int) để chính xác //
				// p + ((nchannels - c - 1)*ws/sizeof(int) là để dịch chuyển đến kênh màu đang xét trong histogram //
				// + pRow[c] là để di chuyển đến giá trị độ xám (hay cột tương ứng của điểm ảnh có kênh màu đang xét) //
				// Sau khi tính toán thì tăng histogram tại kênh màu tương ứng, cột tương ứng lên 1 đơn vị //
				*(p + ((nchannels - c - 1)*ws/4) + pRow[c]) += 1;
			}
			pRow += nchannels; // Di chuyển đến điểm ảnh tiếp theo trong hàng hiện tại của ảnh input.
		}
		pData += widthStep; // Di chuyển đến hàng tiếp theo trong ma trận điểm ảnh input.
	}

	return 1; // Trả về 1 nếu tính histogram ảnh input thành công.
}

int ColorTransformer::HistogramEqualization(const Mat& sourceImage, Mat& destinationImage)
{
	if (sourceImage.empty()) return 0; // Nếu không đọc được ảnh input thì trả về 0 (thất bại).
	Mat histMatrix; // Khởi tạo ma trận histogram cho ảnh input.
	CalcHistogram(sourceImage, histMatrix); // Gọi hàm tính histogram cho ảnh input và lưu vào histMatrix.
	if (!histMatrix.empty()) // Kiểm tra xem có tính được histogram ảnh input không?
	{
		int nGray = 256; // Số mức xám mong muốn của histogram sau khi cân bằng là 256 mức xám.
		int total = sourceImage.rows * sourceImage.cols; // Tổng số điểm ảnh.

		Mat tf = Mat(histMatrix.rows, histMatrix.cols, histMatrix.type()); // Hàm tích lũy mật độ xác suất của histogram đã tính.
		int* pTf = (int*)tf.data; // Con trỏ đến vùng data của hàm tích lũy mật độ xác suất.
		int tfStep = tf.step; // Số byte giữa 2 dòng trong vùng data (ma trận) của hàm tích lũy mật độ xác suất. Vì ảnh có RGB có 3 kênh màu tương ứng có 3 hàm tf cho mỗi kênh.
		int* pHist = (int*)histMatrix.data; // Con trỏ đến histogram của ảnh input đã tính.
		int histStep = histMatrix.step; // Số byte giữa 2 dòng trong vùng data (ma trận) của histogram ảnh input. Vì ảnh có RGB có 3 kênh màu tương ứng có 3 dòng trong ma trận histMatrix.

		Mat f = Mat(histMatrix.rows, histMatrix.cols, sourceImage.type()); // Khởi tạo hàm f để cân bằng mức xám theo hàm tf.
		uchar* pF = (uchar*)f.data; // Con trỏ đến vùng data của hàm f.
		int fStep = f.step; // Số byte giữa 2 dòng trong vùng data (ma trận) của hàm f. Vì ảnh có RGB có 3 kênh màu tương ứng có 3 dòng trong ma trận f.

		// Duyệt qua từng phần tử trong histogram ảnh input đã tính //
		// Ảnh màu RGB thì histMatrix có kích thước 3 x 256 //
		// histMatrix được tính riêng biệt cho từng kênh R, G, B nếu là ảnh RGB //
		for (int i = 0; i < histMatrix.rows; i++) // Duyệt qua histogram của từng kênh màu nếu là ảnh RGB.
		{
			for (int j = 0; j < histMatrix.cols; j++) // Duyệt qua từng mức xám trong kênh màu đang xét.
			{
				// Theo thuật toán cân bằng histogram thì gán Tf[0] = histMatrix[0] // 
				if (j == 0)
				{
					*pTf = *pHist; // Gán Tf[0] = histMatrix[0].
					// Theo thuật toán cân bằng histogram thì f = round((nGray - 1)/total*Tf[i]) //
					// Không +j (cột - mức xám đang xét) vì lúc này là phần từ đầu tiên trong hàng nên cột là 0 j=0 //
					// Nhân 1.0 để ép kiểu về số thực //
					*pF = round(((nGray - 1) * 1.0 / total) * (*pTf)); // Tính f = round((nGray - 1)/total*Tf[i])
				}
				else
				{
					// Theo thuật toán cân bằng histogram thì gán Tf[i] = histMatrix[i] + Tf[i-1] nếu i!=0 // 
					*(pTf + j) = *(pHist + j) + *(pTf + (j - 1)); // Gán Tf[i] = histMatrix[i] + Tf[i-1]
					// Theo thuật toán cân bằng histogram thì f = round((nGray - 1)/total*Tf[i]) //
					// + j để di chuyển đến đúng cột (mức xám) đang xét //
					// Nhân 1.0 để ép kiểu về số thực //
					*(pF + j) = round(((nGray - 1)*1.0 / total) * (*(pTf + j)));
					//cout << *(pF + j) << endl;
					//cout << *(pHist + j) <<"|"<< *(pTf + (j - 1)) <<"|"<<*(pTf + j) << endl;
				}
			}
			pHist += (histStep/4); // Di chuyển đến kênh màu tiếp theo trong histogram của ảnh input.
			pTf += (tfStep/4); // Di chuyển đến kênh màu tiếp theo trong hàm tích lũy xác suất tf.
			pF += fStep; // // Di chuyển đến kênh màu tiếp theo trong hàm f cân bằng histogram.
		}
		
		
		Mat eqlHist = Mat(histMatrix.rows, histMatrix.cols, histMatrix.type()); // Khởi tạo histogram ảnh kết quả.
		int* pEqlHist = (int*)eqlHist.data; // Con trỏ đến histogram đã cân bằng.
		int eqlHistStep = eqlHist.step; // Số byte giữa 2 kênh màu trong histogram đã cân bằng.

		uchar* pOrgImg = (uchar*)sourceImage.data; // Con trỏ đến ma trận điểm ảnh của ảnh input.
		int orgImgStep = sourceImage.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh của ảnh input.

		pF = (uchar*)f.data; // Dịch chuyển con trỏ về lại vị trí bắt đầu của hàm f - dùng cân bằng histogram ảnh input.

		Mat eqlImg = Mat(sourceImage.rows, sourceImage.cols, sourceImage.type()); // Khởi tạo ảnh kết quả có cùng thuộc tính ảnh input.
		uchar* pEqlImg = (uchar*)eqlImg.data; // Con trỏ đến ma trận điểm ảnh của ảnh kết quả đã cân bằng histogram.
		int eqlImgStep = eqlImg.step; // Số byte giữa 2 dòng trong ma trận điểm ảnh của ảnh kết quả đã cân bằng histogram.
		
		// Lần lượt duyệt qua từng điểm ảnh trong ảnh input //
		for (int i = 0; i < sourceImage.rows; i++)
		{
			for (int j = 0; j < sourceImage.cols; j++)
			{
				int nchannel = sourceImage.channels(); // Tổng số kết màu của ảnh input.
				int k =  nchannel * j; // Tính số byte để dịch chuyển đến điểm ảnh có cột j trong dòng đang xét.
				for (int c = 0; c < nchannel; c++) // Duyệt qua từng kênh màu trong ảnh input.
				{
					// Cân bằng từng kênh màu của ảnh input theo hàm f đã tính được //
					// nchannels - c - 1 là cách chuyển đổi BGR sang RGB //
					// + k để di chuyển đến điểm ảnh ở vị trí cột j đang xét  //
					// c*fStep để di chuyển lần lượt từng kênh màu R, G, B của điểm ảnh đang xét và thay bằng giá trị trong hàm f đã tính //
					// Khi đó ta sẽ tính được g(x,y) = F(f(x,y)) vời F là hàm để cân bằng histogram cho kênh màu đang xét //
					// Thay giá trị độ xám mới cho điểm ảnh đang xét với kênh màu, cột tương ứng //
					*(pEqlImg + (nchannel - c - 1) + k) = *(pF + c*fStep + *(pOrgImg + (nchannel - c - 1) + k)); 
				}
			}
			pOrgImg += orgImgStep; // Dịch chuyển đến dòng tiếp theo trong ma trận điểm ảnh input.
			pEqlImg += eqlImgStep; // Dịch chuyển đến dòng tiếp theo trong ma trận điểm ảnh kết quả.
		}
		
		destinationImage = eqlImg; // Gán ảnh kết quả vào destinationImage sau khi đã cân bằng histogram cho từng kênh màu để lưu.
		//imshow("Equalization Image", eqlImg);

		//Mat finalHist;
		//CalcHistogram(eqlImg, finalHist);
		//DrawHistogram(finalHist, destinationImage);
	}
	return 1;
}

int ColorTransformer::DrawHistogram(const Mat& histMatrix, Mat& histImage)
{
	if (histMatrix.empty()) return 0; // Nếu histogram trống thì trả về 0 (thất bại).
	int height = histMatrix.rows; // Số kênh màu (số dòng) của histogram.
	int width = histMatrix.cols; // 256 mức xám (số cột) của histogram.
	int nchannels = histMatrix.rows; // Số kênh màu của histogram.
	int widthStep = histMatrix.step; // Số byte giữa 2 kênh màu (2 dòng) trong histogram ảnh input.
	int* pData = (int*)histMatrix.data; // Con trỏ đến ma trận histogram có kích thước [channel][256].
	
	// Normalize Histogram In Range [0,1] //
	Mat normalHistMat = Mat(height, width, CV_32FC1); // Khởi tạo histogram đã được chuẩn hóa về dạng xác suất [0,1].
	int ws = normalHistMat.step; // Số byte giữa 2 kênh màu (2 dòng) trong histogram đã chuẩn hóa.
	float* p = (float*)normalHistMat.data; // Con trỏ thực đến histogram đã chuẩn hóa về [0,1].

	// Chuẩn hóa các giá trị histogram về [0,1] // 
	// Lần lượt duyệt qua từng kênh màu của histogram //
	for (int i = 0; i < nchannels; i++)
	{
		int sum = 0;
		// Lần lượt duyệt qua từng mức xám trong histogram //
		for (int j = 0; j < width; j++)
		{
			sum += *(pData + j); // Tính tổng điểm ảnh của từng kênh màu có kênh màu i.
		}
		

		//float pro = 0;
		for (int j = 0; j < width; j++)
		{
			*(p + j) = *(pData + j) * 1.0 / sum; // Tính xác suất của điểm hiện tại trong normalHistMat đang xét (có kênh màu i và ở côt j).
			//pro += *(p + j);
			//cout << *(p + j) << ", ";
		}
		//cout << pro;
		pData += (widthStep/4); // Di chuyển con trỏ kênh màu tiếp theo của histogram (histMatrix).
		p += (ws/4); // Di chuyển con trỏ kênh màu tiếp theo của normalHistMat chuẩn hóa về [0,1].
	}

	// Draw Histogram //
	// cv::Rect rect(x, y, width, height);
	// cv::rectangle(img, rect, cv::Scalar(0, 255, 0))
	int nGray = 256, thickness = 5; // Số mức xám trong biểu đồ là 256, thicness là chiều rộng hình chữ nhật cần vẽ.
	int w = nGray * thickness; // Chiều rộng của ảnh histogram cần vẽ.

	int k = 50;
	int deltay = 80;
	int deltax = 160;
	int h = 500; // Chiều cao của ảnh histogram cần vẽ.
	
	p = (float*)normalHistMat.data; // Di chuyển con trỏ về vị trí bắt đầu của normalHistMat.

	// Khởi tạo ma trận histogram kết quả để vẽ và lưu //
	// h + deltay là chiều cao của ảnh giúp ảnh histogram kết quả đẹp hơn do có các hàng đánh giá trị mức xám //
	// w + deltax là chiều rộng của ảnh giúp ảnh histogram kết quả đẹp hơn do có các cột đánh xác suất của mức xám //
	histImage = Mat(h+deltay, w+deltax, CV_8UC3); 
	float max = 0; // Khởi tạo giá trị lớn nhất là 0.

	// Tìm giá trị lớn nhất trong các kênh màu của normalHistogram để vẽ ảnh histogram đẹp hơn  //
	for (int i = 0; i < nchannels; i++) // Duyệt qua từng kênh màu trong normalHistogram.
	{
		for (int j = 0; j < 256; j++) // Duyệt qua từng mức xám trong normalHistogram.
		{
			float pro = *(p + j); // Lấy ra giá trị xác suất của mức xám j kênh màu i trong normalHistogram.
			if (pro > max) max = pro; // Nếu giá trị lớn hơn max thì cập nhật lại max.
		}
		p += (ws/4); // Di chuyển đến kênh màu tiếp theo. Chia cho 4 để dịch chuyển con trỏ p chính xác vì p là con trỏ int.
	}

	// Vẽ trục Oy là các chỉ số xác suất của mức xám đang xét //
	for (int i = 0; i < h; i++)
	{
		int dis = 50; // Chia chiều cao ảnh histogram thành các khoảng, mỗi khoảng chênh lệnh độ cao trong ảnh là 50.
		if (i % dis == 0) // Khoảng cần vẽ lên ảnh.
		{
			int l = 10;
			//Scalar color = Scalar(0, 0, 0);
			//Rect rect(deltax/4,h-i, l, 2);
			//rectangle(histImage, rect, color);
			string s = std::to_string(i*max/h); // Chuyển đổi giá trị nguyên i*max/h sang chuỗi để dùng hàm putText. 
			// Tính toán các tọa độ, màu sắc, font chữ để vẽ //
			putText(histImage, s, Point(deltax / 4-10, h - i), FONT_HERSHEY_COMPLEX_SMALL, 0.4, Scalar(0, 0, 0), 1);
		}
	}

	// Vẽ trục Ox là các giá trị mức xám //
	for (int i = 0; i < 256; i++)
	{
		int dis = 50; // Chia chiều rộng ảnh histogram thành các khoảng, mỗi khoảng chênh lệnh chiều rộng trong ảnh là 50.
		if (i % dis == 0) // Khoảng cần vẽ lên ảnh.
		{
			int l = 10; // Chiều dài nét vẽ.
			Scalar color = Scalar(0, 0, 0); // Màu sắc hình chữ nhật cần vẽ là màu đen.
			// Tính tọa độ x để vẽ: i * thickness+deltax/2 //
			// Tính tọa độ y để vẽ: h+deltay/2-l //
			// Chiều rộng nét vẽ là: 2 //
			// Chiều dài nét vẽ là l //
			Rect rect(i * thickness+deltax/2, h+deltay/2-l, 2, l);
			rectangle(histImage, rect, color);
			string s = std::to_string(i); // Chuyển đổi giá trị nguyên i sang chuỗi để dùng hàm putText. 
			// Thực hiện vẽ hình chữ nhật với tọa độ, màu sắc, kích thước, font chữ đã tính //
			putText(histImage, s , Point(i * thickness+deltax/2, h+deltay-10),FONT_HERSHEY_COMPLEX_SMALL, 0.8, Scalar(0, 0, 0), 1);
		}
	}


	// Tiến hành vẽ histogram cho từng kênh màu //
	p = (float*)normalHistMat.data; // Di chuyển con trỏ p về vị trí bắt đầu của normalHistMat
	// Duyệt qua từng kênh màu trong normalHistMat //
	for (int i = 0; i < nchannels; i++)
	{
		Scalar color; // Màu sắc nét vẽ hình chữ nhật.
		if (i == 0) color = Scalar(0, 0, 255); // Nếu i=0 là kênh đỏ thì màu sắc hình chữ nhật là màu đỏ.
		else if (i == 1) color = Scalar(0, 255, 0); // Nếu i=1 là kênh lục thì màu sắc hình chữ nhật là màu lục.
		else if (i==2) color = Scalar(255, 0, 0); // Nếu i=2 là kênh lam thì màu sắc hình chữ nhật là màu lam.

		for (int j = 0; j < 256; j++) // Duyệt qua các mức xám trong normalHistMat.
		{
			int x = j; // Tọa độ x để vẽ hình chữ nhật chĩnh là giá trị mức xám của kênh i cột j trong normalHistMat.
			float cao = (*(p + j) * h)/max; // Tính toán chiều cao hình chữ nhật.
			int y = h - cao+deltay/2; // Tính tọa độ y của hình chữ nhật.
			//cout << cao << ", ";
			// Tạo hình chữ nhật với các thông số đã tính //
			Rect rect(x*thickness+deltax/2,y, 1,cao);
			// Vẽ hình chữ nhật với các thông số đã tính //
			rectangle(histImage, rect, color);
		}
		p += (ws / 4); // Di chuyển con trỏ p đến kênh màu tiếp theo. Chia cho 4 là vì phép cộng con trỏ p kiểu int.
	}
	return 1; // Trả về 1 nếu vẽ histogram thành công.
}