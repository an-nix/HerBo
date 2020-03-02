#include "HtmlGen.h"

HtmlGenerator::HtmlGenerator(const char* title) {

    int i = strlen(title);
    _headContent = new char[i];
    memcpy(_headContent,title,i);
    
}

void HtmlGenerator::add_content(char* content){    
    int i = strlen(content);
    _bodyContent = new char[i];
    memcpy(_bodyContent,content,i);
}

void HtmlGenerator::clear_content(){

}

int HtmlGenerator::get_html(char* html){
    
    
    int i = strlen(_bodyContent) + strlen(_body);
    char* _b = new char[i];
    sprintf(_b,_body,_bodyContent);

    int j = strlen(_headContent) + strlen(_head);
    char* _h = new char[j];
    sprintf(_h,_head,_headContent);

    int _length = i + j + strlen(_doc);
    html = new char[_length];
    sprintf(html,_doc,_h,_b);

    return _length;
}
