#pragma once
#include "util/json/obj_archive.h"

using RichTextStyle = ObjArchive;

class RichTextStyles : public ObjArchive {
public:
    RichTextStyles();
    RichTextStyles(const string2& styles);
};

struct RichTextChar {
    char c;
    const RichTextStyle& style;

    string2 get_str(string2 key, string2 default_val) const;
    int get_int(string2 key, int default_val) const;
    float get_float(string2 key, float default_val) const;

    RichTextChar(char c, const RichTextStyle& style): c(c), style(style) {}
};

class RichText {
private:
    // TODO: Styles will be resources one day.
    // The RichText shouldn't be responsible for a style's lifecycle.
    RichTextStyles styles;

    string2 text;
    std::vector<RichTextStyle> style_list;
    std::vector<int> style_invchmap;

    void make(const string2& xml);
public:
    RichText();
    RichText(const string2& xml);
    RichText(const string2& xml, RichTextStyles styles);
    RichTextChar at(int i) const;
    RichTextChar operator[](int i) const;
    size_t size() const;
    const string2& get_text() const;
};