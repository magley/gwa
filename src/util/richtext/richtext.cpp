#include "richtext.h"
#include <cassert>


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

static RichTextNode build_node(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles);
static RichTextNode build_node_text(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles);
static RichTextNode build_node_tag(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles);
static bool is_tag(const string2& s);


RichTextStyles::RichTextStyles() {
}

RichTextStyles::RichTextStyles(const string2& styles) {
    from_str(styles);
}

RichTextNode::RichTextNode() {

}

RichTextNode::RichTextNode(const string2& text): text(text), type(RICH_TEXT_NODE_TEXT) {

}

RichTextNode::RichTextNode(const RichTextStyle& style_delta): 
    style(style_delta), type(RICH_TEXT_NODE_TAG) {
}

RichText::RichText(): styles("{}") {
}

RichText::RichText(const string2& xml): styles("{}") {
    make(xml);
}

RichText::RichText(const string2& xml, RichTextStyles styles): styles(styles) {
    make(xml);
}

size_t RichText::size() const {
    return text.size();
}

void RichText::make(const string2& xml) {
    // Including an empty style at the start of the rich-text, it is guaranteed that each character
    // in the string has one style it can map to. Thanks to this, RichTextChar can take a reference
    // (never null) to a RichTextStyle instead of copying or using pointers.
    style_list.push_back(RichTextStyle());
    style_invchmap.push_back(0);

    std::vector<RichTextStyle> style_stack;

    for (const string2& s : xml.split_by_tags()) {
        const RichTextNode node = build_node(s, style_stack, &styles);

        if (node.is_text()) {
            text += node.text;
        } else {
            style_list.push_back(node.style);
            style_invchmap.push_back(text.size());
        }
    }

    text = text.replace("&lt;", "<").replace("&gt;", ">");
}

static RichTextNode build_node(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles) {
    if (is_tag(s)) {
        return build_node_tag(s, style_stack, styles);
    }
    return build_node_text(s, style_stack, styles);
}

static RichTextNode build_node_text(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles) {
    return RichTextNode(s);
}

static RichTextNode build_node_tag(const string2& s, std::vector<RichTextStyle>& style_stack, RichTextStyles* styles) {
    if (styles->count() == 0) {
        return RichTextNode(RichTextStyle());
    }

    const bool is_closing_tag = s[1] == '/';

    if (is_closing_tag && style_stack.size() > 0) {
        style_stack.pop_back();
        if (style_stack.size() > 0) {
            return RichTextNode(style_stack[style_stack.size() - 1]);
        } else {
            return RichTextNode(RichTextStyle());
        }
    } else {
        RichTextStyle style = (*styles)[s.slice(1, -2)];
        if (style_stack.size() > 0) {
            style = style_stack[style_stack.size() - 1].cascade(style);
        }
        style_stack.push_back(style);

        return RichTextNode(style);
    }
}

static bool is_tag(const string2& s) {
    return (s[0] == '<' && s[-1] == '>');
}

static int find_first_greater_than(int what, const std::vector<int>& v) {
    for (int i = 0; i < v.size(); i++) {
        if (what < v[i]) {
            return i - 1;
        }
    }
    return -1;
}

RichTextChar RichText::at(int i) const {
    const char c = text[i];
    const int style_index = find_first_greater_than(i, style_invchmap);
    assert(style_index != -1);
    return RichTextChar(c, style_list[style_index]);
}

RichTextChar RichText::operator[](int i) const {
    return at(i);
}

const string2& RichText::get_text() const {
    return text;
}

string2 RichTextChar::get_str(string2 key, string2 default_val) const {
    if (style.has(key)) {
        return style[key].to<string2>();
    }
    return default_val;
}

int RichTextChar::get_int(string2 key, int default_val) const {
    if (style.has(key)) {
        return style[key].to<int>();
    }
    return default_val;
}

float RichTextChar::get_float(string2 key, float default_val) const {
    if (style.has(key)) {
        return style[key].to<float>();
    }
    return default_val;
}