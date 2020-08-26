/*
 * ClienteDeComunicaciones.cpp
 *
 *  Created on: 24 ago. 2020
 *      Author: Angel
 */

#include <clientedecomunicaciones.h>


ClienteDeComunicaciones::ClienteDeComunicaciones () : m_next_id(0)
{
	m_client.clear_access_channels(websocketpp::log::alevel::all);
	m_client.clear_error_channels(websocketpp::log::elevel::all);

	m_client.init_asio();
	m_client.start_perpetual();

	m_thread = websocketpp::lib::make_shared<websocketpp::lib::thread>(&client::run, &m_client);
}

ClienteDeComunicaciones::~ClienteDeComunicaciones()
{
	m_client.stop_perpetual();

	for (con_list::const_iterator it = m_connection_list.begin(); it != m_connection_list.end(); ++it) {
		if (it->second->get_status() != "Open") {
			// Only close open connections
			continue;
		}

		std::cout << "> Closing connection " << it->second->get_id() << std::endl;

		websocketpp::lib::error_code ec;
		m_client.close(it->second->get_hdl(), websocketpp::close::status::going_away, "", ec);
		if (ec) {
			std::cout << "> Error closing connection " << it->second->get_id() << ": "
					  << ec.message() << std::endl;
		}
	}

	m_thread->join();
}

int ClienteDeComunicaciones::connect(std::string const & uri)
{
	websocketpp::lib::error_code ec;

	client::connection_ptr con = m_client.get_connection(uri, ec);

	if (ec) {
		std::cout << "> Connect initialization error: " << ec.message() << std::endl;
		return -1;
	}

	int new_id = m_next_id++;
	connection_metadata::ptr metadata_ptr = websocketpp::lib::make_shared<connection_metadata>(new_id, con->get_handle(), uri);
	m_connection_list[new_id] = metadata_ptr;

	con->set_open_handler(websocketpp::lib::bind(
		&connection_metadata::on_open,
		metadata_ptr,
		&m_client,
		websocketpp::lib::placeholders::_1
	));
	con->set_fail_handler(websocketpp::lib::bind(
		&connection_metadata::on_fail,
		metadata_ptr,
		&m_client,
		websocketpp::lib::placeholders::_1
	));
	con->set_close_handler(websocketpp::lib::bind(
		&connection_metadata::on_close,
		metadata_ptr,
		&m_client,
		websocketpp::lib::placeholders::_1
	));
	con->set_message_handler(websocketpp::lib::bind(
		&connection_metadata::on_message,
		metadata_ptr,
		websocketpp::lib::placeholders::_1,
		websocketpp::lib::placeholders::_2
	));

	m_client.connect(con);

	return new_id;
}

void ClienteDeComunicaciones::close(int id, websocketpp::close::status::value code, std::string reason)
{
	websocketpp::lib::error_code ec;

	con_list::iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		std::cout << "> No connection found with id " << id << std::endl;
		return;
	}

	m_client.close(metadata_it->second->get_hdl(), code, reason, ec);
	if (ec) {
		std::cout << "> Error initiating close: " << ec.message() << std::endl;
	}
}

void ClienteDeComunicaciones::send(int id, std::string message)
{
	websocketpp::lib::error_code ec;

	con_list::iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		std::cout << "> No connection found with id " << id << std::endl;
		return;
	}

	m_client.send(metadata_it->second->get_hdl(), message, websocketpp::frame::opcode::text, ec);
	if (ec) {
		std::cout << "> Error sending message: " << ec.message() << std::endl;
		return;
	}

	metadata_it->second->record_sent_message(message);
}

connection_metadata::ptr ClienteDeComunicaciones::get_metadata(int id) const
{
	con_list::const_iterator metadata_it = m_connection_list.find(id);
	if (metadata_it == m_connection_list.end()) {
		return connection_metadata::ptr();
	} else {
		return metadata_it->second;
	}
}
void ClienteDeComunicaciones::set_timer()
{
 m_timer = m_client.set_timer(
	 1000,
	 websocketpp::lib::bind(
		 &ClienteDeComunicaciones::on_timer,
		 this,
		 websocketpp::lib::placeholders::_1
	 )
 );
}

void ClienteDeComunicaciones::on_timer(websocketpp::lib::error_code const & ec)
{
 if (ec) {
	 // there was an error, stop telemetry
	 m_client.get_alog().write(websocketpp::log::alevel::app,
			 "Timer Error: "+ec.message());
	 return;
 }

/* std::stringstream val;
 val << "count is " << m_count++;

 // Broadcast count to all connections
 con_list::iterator it;
 for (it = m_connections.begin(); it != m_connections.end(); ++it) {
	 m_endpoint.send(*it,val.str(),websocketpp::frame::opcode::text);
 }
 */

 // montar de nuevo el timer
 set_timer();
}

