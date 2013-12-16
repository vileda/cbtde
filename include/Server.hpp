#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>
#include <iostream>

struct server_config : public websocketpp::config::asio {
    // pull default settings from our core config
    typedef websocketpp::config::asio core;

    typedef core::concurrency_type concurrency_type;
    typedef core::request_type request_type;
    typedef core::response_type response_type;
    typedef core::message_type message_type;
    typedef core::con_msg_manager_type con_msg_manager_type;
    typedef core::endpoint_msg_manager_type endpoint_msg_manager_type;

    typedef core::alog_type alog_type;
    typedef core::elog_type elog_type;
    typedef core::rng_type rng_type;
    typedef core::endpoint_base endpoint_base;

    static bool const enable_multithreading = false;

    struct transport_config : public core::transport_config {
        typedef core::concurrency_type concurrency_type;
        typedef core::elog_type elog_type;
        typedef core::alog_type alog_type;
        typedef core::request_type request_type;
        typedef core::response_type response_type;

        static bool const enable_multithreading = false;
    };

    typedef websocketpp::transport::asio::endpoint<transport_config>
        transport_type;

    static const websocketpp::log::level elog_level =
        websocketpp::log::elevel::none;
    static const websocketpp::log::level alog_level =
        websocketpp::log::alevel::none;
};

typedef websocketpp::server<server_config> server;

using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;
using websocketpp::lib::bind;

// pull out the type of messages sent by our config
typedef server::message_ptr message_ptr;

class Server {
  public:
    Server() {

    }
    
    void runServer(int port, int threads);
};

// Define a callback to handle incoming messages
void on_message(server* s, websocketpp::connection_hdl hdl, message_ptr msg) {
    s->send(hdl, msg->get_payload(), msg->get_opcode());
}

void on_socket_init(websocketpp::connection_hdl hdl, boost::asio::ip::tcp::socket & s) {
    boost::asio::ip::tcp::no_delay option(true);
    s.set_option(option);
}

int run_server(int port, int threads) {
    // Create a server endpoint
    server server_server;

    short port = 9002;
    size_t num_threads = 1;

    if (argc == 3) {
        port = port;
        num_threads = threads;
    }

    try {
        // Total silence
        server_server.clear_access_channels(websocketpp::log::alevel::all);
        server_server.clear_error_channels(websocketpp::log::alevel::all);

        // Initialize ASIO
        server_server.init_asio();

        // Register our message handler
        server_server.set_message_handler(bind(&on_message,&server_server,::_1,::_2));
        server_server.set_socket_init_handler(bind(&on_socket_init,::_1,::_2));

        // Listen on specified port with extended listen backlog
        server_server.set_listen_backlog(8192);
        server_server.listen(port);

        // Start the server accept loop
        server_server.start_accept();

        // Start the ASIO io_service run loop
        if (num_threads == 1) {
            server_server.run();
        } else {
            typedef websocketpp::lib::shared_ptr<websocketpp::lib::thread> thread_ptr;
            std::vector<thread_ptr> ts;
            for (size_t i = 0; i < num_threads; i++) {
                ts.push_back(thread_ptr(new websocketpp::lib::thread(&server::run, &server_server)));
            }

            for (size_t i = 0; i < num_threads; i++) {
                ts[i]->join();
            }
        }

    } catch (const std::exception & e) {
        std::cout << "exception: " << e.what() << std::endl;
    } catch (websocketpp::lib::error_code e) {
        std::cout << "error code: " << e.message() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
