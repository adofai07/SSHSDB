#ifndef U8STRING_H
#define U8STRING_H

int is_continuation(char);
int is_single_byte(char *);
int is_double_byte(char *);
int is_triple_byte(char *);
int is_quadruple_byte(char *);
int num_bytes(char *);
int u8strlen(const char *);
char *u8strcpy(char *, const char *);
char *u8strncpy(char *, const char *, int);
char *u8_remove_char(char *, int);
char *u8_add_char(char *, char *, int);

#endif