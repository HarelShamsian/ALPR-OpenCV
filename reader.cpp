#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdbool.h>
#include <vector>
#define N 8
cv::Mat findPlate(const cv::Mat& img);
void getDigits(const cv::Mat& plateImg, int arr[N]);
std::vector<std::vector<cv::Point>> sortContoursByX(const std::vector<std::vector<cv::Point>>& contours);
bool isDigit(cv::Mat plate, std::vector<cv::Point> object);

//MAIN
int main() {
	
	cv::Mat img = cv::imread("images/carimg/plate1.jpg");
	cv::Mat plate = findPlate(img);

	cv::imshow("plate", plate);
	cv::waitKey(0);

	int digits[N] = { 0 };
	getDigits(plate, digits);
	printf("\n");
	for (int i = 0; i < N; i++) {
		printf("  %d", digits[i]);
	}
	
	return 0;
}

// FIND YELLOW AREA - PLATE
cv::Mat findPlate(const cv::Mat& img){

	cv::Mat hsv, mask;
	cv::cvtColor(img, hsv, cv::COLOR_BGR2HSV);

	cv::Scalar low(20, 100, 100);
	cv::Scalar high(40, 255, 255);
	cv::inRange(hsv, low, high, mask);

	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	cv::Rect plateRect;
	int maxArea = 0;
	for (int i = 0; i < contours.size(); i++) {
		cv::Rect rect = cv::boundingRect(contours[i]);
		int area = rect.width * rect.height;
		if (area > maxArea) {
			maxArea = area;
			plateRect = rect;
		}
	}
	cv::Mat plate = img(plateRect).clone();
	return plate;
}


//FIND THE DIGITS ANT PUT THEM INTO AN ARRAY
void getDigits(const cv::Mat& plateImg, int arr[N]) {
	
  cv::Mat digits[10];

	digits[0] = cv::imread("images/digimg/0.jpg");
	digits[1] = cv::imread("images/digimg/1.jpg");
	digits[2] = cv::imread("images/digimg/2.jpg");
	digits[3] = cv::imread("images/digimg/3.jpg");
	digits[4] = cv::imread("images/digimg/4.jpg");
	digits[5] = cv::imread("images/digimg/5.jpg");
	digits[6] = cv::imread("images/digimg/6.jpg");
	digits[7] = cv::imread("images/digimg/7.jpg");
	digits[8] = cv::imread("images/digimg/8.jpg");
	digits[9] = cv::imread("images/digimg/9.jpg");

	cv::Mat gray1, bw1;
	std::vector<std::vector<cv::Point>> digitContours[10];

	for (int i = 0; i < 10; i++) {
		cv::cvtColor(digits[i], gray1, cv::COLOR_BGR2GRAY);
		cv::threshold(gray1, bw1, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
		cv::findContours(bw1, digitContours[i], cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
		gray1.release();
		bw1.release();
	}

	cv::Mat gray, bw;
	cv::cvtColor(plateImg, gray, cv::COLOR_BGR2GRAY);
		cv::imshow("gray", gray);
		cv::waitKey(0);
	cv::threshold(gray, bw, 0, 255, cv::THRESH_BINARY_INV | cv::THRESH_OTSU);
		cv::imshow("bw", bw);
		cv::waitKey(0);
	std::vector<std::vector<cv::Point>> contours;
	cv::findContours(bw, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
			
	std::vector<std::vector<cv::Point>> sortedContours = sortContoursByX(contours);
	
	int idx = 0;
	for (int i = 0; i < sortedContours.size(); i++) {
		if (!isDigit(bw, sortedContours[i])) continue;
		double bestMatch = 1000., temp;
		int bestDigit = -1;
		for (int j = 0; j < 10; j++) {
			temp = cv::matchShapes(sortedContours[i], digitContours[j][0], cv::CONTOURS_MATCH_I1, 0);
			if (temp < bestMatch) {
				bestMatch = temp;
				bestDigit = j;
			}
		}
		arr[idx++] = bestDigit;
	}
	cv::Mat output = plateImg.clone();
	for (int i = 0; i < sortedContours.size(); i++) {
		if (isDigit(bw, sortedContours[i])) {
			drawContours(output, sortedContours, i, cv::Scalar(0, 255, 0), 2);
		}
	}
	cv::imshow("contours", output);
	cv::waitKey(0);
}

//SORTS DIGITS FROM LEFT TO RIGHT
std::vector<std::vector<cv::Point>> sortContoursByX(const std::vector<std::vector<cv::Point>>&contours) {
	
	std::vector<std::vector<cv::Point>> sorted = contours;

	for (size_t i = 0; i < sorted.size(); ++i) {
		for (size_t j = i + 1; j < sorted.size(); ++j) {
			if (cv::boundingRect(sorted[i]).x > cv::boundingRect(sorted[j]).x) {
				std::swap(sorted[i], sorted[j]);
			}
		}
	}
	return sorted;
}


//GET RID OF GARBAGE CONTOURS
	bool isDigit(cv::Mat plate, std::vector<cv::Point> object) {
		double plateHeight = plate.rows;
		cv::Rect objectRect = cv::boundingRect(object);
		double objectHeight = objectRect.height;
		if (objectHeight > 0.7 * plateHeight) return true;
		return false;
}

