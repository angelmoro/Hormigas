/*
 * servidordecomunicaciones.cpp
 *
 *  Created on: 24 ago. 2020
 *      Author: Angel
 */

#include "servidordecomunicaciones.h"

ServidorDeComunicaciones::ServidorDeComunicaciones()
{
	try {

            m_server.set_access_channels(websocketpp::log::alevel::all);
            m_server.set_error_channels(websocketpp::log::elevel::all);

    //        m_server.set_access_channels(websocketpp::log::alevel::none);
    //        m_server.set_error_channels(websocketpp::log::elevel::none);

        // Initialize ASIO
        m_server.init_asio();

        // Register our message handler
        using websocketpp::lib::placeholders::_1;
        using websocketpp::lib::placeholders::_2;
        using websocketpp::lib::bind;
        m_server.set_message_handler(bind(&ServidorDeComunicaciones::on_message,this,_1,_2));
	   } catch (websocketpp::exception const & e) {
	        std::cout << e.what() << std::endl;
	   } catch (const std::exception & e) {
	        std::cout << e.what() << std::endl;
	   } catch (...) {
	        std::cout << "other exception" << std::endl;
	   }
}
ServidorDeComunicaciones::~ServidorDeComunicaciones()
{

}

void ServidorDeComunicaciones::on_message(websocketpp::connection_hdl hdl, message_ptr msg)
{
/*std::cout << "on_message called with hdl: " << hdl.lock().get()
		  << " and message (" << msg->get_payload().size() << "): " << msg->get_payload()
		  << std::endl;
*/
	// Se hace un eco del mensaje para probar
	try {
	m_server.send(hdl, msg->get_payload(), msg->get_opcode());
	} catch (websocketpp::exception const & e) {
	std::cout << "Echo failed because: "
			  << "(" << e.what() << ")" << std::endl;
	}
}

void ServidorDeComunicaciones::start()
{

	try {

          // Listen on port 9002
        m_server.listen(9002);

        // Start the server accept loop
        m_server.start_accept();

	    // Start the ASIO io_service run loop
        m_server.run();
    } catch (websocketpp::exception const & e) {
        std::cout << e.what() << std::endl;
    } catch (const std::exception & e) {
        std::cout << e.what() << std::endl;
    } catch (...) {
        std::cout << "other exception" << std::endl;
    }
}
void ServidorDeComunicaciones::set_timer()
{
 m_timer = m_server.set_timer(
	 1000,
	 websocketpp::lib::bind(
		 &ServidorDeComunicaciones::on_timer,
		 this,
		 websocketpp::lib::placeholders::_1
	 )
 );
}

void ServidorDeComunicaciones::on_timer(websocketpp::lib::error_code const & ec)
{
 if (ec) {
	 // there was an error, stop telemetry
	 m_server.get_alog().write(websocketpp::log::alevel::app,
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



