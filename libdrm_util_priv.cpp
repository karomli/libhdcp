/* 
	Khairul Anuar Romli <khairul.anuar.bin.romli@gmail.com>
*/

#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "libdrm_util.h"
#include "libdrm_util_priv.h"

static const char *util_lookup_type_name(unsigned int type,
                                         const struct type_name *table,
                                         unsigned int count)
{
        unsigned int i;

        for (i = 0; i < count; i++)
                if (table[i].type == type)
                        return table[i].name;
        return NULL;
}

static const struct type_name cp_name[] = {
        { 0, "unknown" },
        { 1, "Content Protection" },
        { 2, "CP" },
};

const char *util_lookup_cp_type_name(unsigned int type)
{
        return util_lookup_type_name(type, cp_name,
                                        ARRAY_SIZE(cp_name));
}
