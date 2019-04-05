#include "xf86drm.h"
#include "xf86drmMode.h"

drmModeResPtr pDrmModeResPtr = NULL;
drmModeObjectPropertiesPtr pDrmModeObjPropPtr = NULL;
drmModePropertyPtr pDrmModePropPtr = NULL;
drmModeConnectorPtr pDrmModeConnectorPtr = NULL;

int getDrmFd();
int getdrmModeGetRes(int drmFd);
int getContentProtectionProp(int drmFd, drmModeResPtr res);
int getConnectorConnectionStatus(int drmFd, drmModeResPtr res);

void enabledisableHdcp(int drmFd, uint32_t connectorId, int nd);
