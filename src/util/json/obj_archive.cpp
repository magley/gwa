#include "obj_archive.h"

string2 ObjArchive::to_str() const {
    switch (type) {
        case TYPE_LITERAL:
            return literal_to_str();
        case TYPE_ARRAY:
            return array_to_str();
        case TYPE_MAP:
            return map_to_str();
        default:
            return "";
    }
}

string2 ObjArchive::literal_to_str() const {
    return literal;
}

string2 ObjArchive::array_to_str() const {
    return "[" + string2::join(array, ", ") + "]";
}

string2 ObjArchive::map_to_str() const {
    std::stringstream ss;
    ss << "{";

    unsigned i = 0;
    for (const auto& it : map) {
        ss << "\"" << it.first << "\": " << it.second;
        if (i < map.size() - 1) {
            ss << ", ";
        }
        i++;
    }
    ss << "}";
    return ss.str();
}

void ObjArchive::save(ObjArchive& ar) const {
    ar = *this;
}

void ObjArchive::load(const ObjArchive& ar) {
    *this = ar;
}