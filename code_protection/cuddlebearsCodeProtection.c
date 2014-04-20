#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include <openssl/evp.h>

#include "mtwist.h"
#include "randistrs.h"
#include "gensalt.h"
#include "cuddlebearsCodeProtection.h"

/* gensalt.c, gensalt.h and their makefile obtained from
 * perfec.to/gensalt */

int main()
{
	char *first_name, *last_name, *in, *out, *temp, line[FILE_LEN];
	FILE *data_file, *infile, *outfile;
	int a, b;
	
	/* seed the rng */
	mt_seed32(mt_goodseed());
	srand (time(NULL));
	
	/* open the data file */
	data_file = open_file(DATA_FILE, "rw");
	
	get_name(&first_name, 0);
	get_name(&last_name, 1);
	
	/* get 2 integers from the user */
	a = get_int();
	b = get_int();	
	
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
	init_password(data_file);	
	
	/* ask the user for their pw and verify it */
	if(!verify_password(data_file)) {
		printf("Password verification failed. Exiting\n");
		return 0;
	}
	
	/* open input and output files	*/
	infile = open_file(in, "r");
	outfile = open_file(out, "w");	

	fprintf(outfile, "%s %s\n", first_name, last_name);
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
	fclose(data_file);	
		
	return 0;
} /* end main */

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
	*name = NULL;
	
	while( name == NULL ) {
		if(!lname) {
			printf("Enter your first name: ");
		} else {
			printf("Enter your last name: ");
		}
		fgets((*name), NAME_LEN, stdin);
		printf("\n");
		
		if( !valid_name(*name) ) {
			printf("That name input is not valid\n");	
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
		if(!isalpha(name[i]) || name[i] != '\''){
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
		printf("Enter an integer\n");
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
	
	/* check if the user input is too long to be an integer */
	if(strlen(candidate) > INT_LEN) {
		printf("The provided integer is too long\n");
		return 0;
	}
	
	/* check that every element of the string is a digit */	
	for( i = 0; i < strlen(candidate); i++ ) {
		if( strstr(DIGITS, &candidate[i]) == NULL ) {
			printf("integer must include only digits\n");
			return 0;
		}
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
		
/* get a filename from the user */
void get_fname(char** file, int outfile)
{
	*file = NULL;

	/* keep trying to get a filename util the user gives valid input */
	while(*file == NULL) {
		if(!file) {
			printf("Enter the name of the input file (without file extension): ");
		} else {
			printf("Enter the name of the output file (without file extension): ");
		}
		fgets(*file, FILE_LEN, stdin);
		
		/* check that the file name was read in properly */
		if(*file == NULL) {
			printf("No file input found\n");
			continue;
		/* check that all chars are alphanumberic */
		} else if(non_alpha_chars(*file)){
			printf("Invalid characters\n");
			continue;
		/* make sure the file name is not the same as the file that holds data */	
		} else if(strncmp(*file, "data", 5)) {
			printf("Cannot use that filename\n");
			continue;
		} 
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
		/* checks if the character is a letter */
		if(!isalpha(s[i])) {
			/* checks if the char is a number */
			if( strstr(DIGITS, &s[i]) == NULL ) {
				return 0;
			}
		}
	}
	return 1;
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
	
	free(s);
	*s = new_str;
}

void init_password(FILE * data)
{
	/* create a few dummy pw's
	 * get the password from the user then begin salting and hashing
	 * the dummy passwords into the file and salt and hash the user's pw
	 * into the file at a random point during that filling */
	char ** dummies = malloc(sizeof(char*) * NUM_DUMMIES);
	char temp[50];
	char *pw;
	int i, pos, valid_pw = 0;
	
	if(dummies == NULL) {
		fprintf(stderr, "Could not allocate string holder!\n");
		exit(EXIT_FAILURE);
	}
	
	for( i = 0; i < NUM_DUMMIES; i++) {
		dummies[i] = generate_salt();
	}
	
	/* determine when to write the user's pw
	 * need to check the length of ints on my system to see if this works */
	pos = (int) rd_iuniform(INT_MIN, INT_MAX) % NUM_DUMMIES;
	
	while(!valid_pw){
		printf("Enter your password (6-24 characters):\n");
		fgets(temp, sizeof(temp)/sizeof(char) -1, stdin);
		
		if( strlen(temp) < 6 || strlen(temp) > 24 ) {
			printf("Password not in size range\n");
			continue;
		}  
		
		pw = (char*) malloc((sizeof(strlen(temp))+1)*sizeof(char));
		if( pw == NULL ){
			fprintf(stderr, "Password memory allocation failed\n");
			exit(EXIT_FAILURE);
		}
		
		/* copy the password into its holder */
		strncpy(pw, temp, strlen(temp));
	}
	
	for( i = 0; i < NUM_DUMMIES;){
		/* if it is the position we generated, encrypt the user's pw */
		if( i == pos ) {
			while(encrypt_password(&pw, NULL, data)){
				fprintf(stderr, "password encryption failed\n");
				continue;
			}
		}
		
		/* otherwise encrypt a dummy password */
		while(encrypt_password(&dummies[i++], NULL, data)){
			fprintf(stderr, "password encryption failed\n");
			continue;
		}
	} 
	
}
	
/* slightly modified from the resposnse by indiv at:
 * http://stackoverflow.com/questions/9488919/openssl-password-to-key */
int encrypt_password(char **pw, const unsigned char* s, FILE * data) 
{
	const EVP_CIPHER *cipher;
    const EVP_MD *dgst = NULL;
    unsigned char key[EVP_MAX_KEY_LENGTH], iv[EVP_MAX_IV_LENGTH];
    unsigned char *password = (unsigned char*) *pw;
    const unsigned char *salt;
    int i;

	if( s == NULL ) {
		salt = (unsigned char*) generate_salt();
	} else {
		salt = s;
	}

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
    
    if( s == NULL ){
		fprintf(data, "%s, %s\n", key, salt);
	}
	
	*pw = (char*) key;

    /* these will be used for testing only presubmssion */
    printf("Key: "); for(i=0; i<cipher->key_len; ++i) { printf("%02x", key[i]); } printf("\n");
    printf("IV: "); for(i=0; i<cipher->iv_len; ++i) { printf("%02x", iv[i]); } printf("\n");
    
    return 0;
	
}

/* the setup is just so that it is the function prototype allows
 * to send it to getnsalt */
int rand_num(int n) 
{
	return (int) rd_iuniform((int32_t) INT_MIN, (int32_t) INT_MAX);
}

/* Create a salt value for encrypting the password */
char* generate_salt()
{
	/* The template for short MD5 for gensalt */
	char *template = (char*) malloc(sizeof(TEMPLATE)+sizeof(char));
	char * result = NULL;
	int (*foo)(int) = &rand_num;
	
	if(template == NULL) {
		printf("failed to generate template holder\n");
		exit(EXIT_FAILURE);
	}
	
	strncpy(template, TEMPLATE, sizeof(TEMPLATE) / sizeof(char));
	template[sizeof(template)/sizeof(char)-1] = '\0'; 
	
	while(result == NULL) {
		/* this typecast is sketchy, but I don't think it will matter that
		 * much with a small scale example like this */
		result = gensalt(template, foo);
		
		if(result == NULL) {
			fprintf(stderr, "Salt error\n");
		}
	}
	
	free(template);
	
	return result;	
}

int verify_password(FILE* data)
{
	char temp[50];
	char *pw, *line, *hash, *salt;
	int i, valid_pw = 0;
	
	line = hash = salt = NULL;	
	i = 0;
	while(!valid_pw){
		if( i >= MAX_ATTEMPTS ) {
			printf("Too many failed login attempts. Try again later\n");
			return 0;
		}
		
		printf("Enter your password (6-24 characters):\n");
		fgets(temp, sizeof(temp)/sizeof(char) -1, stdin);
		
		if( strlen(temp) < 6 || strlen(temp) > 24 ) {
			printf("Password not in size range\n");
			i++;
			continue;
		}  
		
		/* copy the password into its holder */
		strncpy(pw, temp, strlen(temp));
	}
	
	/* repeat this until eof */
	while(fgets(line, LINE_LEN, data)) {
	
		/* use strtok */
		hash = strtok(line, DELIMIT);
		salt = strtok(line, DELIMIT);
			
		if( hash == NULL || salt == NULL ) {
			fprintf(stderr, "Error: data.txt has been tampered with\n");
			printf("password storage error.\n");
			return 1;
		} else if (strlen(hash) != HASH_LENGTH || strlen(salt) != SALT_LENGTH ||
				   strtok(line, DELIMIT) != NULL) {
			fprintf(stderr, "The data has been corrupted\n");
			printf("password file corruption\n");
			return 1;
		}
		
		if( encrypt_password(&pw, (unsigned char*) salt, data) ) {
			return 1;
		} 
		
		if( !strncmp(pw, hash, HASH_LENGTH) ) {
			printf("Password accepted.\n");
			return 0;
		}
	}
	
	printf("Invalid password\n");
	return 1;
}

