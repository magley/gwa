#include "string2.h"

static int index_of_strlist(std::vector<string2> strings, char c) {
    for (int i = 0; i < strings.size(); i++) {
        if (strings[i].contains(c)) {
            return i;
        }
    }
    return -1;
}

static void calc_pair_sum(char c, const string2& pair, int* symmetric_pair, int* pair_sum) {
    // If opening char, increment pair_sum. If closing, decrement.
    // If opening and closing are the same, determine from symmetric_pair and flip that flag.

    if (pair[0] == pair[1]) {
        if (*symmetric_pair == 1) {
            (*pair_sum)--;
        } else {
            (*pair_sum)++;
        }

        *symmetric_pair = 1 - *symmetric_pair;
    } else {
        if (pair[0] == c) {
            (*pair_sum)++;
        } else if (pair[1] == c) {
            (*pair_sum)--;
        }
    }
}


string2::string2() {
}

string2::string2(char c) {
    _s = c;
}

string2::string2(const char* s): _s(s) {
}

string2::string2(const std::string& s): _s(s) {
}

string2::string2(const string2& s): _s(s._s) {
}

std::string string2::str() const {
    return _s;
}

std::string& string2::str_ref() {
    return _s;
}

std::string* string2::str_ptr() {
    return &_s;
}

const char* string2::c_str() const {
    return _s.c_str();
}

string2& string2::operator =(const string2& rhs) {
    _s = rhs._s;
    return *this;
}

string2 string2::operator +(const string2& rhs) const {
    return _s + rhs._s;
}

string2 operator +(const std::string& lhs, const string2& rhs) {
    return lhs + rhs.str();
}

string2& string2::operator +=(const string2& rhs) {
    _s += rhs._s;
    return *this;
}

std::string& operator +=(std::string& lhs, const string2& rhs) {
    lhs += rhs.str();
    return lhs;
}

bool string2::operator ==(const string2& rhs) const {
    return _s == rhs._s;
}

bool operator ==(const std::string& a, const string2& b) {
    return a == b.str();
}

bool string2::operator !=(const string2& rhs) const {
    return _s != rhs._s;
}

bool operator !=(const std::string& a, const string2& b) {
    return a != b.str();
}

char string2::operator [](int i) const {
    return at(i);
}

std::ostream& operator<<(std::ostream& ss, string2 b) {
    ss << b._s;
    return ss;
}

std::istream& operator>>(std::istream& ss, string2& b) {
    ss >> b._s;
    return ss;
}

char string2::at(int i) const {
    i = (size() + (i % size())) % size();
    return _s[i];
}

int string2::index_of(char c, int start_index) const {
    for (int i = start_index; i < size(); i++) {
        if (at(i) == c) {
            return i;
        }
    }
    return -1;
}

int string2::size() const {
    return _s.size();
}

bool string2::contains(char c) const {
    for (int i = 0; i < size(); i++) {
        if (at(i) == c) {
            return true;
        }
    }
    return false;
}

bool string2::contains(const string2& s, int start_index) const {
    const int end_index = s.size() + start_index;
    if (end_index > size()) {
        return false;
    }
    for (int i = 0; i < s.size(); i++) {
        if (s[i] != _s[start_index + i]) {
            return false;
        }
    }
    return true;
}

bool string2::contains(const string2& s) const {
    return (find(s) != npos);
}

size_t string2::find(const string2& substr, int pos) const {
    return _s.find(substr._s, pos);
}

string2 string2::substr(size_t pos, size_t size) const {
    return _s.substr(pos, size);
}

string2 string2::slice(int l, int r) const {
    l = (size() + (l % size())) % size();
    r = (size() + (r % size())) % size();
    return substr(l, r - l + 1); 
}

string2 string2::trim() const {
    if (size() == 0) {
        return *this;
    }

    int l = 0;
    for (l = 0; l < size(); l++) {
        char c = at(l);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r') {
        } else {
            break;
        }
    }
    int r = size() - 1;
    for (r = size() - 1; r >= 0; r--) {
        char c = at(r);
        if (c == ' ' || c == '\t' || c == '\n' || c == '\f' || c == '\r') {
        } else {
            break;
        }
    }

    if (l > r) {
        return "";
    }

    return substr(l, r - l + 1);
}

std::vector<string2> string2::split_unless_between(string2 delim, const std::vector<string2>& pairs, bool include_empty_tokens) const {
    std::vector<string2> result;
    string2 token;

    int pair_sum = 0;
    int symmetric_pair[pairs.size()] = {0};

    int i = -1;
    while (i < size() - 1) {
        i++;
        char c = at(i);

        const int pair_index = index_of_strlist(pairs, c);
        if (pair_index != -1) {
            calc_pair_sum(c, pairs[pair_index], &symmetric_pair[pair_index], &pair_sum);
        }

        if (pair_sum == 0 && contains(delim, i)) {
            if (token != "" || include_empty_tokens) {
                result.push_back(token);
                token = "";
            }
        } else {
            token += c;
        }
    }

    if (token != "" || include_empty_tokens) {
        result.push_back(token);
    }

    return result;  
}

std::vector<string2> string2::split(string2 delim, bool include_empty_tokens) const {
    return split_unless_between(delim, {}, include_empty_tokens);
}

string2 string2::replace(const string2& from, const string2& to) const {
    string2 res = *this;
    if (from.size() == 0) {
        return res;
    }
    size_t start_pos = 0;
    while ((start_pos = res.find(from, start_pos)) != npos) {
        res._s.replace(start_pos, from.size(), to._s);
        start_pos += to.size();
    }

    return res;
}

string2 string2::join(const std::vector<string2>& parts, string2 joiner) {
    string2 res;
    for (int i = 0; i < parts.size() - 1; i++) {
        res += parts[i] + joiner;
    }
    res += parts[parts.size() - 1];
    return res;
}

string2::iterator string2::begin() {
    return _s.begin();
}

string2::iterator string2::end() {
    return _s.end();
}

std::reverse_iterator<string2::iterator> string2::rbegin() {
    return _s.rbegin();
}

std::reverse_iterator<string2::iterator> string2::rend() {
    return _s.rend();
}

string2::const_iterator string2::cbegin() const {
    return _s.cbegin();
}

string2::const_iterator string2::cend() const {
    return _s.cend();
}

std::reverse_iterator<string2::const_iterator> string2::crbegin() const {
    return _s.crbegin();
}

std::reverse_iterator<string2::const_iterator> string2::crend() const {
    return _s.crend();
}