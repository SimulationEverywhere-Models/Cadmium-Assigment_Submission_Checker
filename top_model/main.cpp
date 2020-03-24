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
#include "../atomics/assignment.hpp"
#include "../atomics/timechecker.hpp"
#include "../atomics/plagiarismchecker.hpp"
#include "../atomics/mailchecker.hpp"
#include "../atomics/evaluator.hpp"

//C++ headers
#include <iostream>
#include <chrono>
#include <algorithm>
#include <string>


using namespace std;
using namespace cadmium;
using namespace cadmium::basic_models::pdevs;

using TIME = NDTime;

/***** Define input port for coupled models *****/
struct T_info : public in_port<Message_t>{};
struct P_info : public in_port<Message_t>{};
struct M_info : public in_port<Message_t>{};
struct SubmitRequest : public in_port<Message_t>{};

/***** Define output ports for coupled model *****/
struct T_output : public out_port<Message_t>{};
struct P_output : public out_port<Message_t>{};
struct M_output : public out_port<Message_t>{};
struct Result : public out_port<Message_t>{};

/****** Input Reader atomic model declaration *******************/
template<typename T>
class InputReader_Int : public iestream_input<Message_t,T> {
public:
    InputReader_Int() = default;
    InputReader_Int(const char* file_path) : iestream_input<Message_t,T>(file_path) {}
};


int main(int argc, char ** argv) {

    if (argc < 2) {
        cout << "Program used with wrong parameters. The program must be invoked as follow:";
        cout << argv[0] << " path to the input file " << endl;
        return 1; 
    }
    /****** Input Reader atomic model instantiation *******************/
    string input = argv[1];
    const char * i_input = input.c_str();
    shared_ptr<dynamic::modeling::model> input_reader = dynamic::translate::make_dynamic_atomic_model<InputReader_Int, TIME, const char* >("input_reader" , move(i_input));

    /****** Assignment atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> assignment1 = dynamic::translate::make_dynamic_atomic_model<Assignment, TIME>("assignment1");

    /****** Evaluator atomic model instantiation *******************/
    shared_ptr<dynamic::modeling::model> evaluator1 = dynamic::translate::make_dynamic_atomic_model<Evaluator, TIME>("evaluator1");

    /****** Checker atomic models instantiation *******************/
    shared_ptr<dynamic::modeling::model> timechecker1 = dynamic::translate::make_dynamic_atomic_model<TimeChecker, TIME>("timechecker1");
    shared_ptr<dynamic::modeling::model> plagiarismchecker1 = dynamic::translate::make_dynamic_atomic_model<PlagiarismChecker, TIME>("plagiarismchecker1");
    shared_ptr<dynamic::modeling::model> mailchecker1 = dynamic::translate::make_dynamic_atomic_model<MailChecker, TIME>("mailchecker1");

    /*******CHECKER COUPLED MODEL********/
    dynamic::modeling::Ports iports_Checker = {typeid(T_info),typeid(P_info),typeid(M_info)};
    dynamic::modeling::Ports oports_Checker = {typeid(T_output), typeid(P_output), typeid(M_output)};
    dynamic::modeling::Models submodels_Checker = {timechecker1, plagiarismchecker1, mailchecker1};
    dynamic::modeling::EICs eics_Checker = {
        dynamic::translate::make_EIC<T_info, TimeChecker_defs::info>("timechecker1"),
        dynamic::translate::make_EIC<P_info, PlagiarismChecker_defs::info>("plagiarismchecker1"),
        dynamic::translate::make_EIC<M_info, MailChecker_defs::info>("mailchecker1")
    };
    dynamic::modeling::EOCs eocs_Checker = {
        dynamic::translate::make_EOC<TimeChecker_defs::T_output,T_output>("timechecker1"),
        dynamic::translate::make_EOC<PlagiarismChecker_defs::P_output,P_output>("plagiarismchecker1"),
        dynamic::translate::make_EOC<MailChecker_defs::M_output,M_output>("mailchecker1")
    };
    dynamic::modeling::ICs ics_Checker = {};
    shared_ptr<dynamic::modeling::coupled<TIME>> CHECKER;
    CHECKER = make_shared<dynamic::modeling::coupled<TIME>>(
        "Checker", submodels_Checker, iports_Checker, oports_Checker, eics_Checker, eocs_Checker, ics_Checker 
    );



    /*******ASC SIMULATOR COUPLED MODEL********/
    dynamic::modeling::Ports iports_ASC = {typeid(SubmitRequest)};
    dynamic::modeling::Ports oports_ASC = {typeid(Result)};
    dynamic::modeling::Models submodels_ASC = {assignment1, evaluator1, CHECKER};
    dynamic::modeling::EICs eics_ASC = {
        cadmium::dynamic::translate::make_EIC<SubmitRequest, Assignment_defs::submitRequest>("assignment1")
    };
    dynamic::modeling::EOCs eocs_ASC = {
        dynamic::translate::make_EOC<Evaluator_defs::Result,Result>("evaluator1"),
    };
    dynamic::modeling::ICs ics_ASC = {
        dynamic::translate::make_IC<Assignment_defs::info, T_info>("assignment1","Checker"),
        dynamic::translate::make_IC<Assignment_defs::info, P_info>("assignment1","Checker"),
        dynamic::translate::make_IC<Assignment_defs::info, M_info>("assignment1","Checker"),
        dynamic::translate::make_IC<T_output, Evaluator_defs::T_output>("Checker","evaluator1"),
        dynamic::translate::make_IC<P_output, Evaluator_defs::P_output>("Checker","evaluator1"),
        dynamic::translate::make_IC<M_output, Evaluator_defs::M_output>("Checker","evaluator1")
    };
    shared_ptr<dynamic::modeling::coupled<TIME>> ASC;
    ASC = make_shared<dynamic::modeling::coupled<TIME>>(
        "ASC", submodels_ASC, iports_ASC, oports_ASC, eics_ASC, eocs_ASC, ics_ASC 
    );

    /*******TOP COUPLED MODEL********/
    dynamic::modeling::Ports iports_TOP = {};
    dynamic::modeling::Ports oports_TOP = {typeid(Result)};
    dynamic::modeling::Models submodels_TOP = {input_reader, ASC};
    dynamic::modeling::EICs eics_TOP = {};
    dynamic::modeling::EOCs eocs_TOP = {
        dynamic::translate::make_EOC<Result,Result>("ASC")
    };
    dynamic::modeling::ICs ics_TOP = {
        dynamic::translate::make_IC<iestream_input_defs<Message_t>::out, SubmitRequest>("input_reader","ASC")
    };
    shared_ptr<cadmium::dynamic::modeling::coupled<TIME>> TOP;
    TOP = make_shared<dynamic::modeling::coupled<TIME>>(
        "TOP", submodels_TOP, iports_TOP, oports_TOP, eics_TOP, eocs_TOP, ics_TOP 
    );

    /*************** Loggers *******************/
    static ofstream out_messages("../results/ASC_output_messages.txt");
    struct oss_sink_messages{
        static ostream& sink(){          
            return out_messages;
        }
    };
    static ofstream out_state("../results/ASC_output_state.txt");
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
    r.run_until_passivate();


    cout<<"Assignment Submission Cecker Model Run Successful."<<"\n";
    cout<<"To see the Output, go to results folder."<<"\n";


    return 0;
}
