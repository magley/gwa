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

void RichText::make(const string2& xml) {
    nodes.clear();
    text_size = 0;
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
            text_size += s.size();
        }

        nodes.push_back(n);
    }
}

void RichText::traverse(rich_text_traversal_callback callback) const {
    for (const RichTextNode& n : nodes) {
        RichTextTraversalNode traversal_node = RichTextTraversalNode(n);
        callback(traversal_node);
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

size_t RichText::size() const {
    return text_size;
}

RichTextChar RichText::at(int i) const {
    int cnt = 0;
    RichTextStyle last_style; // TODO: This is done differently with deltas.
    for (const RichTextNode& node : nodes) {
        if (node.is_text()) {
            if (i >= node.text.size() + cnt) {
                cnt += node.text.size();
            } else {
                return RichTextChar(
                    node.text[i - cnt],
                    last_style
                );
            }
        } else {
            last_style = node.style_delta;
        }
    }
    throw "RichText::at - Out of bounds";
}