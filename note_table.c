#include <stdio.h>
#include "bstr.h"
#include "note_table.h"

static note_t notes[] = {
        { "E0", "E", 0, 0 },
        { "E1", "E", 0, 0 },
        { "E2", "E", 0, 0 },
        { "E3", "E", 0, 0 },
        { "E4", "E", 0, 0 },
        { "E5", "E", 0, 0 },
        { "E6", "E", 0, 0 },
        { "E7", "E", 0, 0 },
        { "E8", "E", 0, 0 },
        { "E9", "E", 0, 0 },
        { "E10", "E", 0, 0 },
        { "E1", "E", 0, 0 },
        { "E1", "E", 0, 0 },
        { "E1", "E", 0, 0 },

        { NULL }
};

note_t *
getnote(const char *fretbc)
{
	note_t	*note;

	if(xstrempty(fretbc))
		return NULL;

	for(note = notes; !xstrempty(note->no_fretb_code); ++note) {
		if(!xstrcasecmp(note->no_fretb_code, fretbc))
			return note;
	}

	return NULL;
}


