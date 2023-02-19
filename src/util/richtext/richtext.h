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

    RichTextNode();
    RichTextNode(const string2& text): 
        text(text), 
        type(RICH_TEXT_NODE_TEXT) {}
    RichTextNode(const RichTextStyle& style_delta): 
        style_delta(style_delta), 
        type(RICH_TEXT_NODE_TAG) {}

    bool is_text() const { return type == RICH_TEXT_NODE_TEXT; }
    bool is_style() const { return type == RICH_TEXT_NODE_TAG; }
};

struct RichTextTraversalNode {
    enum {RICH_TEXT_TRAVERSAL_NODE_TEXT, RICH_TEXT_TRAVERSAL_NODE_TAG};
    int type = RICH_TEXT_TRAVERSAL_NODE_TEXT;

    RichTextTraversalNode(const RichTextNode& rn) {
        if (rn.is_text()) {
            text = rn.text;
            type = RICH_TEXT_TRAVERSAL_NODE_TEXT;
        } else {
            style = rn.style_delta; // TODO: Change once deltas are involved.
            type = RICH_TEXT_TRAVERSAL_NODE_TAG;
        }
    }

    string2 text;
    RichTextStyle style;
    bool is_text() const { return type == RICH_TEXT_TRAVERSAL_NODE_TEXT; }
    bool is_style() const { return type == RICH_TEXT_TRAVERSAL_NODE_TAG; }
};

struct RichTextChar {
    char c;
    const RichTextStyle& style;

    string2 get_str(string2 key, string2 default_val) const {
        if (style.has(key)) {
            return style[key].to<string2>();
        }
        return default_val;
    }

    RichTextChar(char c, const RichTextStyle& style): c(c), style(style) {}
};

class RichText {
private:
    RichTextStyles styles;

    string2 text;
    std::vector<RichTextStyle> style_list;
    std::vector<int> style_invchmap;

    bool is_tag(const string2& s) const;
    RichTextNode build_node(const string2& s, std::vector<RichTextStyle>& style_stack) const;
    RichTextNode build_node_text(const string2& s, std::vector<RichTextStyle>& style_stack) const;
    RichTextNode build_node_tag(const string2& s, std::vector<RichTextStyle>& style_stack) const;
public:
    RichText();
    RichText(const string2& xml, RichTextStyles styles);
    void make(const string2& xml);
    RichTextChar at(int i) const;
    size_t size() const;
};