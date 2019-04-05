#ifndef ARRAY_SIZE
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))
#endif

struct type_name {
        unsigned int type;
        const char *name;
};

const char *util_lookup_connector_status_name(unsigned int status);
const char *util_lookup_connector_type_name(unsigned int type);
int util_open(const char *device, const char *module);
