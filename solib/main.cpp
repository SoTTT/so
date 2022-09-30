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
    std::string str("{} {}");
    auto s = so::format(str, 1, 3.0);
    so::println(s);
}
