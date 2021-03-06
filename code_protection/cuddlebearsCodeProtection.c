#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
/*#include <openssl/evp.h> */

#include "cuddlebearsCodeProtection.h"

/* gensalt.c, gensalt.h and their makefile obtained from
 * perfec.to/gensalt */

int main()
{
	char *first_name, *last_name, *in, *out, *temp, *pw, line[FILE_LEN];
	/*FILE *data_file*/
	FILE *infile, *outfile;
	int a, b, valid_ints = 0;
	
	/* open the data file */
	/*data_file = open_file(DATA_FILE, "rw");*/
	
	get_name(&first_name, 0);
	get_name(&last_name, 1);
	
	while(!valid_ints) {
		/* get 2 integers from the user */
		a = get_int();
		b = get_int();
		flush();	

		/* check if the ints are too large for addition and mult to be safe */
		if( !safe_ops(a,b) )  {
			printf("Integer operations unsafe, select new integers\n");
			continue;
		}		
		break;
	}

	/* get input and output file names from the user */
	get_fname(&in, 0);
	get_fname(&out, 1);

	/* if the filenames are the same, add a 1 to the end of the outfile name */
	if(!strncmp(in, out, min_string_len(in, out))) {
		temp = out;
		out = calloc(strlen(temp)+1, sizeof(char));
		if( out == NULL ) {
			printf("Memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		out[0] = '\0';
		strcat(out, temp);
		strcat(out, "1");
		free(temp);
	}
	
	/* append the extension to the filenames */
	append_extension(&in);
	append_extension(&out);
	
	/* get the user's pw and store it */
	init_password(&pw);	
	
	/* ask the user for their pw and verify it */
	while(!verify_password(&pw)) {
		printf("Password verification failed. Try Again\n");
	}
	
	/* open input and output files	*/
	infile = open_file(in, "r");
	outfile = open_file(out, "w");	

	fprintf(outfile, "%s\n%s\n", first_name, last_name);
	fprintf(outfile, "%d + %d = %d\n", a, b, a+b);
	fprintf(outfile, "%d * %d = %d\n", a, b, a*b);
	
	/* print the contents of the input file into the output file */
	while( fgets(line, LINE_LEN, infile) ) {
		fprintf(outfile, "%s", line);
	}
	
	/* free memory */
	free(first_name);
	free(last_name);
	free(in);
	free(out);
		
	/* close files	*/
	fclose(infile);
	fclose(outfile);
	/*fclose(data_file);*/
		
	return 0;
} /* end main */

/* check that integer operations will be safe */
/* this is all broken */
int safe_ops(int a, int b)
{
	/* checks addition and multiplication overflow and underflow
	 * for a + b and a * b */
	if(a > 0 && b > 0 && (INT_MAX - a) < b ) {
		return 0;
	} 
	else if(a < 0 && b < 0 && (INT_MAX + a) < (-b)) {
		return 0;
	} else if( a > 0 && b > 0 && (INT_MAX / a) < b) {
		return 0;
	} else if(a > 0 && b < 0 && (INT_MIN / a) > b) {
		return 0;
	} else if(a<0 && b>0 && (INT_MIN / b) > a){
		return 0;
	} else if(a < 0 && b < 0 && (INT_MAX / a) > b) {
		return 0;
	} 
	
	return 1;
}

/* open file with the given name in the given mode */
FILE* open_file(char* fname, const char * mode)
{
	FILE* file = fopen(fname, mode);
	
	if(file == NULL) {
		printf("Could not open file %s\n", fname);
		exit(EXIT_FAILURE);
	}
	
	return file;
} /* end open file */

/* get a name from the user */
void get_name(char **name, int lname)
{
	char temp[60];
	*name = NULL;
	
	while( *name == NULL ) {
		if(!lname) {
			printf("Enter your first name: ");
		} else {
			printf("Enter your last name: ");
		}
		if( fgets(temp, sizeof(temp)/sizeof(char)-1, stdin) != NULL ){
			if( strlen(temp) > NAME_LEN) {
				printf("That name is too long\n");
				
				if (temp[strlen(temp)-1] != '\n') {
					flush();
				}
				continue;
			}
			
			/* get rid of the newline character */
			if (temp[strlen(temp)-1] == '\n') {
				temp[strlen(temp)-1] = '\0';
			}
		}
		
		
		if( !valid_name(temp) ) {
			printf("That name input is not valid\n");
		} else {
			/* +1 to null terminate */
			*name = (char*) malloc(sizeof(strlen(temp)+1) * sizeof(char));
			strncpy(*name, temp, strlen(temp));
		}
	}
} /* end get_name */

/* check if the name is valid */
int valid_name(char *name)
{
	int i;
	
	/* name must be at least 2 chars long */
	if(strlen(name) < 2) {
		printf("A single character or less is not a valid name\n");
		return 0;
	}
	
	/* if a every char of the name is not a letter or an 
	 * apostrophe, it is not valid */
	for(i = 0; i < strlen(name); i++){
		if(!isalpha(name[i]) && name[i] != '\'') {
			printf("invalid character in the name\n");
			return 0;
		}
	}
	return 1;
}

/* get an integer from the user */
int get_int()
{
	char check[FILE_LEN];
	int num, is_valid = 0;
	
	/* might need more protection in here */
	while(!is_valid) {
		printf("Enter an integer: ");
		fscanf(stdin, "%s", check);
		
		/* check if the input is valid */
		if(!valid_int(check)) {
			continue;
		}
		
		is_valid = 1;
	}
	
	num = atoi(check);
	
	return num;
}

/* Checks wheter a user's input is a valid integer input */
int valid_int(char* candidate)
{
	long int l;
	int i;
	char temp[2];
	
	/* check if the user input is too long to be an integer */
	if(strlen(candidate) > INT_LEN) {
		printf("The provided integer is too long\n");
		return 0;
	}
	
	/* check that every element of the string is a digit */	
	for( i = 0; i < strlen(candidate); i++ ) {
		temp[0] = candidate[i];
		temp[1] = '\0';
		if( strstr(DIGITS, temp) == NULL && 
		  (i !=0 && candidate[i] != '-' && strlen(candidate) == 1)) {
			
			printf("integer must include only digits\n");
			return 0;
		}
	}
	
	/* check for integers that are of the same legth as INT_MAX and INT_MIN for overrun */
	if(check_overflow(candidate)) {
		printf("Integer causes overflow\n");
		return 0;
	}

		
	/* above verified that the int is short enough to fit in a 
	 * long long int and is comprised of valid digits */
	l = atoi(candidate);

	/* check if the int is larger or smaller than the maximum
	 * or minimum integer value */
	if(l > INT_MAX || l < INT_MIN) {
		printf("The provided integer is out of range\n");
		return 0;
	}
		
	return 1;
}

/* need to finish this check
 * not sure if i'm incrementing in the right direction 
 * 
 * A long, cascading, and difficult to follow if*/
int check_overflow(char* s)
{
	int spacer, val, i;
	/* this could be vulnerable to modification in a hex editor */
	static int max_int_arr[] = {2,1,4,7,4,8,3,6,4,7};
	char temp[2];
	
	/*test printing out max_int_arr */
	
	if(strlen(s) < 10 ) {
		/*if int is this short it is not long enough to trigger overlow, 
		 * assuming 4 byte integers, which isn't always a safe assumption*/
		return 0;
	}
	
	/* if a non negative number is of INT_LEN length, then it is 1 digit too long */
	if(strlen(s) == INT_LEN && strstr(s, "-") == NULL) {
		return 1;
	}

	/* adjust the element being referenced based on 
	 * whether or not the number is negative */
	if(strstr(s, "-") != NULL) {
		spacer = 1;
	} else {
		spacer = 0;
	}
	
	/* if the number is of the maximum length for positive or negitive
	 * numbers, then walk through each digit to see if it is within range */	
	if((strlen(s) == INT_LEN && strstr(s, "-") != NULL) ||
	   (strlen(s) == INT_LEN-1 && strstr(s, "-") == NULL)) {

		temp[1] = '\0';
		for(i = 0; i < sizeof(max_int_arr)/sizeof(int); i++){
			temp[0] = s[i+spacer];
			val = atoi(temp);	
			
			if(val > max_int_arr[i]){
				/* return that there would be overflow*/
				return 1;
			} else if(val == max_int_arr[i]) {
				/* check the next digit */
				continue;
			} else {
				/* return that the value is in range */
				return 0;
			}
		}
	}
	
	return 0;
}
		
/* get a filename from the user */
void get_fname(char** file, int outfile)
{
	char temp[110];
	*file = NULL;

	/* simply using local files dramatically simplifies the process*/
	printf("FILE RESTRICTIONS: Files must be from this directory. Use local paths only\n");
	/* keep trying to get a filename util the user gives valid input */
	while(*file == NULL) {
		if(!outfile) {
			printf("Enter the name an existing file for the input file (without file extension): ");
		} else {
			printf("Enter the name of the output file (without file extension): ");
		}
		
		/* check that the file name was read in properly */		
		if( fgets(temp, FILE_LEN, stdin) == NULL ){
			printf("No file input found\n");
			continue;
		/* check that all chars are alphanumberic */
		}
		
		/* get rid of the newline character */
		if (temp[strlen(temp)-1] == '\n') {
			temp[strlen(temp)-1] = '\0';
		}
		
		if(non_alpha_chars(temp)){
			printf("Invalid characters\n");
			continue;
		}
		/* make sure the file name is not the same as the file that holds data */	
		/*}
		else if(!strncmp(temp, "data", 5)) {
			printf("Cannot use that filename\n");
			continue;
		} */ 
		
		/* +1 to null terminate */
		*file = (char*) malloc(sizeof(strlen(temp)+1) * sizeof(char));
		strncpy(*file, temp, strlen(temp));
	}
}

/* return the length of the shorter of 2 strings */
int min_string_len(char* a, char *b)
{
	if( strlen(a) < strlen(b) ) {
		return strlen(a);
	}
	
	return strlen(b);
}

/* check that all characters in a string are alphanumeric */
int non_alpha_chars(char* s)
{
	int i;	
	for( i = 0; i < strlen(s); i++) {
		
		/* checks if the character is alphanumeric */
		if(!isalnum(s[i])) {
			return 1;
		}
	}
	return 0;
}

/* append the defined file extension to a filename */
void append_extension(char **s)
{
	char * new_str = (char*) malloc(strlen(*s) + sizeof(char)*(EXT_LEN+1));
	
	if(new_str == NULL) {
		printf("Memory alloc failed\n");
		exit(EXIT_FAILURE);
	}
	
	new_str[0] = '\0';
	strcat(new_str, *s);
	strcat(new_str, EXTENSION);
	
	*s = new_str;
}

void init_password(char ** pw)
{
	char temp[50];
	int valid_pw = 0;
	
	while(!valid_pw){
		printf("Enter your password (6-24 characters): ");
		fgets(temp, (sizeof(temp)/sizeof(char))-1, stdin);
		
		/* get rid of the newline character */
		if (temp[strlen(temp)-1] == '\n') {
			temp[strlen(temp)-1] = '\0';
		}
		
		if( strlen(temp) < PW_MIN || strlen(temp) > PW_MAX ) {
			printf("Password not in size range\n");
			continue;
		}  
		
		*pw = (char*) malloc((sizeof(strlen(temp)))*sizeof(char));
		if( pw == NULL ){
			fprintf(stderr, "Password memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
	
		/* copy the password into its holder */
		strncpy(*pw, temp, strlen(temp));
		valid_pw = 1;
	}
	
/*	while(encrypt_password(&pw, data)){
		fprintf(stderr, "password encryption failed\n");
		continue;
	}*/
}
	
/* slightly modified from the resposnse by indiv at:
 * http://stackoverflow.com/questions/9488919/openssl-password-to-key */
 /* This is being scrapped for this project. The code is being retained 
  * show the idea behind it, though its implementation was not
  * entirely complete 
int encrypt_password(char **pw, FILE * data) 
{
	const EVP_CIPHER *cipher;
    const EVP_MD *dgst = NULL;
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
    unsigned char *password = (unsigned char*) pw;
    const unsigned char *salt = NULL;
    int i;

    OpenSSL_add_all_algorithms();

    cipher = EVP_get_cipherbyname("aes-256-cbc");
    if(!cipher) { fprintf(stderr, "no such cipher\n"); return 1; }

    dgst=EVP_get_digestbyname("md5");
    if(!dgst) { fprintf(stderr, "no such digest\n"); return 1; }

    if(!EVP_BytesToKey(cipher, dgst, salt,
        (unsigned char *) password,
        strlen((char*) password), 1, key, iv))
    {
        fprintf(stderr, "EVP_BytesToKey failed\n");
        return 1;
    }
	
    printf("Key: "); for(i=0; i<cipher->key_len; ++i) { printf("%02x", key[i]); } printf("\n");
    printf("IV: "); for(i=0; i<cipher->iv_len; ++i) { printf("%02x", iv[i]); } printf("\n");
    
    
    *pw = malloc(sizeof(key)/sizeof(char));
	for(i = 0; i < cipher->key_len; i++){
		fprintf(data, "%02x", key[i]);
		(*pw)[i] = (char) key[i];
	}
	fprintf(data, "\n");
    
    return 0;
	
} */

int verify_password(char ** pw)
{
	char temp[110];
	char *input;
	int valid_pw = 0;
	
	while(!valid_pw){		
		printf("Verify your password (6-24 characters): ");
		fgets(temp, (sizeof(temp)/sizeof(char))-1, stdin);
		
		if (temp[strlen(temp)-1] == '\n') {
			temp[strlen(temp)-1] = '\0';
		}
		
		if( strlen(temp) < PW_MIN || strlen(temp) > PW_MAX ) {
			printf("Password not in size range\n");
			continue;
		}  
		
		/* copy the password into its holder */
		input = (char*) malloc((sizeof(temp)/sizeof(char)) + 1);
		strncpy(input, temp, strlen(temp));
		break;
	}
	
	/* repeat this until eof */
	/* commented out along with other openssl encryption and file storge
	 * password code
	while(fgets(temp, LINE_LEN, data) != NULL) {
	
		hash = strtok(temp, DELIMIT);
			
		if( hash == NULL ) {
			fprintf(stderr, "Error: data.txt has been tampered with\n");
			printf("password storage error.\n");
			return 1;
		} else if (strlen(hash) != HASH_LENGTH || strtok(line, DELIMIT) != NULL) {
			fprintf(stderr, "The data has been corrupted\n");
			printf("password file corruption\n");
			return 1;
		}
		
		if( encrypt_password(&pw, data) ) {
			return 1;
		} 
		
		if( !strncmp(line, hash, HASH_LENGTH) ) {
			printf("Password accepted.\n");
			return 0;
		}
	} */
	
	/* check that the input pw and the previously stored on are
	 * both the same length */
	if( strlen(input) != strlen(*pw) ){
		return 0;
	} else if (strncmp(input, *pw, strlen(input)) != 0) {
		return 0;
	}
	
	printf("password verified\n");
	return 1;
} 

/* posted by jscmier at :
 * http://stackoverflow.com/questions/2187474/i-am-not-able-to-flush-stdin */
void flush()
{
	int c;
	while ((c = getchar()) != '\n' && c != EOF);
}
