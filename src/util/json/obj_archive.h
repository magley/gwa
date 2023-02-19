#pragma once

#include "util/string/string2.h"
#include "iserializable.h"
#include <unordered_map>
#include <sstream>

class ObjArchive : ISerializable {
public:
    template<typename T>
    void set(T* o) {
        literal = to_str(o);
        type = TYPE_LITERAL;
    }

    template<typename T>
    void get(T* o) const {
        if (type != TYPE_LITERAL) {
            throw "Not a literal.";
        }
        from_str(literal, o); 
    }

    template<typename T>
    void push(const T* const o) {
        array.push_back(to_str(o));
        type = TYPE_ARRAY;
    }

    template<typename T>
    void get(int index, T* o) const {
        if (type != TYPE_ARRAY) {
            throw "Not an array.";
        }
        const string2 res = get_raw(index);
        from_str(res, o);
    }

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
        const string2 res = get_raw(key);
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

//--------------------------------------------------------------------------------------- get_raw()

    string2 get_raw(const string2& key) const;
    string2 get_raw(int index) const;
    ObjArchive operator[](const string2& key) const;
    ObjArchive operator[](int index) const;

    template<typename T>
    T to() const {
        T t;
        from_str(to_str(), &t);
        return t;
    }

    template<typename T>
    static T from_str(const string2& s) {
        ObjArchive ar;
        ar.from_str(s);
        return ar.to<T>();
    }

//---------------------------------------------------------------------------------------- to_str()


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
    static string2 to_str(const string2* const o);

//-------------------------------------------------------------------------------------- from_str()


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

    static void from_str(const string2& s, string2* o);
    void from_str(const string2& s);

//------------------------------------------------------------------------------------------- other

    // Merge 2 ObjArchives. 
    // If overlapping keys, `other`-s value overrides.
    // Does nothing for non-maps.
    ObjArchive cascade(const ObjArchive& other) const;

    void save(ObjArchive& ar) const;
    void load(const ObjArchive& ar);

    string2 literal_to_str() const;
    string2 array_to_str() const;
    string2 map_to_str() const;

    void str_to_literal(const string2& s);
    void str_to_array(const string2& s);
    void str_to_map(const string2& s);
    int infer_type_from_str(const string2& s);
    enum { TYPE_LITERAL, TYPE_ARRAY, TYPE_MAP, TYPE_ERROR };
    enum { ERR_NONE, ERR_BRACE_MISMATCH, ERR_BRACKET_MISMATCH, ERR_BAD_KEYVAL_COUNT };

    int get_type() const;
    int get_err() const;
private:
    int type = TYPE_LITERAL;
    int err = ERR_NONE;

    string2 literal;
    std::vector<string2> array;
    std::unordered_map<string2, string2> map;
};