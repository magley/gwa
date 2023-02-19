#pragma once

#include "richtext.h"
#include "util/test/test2.h"

class richtext_Test : public test2::Test2 {
    void run_tests() {
        EXECUTE_TEST(idk);
    }

    void idk() {
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
            const RichTextChar node = rt.at(i);
            const string2 color = node.get_str("color", "");

            if (color == "#FF0000") printf(ANSI_RED);
            else if (color == "#0000FF") printf(ANSI_CYAN);
            else printf(ANSI_RESET);
            printf("%c", node.c);
        }
    }
};