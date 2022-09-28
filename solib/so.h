#pragma once
#include <type_traits>
#include <iostream>
#include <stack>
#include <string>
#include <cassert>
#include <exception>

namespace so
{
	using std::void_t;
	using std::cout;
	using std::endl;


	template <typename T, typename V = void>
	struct is_printable : std::false_type
	{
	};

	template <typename T>
	struct is_printable<T, void_t<decltype(cout << T())>> : std::true_type
	{
	};

	template <typename T, typename V>
	constexpr bool is_printable_v = is_printable<T, V>::value;

	namespace to_string_ns
	{
		template <typename T, typename char_t>
		std::basic_string<char_t> to_string(const T& value)
		{
			static_assert(sizeof(char_t) != sizeof(char_t), "This template cannot be instantiated");
			return std::basic_string<char_t>{};
		}

#define TO_STRING_FUNCTIONAL(TYPE) \
				template <> \
				inline std::string to_string<TYPE, std::string::value_type>(const TYPE & value) \
				{\
					return std::to_string(value);\
				}

		template <>
		inline std::string to_string<double, std::string::value_type>(const double& value)
		{
			return std::to_string(value);
		}

		TO_STRING_FUNCTIONAL(unsigned)
		TO_STRING_FUNCTIONAL(int)
		TO_STRING_FUNCTIONAL(long)
		TO_STRING_FUNCTIONAL(unsigned long)
		TO_STRING_FUNCTIONAL(long long)
		TO_STRING_FUNCTIONAL(unsigned long long)
		TO_STRING_FUNCTIONAL(float)
		TO_STRING_FUNCTIONAL(long double)
	}

	template <typename T, typename V = void>
	struct is_to_string_able : std::false_type
	{
	};

	template <typename T>
	struct is_to_string_able<T, void_t<decltype(to_string_ns::to_string<T, std::string::value_type>(T())
	                         )>> : std::true_type
	{
	};

	template <typename T, typename V>
	constexpr bool is_to_string_able_v = is_to_string_able<T, V>::value;

	class no_argument_exception : public std::exception
	{
	public:
		explicit no_argument_exception(char const* message)
			: exception()
		{
		}

		no_argument_exception(): no_argument_exception("have no argument")
		{
		}
	};

	inline size_t println()
	{
		throw no_argument_exception();
	}

	template <typename T>
	size_t println(T value) noexcept(noexcept(cout << value << endl))
	{
		static_assert(is_printable<T>::value, R"(assert: the object of type "T" not override operator "<<")");
		cout << value << endl;
		return 1;
	}

	template <typename T, typename ...Args>
	size_t println(T value, Args ... args) noexcept(noexcept(cout << value))
	{
		static_assert(is_printable<T>::value, R"(assert: the object of type "T" not override operator "<<")");
		cout << value;
		return println(args...);
	}


	template <typename char_t, typename T, typename ...Args>
	std::basic_string<char_t> format(std::basic_string<char_t> raw_string, T value)
	{
		static_assert(is_to_string_able<T>::value, R"(the type "T" has functional or callable object "to_string<T>")");
		using std::stack;
		typename std::basic_string<char_t>::size_type begin_of_find_left = 0;
		typename std::basic_string<char_t>::size_type begin_of_find_right = 0;
		typename std::basic_string<char_t>::size_type first_left = 0;
		typename std::basic_string<char_t>::size_type first_right = 0;
		{
			first_left = raw_string.find_first_of("{", begin_of_find_left);
		}
		while (first_left != -1 && (first_left == 0 || raw_string[first_left - 1] != '\\')
			       ? (false)
			       : first_left == -1
			       ? false
			       : (begin_of_find_left = first_left + 1, true));
		begin_of_find_right = first_left + 1;
		{
			first_right = raw_string.find_first_of("}", begin_of_find_right);
		}
		while (first_right != -1 && (first_right == 0 || raw_string[first_right - 1] != '\\')
			       ? (false)
			       : first_right == -1
			       ? false
			       : (begin_of_find_right = first_right + 1, true));
		if (first_left != -1 && first_right != -1)
		{
			// static_assert(std::is_same_v<typename decltype(to_string_ns::to_string<T>())::value_type, char_t>
			// 	, "the return type of to_string is not same as basic_string<char_t>");
			raw_string.replace(first_left, first_right - first_left + 1, to_string_ns::to_string<T, char_t>(value));
		}
		else
		{
#ifdef _MSC_VER
			_wassert(
				L"Please check if the parentheses are paired or if the number of parentheses is the same as the number of parameters",
				_CRT_WIDE(__FILE__), __LINE__);
#else
			assert(false);
#endif
		}
		return raw_string;
	}

	template <typename char_t, typename T, typename ...Args>
	std::basic_string<char_t> format(std::basic_string<char_t> raw_string, T value, Args ... args)
	{
		static_assert(is_to_string_able<T>::value, R"(the type "T" has functional or callable object "to_string<T>")");
		using std::stack;
		typename std::basic_string<char_t>::size_type begin_of_find_left = 0;
		typename std::basic_string<char_t>::size_type begin_of_find_right = 0;
		typename std::basic_string<char_t>::size_type first_left = 0;
		typename std::basic_string<char_t>::size_type first_right = 0;
		{
			first_left = raw_string.find_first_of("{", begin_of_find_left);
		}
		while (first_left != -1 && (first_left == 0 || raw_string[first_left - 1] != '\\')
			       ? (false)
			       : first_left == -1
			       ? false
			       : (begin_of_find_left = first_left + 1, true));
		begin_of_find_right = first_left + 1;
		{
			first_right = raw_string.find_first_of("}", begin_of_find_right);
		}
		while (first_right != -1 && (first_right == 0 || raw_string[first_right - 1] != '\\')
			       ? (false)
			       : first_right == -1
			       ? false
			       : (begin_of_find_right = first_right + 1, true));
		if (first_left != -1 && first_right != -1)
		{
			raw_string.replace(first_left, first_right - first_left + 1, to_string_ns::to_string<T, char_t>(value));
		}
		else
		{
#ifdef _MSC_VER
			_wassert(
				L"Please check if the parentheses are paired or if the number of parentheses is the same as the number of parameters",
				_CRT_WIDE(__FILE__), __LINE__);
#else
			assert(false);
#endif
		}
		return format(raw_string, args...);
	}


	template <typename char_t, typename ...Args>
	std::basic_string<char_t> format(std::basic_string<char_t> raw_string, Args ... args)
	{
		return format(raw_string, args...);
	}
}
