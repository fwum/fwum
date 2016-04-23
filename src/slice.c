#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include "slice.h"
#include "util.h"

slice new_slice(char* string) {
	return make_slice(string, strlen(string));
}

slice make_slice(char* string, int length) {
	slice s;
	s.data = string;
	s.len = length;
	return s;
}

bool equals(slice s1, slice s2) {
	if(s1.len != s2.len)
		return false;
	for(int i = 0; i < s1.len; i++) {
		if(s1.data[i] != s2.data[i]) {
			return false;
		}
	}
	return true;
}

bool starts_with(slice s1, slice s2) {
	if(s1.len < s2.len) {
		return false;
	}
	for(int i = 0; i < s2.len; i++) {
		if(s1.data[i] != s2.data[i]) {
			return false;
		}
	}
	return true;
}

bool equals_string(slice s1, char* data) {
	int string_length = strlen(data);
	if(s1.len != string_length) {
		return false;
	}
	for(int i = 0; i < s1.len; i++) {
		if(s1.data[i] != data[i]) {
			return false;
		}
	}
	return true;
}

char* evaluate(slice str) {
	char *string = array(string, str.len);
	int i = 0;
	for(; i < str.len; i++) {
		string[i] = str.data[i];
	}
	string[i] = '\0';
	return string;
}

bool slice_contains(slice s1, char c) {
	for(int i = 0; i < s1.len; i++) {
		if(s1.data[i] == c) {
			return true;
		}
	}
	return false;
}
