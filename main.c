#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include "bstr.h"
#include "note_table.h"

void usage(char *);

int
main(int argc, char **argv)
{
	int	err;
	char	*execn;
	barr_t	*notes;
	int	ret;

	err = 0;
	notes = NULL;

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

		printf("Got %d notes\n", barr_cnt(notes));
	}

	
end_label:

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
