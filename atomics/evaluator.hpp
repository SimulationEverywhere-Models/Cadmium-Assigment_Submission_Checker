/**
* Shubham Thakur
* 101134521
* Carleton University
*/




#ifndef _EVALUATOR_HPP__
#define _EVALUATOR_HPP__

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
struct Evaluator_defs{
    struct Result : public out_port<Message_t> {};
    struct T_output : public in_port<Message_t> {};
    struct P_output : public in_port<Message_t> {};
    struct M_output : public in_port<Message_t> {};
};

template<typename TIME> class Evaluator{
    public:
    // ports definition
    using input_ports=tuple<typename Evaluator_defs::T_output, typename Evaluator_defs::P_output, typename Evaluator_defs::M_output>;
    using output_ports=tuple<typename Evaluator_defs::Result>;

    // state definition
    struct state_type{
        bool active;
        Message_t packet;
        int index;
        int timeoutput;
        int plagiarismoutput;
        int mailoutput;
        int answer;
    }; 

    state_type state; 

    // default constructor
    Evaluator() {
        state.active = false;
        state.index = 0;
    }     

    // internal transition
    void internal_transition() {
        state.active = false;  
    }

    // external transition
    void external_transition(TIME e, typename make_message_bags<input_ports>::type mbs) { 
        if((get_messages<typename Evaluator_defs::T_output>(mbs).size()+get_messages<typename Evaluator_defs::P_output>(mbs).size()+get_messages<typename Evaluator_defs::M_output>(mbs).size())>3) 
                    assert(false && "One message at a time");                
        state.index ++;   
        

        for(const auto &x : get_messages<typename Evaluator_defs::T_output>(mbs)){
            state.timeoutput = static_cast < int > (x.value);
            state.active = true;

            if(state.timeoutput == 1){
                state.timeoutput = 1;
            }
            else{
                state.timeoutput = 0;
            }
        }

        for(const auto &x : get_messages<typename Evaluator_defs::P_output>(mbs)){
            state.plagiarismoutput = static_cast < int > (x.value);
            state.active = true;

            if(state.plagiarismoutput == 1){
                state.plagiarismoutput = 1;
            }
            else{
                state.plagiarismoutput = 0;
            }
        }

        for(const auto &x : get_messages<typename Evaluator_defs::M_output>(mbs)){
            state.mailoutput = static_cast < int > (x.value);
            state.active = true;

            if(state.mailoutput == 1){
                state.mailoutput = 1;
            }
            else{
                state.mailoutput = 0;
            }
        } 



        if (state.timeoutput == 1 && state.plagiarismoutput == 1 && state.mailoutput == 1){
            state.answer = 1;
        }
        else{
            state.answer = 0;
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

        Message_t Result;
        Result.value = state.answer;     
        get_messages<typename Evaluator_defs::Result>(bags).push_back(Result);

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

    friend ostringstream& operator<<(ostringstream& os, const typename Evaluator<TIME>::state_type& i) {
        os << "Submission Number: " << i.index << " & Evaluator Model State: " << i.active; 
        return os;
    }
};    
#endif // _EVALUATOR_HPP__