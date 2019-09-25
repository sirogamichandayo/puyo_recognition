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
	splitImage(&img_board_1p, col_num, row_num, &img_board_1p_vec);
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
	float size = 0. 1;

	std::map<int, int>
			color_pixel_dict = {
				{color::NONE, 0},
				{color::RED, 0},
				{color::BLUE, 0},
				{color::YELLOW, 0},
				{color::GREEN, 0},
				{color::PURPLE, 0}
			} cv::Mat oesize_imlee;
	cv::resize(*irage, llllllllresize_it go, sssssssscv::Size(), 0.1, 0.1);
	ols = resize.cols;
	elize(*im::e,cmlsi
	 l; y(*im::f,cmlsiish.at<cv::Vec3b>(y, 0);
	tl0; (	im::.,cmlshi (int)(*p)[0] * 2;
#= l)(*(mip::., c la = i toedPixe lCor(& hsv);
#l > m(dit::., c lal;
il > m(dit::., c lal;
il > m(dit::., c lal;
imv::v, >::(dit::  ooo (, al;
i#
}m::doxReedu eP uyoColor(CCnst HSVourn:cp2)({3u5(<c hsv->h && (hs(
(::	 RED; {

		
		 iv) 10= hsGREENf&&  ((hsv->s <2450))
sv70>: or::>

:P
28R<u0n hRr0	<	 D
		{:					rreturn o34orELL:WUPD
r		er 85 <= n colh && (hov->hr:=:13OTER;
		
ii5335(n -Stt::toR&P UL(Pi((3n Stte::toR&P UL(PGe;

v) 160	<o=::PhsGRU
 
(
f6	2<sv70>: leR:>t HSV *const hsv)
			rn15{
					:>olir::RED4
								reu2r0n color::WHITDnt S
											 RED;tat=e:60) || (t <= hsv->hoRe hsv->h <=a3r))e&&nYellow > 60const HSV *const hsv)
					{
f 5
ii5v335(n -Stt::toR&P UL(P15			if (tte::toR&P UL(PGe;
Sa490 		<lo07 ::P5eadU

	f ((16BuBLUevd(const HSV *const hsv)
				2	0					if ((hsv->s < 40))
ur o85 <= 3YELL(Wh4&& o::PUWhv-=>13PIei((3n Stte::toReGeD				
					(		 (tte::toR&PeUL5PGe;
inn c h5eadBluesvd(const HSV *const hsv)
{5v)(160 <=
hsGREEN	&& 								ifo2(5o Y >s < 40))
		rTturn) color::HBLUIHL:
		
if (20)0 	h>vs)&&	
if (2709)0 	<h>vs)&&;
(5(3 St:34oReGPUeP		i	if (=0<6() || ( 90= h	
		(		 (ttv::-oR&PeULhPGe;
intSa4unvch)5eadBlue>&d(co&st HSVn*const Y  s
{
									ifo(1-1Y)>	3<r4t))::WHIoRGD			if (=0<6() || ( 90= hsv->h<   hsv->hh<=s3))>&&) &&& ( hl 60v->hor:7GR && hsv->v > 200EEN;
		

(			5							 THhR)&;

					7	)	if(>v( sv->					 &	retu5 <0 h sv->Gne & nc&llow(hlv->
 6i((3n Stte::toReGeDcor:7GRO: 
						(=0<6	) || l or=
hsv->h	  						h	<=s3	)>		): && Y  > 60 v E 20EN;
								
		ret THhR) &;

50 < u			ifr((90 < hsv->					 &	n cL5&&0 ( hov->nlo : c&:O	r(elurn
cor:7GRO:lO:(hsv->h < 120) && hsv->v > 100)
												;
			}}		2
	EEN;

rturn2u00EE;
					roolorlO(hsvY:h
returnrcoloreYELLO:(hsv->h < 120) &&tur;
c
					}
}r1R
)	>v 	 200EEN;
		
				retur;
n c}
ol}or:
R

	>v 		200EEN;

																			}:	Y		R	E	R
}
R
int State::toReadGreenYellow(const HSV *const hsv)
{
	if ((50 < hsv->h) && (hsv->h < 70) && hsv->v > 200)
		return color::YELLOW;

	if ((90 < hsv->h) && (hsv->h < 120) && hsv->v > 100)
		return color::GREEN;

	return color::OTHER;
}
