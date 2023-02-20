#pragma once
#include "util/json/obj_archive.h"

using RichTextStyle = ObjArchive;

class RichTextStyles : public ObjArchive {
public:
    RichTextStyles();
    RichTextStyles(const string2& styles);
};

struct RichTextNode {
    string2 text;
    RichTextStyle style;
    enum {RICH_TEXT_NODE_TEXT, RICH_TEXT_NODE_TAG};
    int type = RICH_TEXT_NODE_TEXT;

    RichTextNode();
    RichTextNode(const string2& text);
    RichTextNode(const RichTextStyle& style_delta);

    bool is_text() const { return type == RICH_TEXT_NODE_TEXT; }
    bool is_style() const { return type == RICH_TEXT_NODE_TAG; }
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
    // TODO: In the future, a style will be a resource. RichText shouldn't keep track of styles.
    RichTextStyles styles;

    string2 text;
    std::vector<RichTextStyle> style_list;
    std::vector<int> style_invchmap;

    bool is_tag(const string2& s) const;
    RichTextNode build_node(const string2& s, std::vector<RichTextStyle>& style_stack) const;
    RichTextNode build_node_text(const string2& s, std::vector<RichTextStyle>& style_stack) const;
    RichTextNode build_node_tag(const string2& s, std::vector<RichTextStyle>& style_stack) const;
    void make(const string2& xml);
public:
    RichText();
    RichText(const string2& xml);
    RichText(const string2& xml, RichTextStyles styles);
    RichTextChar at(int i) const;
    size_t size() const;
    const string2& get_text() const;
};