#pragma once

#include "util/string/string2.h"
#include "iserializable.h"
#include <unordered_map>
#include <sstream>

struct ObjArchive {
public:
    template<typename T>
    void put(const string2& key, const T* const o) {
        map[key] = to_str(o);
        type = TYPE_MAP;
    }

    template<typename T>
    void insert(const std::vector<T>* arr) {
        map.clear();
        array.clear();
        for (const auto& t : *arr) {
            array.push_back(to_str(&t));
        }
        type = TYPE_ARRAY;
    }

    static string2 to_str(const string2* const o) {
        return "\"" + *o + "\"";
    }

    template<typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, string2>::type
    to_str(const T* const o) {
        std::stringstream ss;
        ss << *o;
        return ss.str();
    }

    template<typename T>
    static typename std::enable_if<std::is_base_of<ISerializable, T>::value, string2>::type
    to_str(const T* const o) {
        ObjArchive ar;
        o->load(&ar);
        return ar.to_str();
    }

    template<typename T>
    static string2 to_str(const std::vector<T>* const o) {
        ObjArchive ar;
        ar.insert(o);
        return ar.to_str();
    }

    string2 to_str() const;
private:
    string2 literal_to_str() const;
    string2 array_to_str() const;
    string2 map_to_str() const;

    enum { TYPE_LITERAL, TYPE_ARRAY, TYPE_MAP };
    int type = TYPE_LITERAL;
    string2 literal;
    std::vector<string2> array;
    std::unordered_map<string2, string2> map;
};