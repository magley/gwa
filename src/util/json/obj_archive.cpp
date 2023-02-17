#include "obj_archive.h"


int ObjArchive::get_type() const {
    return type; 
}

int ObjArchive::get_err() const {
    return err; 
}

string2 ObjArchive::to_str() const {
    switch (type) {
        case TYPE_LITERAL:
            return literal_to_str();
        case TYPE_ARRAY:
            return array_to_str();
        case TYPE_MAP:
            return map_to_str();
        case TYPE_ERROR:
            return "???";
        default:
            return "";
    }
}

void ObjArchive::from_str(const string2& s) {
    string2 s_sanitized = s.trim();
    type = infer_type_from_str(s_sanitized);
    switch (type) {
        case TYPE_LITERAL:
            str_to_literal(s_sanitized);
            break;
        case TYPE_ARRAY:
            str_to_array(s_sanitized);
            break;
        case TYPE_MAP:
            str_to_map(s_sanitized);
            break;
        case TYPE_ERROR:
            throw ObjArchiveException(literal);
            break;
        default:
            break;
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

void ObjArchive::str_to_literal(const string2& s) {
    literal = s;
}

void ObjArchive::str_to_array(const string2& s) {
    const string2 ss = s.slice(1, -2);
    std::vector<string2> parts = ss.split_unless_between(",", {"{}", "[]", "\"\""});
    for (string2& part : parts) {
        part = part.trim();
    }
    this->array = parts;
}

void ObjArchive::str_to_map(const string2& s) {
    const std::vector<string2> unless_pairs = {"{}", "[]", "\"\""};
    const string2 ss = s.slice(1, -2);
    std::vector<string2> keyvals = ss.split_unless_between(",", unless_pairs);
    for (string2& keyval : keyvals) {
        keyval = keyval.trim();
    }

    for (const string2& keyval : keyvals) {
        const std::vector<string2> keyval_tokens = keyval.split_unless_between(":", unless_pairs);
        const string2 key = keyval_tokens[0].trim().slice(1, -2); // No need for quotes.
        const string2 val = keyval_tokens[1].trim();
        map[key] = val;
    }
}

int ObjArchive::infer_type_from_str(const string2& s) {
    if (s[0] == '{' && s[-1] == '}') {
        return TYPE_MAP;
    }

    if (s[0] == '[' && s[-1] == ']') {
        return TYPE_ARRAY;
    }

    if ((s[0] == '{') != (s[-1] == '}')) {
        const string2 ansi_red = "\033[0;31m";
        const string2 ansi_white = "\033[0;37m";
        literal = ansi_red + "Unmatched braces in json:\n"  + ansi_white + s;
        err = ERR_BRACE_MISMATCH;
        return TYPE_ERROR;
    }

    if ((s[0] == '[') != (s[-1] == ']')) {
        const string2 ansi_red = "\033[0;31m";
        const string2 ansi_white = "\033[0;37m";
        literal = ansi_red + "Unmatched brackets in json:\n" + ansi_white + s;
        err = ERR_BRACKET_MISMATCH;
        return TYPE_ERROR;
    }


    return TYPE_LITERAL;
}