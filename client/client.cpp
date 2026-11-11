#include "client.hpp"
//только сеть

using boost::asio::ip::tcp;
client::client(boost::asio::io_context &io_context) 
    : socket(io_context)
{
    // boost::asio::connect(socket, endpoints);
}

void client::connect(tcp::resolver::results_type& endpoints) {
    boost::asio::connect(socket, endpoints);
}

void client::write(const std::string& request) {
    boost::system::error_code error;
    boost::asio::write(socket, boost::asio::buffer(request), error);
    if (error) throw boost::system::system_error(error);
}


std::string client::read() {
    std::array<char, 128> buf;
    boost::system::error_code error;
    size_t len = socket.read_some(boost::asio::buffer(buf), error);
    if (error) throw boost::system::system_error(error);
    return std::string(buf.data(), len);
}
