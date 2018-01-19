#ifndef STL_ALGORITHM_HPP
#define STL_ALGORITHM_HPP

#include"algorithm_base.hpp"
#include<utility>
#include<iterator>
#include<functional>
#include<random>
#include<array>

#define _STD ::std::



template<typename InputIt , typename UnaryPred>
inline bool all_of(InputIt first , InputIt last , UnaryPred pred)
{
	for (; first != last; ++first)
		if (!pred(*first))
			return false

			return true;
}

template<typename InputIt , typename UnaryPred>
inline bool any_of(InputIt first , InputIt last , UnaryPred pred)
{
	for (; first != last; ++first)
		if (pred(*first))
			return true;

	return false;
}

template<typename InputIt , typename UnaryPred>
inline bool none_of(InputIt first , InputIt last , UnaryPred pred)
{
	for (; first != last; ++first)
		if (pred(*first))
			return false;

	return true;
}

template<typename InputIt , typename UnaryFunction>
inline UnaryFunction for_each(InputIt first , InputIt last , UnaryFunction f)
{
	for (; first != last; ++first)
		f(*first);

	return _STD move(f);
}

template<typename InputIt , typename Size , typename UnaryFunction>
inline InputIt for_each_n(InputIt first , Size n , UnaryFunction f)
{
	for (; n; --n)
	{
		f(*first);
		++first;
	}

	return first;
}

template<typename InputIt , typename T>
inline typename _STD iterator_traits<InputIt>::difference_type
count(InputIt first , InputIt last , const T& val)
{
	typename _STD iterator_traits<InputIt>::difference_type _Count = 0;
	for (; first != last; ++first)
		if (*first == val)
			++_Count;

	return _Count;
}

template<typename InputIt , typename UnaryPred>
inline typename _STD iterator_traits<InputIt>::difference_type
count_if(InputIt first , InputIt last , UnaryPred pred)
{
	typename _STD iterator_traits<InputIt>::difference_type _Count = 0;
	for (; first != last; ++first)
		if (pred(*first))
			++_Count;

	return _Count;
}

template<typename InputIt1 , typename InputIt2 , typename BinaryPred = _STD equal_to<>>
_STD pair<InputIt1 , InputIt2> mismatch(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , BinaryPred p = BinaryPred())
{
	for (; first1 != last1 && p(*first1 , *first2);)
	{
		++first1;
		++first2;
	}

	return _STD make_pair(first1 , first2);
}

template<typename InputIt1 , typename InputIt2 , typename BinaryPred = _STD equal_to<>>
_STD pair<InputIt1 , InputIt2> mismatch(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , BinaryPred p = BinaryPred())
{
	for (; first1 != last1 && first2 != last2 && p(*first1 , *first2))
	{
		++first1;
		++first2;
	}

	return _STD make_pair(first1 , first2);
}

template<typename InputIt1 , typename InputIt2 , typename BinaryPred = _STD equal_to<>>
bool equal(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , BinaryPred p = BinaryPred())
{
	for (; first1 != last1; ++first1 , ++first2)
		if (!p(*first1 , *first2))
			return false;

	return true;
}

template<typename InputIt1 , typename InputIt2 , typename BinaryPred = _STD equal_to<>>
bool equal(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , BinaryPred p = BinaryPred())
{
	if (_STD distance(first1 , last1) != _STD distance(first2 , last2))
		return false;

	for (; first1 != last1; ++first1 , ++first2)
		if (!p(*first1 , *first2))
			return false;

	return true;
}

template<typename ForwardIt1 , typename ForwardIt2 , typename BinaryPred = _STD equal_to<>>
ForwardIt1 search(ForwardIt1 first , ForwardIt1 last , ForwardIt2 s_first , ForwardIt2 s_last , BinaryPred p = BinaryPred())
{
	if (s_first == s_last)
		return first;
	int jump = _STD distance(first , last) - _STD distance(s_first , s_last);
	if (jump >= 0)
	{
		ForwardIt1 end = first;
		_STD advance(end , jump + 1);
		for (; first != end; ++first)
		{
			ForwardIt1 it = first;
			ForwardIt2 s_it = s_first;
			for (;; ++it , ++s_it)
			{
				if (s_it == s_last)
					return first;
				if (!p(*it , *s_it))
					break;
			}
		}
	}
	return last;
}

template<typename ForwardIt , typename Size , typename T , typename BinaryPred = _STD equal_to<>>
ForwardIt search_n(ForwardIt first , ForwardIt last , Size count , const T& val , BinaryPred p = BinaryPred())
{
	if (first == last)
		return last;

	ForwardIt end = first;
	_STD advance(end , _STD distance(first , last) - count + 1);
	for (; first != end; ++first)
	{
		if (!p(*first , val))
			continue;

		ForwardIt it = first;
		Size cur_count = 0;
		for (;;)
		{
			if (!p(*it , val))
				break;

			++cur_count;
			if (cur_count == count)
				return first;

			++it;
		}
	}
	return last;
}

template<typename InputIt , typename T>
InputIt find(InputIt first , InputIt last , const T& val)
{
	for (; first != last; ++first)
		if (*first == val)
			break;

	return first;
}

template<typename InputIt , typename UnaryPred>
InputIt find_if(InputIt first , InputIt last , UnaryPred p)
{
	for (; first != last; ++first)
		if (p(*first))
			break;

	return first;
}

template<typename InputIt , typename UnaryPred>
InputIt find_if_not(InputIt first , InputIt last , UnaryPred p)
{
	for (; first != last; ++first)
		if (!p(*first))
			break;

	return first;
}

template<typename ForwardIt1 , typename ForwardIt2 , typename BinaryPred = _STD equal_to<>>
ForwardIt1 find_end(ForwardIt1 first , ForwardIt1 last , ForwardIt2 s_first , ForwardIt2 s_last , BinaryPred p = BinaryPred())
{
	if (s_first == s_last)
		return last;
	ForwardIt1 ret = last;
	for (;; ++first)
	{
		ForwardIt1 new_ret = search(first , last , s_first , s_last , p);
		if (new_ret == last)
			return ret;
		else
			first = ret = new_ret;
	}
}

template<typename InputIt , typename ForwardIt , typename BinaryPred = _STD equal_to<>>
InputIt find_first_of(InputIt first , InputIt last , ForwardIt s_first , ForwardIt s_last , BinaryPred p = BinaryPred())
{
	for (; first != last; ++first)
		for (ForwardIt it = s_first; it != s_last; ++it)
			if (p(*first , *it))
				return first;

	return last;
}

template<typename ForwardIt , typename BinaryPred = _STD equal_to<>>
ForwardIt adjacent_find(ForwardIt first , ForwardIt last , BinaryPred p = BinaryPred())
{
	if (first == last)
		return last;
	ForwardIt it = first;
	++it;
	for (; it != last; ++first , ++it)
		if (p(*first , *it))
			return first;

	return last;
}

template<typename InputIt , typename OutputIt>
OutputIt copy(InputIt first , InputIt last , OutputIt dest_first)
{
	for (; first != last; ++first , ++dest_first)
		*dest_first = *first;

	return dest_first;
}

template<typename InputIt , typename OutputIt , typename UnaryPred>
OutputIt copy_if(InputIt first , InputIt last , OutputIt dest_first , UnaryPred p)
{
	for (; first != last; ++first)
		if (p(*first))
		{
			*dest_first = *first;
			++dest_first;
		}

	return dest_first;
}

template<typename InputIt , typename Size , typename OutputIt>
OutputIt copy_n(InputIt first , Size count , OutputIt dest_first)
{
	for (Size i = 0; i < count; ++i)
		*dest_first++ = *first++;

	return dest_first;
}

template<typename BidirectionalIt1 , typename BidirectionalIt2>
BidirectionalIt2 copy_backward(BidirectionalIt1 first , BidirectionalIt1 last , BidirectionalIt2 dest_last)
{
	for (; first != last;)
		*(--dest_last) = *(--last);

	return dest_last;
}

template<typename InputIt , typename OutputIt>
OutputIt move(InputIt first , InputIt last , OutputIt dest_first)
{
	for (; first != last; ++first , ++dest_first)
		*dest_first = _STD move(*first);

	return dest_first;
}

template<typename BidirectionalIt1 , typename BidirectionalIt2>
BidirectionalIt2 move_backward(BidirectionalIt1 first , BidirectionalIt1 last , BidirectionalIt2 dest_last)
{
	for (; first != last;)
		*(--dest_last) = _STD move(*(--last));

	return dest_last;
}

template<typename ForwardIt , typename T>
void fill(ForwardIt first , ForwardIt last , const T& val)
{
	for (; first != last; ++first)
		*first = val;
}

template<typename OutputIt , typename Size , typename T>
OutputIt fill_n(OutputIt first , Size count , const T& val)
{
	for (Size cur_count = 0; cur_count < count; ++cur_count)
		*(first++) = val;

	return first;
}

template<typename InputIt , typename OutputIt , typename UnaryPred>
OutputIt transform(InputIt first , InputIt last , OutputIt dest_first , UnaryPred p)
{
	for (; first != last; ++first , ++dest_first)
		*dest_first = p(*first);

	return dest_first;
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename BinaryPred>
OutputIt transform(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , OutputIt dest_first , BinaryPred p)
{
	for (; first1 != last1; ++first1 , ++first2 , ++dest_first)
		*dest_first = p(first1 , first2);

	return dest_first;
}

template<typename ForwardIt , typename Generator>
void generate(ForwardIt first , ForwardIt last , Generator g)
{
	for (; first != last; ++first)
		*first = g();
}

template<typename OutputIt , typename Size , typename Generator>
OutputIt generate_n(OutputIt first , Size count , Generator g)
{
	for (Size cur_size = 0; cur_size < count; ++cur_size)
		*(first++) = g();

	return first;
}

template<typename ForwardIt , typename T>
ForwardIt remove(ForwardIt first , ForwardIt last , const T& val)
{
	first = find(first , last , val);
	if (first != last)
		for (ForwardIt it = first; ++it != last;)
			if (*it != val)
			{
				*first = _STD move(*it);
				++first;
			}

	return first;
}

template<typename ForwardIt , typename UnaryPred>
ForwardIt remove_if(ForwardIt first , ForwardIt last , UnaryPred p)
{
	first = find_if(first , last , p);
	if (first != last)
		for (ForwardIt it = first; ++it != last;)
			if (!p(*it))
			{
				*first = _STD move(*it);
				++first;
			}

	return first;
}

template<typename InputIt , typename OutputIt , typename T>
OutputIt remove_copy(InputIt first , InputIt last , OutputIt dest_first , const T& val)
{
	for (; first != last; ++first)
		if (*first != val)
			*(dest_first++) = *first;

	return dest_first;
}

template<typename InputIt , typename OutputIt , typename UnaryPred>
OutputIt remove_copy_if(InputIt first , InputIt last , OutputIt dest_first , UnaryPred p)
{
	for (; first != last; ++first)
		if (!p(*first))
			*(dest_first++) = *first;

	return dest_first;
}

template<typename ForwardIt , typename T>
void replace(ForwardIt first , ForwardIt last , const T& old_val , const T& new_val)
{
	for (; first != last; ++first)
		if (*first == old_val)
			*first = new_val;
}

template<typename ForwardIt , typename UnaryPred , typename T>
void replace_if(ForwardIt first , ForwardIt last , UnaryPred p , const T& new_val)
{
	for (; first != last; ++first)
		if (p(*first))
			*first = new_val;
}

template<typename InputIt , typename OutputIt , typename T>
OutputIt replace_copy(InputIt first , InputIt last , OutputIt dest_first , const T& old_val , const T& new_val)
{
	for (; first != last; ++first , ++dest_first)
		if (*first != old_val)
			*dest_first = *first;
		else
			*dest_first = new_val;

	return dest_first;
}

template<typename InputIt , typename OutputIt , typename UnaryPred , typename T>
OutputIt replace_copy_if(InputIt first , InputIt last , OutputIt dest_first , UnaryPred p , const T& new_val)
{
	for (; first != last; ++first , ++dest_first)
		if (!p(*first))
			*dest_first = *first;
		else
			*dest_first = new_val;

	return dest_first;
}

template<typename T>
void swap(T& a , T& b)//since C++ 11 ,this swap is in <utility>
{
	T temp(_STD move(a));
	a = _STD move(b);
	b = _STD move(temp);
}

template<typename T , _STD size_t N>
void swap(T(&a) [N] , T(&b) [N])
{
	for (int i = 0; i != N; ++i)
		swap(a [i] , b [i]);
}

template<typename ForwardIt1 , typename ForwardIt2>
void iter_swap(ForwardIt1 it_a , ForwardIt2 it_b)
{
	_STD swap(*it_a , *it_b);
}

template<typename ForwardIt1 , typename ForwardIt2>
ForwardIt2 swap_ranges(ForwardIt1 first1 , ForwardIt1 last1 , ForwardIt2 first2)
{
	for (; first1 != last1; ++first1 , ++first2)
		iter_swap(first1 , first2);

	return first2;
}

template<typename BidirectionalIt>
void reverse(BidirectionalIt first , BidirectionalIt last)
{
	for (; first != last;)
	{
		--last;
		if (first == last)
			break;

		iter_swap(first , last);
		++first;
	}
}

template<typename BidirectionalIt , typename OutputIt>
OutputIt reverse_copy(BidirectionalIt first , BidirectionalIt last , OutputIt dest_first)
{
	for (; first != last;)
	{
		--last;
		*dest_first = *last;
		++dest_first;
	}
	return dest_first;
}

template<typename ForwardIt>
ForwardIt rotate(ForwardIt first , ForwardIt new_first , ForwardIt last) //TOTEST
{
	ForwardIt mid = new_first;
	ForwardIt new_mid = mid;
	new_first = first;

	ForwardIt ret = first;
	_STD advance(ret , _STD distance(mid , last));

	for (;;)
	{
		if (new_first == mid)
		{
			first = mid;
			mid = new_mid;
			if (mid == last)
				break;
		}
		if (new_mid == last)
		{
			first = new_first;
			new_mid = mid;
			if (first == mid)
				break;
		}

		iter_swap(new_first++ , new_mid++);
	}

	return ret;
}

template<typename ForwardIt , typename OutputIt>
OutputIt rotate_copy(ForwardIt first , ForwardIt new_first , ForwardIt last , OutputIt dest_first)
{
	dest_first = copy(new_first , last , dest_first);
	dest_first = copy(first , new_first , dest_first);

	return dest_first;
}

template<typename RandomAccessIt , typename RandomGenerator>
void shuffle(RandomAccessIt first , RandomAccessIt last , RandomGenerator&& g)
{
	using diff_t = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	diff_t distance = last - first;
	_STD uniform_int_distribution<diff_t> _dist(0 , distance - 1);
	for (diff_t i = 0; i != distance; ++i)
	{
		diff_t change = _dist(g);
		if (change != i)
			_STD swap(first [i] , first [change]);
	}
}

template<typename InputIt , typename OutputIt , typename Distance , typename RandomGenerator>
OutputIt sample(InputIt first , InputIt last , OutputIt out , Distance n , RandomGenerator&& g) //since C++ 17
{
	using diff_t = typename _STD iterator_traits<InputIt>::difference_type;
	diff_t _distance = _STD distance(first , last);
	_STD uniform_int_distribution<diff_t> _dist(0 , _distance - 1);
	_STD array<diff_t , n> arr;
	for (diff_t i = 0; i != n; ++i)
	{
		diff_t temp = _dist(g);
		for (diff_t j = 0; j != i;)
		{
			if (arr [j] == temp)
			{
				temp = _dist(g);
				j = 0;
			}
			else
				++j;
		}
		arr [i] = temp;
	}
	for (auto&t : arr)
	{
		InputIt it = first;
		_STD advance(it , t);
		*out = *it;
		++out;
	}

	return out;
}

template<typename ForwardIt , typename BinaryPred = _STD equal_to<>>
ForwardIt unique(ForwardIt first , ForwardIt last , BinaryPred p = BinaryPred())
{
	first = adjacent_find(first , last , p);
	if (first == last)
		return last;

	++first;
	for (ForwardIt it = first; it != last;)
	{
		++it;
		if (!p(*first , *it))
		{
			iter_swap(first , it);
			++first;
		}
	}

	return first;
}

template<typename InputIt , typename OutputIt , typename BinaryPred = _STD equal_to<>>
OutputIt unique_copy(InputIt first , InputIt last , OutputIt dest_first , BinaryPred p = BinaryPred())
{
	InputIt it = adjacent_find(first , last);
	if (it != last)
		++it;
	else
		it = last;
	for (; first != it;)
		*(dest_first++) = *(first++);

	for (; it != last;)
	{
		++it;
		if (!p(*first , *it))
		{
			*(dest_first++) = *it;
			first = it;
		}
	}

	return dest_first;
}

template<typename InputIt , typename UnaryPred>
bool is_partitioned(InputIt first , InputIt last , UnaryPred p)
{
	for (; first != last; ++first)
		if (!p(*first))
			break;

	for (; first != last; ++first)
		if (p*(first))
			return false;

	return true;
}

template<typename ForwardIt , typename UnaryPred>
ForwardIt partition(ForwardIt first , ForwardIt last , UnaryPred p)
{
	for (; first != last;)
		if (!p(*first))
			break;
		else
			++first;

	for (ForwardIt it = first;; it != last;)
	{
		++it;
		if (p(*it))
		{
			iter_swap(first , it);
			++first;
		}
	}

	return first;
}

template<typename InputIt , typename OutputIt1 , typename OutputIt2 , typename UnaryPred>
_STD pair<OutputIt1 , OutputIt2> partition_copy
(InputIt first , InputIt last , OutputIt1 dest_first_true , OutputIt2 dest_first_false , UnaryPred p)
{
	for (; first != last; ++first)
	{
		if (p(first))
			*(dest_first_true++) = *first;
		else
			*(dest_first_false++) = *first;
	}

	return _STD make_pair(dest_first_true , dest_first_false);
}

template<typename BidirectionalIt , typename UnaryPred>
BidirectionalIt stable_partition(BidirectionalIt first , BidirectionalIt last , UnaryPred p)
{
	for (; first != last;)
		if (!p(*first))
			break;
		else
			++first;

	for (BidirectionalIt it = first; it != last;)
	{
		++it;
		if (p(*it))
		{
			BidirectionalIt temp1 = it;
			BidirectionalIt temp2 = temp1;
			--temp1;
			for (; temp2 != first; --temp1 , --temp2)
				iter_swap(temp1 , temp2);
			++first;
		}
	}

	return first;
}

template<typename ForwardIt , typename UnaryPred>
ForwardIt partition_point(ForwardIt first , ForwardIt last , UnaryPred p)
{
	for (; first != last;)
		if (p(*first))
			++first;
		else
			break;

	return first;
}

template<typename ForwardIt , typename Compare = _STD less<>>
bool is_sorted(ForwardIt first , ForwardIt last , Compare p = Compare())
{
	if (first == last)
		return true;
	ForwardIt next = first;
	for (++next; next != last;)
	{

		if (!p(*first , *last))
			return false;

		++first;
		++next;
	}

	return true;
}

template<typename ForwardIt , typename Compare = _STD less<>>
ForwardIt is_sorted_until(ForwardIt first , ForwardIt last , Compare p = Compare())
{
	if (first == last)
		return first;

	ForwardIt next = first;
	for (++next; next != last;)
	{
		if (!p(*first , *next))
			break;

		++first;
		++next;
	}

	return first;
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void sort(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	constexpr int _Insertion_Sort_Max = 32;
	typename _STD iterator_traits<RandomAccessIt>::difference_type dist = last - first;
	if (dist < _Insertion_Sort_Max)
		_insertion_sort(first , last , p);
	else
		_maxheap_sort(first , last , dist , p);
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void partail_sort(RandomAccessIt first , RandomAccessIt mid , RandomAccessIt last , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = last - first;
	for (dist_type i = dist / 2; i != dist; ++i)
		_reverse_make_heaplfy(last - 1 , dist , i , p);

	dist_type count = mid - first;

	for (; first != mid; ++first)
	{
		_STD swap(*first , last [-1]);
		_reverse_make_heaplfy(last - 1 , --dist , 0 , p);
	}
}

template<typename InputIt , typename RandomAccessIt , typename Compare = _STD less<>>
RandomAccessIt partial_sort_copy
(InputIt first , InputIt last , RandomAccessIt dest_first , RandomAccessIt dest_last , Compare p = Compare())
{
	RandomAccessIt end = dest_first;
	for (; (first != last) && (end != dest_last); ++first , ++end)
		*end = *first;

	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = end - dest_first;
	for (dist_type i = dist / 2; i >= 0; --i)
		_make_heaplfy(dest_first , dist , i , p);

	for (; first != last; ++first)
	{
		if (p(*first , *dest_first))
		{
			*dest_first = *first;
			_make_heaplfy(dest_first , dist , 0 , p);
		}
	}

	sort(dest_first , end , p);

	return end;
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void stable_sort(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	_insertion_sort(first , last , p);
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void nth_element(RandomAccessIt first , RandomAccessIt nth , RandomAccessIt last , Compare p = Compare())
{
	if (nth == last || nth == first)
		return;

	RandomAccessIt pivot = last - 1;
	RandomAccessIt left = first;
	RandomAccessIt right = pivot - 1;
	for (;;)
	{
		for (;;)
		{
			for (; left < pivot; ++left)
				if (p(*pivot , *left))
					break;

			for (; right > first; --right)
				if (p(*right , *pivot))
					break;

			if (left >= right)
				break;
			else
				_STD iter_swap(left , right);
		}
		_STD swap(left , pivot);
		if (left == nth)
			break;
		else if (left < nth)
		{
			first = left = left + 1;
			right = pivot - 1;
		}
		else
		{
			pivot = left - 1;
			right = pivot - 1;
			left = first;
		}
	}
}

template<typename ForwardIt , typename T , typename Compare = _STD less<>>
ForwardIt lower_bound(ForwardIt first , ForwardIt last , const T& value , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<ForwardIt>::difference_type;
	dist_type length = _STD distance(first , last);
	ForwardIt it;
	for (; length>0;)
	{
		it = first;
		_STD advance(it , length / 2);
		if (p(*it , value))
		{
			first = ++it;
			length /= 2;
			--length;
		}
		else
			length /= 2;
	}
	return first;
}

template<typename ForwardIt , typename T , typename Compare = _STD less<>>
ForwardIt upper_bound(ForwardIt first , ForwardIt last , const T& value , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<ForwardIt>::difference_type;
	dist_type length = _STD distance(first , last);
	ForwardIt it;
	for (; length > 0;)
	{
		it = first;
		_STD advance(it , length / 2);
		if (!p(value , *it))
		{
			first = ++it;
			length /= 2;
			--length;
		}
		else
			length /= 2;
	}
	return first;
}

template<typename ForwardIt , typename T , typename Compare = _STD less<>>
bool binary_search(ForwardIt first , ForwardIt last , const T& value , Compare p = Compare())
{
	first = lower_bound(first , last , value , p);
	if (first == last)
		return false;
	if (p(value , *first))
		return false;
	else
		return true;
}

template<typename ForwardIt , typename T , typename Compare = _STD less<>>
_STD pair<ForwardIt , ForwardIt> equal_range(ForwardIt first , ForwardIt last , const T& value , Compare p = Compare())
{
	return _STD make_pair(lower_bound(first , last , value , p) , upper_bound(first , last , value , p));
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename Compare = _STD less<>>
OutputIt merge(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , OutputIt dest_frst , Compare p = Compare())
{
	for (; (first1 != last1) && (first2 != last2);)
	{
		if (p(*first2 , *first1))
			*(dest_frst++) = *(first2++);
		else
			*(dest_frst++) = *(first1++);
	}
	for (; first1 != last1;)
		*(dest_frst++) = *(first1++);
	for (; first2 != last2;)
		*(dest_frst++) = *(first2++);

	return dest_frst;
}

template<typename BidirectionalIt , typename Compare = _STD less<>>
void inplace_merge(BidirectionalIt first , BidirectionalIt mid , BidirectionalIt last , Compare p = Compare())
{
	for (; mid != last; ++mid)
	{
		BidirectionalIt it = mid;
		BidirectionalIt prev = it;
		--prev;
		for (; it != first; --it , --prev)
			if (p(*it , *prev))
				iter_swap(it , prev);
			else
				break;
	}
}

template<typename InputIt1 , typename InputIt2 , typename Compare = _STD less<>>
bool includes(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , Compare p = Compare())
{
	for (; first1 != last1; ++first1)
	{
		if (first2 == last2)
			return true;
		if (p(*first1 , *first2))
			continue;
		else if (!p(*first2 , *first1))
			++first2;
		else
			return false;
	}
	if (first2 != last2)
		return false;
	else
		return true;
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename Compare = _STD less<>>
OutputIt set_difference
(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , OutputIt dest_first , Compare p = Compare())
{
	for (; first1 != last1;)
	{
		if (first2 == last2)
			return copy(first1 , last1 , dest_first);

		if (p(*first1 , *first2))
			*(dest_first++) = *(first1++);
		else
		{
			if (!p(*first2 , *first1))
				++first1;
			++first2;
		}
	}
	return dest_first;
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename Compare = _STD less<>>
OutputIt set_intersection
(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , OutputIt dest_first , Compare p = Compare())
{
	for (; first1 != last1;)
	{
		if (p(*first1 , *first2))
			++first1;
		else
		{
			if (!p(*first2 , *first1))
				*(dest_first++) = *(first1++);
			++first2;
		}
	}
	return dest_first;
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename Compare = _STD less<>>
OutputIt set_symmetric_difference
(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , OutputIt dest_first , Compare p = Compare())
{
	for (; first1 != last1;)
	{
		if (first2 == last2)
			return copy(first1 , last1 , dest_first);

		if (p(*first1 , *first2))
			*(dest_first++) = *(first1++);
		else
		{
			if (p(*first2 , *first1))
				*(dest_first++) = *(first2++);
			else
			{
				++first1;
				++first2;
			}
		}
	}

	return copy(first2 , last2 , dest_first);
}

template<typename InputIt1 , typename InputIt2 , typename OutputIt , typename Compare = _STD less<>>
OutputIt set_union
(InputIt1 first1 , InputIt1 last1 , InputIt2 first2 , InputIt2 last2 , OutputIt dest_first , Compare p = Compare())
{
	for (; first1 != last1;)
	{
		if (first2 == last2)
			return copy(first1 , last1 , dest_first);

		if (p(*first1 , *first2))
			*(dest_first++) = *(first1++);
		else
		{
			if (p(*first2 , *first1))
				*(dest_first++) = *(first2++);
			else
			{
				*(dest_first++) = *(first1);
				++first1;
				++first2;
			}
		}
	}

	return copy(first2 , last2 , dest_first);
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
bool is_heap(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = last - first;
	if (_heap_test(first , last , p) != dist)
		return false;
	else
		return true;
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
RandomAccessIt is_heap_until(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	_STD advance(first , _heap_test(first , last , p) - 1);
	return first;
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void make_heap(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = last - first;
	for (dist_type i = dist / 2; i >= 0; --i)
		_make_heaplfy(first , dist , i , p);
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void pop_heap(RandomAccessIt first , RandomAccessIt last , Compare p)
{
	_STD swap(first [0] , last [-1]);
	_make_heaplfy(first , last - first - 1 , 0 , p);
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void push_back(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = last - first;
	dist_type par = _heap_parent(dist - 1);
	dist_type now = dist - 1;
	for (; now>0;)
	{
		if (p(first [par] , first [now]))
		{
			_STD swap(first [par] , first [now]);
			if (par == 0)
				break;
			else
			{
				now = par;
				par = _heap_parent(now);
			}
		}
		else
			break;
	}
}

template<typename RandomAccessIt , typename Compare = _STD less<>>
void sort_heap(RandomAccessIt first , RandomAccessIt last , Compare p = Compare())
{
	using dist_type = typename _STD iterator_traits<RandomAccessIt>::difference_type;
	dist_type dist = last - first;
	for (--dist; dist != 0; --dist)
	{
		_STD swap(first [0] , first [dist]);
		_make_heaplfy(first , dist , 0 , p);
	}
}

template<typename T , typename Compare = _STD less<>>
inline const T& max(const T& a , const T& b , Compare p = Compare())
{
	return (p(a , b)) ? b : a;
}

template<typename ForwardIt , typename Compare = _STD less<>>
ForwardIt max_element(ForwardIt first , ForwardIt last , Compare p = Compare())
{
	if (first == last)
		return last;
	ForwardIt it = first;
	for (++first; first != last; ++first)
	{
		if (p(*it , *first))
			it = first;
	}
	return it;
}

template<typename T , typename Compare = _STD less<>>
T max(_STD initializer_list<T> il , Compare p = Compare())
{
	return *max_element(il.begin() , il.end() , p);
}

template<typename T , typename Compare = _STD less<>>
inline const T& min(const T& a , const T& b , Compare p = Compare())
{
	return (p(b , a)) ? b : a;
}

template<typename ForwardIt , typename Compare = _STD less<>>
ForwardIt min_element(ForwardIt first , ForwardIt last , Compare p = Compare())
{
	if (first == last)
		return last;
	ForwardIt it = fisrt;
	for (++first; first != last; ++first)
	{
		if (p(*first , *it))
			it = first;
	}
	return it;
}

template<typename T , typename Compare = _STD less<>>
T min(_STD initializer_list<T> il , Compare p = Compare())
{
	return *min_element(il.begin() , il.end() , p);
}

template<typename T , typename Compare = _STD less<>>
_STD pair<const T& , const T&> minmax(const T& a , const T& b , Compare p = Compare())
{
	return (p(b , a)) ? _STD make_pair<const T& , const T&>(b , a) :
		_STD make_pair<const T& , const T&>(a , b);
}

template<typename ForwardIt , typename Compare = _STD less<>>
_STD pair<ForwardIt , ForwardIt> minmax_element(ForwardIt first , ForwardIt last , Compare p = Compare())
{
	if (first == last)
		return _STD make_pair(last , last);

	ForwardIt min = min_element(first , last , p);
	ForwardIt max = first;
	for (++first; first != last; ++first)
	{
		if (p(*max , *first))
			max = first;
		else if (!p(*first , *max))
			max = first;
	}
	return _STD make_pair(min , max);
}

template<typename T , typename Compare = _STD less<>>
_STD pair<T , T> minmax(_STD initializer_list<T> il , Compare p = Compare())
{
	auto ret = minmax_element(il.begin() , il.end() , p);
	return _STD make_pair(*(ret.first) , *(ret.second));
}





#endif // !STL_ALGORITHM_HPP
