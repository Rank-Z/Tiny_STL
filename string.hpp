#include<xstring>//basic_string<>
#include<iosfwd>//char_traits
#include<memory>//allocator

using string = ::std::basic_string<char,::std:: char_traits<char>,::std:: allocator<char>>;

inline int stoi(const string& _Str, size_t *_Idx = 0, int _Base = 10);

inline long stol(const string& _Str, size_t *_Idx = 0, int _Base = 10);

inline unsigned long stoul(const string& _Str, size_t *_Idx = 0, int _Base = 10);

inline long long stoll(const string& _Str, size_t *_Idx = 0, int _Base = 10);

inline unsigned long long stoull(const string& _Str, size_t *_Idx = 0, int _Base = 10);

inline float stof(const string& _Str, size_t *_Idx = 0);

inline double stod(const string& _Str, size_t *_Idx = 0);

inline long double stold(const string& _Str, size_t *_Idx = 0);

