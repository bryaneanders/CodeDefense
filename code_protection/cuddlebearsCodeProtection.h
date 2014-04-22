
/* gensalt.c, gensalt.h and their makefile obtained from
 * perfec.to/gensalt */

#define NAME_LEN 50
#define FILE_LEN 100
#define PW_MAX 24
#define PW_MIN 6
#define NUM_DUMMIES 5
#define MAX_ATTEMPTS 5
#define LINE_LEN 100
#define DELIMIT " \n.,*()|\\/?><,!@#$%^&*~`;:[]{}abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DIGITS "0123456789"
#define HASH_LENGTH 64
#define SALT_LENGTH 8
#define INT_LEN 11
#define EXTENSION ".txt"
#define EXT_LEN 4
#define TEMPLATE "_[A-Za-z0-9./]{8}"

/*#define DATA_FILE "data.txt"*/

int safe_ops(int a, int b);
FILE* open_file(char* fname, const char * mode);
void get_name(char **name, int lname);
int valid_name(char *name);
int get_int();
int valid_int(char* candidate);
int check_overflow(char* s);
void get_fname(char** file, int outfile);
int min_string_len(char* a, char *b);
int non_alpha_chars(char* s);
void append_extension(char **s);
void init_password(char **pw);
int encrypt_password(char **pw, FILE * data) ;
char* generate_salt();
int verify_password(char **pw);
void flush();
