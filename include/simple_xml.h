#ifndef SIMPLE_XML_H_
#define SIMPLE_XML_H_

#include <string>
#include <vector>

#include "../rapidxml-1.13/rapidxml_print.hpp"

class SimpleXML {
  public:
    explicit SimpleXML(const std::string source);
    virtual ~SimpleXML();

    SimpleXML& Node(const std::string& node_name);
    SimpleXML& Node(const char* node_name);
    SimpleXML& SetValue(const std::string& value);
    SimpleXML& SetValue(const char* value);
    SimpleXML& AppendAttribute(const std::string& attr_name,
                               const std::string& value);
    SimpleXML& AppendAttribute(const char* attr_name, const char* value);
    SimpleXML& ResetAttributes();

    std::string GetValue();
    std::string GetXML();

  private:
     rapidxml::xml_document<> raw_xmldoc_;
     mutable rapidxml::xml_node<> *cur_node_;
     std::string string_buf_;
     std::vector<char> vector_buf_;

};

#endif //SIMPLE_XML_H_
