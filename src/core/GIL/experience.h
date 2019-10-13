#include <deque>
#include <vector>
#include <numeric>

struct E
{
	E(const std::vector<int> &s, const std::vector<int> &n_s, 
		const int &a, const int &r, const bool &d)
		: state(s),
			next_state(n_s),
			action(a),
			reward(r),
			done(d) {};

	std::vector<int> state;
	std::vector<int> next_state;
	int action;
	double reward;
	bool done;
};

class Experience
{
public:	
	Experience(const int &b_size = 10000) : buffer_size(b_size) {};

	void push(const std::vector<int>& s, const std::vector<int> &n_s, 
						const int &a, const double &r, const double &d)
	{
		if (exp_deque.size() == buffer_size)
		{
			exp_deque.pop_front();
		}
		E e(s, n_s, a, r, d);
		exp_deque.push_back(e);
	}

	double get_recent(const int &size)
	{
		// Warning.
		auto end = exp_deque.end();
		auto first = std::prev(end, size);
		return std::accumulate(end, first,  
	
	}
public:	
	std::deque<E> exp_deque;
	const int buffer_size;
};