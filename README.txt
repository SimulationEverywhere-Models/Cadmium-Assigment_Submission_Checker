This folder contains the ASSIGNMENT SUBMISSION CHECKER DEVS model implemented in Cadmium

/**************************/
/****FILES ORGANIZATION****/
/**************************/

README.txt	
FINAL-REPORT_SYSC5104_ASSIGNMENT_1[SHUBHAM_THAKUR_101134521_ASSIGNMENT_SUBMISSION_CHECKER_MODEL].doc
FINAL-REPORT_SYSC5104_ASSIGNMENT_1[SHUBHAM_THAKUR_101134521_ASSIGNMENT_SUBMISSION_CHECKER_MODEL].pdf
makefile

atomics [This folder contains atomic models (header files) implemented in Cadmium]
	assignment.hpp
	timechecker.hpp
	plagiarismchecker.hpp
	mailchecker.hpp
	evaluator.hpp

bin [This folder will be created automatically the first time you compile the poject.
     It will contain all the executables]

build [This folder will be created automatically the first time you compile the poject.
       It will contain all the build files (.o) generated during compilation]

data_structures [This folder contains message data structure used in the model]
	message.hpp
	message.cpp

input [This folder contains all the input data to run the model and the tests]
	asc_input.txt
	assignment_input_test.txt
	timechecker_input_test.txt
	plagiarismchecker_input_test.txt
	mailchecker_input_test.txt
	evaluator_input_test_timechecker.txt
	evaluator_input_test_plagiarismchecker.txt
	evaluator_input_test_mailchecker.txt

results [This folder will be created automatically the first time you compile the poject.
                    It will store the outputs from your simulations and tests]

test [This folder the unit test of the atomic models]
	assignment.cpp
	timechecker.cpp
	plagiarismchecker.cpp
	mailchecker.cpp
	evaluator.cpp

top_model [This folder contains the Assignment Submission Checker top model]	
	main.cpp
	
/*************/
/****STEPS****/
/*************/

0 - FINAL-REPORT_SYSC5104_ASSIGNMENT_1[SHUBHAM_THAKUR_101134521_ASSIGNMENT_SUBMISSION_CHECKER_MODEL].doc contains the explanation of this model

1 - Compile the project and the tests
	1 - Open the terminal.
	2 - Go to the project directory. To do so, type in the terminal the path to the project folder.

		> Example: cd /Desktop/AssignmentSubmissionChecker/Cadmium-Simulation-Environment/DEVS-Models/ASC

	3 - To compile the project and the tests, type in the terminal:

		> make clean; make all

2 - Run Assignment test
	1 - To run the unit tests, go the folder with executable files (i.e., bin folder). To do so, type in the terminal the path to this folder.

		> Example: cd bin

	2 - To run the test, type in the terminal "./NAME_OF_THE_COMPILED_FILE". For this specific test you need to type: 
		For this specific test you need to type:
			> ./ASSIGNMENT_TEST 
	3 - To check the output of the test, open  "../result/assignment_test_output_messages.txt" and "../result/assignment_test_output_state.txt"

3 - To run receiver and sender tests you just need to select the proper executable name in step 2.2
			
4 - Run the Simulator(top model)
	1 - Open the terminal.
	2 - Go to the project directory. To do so, type in the terminal the path to the project folder.

		> Example: cd /Desktop/AssignmentSubmissionChecker/Cadmium-Simulation-Environment/DEVS-Models/ASC

	3 - To compile the project and the tests, type in the terminal:

		> make clean; make all

	4 - To run the model, type in the terminal "./NAME_OF_THE_COMPILED_FILE NAME_OF_THE_INPUT_FILE". For this test you need to type:

		> ./ASC ../input/asc_input.txt

	4 - To check the output of the model, go to the folder results and open "ASC_output_messasges.txt" and "ASC_output_state.txt"

	5 - To run the model with different inputs
		5.1. Create new .txt files with the same structure as asc_input.txt in the folder input and while executing the ASC compiled file give this new .txt file as an input.
		5.2. Run the model using the instructions in step 4.
		5.3. If you want to keep the output, rename a"ASC_output_messasges.txt" and "ASC_output_state.txt". Otherwise it will be overwritten when you run the next simulation.

