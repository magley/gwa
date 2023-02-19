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
    RichTextStyle style;

    string2 get_str(string2 key, string2 default_val) const {
        if (style.has(key)) {
            return style[key].to<string2>();
        }
        return default_val;
    }

    RichTextChar(char c, RichTextStyle style): c(c), style(style) {}
};

using rich_text_traversal_callback = void(*)(const RichTextTraversalNode& ctx);

class RichText {
private:
    size_t text_size = 0;
    std::vector<RichTextNode> nodes;
    RichTextStyles styles;

    bool is_tag(const string2& s) const;
    std::vector<string2> split_by_tags_inclusive(const string2& xml) const;
public:

    RichText();
    RichText(const string2& xml, RichTextStyles styles);
    void make(const string2& xml);
    void traverse(rich_text_traversal_callback callback) const;
    size_t size() const;
    RichTextChar at(int i) const;
};