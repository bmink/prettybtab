#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include "bstr.h"
#include "note_table.h"

void usage(char *);

#define PBT_STRINGCNT	4

char *string_pref[PBT_STRINGCNT] = {
	"G ---",
	"D ---",
	"A ---",
	"E ---"
};


int
main(int argc, char **argv)
{
	int	err;
	char	*execn;
	int	i;
	bstr_t	*note;
	barr_t	*notes;
	int	ret;
	bstr_t	*strings[PBT_STRINGCNT];

	err = 0;
	notes = NULL;
	for(i = 0; i < PBT_STRINGCNT; ++i) {
		strings[i] = NULL;
	}

	if(xstrempty(argv[0])) {
		fprintf(stderr, "Invalid argv[0]\n");
		err = EINVAL;
		goto end_label;
	}

	execn = basename(argv[0]);
	if(xstrempty(execn)) {
		fprintf(stderr, "Can't get executable name\n");
		err = ENOEXEC;
		goto end_label;
	}
	
	if(argc > 2) {
		usage(execn);
		exit(-1);
	}

	if(argc == 1) {
		/* No argument -- print an empty line. */
		notes = barr_init(sizeof(bstr_t *));
	} else {
		ret = xstrsplit(argv[1], ":", 0, &notes);
		if(ret != 0) {
			fprintf(stderr, "Couldn't parse notes: %s\n",
			    strerror(ret));
			err = ret;
			goto end_label;
		}
	}

	printf("Got %d note(s).\n", barr_cnt(notes));

	for(note = (bstr_t *) barr_begin(notes);
	    note < (bstr_t *) barr_end(notes);
	    ++note) {
		printf("(%d) %s\n", bstrlen(note), bget(note));
	}

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		strings[i] = binit();
		if(strings[i] == NULL) {
			fprintf(stderr,
			    "Couldn't allocate buffer for string\n");
			err = ENOMEM;
			goto end_label;
		}
		bstrcat(strings[i], string_pref[i]);
	}
	

	for(note = (bstr_t *) barr_begin(notes);
	    note < (bstr_t *) barr_end(notes);
	    ++note) {
	}

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		printf("%s\n", bget(strings[i]));
	}

	
end_label:


	if(notes != NULL) {
		for(note = (bstr_t *) barr_begin(notes);
		    note < (bstr_t *) barr_end(notes);
		    ++note) {
			buninit_(note);
		}
		barr_uninit(&notes);
	}

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		if(strings[i] != NULL) {
			buninit(&strings[i]);
		}
	}

	return err?EINVAL:0;
}


void
usage(char *execn)
{
	if(xstrempty(execn))
		return;

	printf("Usage: %s [notes]\n", execn);
	printf("\n");
	printf("       notes - e.g. \"E0:A2::D3:::G3:D3\"\n");
	printf("\n");


}
