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
    const std::vector<string2> parts = xml.split_by_tags();
    std::vector<RichTextNode> nodes;
    std::vector<RichTextStyle> style_stack;

    for (const string2& s : parts) {
        RichTextNode n;

        if (is_tag(s)) {
            n.type = RichTextNode::RICH_TEXT_NODE_TAG;
            const bool is_closing_tag = s[1] == '/';

            // TODO: No need to store all styles.
            if (is_closing_tag && style_stack.size() > 0) {
                style_stack.pop_back();
                if (style_stack.size() > 0) {
                    n.style_delta = style_stack[style_stack.size() - 1];
                }
            } else {
                RichTextStyle style = styles[s.slice(1, -2)];

                if (style_stack.size() > 0) {
                    style = style_stack[style_stack.size() - 1].cascade(style);
                }
                style_stack.push_back(style);

                n.style_delta = style;
            }
        } else {
            n.type = RichTextNode::RICH_TEXT_NODE_TEXT;
            n.text = s;
        }

        nodes.push_back(n);
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