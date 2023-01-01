/*-----------------------------------------------------------------------------------------------------------------------------------
- Software Name: Deterministic and Nondeterministic Automata
- Version: 1.0
- Language: C++
- Developer: Emilio Rivera Macías
- Date: 10/18/2022
- Contact: emilioriveramacias@gmail.com
-----------------------------------------------------------------------------------------------------------------------------------*/

#ifndef AUTOMATON_H
#define AUTOMATON_H

// necessary includes ---------------------------->
#include <fstream>
using std::ifstream;
#include <cstring>
#include <stdexcept>
#include "Grammar.h"
#include "DataStructures.h"

// MACROS declaration ---------------------------->
#define MAX_FILE_NAME_SIZE 50
#define MAX_STREAM_SIZE 100
#define BUFFER_SIZE 100
#define ITERATION_LIMIT 20

// functions declaration and definition ---------------------------->

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: print the menu of the program, get the user input option and return it to execute the chosen automaton
-----------------------------------------------------------------------------------------------------------------------------------*/
short int Automata_Menu() {
	// variables declaration & initialization ---------------------------->
	short int n_op;
	// procedure ---------------------------->
	cout <<
		R"(	-> Automata <-
1. Run Deterministic Automaton
2. Run Nondeterministic Automaton once
3. Run Nondeterministic Automaton until string is accepted
0. Exit
Option-> )";
	std::cin >> n_op;
	return n_op;
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: print a little menu for the user to choose either to generate a random string or type a string to be evaluated by 
	the automaton
-----------------------------------------------------------------------------------------------------------------------------------*/
const string Input_Menu(const vector<string>& v_Alphabet) {
	// variables declaration & initialization ---------------------------->
	short int n_op;
	string s_Temp;
	// procedure ---------------------------->
	cout <<
		R"(
1. Generate random string
2. Type string
Option-> )";
	std::cin >> n_op;
	switch (n_op)
	{
	case 1:
		s_Temp = generate_string(v_Alphabet);
		break;
	case 2:
		cout << "Type the string: ";
		std::cin >> s_Temp;
		break;
	default:
		cout << "\nEnter a valid option!" << endl;
		break;
	}
	return s_Temp;
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: clears all the data structures used by the automata to enable reusing them without getting a problem
-----------------------------------------------------------------------------------------------------------------------------------*/
void clear_data_structures(vector<string>& v_A, vector<string>& v_F_S, map<key_State_Symbol, string>& m_P_F_D, multimap<key_State_Symbol, string>& m_P_F_ND) {
	// procedure ---------------------------->
	v_A.clear();
	v_F_S.clear();
	m_P_F_D.clear();
	m_P_F_ND.clear();
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: print the current state, the current symbol and the next state that the automaton will change to (parameters)
-----------------------------------------------------------------------------------------------------------------------------------*/
void print_State(const string& s_A_S, const char& c_A_S, const string& s_F_S) {
	// procedure ---------------------------->
	cout << "(" << s_A_S << ", " << c_A_S << ") --> " << s_F_S << std::flush;
}

// <Deterministic Automaton ------------------------------------------------------------------------------------------------------------------------------------------>

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: determines if the given string (parameter) is accepted or not by the deterministic automaton defined by the 
	transition functions, initial and final states and the alphabet (parameters)
-----------------------------------------------------------------------------------------------------------------------------------*/
bool Automaton_D(string& s_string, const map<key_State_Symbol, string>& m_P_F_D, const string& s_I_S, const vector<string>& v_F_S) {
	// variables declaration & initialization ---------------------------->
	string s_Current_State = s_I_S;
	// procedure ---------------------------->
	// iteration through each symbol from s_string to change to a different state depending on the current symbol
	for (char it : s_string) {
		// print the current state, current symbol and future state
		try {
			print_State(s_Current_State, it, m_P_F_D.at(key_State_Symbol(s_Current_State, it)));
			cout << endl;
			s_Current_State = m_P_F_D.at(key_State_Symbol(s_Current_State, it));
		}
		// exception when there is no transition function for the current state and symbol
		catch (const std::out_of_range&) {
			cout << "\nAutomaton does not have a transition function for the current state: " << s_Current_State << " and current symbol: " << it << std::flush;
			return 0;
		}
		catch (const std::exception& e) {
			cout << "\n" << e.what() << std::flush;
			return 0;
		}
	}
	// check if the las state of the iteration is final, if that's the case then s_string is accepted by the automaton
	for (auto& it : v_F_S) {
		if (it == s_Current_State)
			return 1;
	}
	// if not accepted
	return 0;
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: prints the deterministic automaton: its alphabet, states (denoting the initial an final ones) and the transition functions
-----------------------------------------------------------------------------------------------------------------------------------*/
void print_Automaton_D(const vector<string>& v_A, const string& s_I_S, const vector<string>& v_F_S, const map<key_State_Symbol, string>& m_P_F_D) {
	// procedure ---------------------------->
	cout << "\n\t\t--> Deterministic Automaton <--" << endl;
	cout << "\t-> Alphabet:" << endl;
	for (auto& i : v_A)
		cout << i << " ";
	cout << endl;
	cout << "\tInitial State: " << endl << s_I_S << endl;
	cout << "\tFinal State(s): " << endl;
	for (auto& i : v_F_S)
		cout << i << " ";
	cout << endl;
	cout << "\tProduction Functions:" << endl;
	for (auto& i : m_P_F_D) {
		cout << "<<" << i.first.first << ", " << i.first.second << ">, " << i.second << ">" << endl;
	}
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: read a given deterministic automaton from a text file that follows the specified format
-----------------------------------------------------------------------------------------------------------------------------------*/
void read_automaton_d_from_txt(const char* f_N, vector<string>& v_A, string& s_I_S, vector<string>& v_F_S, map<key_State_Symbol, string>& m_P_F_D) {
	// variables declaration & initialization ---------------------------->
	char cp_TempLine[BUFFER_SIZE]{}, * cp_TempToken = nullptr, * cp_CurrentState = nullptr, * cp_CurrentSymbol = nullptr, * cp_FutureState = nullptr;
	char* cp_Position = nullptr;
	ifstream f_AutomatonFile(f_N);
	// procedure ---------------------------->
	// * this project was programmed on Windows OS so strtok_s was used
	if (f_AutomatonFile.is_open()) {
		// reading the alphabet
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		while (cp_TempToken != nullptr) {
			v_A.push_back(cp_TempToken);
			cp_TempToken = strtok_s(nullptr, " ", &cp_Position);
			//cp_TempToken = strtok(nullptr, " ");
		}
		// reading the initial state
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		s_I_S = cp_TempToken;
		// reading the final state(s)
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		while (cp_TempToken != nullptr) {
			v_F_S.push_back(cp_TempToken);
			cp_TempToken = strtok_s(nullptr, " ", &cp_Position);
			//cp_TempToken = strtok(nullptr, " ");
		}
		// reading the transition functions
		while (!f_AutomatonFile.eof()) {
			f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
			cp_CurrentState = strtok_s(cp_TempLine, " ", &cp_Position);
			//cp_CurrentState = strtok(cp_TempLine, " ");
			cp_CurrentSymbol = strtok_s(nullptr, " ", &cp_Position);
			//cp_CurrentSymbol = strtok(nullptr, " ");
			cp_FutureState = strtok_s(nullptr, " ->", &cp_Position);
			//cp_FutureState = strtok(nullptr, " ->");
			m_P_F_D.insert(pair<key_State_Symbol, string>((key_State_Symbol(cp_CurrentState, *cp_CurrentSymbol)), cp_FutureState));
		}
	}
	else
		cout << "Program failed to open " << f_N << endl;

}

// <NONDETERMINISTIC AUTOMATON------------------------------------------------------------------------------------------------------------------------------------------>

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: determines if the given string (parameter) is accepted or not by the nondeterministic automaton defined by the
	transition functions, initial and final states and the alphabet (parameters)
-----------------------------------------------------------------------------------------------------------------------------------*/
bool Automaton_ND(string& s_string, const multimap<key_State_Symbol, string>& m_P_F_ND, const string& s_I_S, const vector<string>& v_F_S) {
	// variables declaration & initialization ---------------------------->
	int n_State{};
	string s_Current_State = s_I_S;
	multimap<key_State_Symbol, string>::const_iterator m_it{};
	// procedure ---------------------------->
	// iteration through each symbol from s_string to change to a different state depending on the current symbol
	for (char it : s_string) {
		// print the current state, current symbol and future state
		try {
			// no transition function for the current state and symbol
			if (!m_P_F_ND.count(key_State_Symbol(s_Current_State, it)))
				throw std::out_of_range("\nNo transition function found");
			// only one transition function for the curren state and symbol exists
			else if (m_P_F_ND.count(key_State_Symbol(s_Current_State, it)) == 1) {
				m_it = m_P_F_ND.find(key_State_Symbol(s_Current_State, it));
				print_State(s_Current_State, it, m_it->second);
				cout << endl;
				s_Current_State = m_it->second;
			}
			// more than one transition function for the current state and symbol exists
			else {
				// get one transition function randomly 
				n_State = rand() % (m_P_F_ND.count(key_State_Symbol(s_Current_State, it)));
				m_it = (m_P_F_ND.equal_range(key_State_Symbol(s_Current_State, it))).first;
				// move to the randomly chosen state
				for (size_t i = 0; i < n_State; i++)
					++m_it;
				// change the current state to the next one indicated by the chosen transition function
				print_State(s_Current_State, it, m_it->second);
				cout << endl;
				s_Current_State = m_it->second;
			}
		}
		// exception when there is no transition function for the current state and symbol
		catch (const std::out_of_range&) {
			cout << "\nAutomaton does not have a transition function for the current state: " << s_Current_State << " and current symbol: " << it << std::flush;
			return 0;
		}
		catch (const std::exception& e) {
			cout << "\n" << e.what() << std::flush;
			return 0;
		}
	}
	// check if the las state of the iteration is final, if that's the case then s_string is accepted by the automaton
	for (auto& it : v_F_S) {
		if (it == s_Current_State)
			return 1;
	}
	// if not accepted
	return 0;

}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: prints the nondeterministic automaton: its alphabet, states (denoting the initial an final ones) and the transition
	functions
-----------------------------------------------------------------------------------------------------------------------------------*/
void print_Automaton_ND(const vector<string>& v_A, const string& s_I_S, const vector<string>& v_F_S, const multimap<key_State_Symbol, string>& m_P_F_ND) {
	// procedure ---------------------------->
	cout << "\n\t\t--> Nondeterministic Automaton <--" << endl;
	cout << "\t-> Alphabet:" << endl;
	for (auto& i : v_A)
		cout << i << " ";
	cout << endl;
	cout << "\tInitial State: " << endl << s_I_S << endl;
	cout << "\tFinal State(s): " << endl;
	for (auto& i : v_F_S)
		cout << i << " ";
	cout << endl;
	cout << "\tProduction Functions:" << endl;
	for (auto& i : m_P_F_ND) {
		cout << "<<" << i.first.first << ", " << i.first.second << ">, " << i.second << ">" << endl;
	}
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: read a given nondeterministic automaton from a text file that follows the specified format
-----------------------------------------------------------------------------------------------------------------------------------*/
void read_automaton_nd_from_txt(const char* f_N, vector<string>& v_A, string& s_I_S, vector<string>& v_F_S, multimap<key_State_Symbol, string>& m_P_F_ND) {
	// variables declaration & initialization ---------------------------->
	char cp_TempLine[BUFFER_SIZE]{}, * cp_TempToken = nullptr, * cp_CurrentState = nullptr, * cp_CurrentSymbol = nullptr, * cp_FutureState = nullptr;
	char* cp_Position = nullptr;
	ifstream f_AutomatonFile(f_N);
	// procedure ---------------------------->
	// * this project was programmed on Windows OS so strtok_s was used
	if (f_AutomatonFile.is_open()) {
		// reading the alphabet
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		while (cp_TempToken != nullptr) {
			v_A.push_back(cp_TempToken);
			cp_TempToken = strtok_s(nullptr, " ", &cp_Position);
			//cp_TempToken = strtok(nullptr, " ");
		}
		// reading the initial state
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		s_I_S = cp_TempToken;
		// reading the final state(s)
		f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
		cp_TempToken = strtok_s(cp_TempLine, " ", &cp_Position);
		//cp_TempToken = strtok(cp_TempLine, " ");
		while (cp_TempToken != nullptr) {
			v_F_S.push_back(cp_TempToken);
			cp_TempToken = strtok_s(nullptr, " ", &cp_Position);
			//cp_TempToken = strtok(nullptr, " ");
		}
		// reading the transition functions
		while (!f_AutomatonFile.eof()) {
			f_AutomatonFile.getline(cp_TempLine, MAX_STREAM_SIZE);
			cp_CurrentState = strtok_s(cp_TempLine, " ", &cp_Position);
			//cp_CurrentState = strtok(cp_TempLine, " ");
			cp_CurrentSymbol = strtok_s(nullptr, " ", &cp_Position);
			//cp_CurrentSymbol = strtok(nullptr, " ");
			cp_FutureState = strtok_s(nullptr, " ->", &cp_Position);
			//cp_FutureState = strtok(nullptr, " ->");
			m_P_F_ND.insert(pair<key_State_Symbol, string>((key_State_Symbol(cp_CurrentState, *cp_CurrentSymbol)), cp_FutureState));
		}
	}
	else
		cout << "Program failed to open " << f_N << endl;
}

#endif // !AUTOMATON_H