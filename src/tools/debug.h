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
#include <sys/stat.h>

namespace debug
{

	const std::string DIR_PATH = "/mnt/programming/data/MO/tokopuyo/recognition_data8/";

	// For debug and log
	extern bool existExstension(const std::string& original, const std::string& extension);
	
	extern bool makeDir(const std::string& dir_name);
	extern bool fileExists(const std::string& str);

	// std::map<std::string, std::Mat>
	// std::string : file name.
	template<class saveIterator>
	void saveImg(saveIterator begin, saveIterator end, const std::string &dir_path, bool is_hsv=false) 
	{
		if (makeDir(dir_path)==-1)
			LOG("To make dir (name : \"" + dir_path + "\") failed.");

		cv::Mat img;
		for (;begin != end; ++begin)
		{
			if (is_hsv)
			{
				cv::cvtColor(begin->second, img, cv::COLOR_HSV2BGR);
			} 
			else
			{
				img = begin->second;
			}
			
			// check extension.
			std::string file_name = begin->first;
			if (!(existExstension(file_name, ".jpg") || existExstension(file_name, ".png")))
			{
				file_name += ".jpg";
			}

			std::string log_path_img;
			if (!(existExstension(dir_path, "/")))
			{
				log_path_img = DIR_PATH+dir_path+"/"+file_name;
			}
			else
			{
				log_path_img = DIR_PATH+dir_path+file_name;
			}
			
			cv::imwrite(log_path_img, img);
		}
	}

	extern void showForDebug(const std::vector<cv::Mat> &img_vec, const unsigned int wait, const bool is_hsv = false);
	extern void showForDebug(const cv::Mat &image, const unsigned int wait, const bool is_hsv = false);

	// for csv.
	// std::pair<std::vector<std::string>, std::vector<std::vector>>>
	// std::vector<std::string> title.
	// std::vector<std::vector> data.
	template<class csvT>
	void saveElem(const csvT& csv, const std::string& file_name, const bool overwrite = false)
	{
		int title_size = csv.first.size();
		int data_cols_size = csv.second[0].size();
		if (title_size != data_cols_size)
		{
			LOG("Size is different. Not saved.");
		}

		std::string SAVE_PATH;
		if (!(existExstension(file_name, ".txt") || existExstension(file_name, ".csv")))
		{
			SAVE_PATH = DIR_PATH + file_name + ".csv";
		} 
		else
		{
			SAVE_PATH = DIR_PATH + file_name;
		}
		std::ofstream write_file;

		if (overwrite)
		{
			write_file.open(SAVE_PATH, std::ios::trunc);
		}
		else
		{
			write_file.open(SAVE_PATH, std::ios_base::app);
		}


		std::string str = "";
		// write title
		if (overwrite)
		{
			for (int i = 0; i < title_size-1; ++i)
			{
				str += (csv.first[i] + ",");
			}
			str += (csv.first[title_size-1] + "\n");
		}

		for (const auto &elem_vec : csv.second)
		{
			for (int i = 0; i < data_cols_size; ++i)
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
	void saveElem(saveTextIterator begin, saveTextIterator end, const std::string& file_name, bool init=false)
	{
		static int save_color_elem_count = 0;
		if (init)
		{
			save_color_elem_count = 0;
			return;
		}
		
		// check extension.
		std::string SAVE_PATH;
		if (!(existExstension(file_name, ".csv") || existExstension(file_name, ".txt")))
		{
			SAVE_PATH = DIR_PATH + file_name + ".csv";
		}

		std::ofstream write_file;

		if (!save_color_elem_count)
			write_file.open(SAVE_PATH, std::ios::trunc);
		else
			write_file.open(SAVE_PATH, std::ios_base::app);

		std::ostringstream ss;
		ss << std::setw(3) << std::setfill(' ') << save_color_elem_count << " -> ";

		std::string str = "";
		str += ss.str();
		for (; begin != end; ++begin)
		{
			std::ostringstream ss_color, ss_num;
			ss_color << begin->first << " : ";
			ss_num << std::setw(3) << std::setfill('0') << begin->second;

			str += (ss_color.str() + ss_num.str() + ", ");
		}
		str += '\n';
		write_file << str;
		++save_color_elem_count;
	}
};

#endif 