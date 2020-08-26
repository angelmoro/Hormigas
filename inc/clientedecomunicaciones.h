/*
 * clientedecomunicaciones.h
 *
 *  Created on: 24 ago. 2020
 *      Author: Angel
 */

#ifndef INC_CLIENTEDECOMUNICACIONES_H_
#define INC_CLIENTEDECOMUNICACIONES_H_

#include <websocketpp/config/asio_no_tls_client.hpp>
#include <websocketpp/client.hpp>

#include <websocketpp/common/thread.hpp>
#include <websocketpp/common/memory.hpp>

#include <cstdlib>
#include <iostream>
#include <map>
#include <string>
#include <sstream>

#include "connection_metadata.h"

typedef websocketpp::client<websocketpp::config::asio_client> client;

class ClienteDeComunicaciones {
public:
    ClienteDeComunicaciones ();
    ~ClienteDeComunicaciones();
    int connect(std::string const & uri);
    void close(int id, websocketpp::close::status::value code, std::string reason);
    void send(int id, std::string message);
    connection_metadata::ptr get_metadata(int id) const;
	void set_timer();
	void on_timer(websocketpp::lib::error_code const & ec);
private:
    typedef std::map<int,connection_metadata::ptr> con_list;
    client m_client;
    websocketpp::lib::shared_ptr<websocketpp::lib::thread> m_thread;
    con_list m_connection_list;
    int m_next_id;
	client::timer_ptr m_timer;
};



#endif /* INC_CLIENTEDECOMUNICACIONES_H_ */
