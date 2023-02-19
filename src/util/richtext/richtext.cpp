#include "richtext.h"
#include <cassert>

RichTextStyles::RichTextStyles() {
}

RichTextStyles::RichTextStyles(const string2& styles) {
    from_str(styles);
}

RichText::RichText() {
}

RichText::RichText(const string2& xml, RichTextStyles styles): styles(styles) {
    make(xml);
}

size_t RichText::size() const {
    return text.size();
}

void RichText::make(const string2& xml) {
    std::vector<RichTextNode> nodes;
    std::vector<RichTextStyle> style_stack;

    for (const string2& s : xml.split_by_tags()) {
        const RichTextNode node = build_node(s, style_stack);
        nodes.push_back(node);
    }

    text = "";
    style_list.clear();
    style_invchmap.clear();

    // Including an empty style at the start of the rich-text, it is guaranteed that each character
    // in the string has one style it can map to. Thanks to this, RichTextChar can take a reference
    // (never null) to a RichTextStyle instead of copying or using pointers.
    style_list.push_back(RichTextStyle());
    style_invchmap.push_back(0);

    for (const RichTextNode& node : nodes) {
        if (node.is_text()) {
            text += node.text;
        } else {
            style_list.push_back(node.style_delta); // TODO: Delta vs full?
            style_invchmap.push_back(text.size());
        }
    }
}

RichTextNode RichText::build_node(const string2& s, std::vector<RichTextStyle>& style_stack) const {
    if (is_tag(s)) {
        return build_node_tag(s, style_stack);
    }
    return build_node_text(s, style_stack);
}

RichTextNode RichText::build_node_text(const string2& s, std::vector<RichTextStyle>& style_stack) const {
    return RichTextNode(s);
}

RichTextNode RichText::build_node_tag(const string2& s, std::vector<RichTextStyle>& style_stack) const {
    const bool is_closing_tag = s[1] == '/';

    if (is_closing_tag && style_stack.size() > 0) {
        style_stack.pop_back();
        if (style_stack.size() > 0) {
            return RichTextNode(style_stack[style_stack.size() - 1]);
        } else {
            return RichTextNode(RichTextStyle());
        }
    } else {
        RichTextStyle style = styles[s.slice(1, -2)];
        if (style_stack.size() > 0) {
            style = style_stack[style_stack.size() - 1].cascade(style);
        }
        style_stack.push_back(style);

        return RichTextNode(style);
    }
}

bool RichText::is_tag(const string2& s) const {
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