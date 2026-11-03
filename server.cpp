#include <ctime>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <boost/asio.hpp> 

using boost::asio::ip::tcp;
std::string make_daytime_string()
{
  using namespace std; // For time_t, time and ctime;
  time_t now = time(0);
  return ctime(&now);
}

class tcp_connection
  : public std::enable_shared_from_this<tcp_connection>
{
public:
  typedef std::shared_ptr<tcp_connection> pointer;

  static pointer create(boost::asio::io_context& io_context) //функцйия эта принимает ссылку на вектор и перекидывает ссылку на вектор 
  {
    return pointer(new tcp_connection(io_context)); //
  }

  tcp::socket& socket()
  {
    return socket_;
  }

  void start()
  { 
    read(boost::asio::buffer(buf_));

  }

   void write(boost::asio::const_buffer b) {
      socket_.async_write_some(b,
        std::bind(&tcp_connection::handle_write, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred));
    }

    void broad_cast (); //если тот, на кого мы смотрим - это не я -> write();

  
private:
  tcp_connection(boost::asio::io_context& io_context)
    : socket_(io_context) //cюда вектор надо добавить (чтоб тсп коннекнш кроме сокета и ио контекста имел еще ссылку на вектор)
  {
  }


  void read(boost::asio::mutable_buffer b) {
        socket_.async_read_some(b,
          std::bind(&tcp_connection::handle_read, shared_from_this(),
          boost::asio::placeholders::error,
          boost::asio::placeholders::bytes_transferred)); 
  }

  void handle_read(const boost::system::error_code& error,
     std::size_t bytes_transferred) {
      if(error) 
      return;
     std::cout.write(buf_.data(), bytes_transferred);
     std::cout << std::endl;
      //вместо этой функции(что ниже) вызывать бродкаст
      write(boost::asio::buffer(buf_, bytes_transferred)); //в сокет типу кидаем обратно(в его терминал)

    }
  void handle_write(const boost::system::error_code& /*error*/,
      size_t /*bytes_transferred*/)
  {
    read(boost::asio::buffer(buf_));
  }
  tcp::socket socket_;
  std::string message_;
  std::array<char, 512> buf_;
};

class tcp_server
{
public:
  tcp_server(boost::asio::io_context& io_context)
    : io_context_(io_context),
      acceptor_(io_context, tcp::endpoint(tcp::v4(), 4567))
  {
    start_accept();
  }
private:
  void start_accept()
  {
    tcp_connection::pointer new_connection =
      tcp_connection::create(io_context_);
    //после успешного коннекшина добалвять в вектор тут и каждый тсп коннекшнин будет с ссылкой на вектор 

    acceptor_.async_accept(new_connection->socket(),
        std::bind(&tcp_server::handle_accept, this, new_connection,
          boost::asio::placeholders::error));
  }

  void handle_accept(tcp_connection::pointer new_connection,
      const boost::system::error_code& error)
  {
    if (!error)
    {
      new_connection->start();
    }

    

    start_accept();
  }

  boost::asio::io_context& io_context_;
  tcp::acceptor acceptor_;
  //тут будет вектор с тсп коннекшинами
};

int main()
{
  try
  {
    boost::asio::io_context io_context;
    tcp_server server(io_context);
    io_context.run();
  }
  catch (std::exception& e)
  {
    std::cerr << e.what() << std::endl;
  
  return 0;
}
}
