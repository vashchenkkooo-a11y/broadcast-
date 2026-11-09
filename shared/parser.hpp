#include <string>
#include <boost/json.hpp>
class parser {

public:

boost::json::object parse(std::string json);
};