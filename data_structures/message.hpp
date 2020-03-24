#ifndef BOOST_SIMULATION_MESSAGE_HPP
#define BOOST_SIMULATION_MESSAGE_HPP

#include <assert.h>
#include <iostream>
#include <string>

using namespace std;

/*******************************************/
/**************** Message_t ****************/
/*******************************************/
struct Message_t{
  // Message_t(){}
  // Message_t(int i_packet, int i_bit)
  //  :packet(i_packet), bit(i_bit){}

  // 	int   packet;
  // 	int   bit;

	Message_t(){}
  	Message_t(int in_value)
  	:value(in_value){}
  	int value; /**< The float variable to store the value of message */
  	/**
  	 *Function to clear the contents of message
  	 */
  	void clear(){
  		Message_t(); 
  	}
};

istream& operator>> (istream& is, Message_t& msg);

ostream& operator<<(ostream& os, const Message_t& msg);


#endif // BOOST_SIMULATION_MESSAGE_HPP