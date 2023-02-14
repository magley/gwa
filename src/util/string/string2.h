#pragma once
#include <string>
#include <vector>

struct string2 {
private:
    std::string _s;
public:
    const static size_t npos = std::string::npos;

    string2();
    string2(char c);
    string2(const char* s);
    string2(const std::string& s);
    string2(const string2& s);

    std::string str() const;
    std::string& str_ref();
    std::string* str_ptr();
    const char* c_str() const;

    string2& operator  =(const string2& rhs);
    string2 operator  +(const string2& rhs) const;
    string2& operator +=(const string2& rhs);
    bool operator     ==(const string2& rhs) const;
    bool operator     !=(const string2& rhs) const;
    char operator     [](int i) const;
    char& operator     [](int i);
    friend std::ostream& operator <<(std::ostream& ss, string2 b);
    friend std::istream& operator >>(std::istream& ss, string2& b);

    char at(int i) const;
    char& at_ref(int i);
    int index_of(char c, int start_index) const;
    int size() const;
    size_t find(const string2& substr, int pos = 0) const;
    bool contains(char c) const;
    bool contains(const string2& s, int start_index) const;
    bool contains(const string2& s) const;
    string2 substr(size_t pos, size_t size) const;
    string2 slice(int l, int r) const;
    string2 trim() const;
    std::vector<string2> split(string2 delimiter, bool include_empty_tokens = false) const;
    std::vector<string2> split_unless_between(string2 delimiter, const std::vector<string2>& pairs, bool include_empty_tokens = false) const;
    string2 replace(const string2& from, const string2& to) const;
    static string2 join(const std::vector<string2>& parts, string2 joiner);
    string2 upper() const;
    string2 lower() const;
    string2 title(const string2& whitespace = " \t\n") const;
    int to_i() const;
    long to_l() const;
    double to_d() const;
    float to_f() const;

    using iterator = std::string::iterator;
    using const_iterator = std::string::const_iterator;

    iterator begin();
    iterator end();
    std::reverse_iterator<iterator> rbegin();
    std::reverse_iterator<iterator> rend();
    const_iterator cbegin() const;
    const_iterator cend() const;
    std::reverse_iterator<const_iterator> crbegin() const;
    std::reverse_iterator<const_iterator> crend() const;
};

bool operator ==(const std::string& lhs, const string2& rhs);
bool operator !=(const std::string& lhs, const string2& rhs);
string2 operator +(const std::string& lhs, const string2& rhs);
std::string& operator +=(std::string& lhs, const string2& rhs);

namespace std {
    template<> struct hash<string2> {
        size_t operator()(string2 const& t) const {
            const std::hash<std::string> hasher;
            return hasher(t.str());
        }
    };
}