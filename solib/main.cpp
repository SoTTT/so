#include "so.h"

class A
{
public:
};

namespace so
{
	namespace to_string_ns
	{
		template <>
		std::string to_string<A, std::string::value_type>(const A& value)
		{
			return "1234";
		}
	}
}

int main(int argc, char* argv[])
{
	A a{};
	std::string str("{}{}{}{}");
	str = so::format(str, 1, 2, 3, a);
	so::println(str);
}
