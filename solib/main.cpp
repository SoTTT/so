#include "so.h"

class A {
public:
};

namespace so {
    namespace to_string_ns {
        template<>
        std::string to_string<A, std::string::value_type>(const A &value, const char *format) {
            return "1234";
        }
    }
}

int main(int argc, char *argv[]) {
    A a{};
    std::string str("}{}{{}");
    str = so::format(str,1);
    so::println(str);
}
