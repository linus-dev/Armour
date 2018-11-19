#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <nlohmann/json.hpp>
#include <iostream>
#include <msgpack.hpp>
#include <signal.h>
#include <stdio.h>
#include "boost/date_time/gregorian/gregorian.hpp" 
#include "boost/date_time/posix_time/posix_time.hpp"

typedef websocketpp::server<websocketpp::config::asio> server;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using json = nlohmann::json;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
  std::string command = msg->get_payload();
  std::cout << "message: " << command << std::endl;

  /* Build json return data. */
  json j;
  j["node"] = 0;
  j["time"] = boost::posix_time::to_simple_string(boost::posix_time::second_clock::local_time());
  j["username"] = "Linus";
  j["msg"] = "Msgpack test successful!";
  j["data"] = {1, 2, "Does this work?", 4};
  std::vector<std::uint8_t> v_msgpack = json::to_msgpack(j);
  std::cout << websocketpp::utility::to_hex(v_msgpack.data(), 20) << std::endl;
  std::cout << v_msgpack.size()*sizeof(std::uint8_t) << std::endl
            << j.dump(4) << std::endl;
  
  try {
    s->send(hdl, v_msgpack.data(), v_msgpack.size()*sizeof(std::uint8_t),
            websocketpp::frame::opcode::binary);
  } catch (websocketpp::exception const& e) {
    std::cout << "Echo failed because: "
              << "(" << e.what() << ")" << std::endl;
  }
}

// Define a callback to handle incoming messages
void on_open(server* s, websocketpp::connection_hdl hdl) {
  try {
    s->send(hdl, "Welcome to the Armour Interactive Fiction Game!",
            websocketpp::frame::opcode::text);
  } catch (websocketpp::exception const& e) {
    std::cout << "Echo failed because: " << "(" << e.what() << ")" << std::endl;
  }
}

/* Exit this cleanly? */
/* This is needed for global access to the server. */
server echo_server;

void end_server(int sig) {
  /* Exit server cleanly */
  echo_server.stop_listening();
}

int main() {
  // Create a server endpoint

  try {
    // Set logging settings
    //echo_server.set_access_channels(websocketpp::log::alevel::fail);
    echo_server.clear_access_channels(websocketpp::log::alevel::all);
    //echo_server.clear_access_channels(websocketpp::log::alevel::frame_payload);

    // Initialize Asio
    echo_server.init_asio();

    // Register our message handler
    echo_server.set_message_handler(bind(&on_message, &echo_server, ::_1, ::_2));
    echo_server.set_open_handler(bind(&on_open, &echo_server, ::_1));
    // Listen on port 9002
    echo_server.listen(8080);

    // Start the server accept loop
    echo_server.start_accept();
    
    struct sigaction sig_handle;
    sig_handle.sa_handler = end_server;
    sigemptyset(&sig_handle.sa_mask);
    sig_handle.sa_flags = 0;
    sigaction(SIGINT, &sig_handle, NULL);

    // Start the ASIO io_service run loop
    echo_server.run();
  } catch (websocketpp::exception const& e) {
    std::cout << e.what() << std::endl;
  } catch (...) {
    std::cout << "other exception" << std::endl;
  }
  return 0;
}
