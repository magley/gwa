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
                    "color": "#FF0000"
                },
                "blue": {
                    "color": "#0000FF"
                }
            }
        )";

        const string2 text_xml = "Hello <red>world <blue>ABCD</blue></red>!";

        RichTextStyles styles(styles_json);
        RichText rt(text_xml, styles);
    }
};