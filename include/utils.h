#pragma once

#include <iostream>
#include <iomanip>
#include <fstream>

#define DEBUG
#ifdef DEBUG
    #define LOG(x) print<decltype(x)>(x)
#else
    #define LOG(x)
#endif

template<typename T>
inline void print(const T& log) {{
    using namespace std;
    time_t now = time(0);
    cout << put_time(localtime(&now), "%F %T%z: ") << log << endl;
}}

template<>
inline void print(const std::wstring& log) {{
    using namespace std;
    time_t now = time(0);
    wcout << put_time(localtime(&now), L"%F %T%z: ") << log << endl;
}}
