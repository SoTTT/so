# 输出工具： format & println

## 介绍

某天写作业，因为C++的字符串拼接太难用，std::format又要等到C++20，所以自己写了几个用于拼接、转换字符串的的小工具函数，基于C++11

## 使用说明

println用来打印一行字符串，函数：

```c++
template<typename T, typename ...Args> size_t println(T value, Args ... args)
```

用于打印不定个参数，打印结束后换行，其中，所用的类型（T以及Args）应该重载了流插入操作符（ostream << T），否则实例化时引发编译时错误；
这个函数至少要有一个参数，无参调用则抛出no_arguments_exception异常;

format用于拼接字符串，原始字符串通过花括号对来指明插入的位置，该函数通过将不定参数列表中的参数替换到花括号对处来实现拼接；

```c++
std::string str("{} {}");
auto s = so::format(str, 1, 3.0);
```

代码展示了一个包含两个花括号对的字符串，在经过format处理后，返回的值是将整形值1和浮点型值3.0替换到第一和第二个花括号处的结果，即：1
3.000000
两个值均以默认格式转换，要指示字符串格式，可以使用格式说明符指出，由于format内部使用std::
sprintf处理有格式说明符时的转换，指示符会被转发给sprintf，可用的指示符与sprintf完全一致，隐式转换和收窄的规则也与sprintf完全一致，format没有处理空格的机制，花括号对中包含无用的空格可能会导致错误；

```c++
std::string str("{:%.2f}");
auto s = so::format(str, 2.0);
```

浮点数2.0会被格式化成2.00

对于自定义的类型，例如一个类A，要让format可以完成插入，应该为A提供转换函数，自定义类型的基础转换模板定义在so::to_string_ns中：

```c++
template<typename T, typename char_t> std::basic_string<char_t> to_string(const T &value, const char *format = nullptr)
```

这个模板无法实例化，实例化这个模板会引发一个编译时错误；
让format可以处理某个类型的方法是在so::to_string_ns中为这个类型提供特化，例如为A提供特化

```c++
namespace so {
    namespace to_string_ns {
        template<>
        std::string to_string<A, std::string::value_type>(const A &value, const char *format) {
            return "1234";
        }
    }
}
```

那么：

```c++
A a{};
std::string str("{}");
auto s = so::format(str, a);
```

s的值为"1234"

特化必须在so::to_string_ns命名空间中；

其中，const char *format的值是可选的，如果你的原始字符串中对应的花括号对中不包含任何字符，那么format的值为nullptr

如果有自定义的格式化指示符，那么format指向一个以'\0'结束的字符串，通过c函数strlen可以返回这个字符串的长度；

该c风格字符串的生命周期绑定在一个std::string对象上，在format调用完成后指针指向的内存即被释放，在to_string函数的作用域外传递、存储、使用这个串是未定义行为；

有时候，你可能不希望原始串中包含的花括号被识别为格式化标记，那么就要为它们加上双反斜杠：

```c++
std::string str("\\{{:%.2f}\\}");
auto s = so::format(str, 2.0);
```

在这个串中，第一个左花括号和最后一个右花括号不会被替换，最终的值为"{2.00}"

## 错误处理

### println

println会抛出一个库定义的异常：no_argument_exception，这个异常说明调用println时没有传参

### format

println会抛出库定义的异常：no_argument_exception 和 arguments_no_match_exception，前者表示除了原始串外没有其它参数，后者表示了花括号对和参数的数量不匹配；
