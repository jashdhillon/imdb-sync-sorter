#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <sys/types.h>
#include <regex.h>

#include "merge.h"
#include "simpleCSVsorter.h"

//Prints error message
void print_error(char* msg) {
	fprintf(stderr, "\nOops! Somethings seems to have gone wrong here: %s\n\n", msg);
}

//Check if string s1 and s2 are equal
int is_string_equal(char* s1, char* s2)
{
	return !strcmp(s1, s2);
}

//Returns start and end indicies for string, trimming off the leading white spaces inside and outside quotation marks
void trim(char* str, int* start_index, int* end_index) {
	(*start_index) = 0;
	(*end_index) = strlen(str) - 1;

	if(str[*start_index] == '\0') {
		(*start_index) = 0;
		(*end_index) = 0;
		return;
	}

	while(isspace(str[*start_index]) && str[*start_index] != '\0') {
		(*start_index)++;
	}

	if(str[*start_index] == '"') {
		(*start_index)++;
	}

	while(isspace(str[*start_index]) && str[*start_index] != '\0') {
		(*start_index)++;
	}

	if(str[0] == '\0') {
		(*start_index) = 0;
		(*end_index) = 0;
		return;
	}

	while((*end_index) > (*start_index) && (isspace(str[*end_index]) || str[*end_index] == '\0')) {
		(*end_index)--;
	}

	if(str[*end_index] == '"') {
		(*end_index)--;
	}

	while((*end_index) > (*start_index) && (isspace(str[*end_index] || str[*end_index] == '\0'))) {
		(*end_index)--;
	}
}

//Returns number of occurances of character in the string provided
int contains(char* str, char c) {
	int i = 0, count = 0;

	while(str[i] != '\0') {
		count = (str[i] == c ? count + 1 : count);
		i++;
	}

	return count;
}

//Gets the next line from standard input
//Must free results
char* get_line() {
	char *input;
	int c = NULL, size = DEFAULT_BUFFER_SIZE, len = 0;
	
	input = realloc(NULL, sizeof(char) * size);

	if(input == NULL) {
		print_error("Cannot Allocate Memory");
		exit(1);
	}

	if(!input) {
		return input;
    	}

	c = fgetc(stdin);

	while(c != EOF && c != '\n') {
		if(c != '\r') {
	       		input[len] = c;

			len++;

			if(len == (size - 1)) {
				size += DEFAULT_BUFFER_INCREMENT;
        			input = realloc(input, sizeof(char) * size);
				if(input == NULL) {
					print_error("Cannot Allocate Memory");
					exit(1);
				}


         			if(!input) {
					return input;
				}
        		}
		}

		c = fgetc(stdin);
    	}

	input[len] = '\0';

	if(len < (size - 1)) {
		input = realloc(input, sizeof(char) * (len + 1));
		if(input == NULL) {
			print_error("Cannot Allocate Memory");
			exit(1);
		}
	}

	return input;
}

//Tokenizes the input string on the delimeter provided as an array of strings
//Must free result
char** tokenize(char* data, char delim, int* result_size) {
	int data_index = 0, buffer_index = 0, result_index = 0, buffer_size = DEFAULT_BUFFER_SIZE;
	*result_size = DEFAULT_ARRAY_SIZE;
	char** result = (char**) realloc(NULL, sizeof(char*) * (*result_size));

	if(result == NULL) {
		print_error("Cannot Allocate Memory");
		exit(1);
	}


	if(!result) {
		return result;
	}
	
	char* buffer = realloc(NULL, sizeof(char) * buffer_size);

	if(buffer == NULL) {
		print_error("Cannot Allocate Memory");
		exit(1);
	}

	while(data[data_index] != '\0') {

	    	char c = data[data_index];
		data_index++;

		//Resize buffer if more space is needed
		if(buffer_index == (buffer_size - 1)) {
			buffer_size += DEFAULT_BUFFER_INCREMENT;
			buffer = realloc(buffer, sizeof(char) * buffer_size);
			if(buffer == NULL) {
				print_error("Cannot Allocate Memory");
				exit(1);
			}

		}

		//Resize result if more space is needed
		if(result_index == ((*result_size) - 1)) {
			*result_size += DEFAULT_ARRAY_INCREMENT;
			result = realloc(result, sizeof(char*) * (*result_size));
			if(result == NULL) {
				print_error("Cannot Allocate Memory");
				exit(1);
			}

		}

		buffer[buffer_index] = '\0';

		int occur = contains(buffer, '"');

		if(c == delim && (!occur || occur > 1)) {
			
			//Remove "" when 2 are present
			if(0 && occur == 2) {
				int j = 1;
				
				buffer[buffer_index - 1] = '\0';

				for(j = 1; j <= buffer_index; j++) {
					buffer[j-1] = buffer[j];
				}
			}

			result[result_index] = buffer;
			result_index++;

			buffer_size = DEFAULT_BUFFER_SIZE;
			buffer_index = 0;
			buffer = realloc(NULL, sizeof(char) * buffer_size);
			if(buffer == NULL) {
				print_error("Cannot Allocate Memory");
				exit(1);
			}

		} else {
			buffer[buffer_index] = c;
			buffer_index++;
		}
	}

	buffer[buffer_index] = '\0';
	result[result_index] = buffer;
	result_index++;

	*result_size = result_index;

	return result;
}

//Finds the index of the target string in the provided list
int find(char** list, int list_len, char* target) {
	int i;

	for(i = 0; i < list_len; i++) {
		if(is_string_equal(list[i], target)) return i;
	}

	return -1;
}

//Checks to see if the string matches the given regex expression
int match(regex_t *exp, char* str) {
	regmatch_t matches[1];

	if(regexec(exp, str, 1, matches, 0) == 0) {
		return 1;
	} else {
		return 0;
	}
}

//Checks to see if the input string is an integer
int is_integer(char* data) {
	
	int rv;
	regex_t exp;

	rv = regcomp(&exp, "^-?[0-9]+$", REG_EXTENDED);

	if(rv != 0) {
		print_error("Regex expression could not be compiled");
		exit(1);
	}

	return match(&exp, data);
}

//Checks to see if the input string is a double
int is_double(char* data) {
	
	int rv;
	regex_t exp;

	rv = regcomp(&exp, "^-?[0-9]+(\\.[0-9]+)?$", REG_EXTENDED);

	if(rv != 0) {
		print_error("Regex expression could not be compiled");
		exit(1);
	}

	return match(&exp, data);
}

//Checks the type of the string provided
Type get_type(char* data) {
	if(is_integer(data)) {
		return Integer;
	} else if(is_double(data)) {
		return Double;
	}
	return String;
}

//Pulls the elements with a value of null for the data at the index cat_index to the front in the original order
int bring_null_forward(Movie* movies, int cat_index, int size) {
	int i, j, count = 0;

	for(i = 0; i < size; i++) {
		char* data = movies[i].data[cat_index];
		
		if(data[0] == '\0') {
			count++;
			int null_index = i;
			
			for(j = 0; j < size; j++) {
				char* other_data = movies[j].data[cat_index];
				if(other_data[0] != '\0') {
					int replace_index = j;
					Movie temp = movies[null_index];
					movies[null_index] = movies[replace_index];
					movies[replace_index] = temp;
				}
			}
		}
	}

	int last_null_index = -1;

	for(i = 0; i < size; i++) {
		char* data = movies[i].data[cat_index];

		if(data[0] == '\0') {
			last_null_index = i;
		}
	}
	
	if(DEBUG) printf("\n\nNULL Count: %d", count);

	return last_null_index;
}

//Utility function for displaying additional information on the data that may include the mean, median and mode
void display_additional_data(Movie* movies, int start_index, int cat_index, char* cat_name, int size) {
	int i, j;

	Type data_type = None;
	
	for(i = 0; i < size; i++) {
	    	Movie movie = movies[i];
		char* data = movie.data[cat_index];
		if(*data != '\0') {
		    	data_type = get_type(data);

			if(data_type == Integer || data_type == Double)
				break;
		}
	}

	int mode = 0;
	char* mode_val = '\0';
	int* mode_count = malloc(size * sizeof(int));
	double mean = 0;
	double median = 0;

	for(i = start_index; i < size; i++) {
		Movie movie = movies[i];
		char* data = movie.data[cat_index];

		mode_count[i] = 1;

		for(j = i; j < size; j++) {
			Movie other_movie = movies[j];
			char* other_data = other_movie.data[cat_index];

			if(is_string_equal(data, other_data)) {
			    mode_count[i]++;
			}
		}

		if(data_type == Integer || data_type == Double) {
			double val;
			sscanf(data, "%lf", &val);
			mean += val;
		}
	}

	for(i = start_index; i < size; i++) {
		if(mode_count[i] > mode) {
			mode = mode_count[i];
			mode_val = movies[i].data[cat_index];
		}
	}

	printf("\nThe mode of the data set for the category: %s is %s with %d occurences\n", cat_name, mode_val, mode);

	if(data_type == Integer || data_type == Double) {
		mean /= (double)size;
		printf("\nThe mean of the data set for the category: %s is %lf\n", cat_name, mean);

		if(size % 2 == 0) {

			double l_val, r_val;
			char* l_data = movies[size/2].data[cat_index];
			char* r_data = movies[size/2 + 1].data[cat_index];

			sscanf(l_data, "%lf", &l_val);
			sscanf(r_data, "%lf", &r_val);
			
			median = (l_val + r_val) / 2.0;

		} else {
			Movie movie = movies[size / 2 + 1];
			char* data = movie.data[cat_index];
			sscanf(data, "%lf", &median);
		}

		printf("\nThe median of the data set for the category: %s is %lf\n", cat_name, median);
	}

	free(mode_count);
}

//Main function of execution
int main(int argc, char **argv) {

    	//printf("%d\n", is_integer(" 77"));
	//return 0;
	Mode mode = Column;
	
	if(argc < 2) {
		print_error("Invalid Argument Count. Must specify sorting mode<c> and category<category_name>");
		exit(1);
	}

	if(is_string_equal(argv[1], "-c")) {
	    mode = Column;
	} else {
		print_error("Invalid sorting mode selected. Must specify sorting mode<c> and category<category_name>");
		exit(1);
	}

	char* sort_cat = argc > 2 ? argv[2] : "";
	if(DEBUG) printf("Current Sort Mode is %d and sorting based on %s\n", mode, sort_cat);

	char* data = get_line();
	
	int cat_count = 0;
	char** categories = tokenize(data, ',', &cat_count);

	free(data);

	int i, j;

	if(DEBUG) {
		for(i = 0; i < cat_count; i++) {
			printf("[%d]: %s\n", i, categories[i]);
		}
	}
	
	int cat_index = find(categories, cat_count, sort_cat);

	if(cat_index < 0) {
		print_error("Invalid category selected");
		exit(1);
	}

	if(DEBUG) {
		printf("\nIndex of search is %d\n", cat_index);
		printf("%s\n", sort_cat);
	}

	int count = 0;

	data = get_line();

	int movie_size = DEFAULT_ARRAY_SIZE;

	Movie* movies = realloc(NULL, movie_size * sizeof(Movie));
	if(movies == NULL) {
		print_error("Cannot Allocate Memory");
		exit(1);
	}

	while(data[0] != '\0') {
		Movie temp;
		
		int data_len = 0;
		char** temp_data = tokenize(data, ',', &data_len);
		temp.data = temp_data;
		temp.raw_data = data;
		temp.size = data_len;
		movies[count] = temp;

		count++;

		if(count == (movie_size - 1)) {
			movie_size += DEFAULT_ARRAY_INCREMENT;
			movies = realloc(movies, movie_size * sizeof(Movie));
			if(movies == NULL) {
				print_error("Cannot Allocate Memory");
				exit(1);
			}

		}

		data = get_line();
	}

	if(DEBUG) {
		printf("---START---\nCOUNT:%d\n", count);

		for(i = 0; i < 10; i++) {
			printf("%s\n", movies[i].data[cat_index]);
		}
	}

	int last_null = bring_null_forward(movies, cat_index, count);

	if(last_null < 0) {
		last_null = 0;
	}

	sort(movies, cat_index, last_null, count - 1);

	if(DEBUG) printf("\n\n\n");

	for(i = last_null + 1; i < count; i++) {
		if(DEBUG) {	
			printf("[%d]:%s\n", i, movies[i].data[cat_index]);
			//printf("[%d]:%s\n", i, movies[i].raw_data);
		} else {
			printf("%s\n", movies[i].raw_data);
		}
	}

	if(argc > 3 && is_string_equal(argv[3], "-d")) {
		display_additional_data(movies, last_null + 1, cat_index, categories[cat_index], count);
	}

	//Freeing the data
	for(i = 0; i < count; i++) {
		Movie movie = movies[i];
		for(j = 0; j < movie.size; j++) {
			free(movie.data[j]);
		}

		free(movie.data);
		free(movie.raw_data);
	}

	free(movies);
	
	
	return 0;
}
