/**
* Shubham Thakur
* 101134521
* Carleton University
*/




#ifndef _PLAGIARISMCHECKER_HPP__
#define _PLAGIARISMCHECKER_HPP__

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
struct PlagiarismChecker_defs{
    struct P_output : public out_port<Message_t> {};
    struct info : public in_port<Message_t> {};
};

template<typename TIME> class PlagiarismChecker{
    public:
    // ports definition
    using input_ports=tuple<typename PlagiarismChecker_defs::info>;
    using output_ports=tuple<typename PlagiarismChecker_defs::P_output>;

    // state definition
    struct state_type{
        bool active;
        Message_t packet;
        int index;
        int temp;
    }; 

    state_type state; 

    // default constructor
    PlagiarismChecker() {
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
        bag_port_in = get_messages<typename PlagiarismChecker_defs::info>(mbs);
        if(bag_port_in.size()>1) assert(false && "One message at a time");                
        state.index ++;  
        state.packet = bag_port_in[0]; 
        

        for(const auto &x : get_messages<typename PlagiarismChecker_defs::info>(mbs)){
            state.temp = static_cast < int > (x.value);
            state.active = true;
        } 

        int arr[6];
        int i = 0;
        for(i=5;i>=0;i--){
            int mod = state.temp%10;
            arr[i] = mod;
            state.temp = state.temp/10;
        }        
        
        if (arr[3]==1 && arr[4]==1){
            state.temp = 1;
        }  
        else{
            state.temp = 0;
        }     
    }
    // confluence transition
    void confluence_transition(TIME e, typename make_message_bags<input_ports>::type mbs) {
        internal_transition();
        external_transition(TIME(), move(mbs));
    }

    // output function
    typename make_message_bags<output_ports>::type output() const {
        typename make_message_bags<output_ports>::type bags;
        // vector<Message_t> bag_port_out;
        // bag_port_out.push_back(state.packet);
        // get_messages<typename TimeChecker_defs::T_output>(bags) = bag_port_out;

        Message_t P_output;
        P_output.value = state.temp;     
        get_messages<typename PlagiarismChecker_defs::P_output>(bags).push_back(P_output);

        return bags;
    }

    // time_advance function
    TIME time_advance() const {
        TIME next_internal;
        if (state.active) {            
            next_internal = TIME("00:00:01:000"); //T_time is 1 seconds.
        }else {
            next_internal = numeric_limits<TIME>::infinity();
        }    
        return next_internal;
    }

    friend ostringstream& operator<<(ostringstream& os, const typename PlagiarismChecker<TIME>::state_type& i) {
        os << "Submission Number: " << i.index << " & Plagiarism Checker Model State: " << i.active; 
        return os;
    }
};    
#endif // _PLAGIARISMCHECKER_HPP__