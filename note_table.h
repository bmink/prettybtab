#ifndef NOTE_TABLE_H
#define NOTE_TABLE_H

typedef struct note {
	char    *no_fretb_code;
	char    *no_name;
	int     no_stringnr;    /* E = 0, A = 1, D = 2, G = 3 */
	int     no_fretpos;     /* 0 = open string */
} note_t;

note_t *getnote(const char *);

#endif
