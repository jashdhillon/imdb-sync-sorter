#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "merge.h"

//Merges 2 lists in sorted order
//Core of the merge sort sorting fucntion
void merge(Movie* movies, int cat_index, int l, int m, int r) {
	
	int i, j, k;
	int n1 = m - l + 1;
	int n2 = r - m;

	Movie* L = malloc(n1 * sizeof(Movie));
	Movie* R = malloc(n2 * sizeof(Movie));

	for(i = 0; i < n1; i++) {
		Movie temp;
		temp.raw_data = movies[l + i].raw_data;
		temp.data = movies[l + i].data;
		temp.size = movies[l + i].size;

		L[i] = temp;
	}

	for(j = 0; j < n2; j++) {
	    	Movie temp;
		temp.raw_data = movies[m + j + 1].raw_data;
		temp.data = movies[m + j + 1].data;
		temp.size = movies[m + j + 1].size;

		R[j] = temp;
	}

	i = 0;
	j = 0;
	k = l;

	while(i < n1 && j < n2) {		

		char* l_data = L[i].data[cat_index];
		char* r_data = R[j].data[cat_index];

		Type data_type = get_type(l_data);

		if(data_type == Integer || data_type == Double) {
			double l_val, r_val;

			sscanf(l_data, "%lf", &l_val);
			sscanf(r_data, "%lf", &r_val);

			if(l_val <= r_val) {
				movies[k] = L[i];
				i++;
			} else {
				movies[k] = R[j];
				j++;
			}
				
		} else {
			int left_start = 0, left_end = 0;
			int right_start = 0, right_end = 0;

			trim(L[i].data[cat_index], &left_start, &left_end);
			trim(R[j].data[cat_index], &right_start, &right_end);

			l_data = malloc((left_end - left_start) * sizeof(char*));
			r_data = malloc((right_end - right_start) * sizeof(char*));

			int p, q;

			for(p = 0; p < left_end; p++) {
				l_data[p] = L[i].data[cat_index][left_start + p];
			}

			for(q = 0; q < right_end; q++) {
				r_data[q] = R[j].data[cat_index][right_start + q];
			}

			if(strcmp(l_data, r_data) <= 0) {	
				movies[k] = L[i];
				i++;
			} else {
				movies[k] = R[j];
				j++;
			}

			free(l_data);
			free(r_data);
		}

		k++;
	}

	while(i < n1) {
		movies[k] = L[i];
		i++;
		k++;
	}

	while(j < n2) {
		movies[k] = R[j];
		j++;
		k++;
	}

	//delete temp arrays
	free(L);
	free(R);
}

//Helper function for the merge sort sorting algorithm
void sort(Movie* movies, int cat_index, int l, int r) {
	if(l < r) {
		int m = (l + r) / 2;

		sort(movies, cat_index, l, m);
		sort(movies, cat_index, m+1, r);
		merge(movies, cat_index, l, m, r);
	}
}
