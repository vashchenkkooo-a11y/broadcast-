#include "parser.hpp"


boost::json::object parser::parse(std::string json) {
    boost::json::value jv = boost::json::parse(json);
    boost::json::object obj = jv.as_object();
    return obj;
}