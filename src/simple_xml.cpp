#include "../include/simple_xml.h"

#include <string.h>
#include <iostream>

SimpleXML::SimpleXML(const std::string source) : cur_node_(0), string_buf_(source),
    vector_buf_(source.begin(), source.end()) {
    vector_buf_.push_back('\0');
    raw_xmldoc_.parse</*rapidxml::parse_non_destructive |*/
                      rapidxml::parse_no_data_nodes>(&vector_buf_[0]);
}

SimpleXML::~SimpleXML() {
    vector_buf_.clear();
    string_buf_.clear();
    raw_xmldoc_.clear();
}

SimpleXML& SimpleXML::Node(const std::string& node_name) {
    if (cur_node_ == 0) {
        cur_node_ = raw_xmldoc_.first_node(node_name.c_str());
    }
    else
    {
        cur_node_ = cur_node_->first_node(node_name.c_str());
    }

    return *this;
}

SimpleXML& SimpleXML::Node(const char* node_name) {
    if (cur_node_ == 0) {
        cur_node_ = raw_xmldoc_.first_node(node_name);
    }
    else
    {
        cur_node_ = cur_node_->first_node(node_name);
    }

    return *this;
}

std::string SimpleXML::GetValue() {
    if (cur_node_ == 0) {
        return std::string("");
    }

    return std::string(cur_node_->value());
}

SimpleXML& SimpleXML::SetValue(const std::string& value) {
    if (cur_node_ == 0) {
        return *this;
    }

    const char* text = raw_xmldoc_.allocate_string(value.c_str(), strlen(value.c_str()) + 1);

    cur_node_->value(text);

    return *this;
}


SimpleXML& SimpleXML::SetValue(const char* value) {
    if (cur_node_ == 0) {
        return *this;
    }

    const char* text = raw_xmldoc_.allocate_string((value), strlen(value));

    cur_node_->value(text);

    return *this;
}

SimpleXML& SimpleXML::AppendAttribute(const std::string& attr_name, const std::string& value) {
    if (cur_node_ == 0) {
        return *this;
    }

    rapidxml::xml_attribute<>* attr = raw_xmldoc_.allocate_attribute(attr_name.c_str(), value.c_str());

    cur_node_->append_attribute(attr);

    return *this;
}

SimpleXML& SimpleXML::AppendAttribute(const char* attr_name, const char* value) {
    if (cur_node_ == 0) {
        return *this;
    }

    rapidxml::xml_attribute<>* attr = raw_xmldoc_.allocate_attribute(attr_name, value);

    cur_node_->append_attribute(attr);

    return *this;
}

SimpleXML& SimpleXML::ResetAttributes() {
    if (cur_node_ == 0) {
        return *this;
    }

    cur_node_->remove_all_attributes();

    return *this;
}

std::string SimpleXML::GetXML() {
    string_buf_.clear();

    rapidxml::print(std::back_inserter(string_buf_), raw_xmldoc_, 0);

    cur_node_ = 0;

    return string_buf_;
}
