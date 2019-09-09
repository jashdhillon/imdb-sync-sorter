#include "data.h"

#ifndef PROTOTYPES
#define PROTOTYPES
	//Prints error message
	void print_error(char* msg);

	//Check if string s1 and s2 are equal
	int is_string_equal(char* s1, char* s2);

	//Returns start and end indicies for string, trimming off the leadin		g white spaces inside and outside quotation marks
	void trim(char* str, int* start_index, int* end_index);

	//Returns number of occurances of character in the string provided
	int contains(char* str, char c);

	//Gets the next line from standard input
	//Must free results
	char* get_line();

	//Tokenizes the input string on the delimeter provided as an array o		f strings
	//Must free result
	char** tokenize(char* data, char delim, int* result_size);

	//Finds the index of the target string in the provided list
	int find(char** list, int list_len, char* target);

	//Checks to see if the string matches the given regex expression
	int match(regex_t *exp, char* str);

	//Checks to see if the input string is an integer
	int is_integer(char* data);

	//Checks to see if the input string is a double
	int is_double(char* data);

	//Checks the type of the string provided
	Type get_type(char* data);

	//Pulls the elements with a value of null for the data at the index 		cat_index to the front in the original order
	int bring_null_forward(Movie* movies, int cat_index, int size);

	//Utility function for displaying additional information on the data 		that may include the mean, median and mode
	void display_additional_data(Movie* movies, int start_index, int cat_index, char* cat_name, int size);
#endif
