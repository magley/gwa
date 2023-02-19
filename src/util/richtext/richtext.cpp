#include "richtext.h"

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
    const std::vector<string2> parts = split_by_tags_inclusive(xml);
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

    // Better:

    text = "";
    style_list.clear();
    style_invchmap.clear();

    for (const RichTextNode& node : nodes) {
        if (node.is_text()) {
            text += node.text;
        } else {
            style_list.push_back(node.style_delta); // TODO: Delta vs full?
            style_invchmap.push_back(text.size());
        }
    }
}

std::vector<string2> RichText::split_by_tags_inclusive(const string2& xml) const {
    std::vector<string2> result;
    string2 token = "";
    int i = -1;
    bool in_tag = false;

    while (i < xml.size() - 1) {
        i++;
        char c = xml[i];

        if (c == '<' && !in_tag) {
            in_tag = true;
            if (token != "") {
                result.push_back(token);
            }
            token = "";
        } 

        token += c;
        
        if (c == '>' && in_tag) {
            in_tag = false;
            if (token != "") {
                result.push_back(token);
            }
            token = "";
        } 
    }

    if (token != "") {
        result.push_back(token);
    }

    return result;
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
    RichTextStyle style;
    if (style_index != -1) {
        style = style_list[style_index];
    }

    return RichTextChar(c, style);
}