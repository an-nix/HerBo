#ifndef _HTML_GENERATOR_HEADER_
#define _HTML_GENERATOR_HEADER_

#include <string.h>
#include <stdio.h>

#define HTML_BUFFER 4096

static const char* _doc = R"(
    <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN"
        "http://www.w3.org/TR/html4/strict.dtd">
    <HTML>
        %s
        %s
    </HTML>
)";

static const char* _head = R"(
    <head>
      <title>%s</title>
   </head>
)";

static const char* _body = R"(
    <body>
        %s
    </body>
)";

class HtmlGenerator {

    private:
        char* _buffer;
        char* _bodyContent;
        char* _headContent;

    public:
        //Constructor
        HtmlGenerator(const char*);

        void add_content(char*);
        void clear_content();
        int get_html(char*);
};

#endif