/*
 * servidordecomunicaciones.h
 *
 *  Created on: 24 ago. 2020
 *      Author: Angel
 */

#ifndef INC_SERVIDORDECOMUNICACIONES_H_
#define INC_SERVIDORDECOMUNICACIONES_H_

#include <iostream>

#include <websocketpp/config/debug_asio_no_tls.hpp>
#include <websocketpp/server.hpp>

#include <websocketpp/extensions/permessage_deflate/enabled.hpp>

struct deflate_config : public websocketpp::config::debug_core {
    typedef deflate_config type;
    typedef debug_core base;

    typedef base::concurrency_type concurrency_type;

    typedef base::request_type request_type;
    typedef base::response_type response_type;

    typedef base::message_type message_type;
    typedef base::con_msg_manager_type con_msg_manager_type;
    typedef base::endpoint_msg_manager_type endpoint_msg_manager_type;

    typedef base::alog_type alog_type;
    typedef base::elog_type elog_type;

    typedef base::rng_type rng_type;

    struct transport_config : public base::transport_config {
        typedef type::concurrency_type concurrency_type;
        typedef type::alog_type alog_type;
        typedef type::elog_type elog_type;
        typedef type::request_type request_type;
        typedef type::response_type response_type;
        typedef websocketpp::transport::asio::basic_socket::endpoint
            socket_type;
    };

    typedef websocketpp::transport::asio::endpoint<transport_config>
        transport_type;

    /// permessage_compress extension
    struct permessage_deflate_config {};

    typedef websocketpp::extensions::permessage_deflate::enabled
        <permessage_deflate_config> permessage_deflate_type;
};

typedef websocketpp::server<deflate_config> server;

typedef server::message_ptr message_ptr;

class ServidorDeComunicaciones
{
	public:
		ServidorDeComunicaciones();
		~ServidorDeComunicaciones();
		void start();
// Define a callback to handle incoming messages
		void on_message(websocketpp::connection_hdl hdl, message_ptr msg);
		void set_timer();
		void on_timer(websocketpp::lib::error_code const & ec);

	private:
		server m_server;
		server::timer_ptr m_timer;
};



#endif /* INC_SERVIDORDECOMUNICACIONES_H_ */
