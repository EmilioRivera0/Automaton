/*-----------------------------------------------------------------------------------------------------------------------------------
- Software Name: Deterministic and Nondeterministic Automata
- Version: 1.0
- Language: C++
- Developer: Emilio Rivera Macías
- Date: 10/18/2022
- Contact: emilioriveramacias@gmail.com
-----------------------------------------------------------------------------------------------------------------------------------*/

// necessary includes ---------------------------->
#include <clocale>
#include "Automaton.h"

int main(int argc, char** argv)
{
	// variables declaration & initialization ---------------------------->
	char cs_FileName[MAX_FILE_NAME_SIZE];
	short int n_Option{}, n_it{};
	/*
	1. alphabet vector
	2. initial state
	3. final states
	4. deterministic transitionfunctions
	5. nondeterministic transitionfunctions
	*/
	vector<string> v_ALPHABET{};
	string s_INITIAL_STATE{};
	vector<string> v_FINAL_STATEs{};
	map<key_State_Symbol, string> m_TRANSITION_FUNCTIONS_D{};
	multimap<key_State_Symbol, string> m_TRANSITION_FUNCTIONS_ND{};
	string s_string{};
	// procedure ---------------------------->
	/* used to change the locale if needed
	setlocale(LC_ALL, "es");*/

	// ask the user for the automaton file name
	cout << "File Name: ";
	std::cin >> cs_FileName;
	cout << "\n";

	do{
		// user will decide either to run a deterministic automaton, a nondeterministic automaton once or a nondeterministic 
		// automaton until it is accepted or the ITERATION LIMIT is reached
		n_Option = Automata_Menu();
		switch (n_Option)
		{
		case 1:
			read_automaton_d_from_txt(cs_FileName, v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_D);
			print_Automaton_D(v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_D);
			s_string = Input_Menu(v_ALPHABET);
			cout << "\n\n-> Generated String: " << s_string << "\n\nAutomaton Process:\n" << endl;
			if (Automaton_D(s_string, m_TRANSITION_FUNCTIONS_D, s_INITIAL_STATE, v_FINAL_STATEs))
				cout << "--> String accepted by Automaton" << endl;
			else
				cout << "--> String not accepted by Automaton" << endl;
			break;
		case 2:
			read_automaton_nd_from_txt(cs_FileName, v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_ND);
			print_Automaton_ND(v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_ND);
			s_string = Input_Menu(v_ALPHABET);
			cout << "\n\n-> Generated String: " << s_string << "\n\nAutomaton Process:\n" << endl;
			if (Automaton_ND(s_string, m_TRANSITION_FUNCTIONS_ND, s_INITIAL_STATE, v_FINAL_STATEs))
				cout << "--> String accepted by Automaton" << endl;
			else
				cout << "--> String not accepted by Automaton" << endl;
			break;
		case 3:
			read_automaton_nd_from_txt(cs_FileName, v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_ND);
			print_Automaton_ND(v_ALPHABET, s_INITIAL_STATE, v_FINAL_STATEs, m_TRANSITION_FUNCTIONS_ND);
			s_string = Input_Menu(v_ALPHABET);
			cout << "\n\n-> Generated String: " << s_string << "\n\nAutomaton Process:\n" << endl;
			// iterate until the automaton accepts the word or the ITERATION_LIMIT is reached
			for (n_it = 1; n_it < ITERATION_LIMIT; n_it++){
				if (Automaton_ND(s_string, m_TRANSITION_FUNCTIONS_ND, s_INITIAL_STATE, v_FINAL_STATEs)) {
					cout << "--> String accepted by Automaton" << endl;
					break;
				}
				else
					cout << "--> String not accepted by Automaton" << endl;
			}
			if (n_it == ITERATION_LIMIT)
				cout << "\n\t---> Iteration Limit reached <---\n" << endl;
			break;
		default:
			// hack the internet:)
			break;
		}
		clear_data_structures(v_ALPHABET,v_FINAL_STATEs,m_TRANSITION_FUNCTIONS_D,m_TRANSITION_FUNCTIONS_ND);
	} while (n_Option);

	return 0;
}