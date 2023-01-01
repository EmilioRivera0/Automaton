/*-----------------------------------------------------------------------------------------------------------------------------------
- Software Name: Deterministic and Nondeterministic Automata
- Version: 1.0
- Language: C++
- Developer: Emilio Rivera Macías
- Date: 10/18/2022
- Contact: emilioriveramacias@gmail.com
-----------------------------------------------------------------------------------------------------------------------------------*/

#ifndef GRAMMAR_H
#define GRAMMAR_H

// necessary includes ---------------------------->
#include <vector>
using std::vector;
#include <string>
using std::string;
#include <cstdlib>
#include <iostream>
using std::cout;
using std::endl;

// functions declaration and definition ---------------------------->
/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: generate the universe of speech with the given Alphabet (parameter) of n length (parameter)
-----------------------------------------------------------------------------------------------------------------------------------*/
const vector<string> generate_universe_of_speech_of_n_lenght(const vector<string>& v_Alphabet, const int& n_Length) {
	// variables declaration & initialization ---------------------------->
	int n_TemporalLength{ 0 };
	vector<string> v_Universe_of_Speech{};
	vector<string>::iterator it_first, it_last;

	// procedure ---------------------------->
	// if the universe of speech is empty (lambda)
	if (n_Length == 0)
		return v_Universe_of_Speech;
	// length is of 1
	else if (n_Length == 1)
		return v_Alphabet;
	// length > 1
	else {
		v_Universe_of_Speech = v_Alphabet;
		// vector capacity (memory) is reserved before anything to optimize each push_back
		v_Universe_of_Speech.reserve((pow(v_Alphabet.size(), n_Length) + pow(v_Alphabet.size(), n_Length - 1)));
		// elevating the Alphabet to the n_Length power	
		for (size_t un_Actual_Length = 2; un_Actual_Length <= n_Length; un_Actual_Length++) {
			n_TemporalLength = v_Universe_of_Speech.size();
			for (size_t iT = 0; iT < n_TemporalLength; iT++) {
				for (size_t iA = 0; iA < v_Alphabet.size(); iA++)
				{
					v_Universe_of_Speech.push_back(v_Universe_of_Speech[iT] + v_Alphabet[iA]);
				}
			}
			//the strings generated in the previous iteration are deleted
			it_first = v_Universe_of_Speech.begin();
			it_last = it_first + n_TemporalLength;
			v_Universe_of_Speech.erase(it_first, it_last);
		}
	}
	return v_Universe_of_Speech;
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: generate a random string with the Alphabet (parameter) of a random size between 1 - 9
-----------------------------------------------------------------------------------------------------------------------------------*/
const string generate_string(const vector<string>& v_Alphabet) {
	// variables declaration & initialization ---------------------------->
	string s_String{};
	short int n_size{ rand() % 9 + 1 }, n_it{ 0 };
	// procedure ---------------------------->
	srand(time(NULL));
	// generate the random string
	for (size_t i = 0; i < n_size; i++) {
		n_it = rand() % v_Alphabet.size();
		s_String += v_Alphabet[n_it];
	}
	return s_String;
}

/*-----------------------------------------------------------------------------------------------------------------------------------
- Functionality: print the universe of speech, alphabet and length of strings (parameters)
-----------------------------------------------------------------------------------------------------------------------------------*/
void print_universe_of_speech(const vector<string>& v_Universe_of_Speech, const vector<string>& v_Alphabet, const int& n_Length) {
	// procedure ---------------------------->
	cout << "\n\t-> Universe of Speech <-" << endl;
	cout << "Alphabet:";
	for (auto& iA : v_Alphabet)
		cout << " " << iA;
	cout << endl;
	cout << "Length of Strings: " << n_Length << endl;
	cout << "\nGenerated Strings:" << endl;
	for (auto& iV : v_Universe_of_Speech) {
		cout << " " << iV << " /";
	}
}
#endif // !GRAMMAR_H