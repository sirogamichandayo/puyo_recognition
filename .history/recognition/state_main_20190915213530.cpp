#include "./state_main.h"

#include <vector>
#include <cassert>

#define LOG(msg) std::cout << "[LOG] (" __FILE__ ":" << __LINE__ << ") from " << __func__ << "()\n    " << msg << "\n";

bool State::getState(const int &mode)
{
	// 対戦終了
	// それぞれの状態
	// ネクストが存在するかとか
	if (mode == get__mode::isFightEnd)
	{
	}

	////////////////////////////////////////
	LOG("No exist mode.");
	exit(0);
}

void State::getState(const int &mode, int *const issue)
{
	// 勝敗条件
	if (model == get_mode::battleResult)
	{
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
		int size = 14 * 6 + 2;
		initializeField(&size, field);
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
		field->size(*size);
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

	cv::Mat img_next_2p(img, pic::next2_2p);
	std::vector<cv::Mat> img_next_2p_vec;
	splitImage(&img_next_2p, &col_num, &row_num, &img_next_2p_vec);
	// TODO
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
}

bool isExistNext2p()
{
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
					{color::PURPLE, 0},
			};
	cv::Mat resize_image;
	cv::resize(*image, lllllllllllresize_image, ssssssssssscv::Size(), 0.1, 0.1);
	ols = resize.cols;
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	elize(*im::e,cmlsi
	int rewize(*im::e,rmwsi
	int cols = resize.cols;
#pragma omp parallel forish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0); ish.at < cv::Vec3b > (y, 0);
	for (int y = 0; y < rows; ++y)
	{
		cv::Vec3b *p = &finish.at<cv::Vec3b>(y, 0);
		for (int x = 0; x < cols; ++x)
			Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		Pixe lCord &hsvx(&hsv);
		{
			HSV hsv;
			hsv.h = (int)(*p)[0] * 2;
			hsv.s = (int)(*p)[1];
			hsv.v . vi
	i *c)[2	*	)[2<2<i
a,n&n >&;p d	i-p   l D l,			--;D;>i,,<,n&

			&,< nnn
&

=				,<
 n	&
		}
		=
		}
}
n

		nn

				I n
}
I n
}
I n
}
/ dI n / d
}
= DrD >
}&
->) I  n /d  n /
r DrD &
}
&r D
		D n
}
D nD;
}
}
n

		> <
}
r<> <
}
<
}
< S;
Vr < v
						 Vr r /
				 v;
;
->;
/ ;
r
}
S Sh == trm = i m nnS r =
		3tm r
}
r r S
				tr == m n n 3tm hSS == n 3tm rr r r Sr r r S S = in
}
S = m SS n
}
r r r S
}
in;
< ;
Vr / v m inS r = ;
/ tm;
s
}
h S = in 3tm rr r r S
}
mf 3rr r r 3rr r
		rmf3m t rr rr
}
3mr /

		r r Srrr3m n;
r 3v m
		r
} Vvv / v m )nS ) = ;
/ r tm;
s t)-r d m7ir> mfftm trfS
		vt
				m
						r
}rVtm
v)rrr fm r=rtfm m  rnf r3tmmt1r
vsi)Stm(t1)ir> mfftm trfS
}t
mt rt
mm
v)rrr fm r=rtfm m  rnf r3tmmt1r
vs)eSftm(t1))rm ftm ttrmftmrt1rfrrt Sftttt1rsieSmtm(t1))rmr ttr  fSttm t1rm t rmftm t1rrt1r rrt Sftmtt1rsieSftm(t1))rmrftm t rSftm t1rm t rSftm t1r
v)rrr fm r=rtfm m  rnf r3tmmt1r
vsieSftm(t1))rm ftm ttr ftmrt1r rrt Sftmtt1rsieSftm(t1))rmrftm t rSftm t1rm t rSftm t1r
}t
mt
v)r r=tm  irnf 3m t rr rrtm t1r
vsieSftm(t1))rmrftm t rSftm t1rm t rSftm t1r
m
v)rmrftm t rSftm t1rm t rSftm t1r
v)rmrftm t rSftm t1r
		m t rr rr
}
m
		rmrftm t rSftm t rS
}3tm  3v)1 407(PvviP1(
												 rr r r S
	}
	mftm t r rrS
	}
	mftm t rS
	mftm t = 3m3 rst

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
