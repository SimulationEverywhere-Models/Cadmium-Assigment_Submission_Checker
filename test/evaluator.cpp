/**
* Shubham Thakur
* 101134521
* Carleton University
*/




//Cadmium Simulator headers
#include <cadmium/modeling/ports.hpp>
#include <cadmium/modeling/dynamic_model.hpp>
#include <cadmium/modeling/dynamic_model_translator.hpp>
#include <cadmium/engine/pdevs_dynamic_runner.hpp>
#include <cadmium/logger/common_loggers.hpp>

//Time class header
#include <NDTime.hpp>

//Messages structures
#include "../data_structures/message.hpp"

//Atomic model headers
#include <cadmium/basic_model/pdevs/iestream.hpp> //Atomic model for inputs
#include "../atomics/evaluator.hpp"

//C++ libraries
#include <iostream>
#include <string>

using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

#define INPUT_FILEPATH1 "../input/evaluator_input_test_timechecker.txt"
#define INPUT_FILEPATH2 "../input/evaluator_input_test_plagiarismchecker.txt"
#define INPUT_FILEPATH3 "../input/evaluator_input_test_mailchecker.txt"
#define OUTPUT_FILEPATH "../results/evaluator_test_output_messages.txt"

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct top_T_output: public in_port<Message_t>{};
struct top_P_output: public in_port<Message_t>{};
struct top_M_output: public in_port<Message_t>{};

/***** Define output ports for coupled model *****/
struct top_Result: public out_port<Message_t>{};

/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Message_t : public iestream_input<Message_t,T> {
    public:
        InputReader_Message_t () = default;
        InputReader_Message_t (const char* file_path) : iestream_input<Message_t,T>(file_path) {}
};

int main(){

    /****** Input Reader for Time Checker atomic model instantiation *******************/
    const char * i_input_data_timechecker = INPUT_FILEPATH1;
    shared_ptr<dynamic::modeling::model> input_reader_timechecker;
    input_reader_timechecker = dynamic::translate::make_dynamic_atomic_model<InputReader_Message_t, TIME, const char*>("input_reader_timechecker", move(i_input_data_timechecker));

    /****** Input Reader for Plagiarism Checker atomic model instantiation *******************/
    const char * i_input_data_plagiarismchecker = INPUT_FILEPATH2;
    shared_ptr<dynamic::modeling::model> input_reader_plagiarismchecker;
    input_reader_plagiarismchecker = dynamic::translate::make_dynamic_atomic_model<InputReader_Message_t, TIME, const char*>("input_reader_plagiarismchecker", move(i_input_data_plagiarismchecker));

    /****** Input Reader for Mail Checker atomic model instantiation *******************/
    const char * i_input_data_mailchecker = INPUT_FILEPATH3;
    shared_ptr<dynamic::modeling::model> input_reader_mailchecker;
    input_reader_mailchecker = dynamic::translate::make_dynamic_atomic_model<InputReader_Message_t, TIME, const char*>("input_reader_mailchecker", move(i_input_data_mailchecker));

    /****** Evaluator atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> evaluator1;
    evaluator1 = dynamic::translate::make_dynamic_atomic_model<Evaluator, TIME>("evaluator1");

    /*******TOP MODEL********/
    dynamic::modeling::Ports iports_TOP;
    iports_TOP = {};
    dynamic::modeling::Ports oports_TOP;
    oports_TOP = {typeid(top_Result)};
    dynamic::modeling::Models submodels_TOP;
    submodels_TOP = {input_reader_timechecker, input_reader_plagiarismchecker, input_reader_mailchecker, evaluator1};
    dynamic::modeling::EICs eics_TOP;
    eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP;
    eocs_TOP = {
        dynamic::translate::make_EOC<Evaluator_defs::Result, top_Result>("evaluator1")
    };
    dynamic::modeling::ICs ics_TOP;
    ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<Message_t>::out,Evaluator_defs::T_output>("input_reader_timechecker","evaluator1"),
        dynamic::translate::make_IC<iestream_input_defs<Message_t>::out,Evaluator_defs::P_output>("input_reader_plagiarismchecker","evaluator1"),
        dynamic::translate::make_IC<iestream_input_defs<Message_t>::out,Evaluator_defs::M_output>("input_reader_mailchecker","evaluator1")

    };
    shared_ptr<dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    /*************** Loggers *******************/
    static ofstream out_messages(OUTPUT_FILEPATH);
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../results/evaluator_test_output_state.txt");
    struct oss_sink_state{
        static ostream& sink(){          
            return out_state;
        }
    };
    
    using state=logger::logger<logger::logger_state, dynamic::logger::formatter<TIME>, oss_sink_state>;
    using log_messages=logger::logger<logger::logger_messages, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_mes=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_messages>;
    using global_time_sta=logger::logger<logger::logger_global_time, dynamic::logger::formatter<TIME>, oss_sink_state>;

    using logger_top=logger::multilogger<state, log_messages, global_time_mes, global_time_sta>;

    /************** Runner call ************************/ 
    dynamic::engine::runner<NDTime, logger_top> r(TOP, {0});
    r.run_until(NDTime("04:00:00:000"));

    cout<<"Evaluator Model Run Successful."<<"\n";
    cout<<"To see the Output, go to results folder."<<"\n";
	return 0;
}