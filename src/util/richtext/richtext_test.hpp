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
                "red-wave": {
                    "color": "#FF0000",
                    "wave": "fast"
                },
                "blue": {
                    "color": "#0000FF"
                }
            }
        )";

        const string2 text_xml = "Hello <red-wave>world <blue>AB<red-wave>CD</red-wave></blue></red-wave>!";

        RichTextStyles styles(styles_json);
        RichText rt(text_xml, styles);

        rt.traverse([](const RichTextTraversalNode& node) {
            if (node.is_text()) {
                printf("[%s]\n", node.text.c_str());
            } else {
                printf("{%s}\n", node.style.to_str().c_str());
            }
        });
    }
};