#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "../log/log.h"

#include "opencv2/opencv.hpp"
#include "opencv2/highgui.hpp"


#include <string>
#include <map>
#include <iterator>
#include <vector>
#include <ostream>
#include <filesystem>

extern std::string DEBUG_DIR_PATH; // in debug.cpp
using namespace std;
namespace fs = std::filesystem;

namespace debug
{
	extern void initializeDir(const std::string& path, bool overwrite=false);
	extern void initializeFile(std::ofstream& write_file, const std::string& path, bool overwrite=false);

	// For debug and log
	extern bool existExstension(const std::string& original, const std::string& extension);
	
	extern bool makeDir(const std::string& dir_name);
	extern bool fileExists(const std::string& str);

	// std::map<std::string, std::Mat>
	// std::string : file name.
	template<class saveIterator>
	void saveImg(saveIterator begin_i, saveIterator end_i, const std::string &dir_path, bool is_hsv=false) 
	{
		if (makeDir(DEBUG_DIR_PATH + dir_path) == -1)
			LOG("To make dir (name : \"" + dir_path + "\") failed.");

		cv::Mat img;
		for (;begin_i != end_i; ++begin_i)
		{
			if (is_hsv)
			{
				cv::cvtColor(begin_i->second, img, cv::COLOR_HSV2BGR);
			} 
			else
			{
				img = begin_i->second;
			}
			
			// check extension.
			std::string file_name = begin_i->first;
			if (!(existExstension(file_name, ".jpg") || existExstension(file_name, ".png")))
			{
				file_name += ".png";
			}

			std::string log_path_img;
			if (!(existExstension(dir_path, "/")))
			{
				log_path_img = DEBUG_DIR_PATH+dir_path+"/"+file_name;
			}
			else
			{
				log_path_img = DEBUG_DIR_PATH+dir_path+file_name;
			}
			cv::imwrite(log_path_img, img);
		}
	}

	extern void showForDebug(const std::vector<cv::Mat> &img_vec, const unsigned int wait, const bool is_hsv = false);
	extern void showForDebug(const cv::Mat &image, const unsigned int wait, const bool is_hsv = false);


	// if file exist overwrite,

	// for csv.
	// std::pair<std::vector<std::string>, std::vector<std::vector>>>
	// std::vector<std::string> title.
	// std::vector<std::vector> data.
	template<class csvT>
	void saveElem(const csvT& csv, const std::string& file_name, const bool overwrite=false)
	{
		int title_size = csv.first.size();
		int data_cols_size = csv.second[0].size();
		if (title_size != data_cols_size)
		{
			LOG("Size is different. Not saved.");
			return;
		}

		std::string save_path;
		if (!(existExstension(file_name, ".txt") || existExstension(file_name, ".csv")))
		{
			save_path = DEBUG_DIR_PATH + file_name + ".csv";
		} 
		else
		{
			save_path = DEBUG_DIR_PATH + file_name;
		}
		std::ofstream write_file;
		initializeFile(write_file, save_path, overwrite);

		std::string str = "";
		for (int i = 0; i < title_size-1; ++i)
			str += csv.first[i] + ",";
		str += csv.first[title_size-1] + "\n";


		for (const auto &elem_vec : csv.second)
		{
			for (int i = 0; i < data_cols_size-1; ++i)
			{
				str += (std::to_string(elem_vec[i]) + ",");
			}
			str += (std::to_string(elem_vec[data_cols_size-1])) + "\n";
		}
		write_file << str;
	}

	// for display.
	// std::map<std::string, scaler>
	template<class saveTextIterator>
	void saveElem(saveTextIterator begin_i, saveTextIterator end_i, const std::string& file_name, bool overwrite=false)
	{
		// check extension.
		std::string save_path;
		if (!(existExstension(file_name, ".csv") || existExstension(file_name, ".txt")))
		{
			save_path = DEBUG_DIR_PATH + file_name + ".csv";
		}

		std::ofstream write_file;
		initializeFile(write_file, save_path, overwrite);

		std::string str = "";
		for (; begin_i != end_i; ++begin_i)
		{
			std::ostringstream ss_color, ss_num;
			ss_color << begin_i->first << " : ";
			ss_num << std::setw(3) << std::setfill('0') << begin_i->second;

			str += (ss_color.str() + ss_num.str() + ", ");
		}
		str += '\n';
		write_file << str;
	}
}

#endif 
