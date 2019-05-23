#include <errno.h>
#include <libgen.h>
#include <stdio.h>
#include "bstr.h"
#include "note_table.h"

void usage(char *);

#define PBT_STRINGCNT	4
#define PBT_FRETNRWIDTH	3
#define PBT_INITSPACER	2
#define PBT_LINECHAR	'-'
#define PBT_LINEWIDTH	78

/* Note: we treat the E string as string[0]. */
char *string_pref[PBT_STRINGCNT] = {
	"E ",
	"A ",
	"D ",
	"G "
};


int
main(int argc, char **argv)
{
	int	err;
	char	*execn;
	bstr_t	*tempstr;
	int	i;
	note_t	*note;
	bstr_t	*noteln;
	bstr_t	*nstr;
	barr_t	*nstrs;
	int	ret;
	bstr_t	*strings[PBT_STRINGCNT];

	err = 0;
	tempstr = NULL;
	noteln = NULL;
	nstrs = NULL;
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
		nstrs = barr_init(sizeof(bstr_t *));
	} else {
		ret = xstrsplit(argv[1], ":", 0, &nstrs);
		if(ret != 0) {
			fprintf(stderr, "Couldn't parse notes: %s\n",
			    strerror(ret));
			err = ret;
			goto end_label;
		}
	}

#if 0
	printf("Got %d note(s).\n", barr_cnt(nstrs));

	for(nstr = (bstr_t *) barr_begin(nstrs);
	    nstr < (bstr_t *) barr_end(nstrs);
	    ++nstr) {
		printf("(%d) %s\n", bstrlen(nstr), bget(nstr));
	}
#endif

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		strings[i] = binit();
		if(strings[i] == NULL) {
			fprintf(stderr,
			    "Couldn't allocate buffer for string\n");
			err = ENOMEM;
			goto end_label;
		}
		bstrcat(strings[i], string_pref[i]);
		/* Initialize with one empty note's worth of chars */
		bstrpad(strings[i], bstrlen(strings[i]) + PBT_INITSPACER, '-');
	}

	noteln = binit();
	if(noteln == NULL) {
		fprintf(stderr, "Couldn't allocate buffer for noteln\n");
		err = ENOMEM;
		goto end_label;
	}
	bstrpad(noteln, bstrlen(strings[0]), ' ');
	
	tempstr = binit();
	if(tempstr == NULL) {
		fprintf(stderr, "Couldn't allocate buffer for tempstr\n");
		err = ENOMEM;
		goto end_label;
	}

	for(nstr = (bstr_t *) barr_begin(nstrs);
	    nstr < (bstr_t *) barr_end(nstrs);
	    ++nstr) {
		if(bstrlen(nstr) == 0) {
			bclear(tempstr);
			bstrpad(tempstr, PBT_FRETNRWIDTH, '-');
		
			for(i = PBT_STRINGCNT - 1; i >=0; --i) {
				bstrcat(strings[i], bget(tempstr));
			}

			bclear(tempstr);
			bstrpad(tempstr, PBT_FRETNRWIDTH, ' ');
			bstrcat(noteln, bget(tempstr));
			continue;
		}

		note = getnote(bget(nstr));
		if(note == NULL) {
			fprintf(stderr, "Invalid note specified: %s\n",
			    bget(nstr));
			err = ENOENT;
			goto end_label;
		}
		for(i = PBT_STRINGCNT - 1; i >=0; --i) {
			bclear(tempstr);
			if(i == note->no_stringnr) {
				bprintf(tempstr, "%d", note->no_fretpos);
			}
			bstrpad(tempstr, PBT_FRETNRWIDTH, '-');
			bstrcat(strings[i], bget(tempstr));
		}

		bclear(tempstr);
		for(i = 0; i < PBT_STRINGCNT; ++i) {
			if(i == note->no_stringnr) {
				bprintf(tempstr, "%s", note->no_name);
				break;
			}
		}
		bstrpad(tempstr, PBT_FRETNRWIDTH, ' ');
		bstrcat(noteln, bget(tempstr));
	}

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		bstrpad(strings[i], PBT_LINEWIDTH, '-');
	}
	
	printf("\n");
	for(i = PBT_STRINGCNT - 1; i >=0; --i) {
		printf("%s\n", bget(strings[i]));
	}
	printf("%s\n", bget(noteln));
	printf("\n");
	
end_label:

	if(nstrs != NULL) {
		for(nstr = (bstr_t *) barr_begin(nstrs);
		    nstr < (bstr_t *) barr_end(nstrs);
		    ++nstr) {
			buninit_(nstr);
		}
		barr_uninit(&nstrs);
	}

	for(i = 0; i < PBT_STRINGCNT; ++i) {
		if(strings[i] != NULL) {
			buninit(&strings[i]);
		}
	}

	if(tempstr != NULL)
		buninit(&tempstr);
	if(noteln != NULL)
		buninit(&noteln);

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
