/**
* Shubham Thakur
* 101134521
* Carleton University
*/




#ifndef _ASSIGNMENT_HPP__
#define _ASSIGNMENT_HPP__

#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/message_bag.hpp>

#include <limits>
#include <assert.h>
#include <string>
#include <random>

#include "../data_structures/message.hpp"

using namespace cadmium;
using namespace std;

//Port definition
struct Assignment_defs{
    struct info : public out_port<Message_t> {};
    struct submitRequest : public in_port<Message_t> {};
};

template<typename TIME> class Assignment{
    public:
    // ports definition
    using input_ports=tuple<typename Assignment_defs::submitRequest>;
    using output_ports=tuple<typename Assignment_defs::info>;

    // state definition
    struct state_type{
        bool active;
        Message_t packet;
        int index;
    }; 

    state_type state; 

    // default constructor
    Assignment() {
        state.active = false;
        state.index = 0;
    }     

    // internal transition
    void internal_transition() {
        state.active = false;  
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        vector<Message_t> bag_port_in;
        bag_port_in = get_messages<typename Assignment_defs::submitRequest>(mbs);
        if(bag_port_in.size()>1) assert(false && "One message at a time");                
        state.index ++;  
        state.packet = bag_port_in[0];
        state.active = true;                            
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }
    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        vector<Message_t> bag_port_out;            
        bag_port_out.push_back(state.packet);
        get_messages<typename Assignment_defs::info>(bags) = bag_port_out; 
        return bags;
    }
    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active) {            
            next_internal = TIME("00:00:01:000"); //A_time is 3 seconds.
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename Assignment<TIME>::state_type& i) {
        os << ":- Submission Number: " << i.index << " & Assignment Model State: " << i.active; 
        return os;
    }
};    
#endif // _ASSIGNMENT_HPP__