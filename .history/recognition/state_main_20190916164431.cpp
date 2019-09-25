#include "./state_main.h"

#include <vector>
#include <cassert>

#define LOG(msg) std::cout << "[LOG] (" __FILE__ ":" << __LINE__ << ") from " << __func__ << "()\n    " << msg << "\n";

bool State::getState(const int &mode)
{
	// 対戦終了
	// それぞれの状態
	// ネクストが存在するかとか
	if (mode == get_mode::isFightEnd)
	{
		return isJudgeFightEnd();
	}

	if (mode = get_mode::existNext_1p)
	{
		return isExistNext_1p();
	}

	if (mode = get_mode::existNext_2p)
	{
		return isExistNext_2p();
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	exit(0);
}

void State::getState(const int &mode, int &issue)
{
	// 勝敗条件
	if (model == get_mode::battleResult)
	{
		issue = getResult();
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	exit(0);
}

void State::getState(const int &mode, std::vector<int> *const field)
{
	// ぷよとかを受け取る。
	if (mode == get_mode::allPuyo_1p)
	{
		int size = 14 * 6 + 2 + 2;
		initializeField(&size, field);

		std::vector<int> board(14*6);
		getBoard_1p(&board);
		std::vector<int> next1(2);
		getNext_1p(&next1);
		std::vector<int> next2(2);
		getNext2_1p(&next2);
		
		std::move(board.begin(), board.end(), field->begin());
		std::move(next1.begin(), next1.end(), field->begin() + 14*6);
		std::mvoe(next2.begin(), next2.end(), field->begin() + 14*6 + 2);
	}
	if (mode == get_mode::allPuyo_2p)
	{
		int size = 14 * 6 + 2;
		initializeField(&size, field);
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	exit(0);
}

////////////////////////////////////////////////////////////////////////
/* private */
template <typename T>
void initializeField(const int *const size, std::vector<T> *const field)
{
	field->clear();
	if (field->size() < *size)
		field->resize(*size);
}

void cut_img(const cv::Mat *const img_)
{
	for (const auto &[p_, rect_] : player_resize)
		if (player == p_)
		{
			this->img = img_hsv(rect_);
		}
}

// もしかしたらポインタにするかも
int changeColorNum2ForBitNum(int color)
{
	/*
		 Puyo real color has no measning.
		 So change real color number to number for bit operatorn.
		 */
	if (color == color::NONE || color == color::DIST)
		return color;
	if (color == puyo_color_list[0])
		return 4;
	if (color == puyo_color_list[1])
		return 5;
	if (color == puyo_color_list[2])
		return 6;
	if (color == puyo_color_list[3])
		return 7;

	//  Logger
	// Recognition miss.
	LOG("Recognition miss");
	return 0;
}

void State::getNext_1p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next_1p(img, pic::next2_2p);
	std::vector<cv::Mat> img_next_1p_vec;
	splitImage(&img_next_1p, &col_num, &row_num, &img_next_1p_vec);
	// TODO
}

void State::getNext2_1p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	// cut image
	cv::Mat img_next_2p(img, pic::next2_2p);
	std::vector<cv::Mat> img_next_2p_vec;
	splitImage(&img_next_2p, &col_num, &row_num, &img_next_2p_vec);
	// TODO
	int size = img_next_2p_vec.size();
	for (int i = 0; i < size; ++i)
		*field[i] = toGetPuyoColorPerPiece(&img_next_2p_vec[i]);
}

void State::getBoard_1p(std::vector<int> *field)
{
	int col_num = 6;
	int row_num = 14;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_board_1p(img, pic::board_1p);
	std::vector<cv::Mat> img_board_1p_vec;
	splitImage(&img_board_1p, &col_num, &row_num, &img_board_1p_vec);
	// TODO
	int 
}

void State::getNext_2p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next_2p(img, pic::next_2p);
	std::vector<cv::Mat> img_next_2p_vec;
	splitImage(&img_next_2p, &col_num, &row_num, &img_next_2p_vec);
	// TODO
}

void State::getNext2_2p(std::vector<int> *field)
{
	int col_num = 1;
	int row_num = 2;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_next2_2p(img, pic::next2_2p);
	std::vector<cv::Mat> img_next2_2p_vec;
	splitImage(&img_next2_2p, &col_num, &row_num, &img_next2_2p_vec);
	// TODO
}

void State::getBoard_2p(std::vector<int> *field)
{
	int col_num = 6;
	int row_num = 14;
	if (!field->size())
		field->resize(col_num * row_num);

	cv::Mat img_board_2p(img, pic::next2_2p);
	std::vector<cv::Mat> img_board_2p_vec;
	splitImage(&img_board_2p, &col_num, &row_num, &img_board_2p_vec);
}

bool isExistNext1p()
{
	cv::Mat is_next_img_1p(img, pic::is_next_1p);
	return (color::NONE == toGetPuyoColorPerPiece(&is_next_img_1p));
}

bool isExistNext2p()
{
	cv::Mat is_next_img_2p(img, pic::is_next_2p);
	return (color::NONE == toGetPuyoColorPerPiece(&is_next_img_2p));
}

void splitImage(const cv::Mat *const image, const int *const col_num, const int *const row_num, std::vector<cv::Mat> *const image_vec)
{
	/* Order of cutting.
	+=========+
	|4|8|12|16|
	|3|7|11|15|
	|2|6|10|14|
	|1|5|9 |13|
	+=========+
	*/

	int cols = image->cols;
	int rows = image->rows;

	int splitCols = cols / *col_num;
	int splitRows = rows / *row_num;

	std::vector<cv::Rect> p(*col_num * *row_num);

	int index = 0;
	for (int c = 0; c < *col_num; ++c)
	{
		int cols_ = c * splitCols;
		int index_cols = c * *row_num;
		for (int r = 0; r < *row_num; ++r)
		{
			int rows_ = (*col_num - r + 1) * splitRows;
			cv::Rect rec = cv::Rect(cols_, rows_, splitCols, splitRows);
			p[index_cols + r] = rec;
		}
	}

	// initialize.
	int size = col_num * row_num;
	initializeField(*size, image_vec);

	int index = 0;
	for (const auto &rec_ : p)
	{
		cv::Mat cropped(*image, rec_);
		image_vec[index] = cropped;
		++index;
	}
}

//  TODO: include constant(pic, )
// Judge between fight or not.
bool State::isJudgeFightEnd()
{
	int yellow = 0;
	int green = 0;
	int other = 0;
	cv::Mat finish(img, pic::finish);
	cv::resize(finish, finish, cv::Size(), 0.1, 0.1);

#pragma omp parallel for
	for (int y = 0; y < finish.rows; y++)
	{
		cv::Vec3b *p = &finish.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < finish.cols; x++)
		{
			HSV hsv;
			hsv.h = (int)(*p)[0] * 2; // H : 0 ~ 180 in Opencv. Change 0 ~ 360
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];

			int color = toReadGreenYellow(&hsv);
			switch (color)
			{
			case tumo::YELLOW:
				yellow++;
				break;
			case tumo::GREEN:
				green++;
				break;
			case tumo::OTHER:
				other++;
				break;
			}
			p++;
		}
	}
	/*
	cout << "####################" << endl;

	cout << "yellow : " << yellow << endl;
	cout << "green  : " << green  << endl;
	cout << "other  : " << other  << endl;
	*/
	if (((yellow > 1000) && (green > 500)) && (other < 500))
		return true; // finish.
	return false;	// not finish.
}

int State::getResult()
{
	int count_white_1p = 0;
	int count_white_2p = 0;
	int count_blue_1p = 0;
	int count_blue_2p = 0;
	int count_red_1p = 0;
	int count_red_2p = 0;
	int other = 0;

#pragma omp parallel for
	// 1p
	cv::Mat end_1p(img, pic::result_1p);
	cv::resize(end_1p, end_1p, cv::Size(), 0.1, 0.1);
	for (int y = 0; y < end_1p.rows; ++y)
	{
		cv::Vec3b *p = &end_1p.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < end_1p.cols; ++x)
		{
			HSV hsv;
			hsv.h = (int)(*p)[0] * 2; // H : 0 ~ 180 in Opencv. Change 0 ~ 360
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];
			int color = toReadBlueRed(&hsv);
			switch (color)
			{
			case tumo::WHITE:
				++count_white_1p;
				break;
			case tumo::BLUE:
				++count_blue_1p;
				break;
			case tumo::RED:
				++count_red_1p;
				break;
			default:
				++other;
				break;
			}
			++p;
		}
	}
// 2p
#pragma omp parallel for
	cv::Mat end_2p(img, pic::result_2p);
	cv::resize(end_2p, end_2p, cv::Size(), 0.1, 0.1);
	for (int y = 0; y < end_2p.rows; ++y)
	{
		cv::Vec3b *p = end_2p.ptr<cv::Vec3b>(y, 0);
		for (int x = 0; x < end_2p.cols; ++x)
		{
			HSV hsv;
			hsv.h = (int)(*p)[0] * 2; // H : 0 ~ 180 in Opencv. Change 0 ~ 360
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];
			// cout << hsv.h << ", "<< hsv.s << ", " << hsv.v << endl; debug
			int color = toReadBlueRed(&hsv);
			switch (color)
			{
			case tumo::WHITE:
				++count_white_2p;
				break;
			case tumo::BLUE:
				++count_blue_2p;
				break;
			case tumo::RED:
				++count_red_2p;
				break;
			default:
				++other;
				break;
			}
			++p;
		}
	}

	// debug
	// cout << "1p (<=)" << endl;
	// cout << "WHITE : " << count_white_1p << endl;
	// cout << "BLUE  : " << count_blue_1p << endl;
	// cout << "RED   : " << count_red_1p  << endl;
	// cout << "2p (=>)" << endl;
	// cout << "WHITE : " << count_white_2p << endl;
	// cout << "BLUE  : " << count_blue_2p << endl;
	// cout << "RED   : " << count_red_2p  << endl;
	// imageShow(end_1p);
	// imageShow(end_2p);
	if ((70 < count_white_1p && 70 < count_white_2p) && other < 25)
	{
		// 1p win.
		if ((count_blue_1p < 20 && 60 < count_red_1p) && (count_red_2p < 20 && 60 < count_blue_2p))
			return judge::WIN;
		else if ((count_red_1p < 20 && 60 < count_blue_1p) && (count_blue_2p < 20 && 60 < count_red_2p))
			return judge::LOSE;
		else if ((count_red_1p < 20 && 60 < count_blue_1p) && (count_red_2p < 20 && 60 < count_blue_2p)) // drow.
			return judge::DROW;
	}
	return judge::NONE;
}


int toGetPuyoColorPerPiece(const cv::Mat *const image)
{
	// Receive image(piece puyo), and judge puyo color
	float size = 0.1;

	std::map<int, int>
			color_pixel_dict = {
					{color::NONE, 0},
					{color::RED, 0},
					{color::BLUE, 0},
					{color::YELLOW, 0},
					{color::GREEN, 0},
					{color::PURPLE, 0}};
	cv::Mat resize_image;
	cv::resize(*image, resize_image, cv::Size(), 0.1, 0.1);

	int rows = resize_image.rows;
	int cols = resize_image.cols;
#pragma omp parallel for
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &resize_image.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x);
		{
			HSV hsv;
			hsv.h = (int)(*p)[0] * 2;
			hsv.s = (int)(*p)[1];
			hsv.v = (int)(*p)[2];

			color_pixel_dict[toReadPixelColor(&hsv)]++;
      ++p;
		}
	}

	std::pair<int, int> max_color = *std::max_element(m.begin(), m.end(),
    [](const pair<int, int>& p1, const pair<int, int>& p2) {
			return p1.second < p2.second; 
			}
	);
	return max_color.first;
}

int State::toReadPixelPuyoColor(const HSV *const hsv)
{
	// HSV *hsv = new HSV();
	// toHSV(rgb, hsv);
	if (hsv->v < 150)
		return color::NONE;

	if (hsv->s < 50 && 120 < hsv->h)
		return color::DIST;

	if (hsv->h <= 15 && 70 < hsv->v)
		return color::RED;

	if (35 <= hsv->h && (hsv->h <= 70 && 150 < hsv->v))
		return color::YELLOW;

	if (85 <= hsv->h && (hsv->h <= 135 && 120 < hsv->v))
		return color::GREEN;

	if (160 <= hsv->h && (hsv->h <= 255 && 150 < hsv->v))
		return color::BLUE;

	if (250 <= hsv->h && (hsv->h < 340 && 150 < hsv->v))
		return color::PURPLE;

	return color::NONE;
}

int State::toReadBlueRed(const HSV *const hsv)
{
	if ((hsv->s < 40))
		return color::WHITE;

	if ((165 < hsv->h && hsv->h <= 270) && hsv->v > 60)
		return color::BLUE;

	if (((320 < hsv->h && hsv->h <= 360) || (0 <= hsv->h && hsv->h <= 30)) && hsv->v > 60)
		return color::RED;
	return color::OTHER;
}

int State::toReadGreenYellow(const HSV *const hsv)
{
	if ((50 < hsv->h) && (hsv->h < 70) && hsv->v > 200)
		return color::YELLOW;

	if ((90 < hsv->h) && (hsv->h < 120) && hsv->v > 100)
		return color::GREEN;

	return color::OTHER;
}
