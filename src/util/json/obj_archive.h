#pragma once

#include "util/string/string2.h"
#include "iserializable.h"
#include <unordered_map>
#include <sstream>

struct ObjArchive : ISerializable {
public:
    template<typename T>
    void put(const string2& key, const T* const o) {
        map[key] = to_str(o);
        type = TYPE_MAP;
    }

    template<typename T>
    void get(const string2& key, T* o) const {
        if (type != TYPE_MAP) {
            throw "Not a map.";
        }
        const string2 res = map.at(key); // Will throw if key not found.
        from_str(res, o);
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

//---------------------------------------------------------------------------------------- to_str()

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
        o->save(ar);
        return ar.to_str();
    }

    template<typename T>
    static string2 to_str(const std::vector<T>* const o) {
        ObjArchive ar;
        ar.insert(o);
        return ar.to_str();
    }

    string2 to_str() const;

//-------------------------------------------------------------------------------------- from_str()

    static void from_str(const string2& s, string2* o) {
        *o = s.slice(1, -2);
    }

    template<typename T>
    static typename std::enable_if<std::is_arithmetic<T>::value, void>::type
    from_str(const string2& s, T* o) {
        std::stringstream ss;
        ss << s;
        ss >> *o;
    }

    template<typename T>
    static typename std::enable_if<std::is_base_of<ISerializable, T>::value, void>::type
    from_str(const string2& s, T* o) {
        ObjArchive ar;
        ar.from_str(s);
        o->load(ar);
    }

    template<typename T>
    static typename std::enable_if<std::is_base_of<ISerializable, T>::value, void>::type
    from_str(const string2& s, std::vector<T>* o) {
        ObjArchive ar;
        ar.from_str(s);
        for (const string2& arr_element_str : ar.array) {
            T t;
            from_str(arr_element_str, &t);
            o->push_back(t);
        }
    }

    void from_str(const string2& s);

//------------------------------------------------------------------------------------------- other

    void save(ObjArchive& ar) const;
    void load(const ObjArchive& ar);

    string2 literal_to_str() const;
    string2 array_to_str() const;
    string2 map_to_str() const;

    void str_to_literal(const string2& s);
    void str_to_array(const string2& s);
    void str_to_map(const string2& s);
    int infer_type_from_str(const string2& s) const;
// private:
    enum { TYPE_LITERAL, TYPE_ARRAY, TYPE_MAP };
    int type = TYPE_LITERAL;
    string2 literal;
    std::vector<string2> array;
    std::unordered_map<string2, string2> map;
};