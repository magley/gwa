#pragma once
#include "util/json/obj_archive.h"

class RichTextStyles : public ObjArchive {
public:
    RichTextStyles();
    RichTextStyles(const string2& styles);
};

using RichTextStyle = ObjArchive;

struct RichTextNode {
    string2 text;
    RichTextStyle style_delta;
    enum {RICH_TEXT_NODE_TEXT, RICH_TEXT_NODE_TAG};
    int type = RICH_TEXT_NODE_TEXT;
};

class RichText {
private:
    std::vector<RichTextNode> nodes;
    RichTextStyles styles;

    bool is_tag(const string2& s) const;
    std::vector<string2> split_by_tags_inclusive(const string2& xml) const;
public:

    RichText();
    RichText(const string2& xml, RichTextStyles styles);
    void make(const string2& xml);
};