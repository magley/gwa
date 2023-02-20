#pragma once

#include "richtext.h"
#include "util/test/test2.h"

class richtext_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(example);

        EXECUTE_TEST(should_properly_contain_the_text);
        EXECUTE_TEST(should_not_crash_if_styles_missing);
    }

    void example() {
        const string2 styles_json = R"(
            {
                "red": {
                    "color": "#FF0000",
                    "wave": "fast"
                },
                "blue": {
                    "color": "#0000FF"
                }
            }
        )";

        const string2 text_xml = "Hello <red>world <blue>AB</blue>!</red>";

        RichTextStyles styles(styles_json);
        RichText rt(text_xml, styles);

        for (int i = 0; i < rt.size(); i++) {
            const RichTextChar node = rt[i];
            const string2 color = node.get_str("color", "");

            if (color == "#FF0000") printf(ANSI_RED);
            else if (color == "#0000FF") printf(ANSI_CYAN);
            else printf(ANSI_RESET);
            printf("%c", node.c);
        }
    }

    void should_properly_contain_the_text() {
        const string2 styles_json = R"(
            {
                "red": {
                    "color": "#FF0000",
                    "wave": "fast"
                },
                "blue": {
                    "color": "#0000FF"
                }
            }
        )";

        const string2 text_xml = "Hello <red>world <blue>AB</blue>!</red>";
        const string2 text_just_text = "Hello world AB!";

        RichTextStyles styles(styles_json);
        RichText rt(text_xml, styles);

        ASSERT2_EQ(text_just_text, rt.get_text());
    }

    void should_not_crash_if_styles_missing() {
        const string2 text_xml = "Hello <red>world <blue>AB</blue>!</red>";
        const string2 text_just_text = "Hello world AB!";

        RichText rt(text_xml);

        ASSERT2_EQ(text_just_text, rt.get_text());
    }
};