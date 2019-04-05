/* 
        Author: Khairul Anuar Romli <khairul.anuar.bin.romli@gmail.com>
*/

#include <iostream>
#include <string.h>
#include "xf86drm.h"
#include "xf86drmMode.h"
#include "libhdcp.h"
#include "libdrm_util.h"
#include "libdrm_util_priv.h"
#include <errno.h>

const char *I915_DEV = "i915";

const int ENABLE_HDCP = 1;
const int DISABLE_HDCP = 0;

#if ANDROID
#include <hwcserviceapi.h>
#include <log/log.h>
#include <binder/IServiceManager.h>
#include <binder/ProcessState.h>
#include <iservice.h>
#endif

#define CONTENT_PROTECTION      1

void enabledisableHdcp(int drmFd, uint32_t connectorId, int nd) {
	int sts;
	int i;

	drmModePropertyPtr prop;
	drmModeObjectPropertiesPtr props;
	drmModeConnectorPtr c = NULL;

	sts = getdrmModeGetRes(drmFd);
	if (sts < 0) {
		return;
	}

#if ANDROID
	android::ProcessState::initWithDriver("/dev/vndbinder");

	// Connect to HWC service
	HWCSHANDLE hwcs = HwcService_Connect();
	if (hwcs == NULL) {
		std::cout<< "Could not connect to hwcservice" << std::endl;
		return;
	}
#endif

        for (i = 0; i < pDrmModeResPtr->count_connectors; i++) {
                pDrmModeConnectorPtr = drmModeGetConnector(drmFd, pDrmModeResPtr->connectors[i]);
                if(!pDrmModeConnectorPtr)
                        continue;
                if(strcmp(util_lookup_connector_status_name(pDrmModeConnectorPtr->connection), "connected") == 0 ) {
                        std::cout << pDrmModeConnectorPtr->connector_id << " " <<
                        util_lookup_connector_type_name(
                        pDrmModeConnectorPtr->connector_type)
                        << std::endl;

                        props = drmModeObjectGetProperties(drmFd,
                                                pDrmModeConnectorPtr->connector_id,
                                                DRM_MODE_OBJECT_CONNECTOR);

                        for (i = 0; i < (int)props->count_props; i++) {
                                prop = drmModeGetProperty(drmFd, props->props[i]);

                                if (strcmp(prop->name, util_lookup_cp_type_name(CONTENT_PROTECTION)) == 0)
                                {
#if ANDROID
					if (nd == 1) {
						sts = (int)HwcService_Video_EnableHDCPSession_ForDisplay(hwcs,
						connectorId,
						(EHwcsContentType)ENABLE_HDCP);
					} else {
						sts = (int)HwcService_Video_DisableHDCPSession_ForDisplay(hwcs,
                	        		c->connector_id);
					}
#endif
					sts = drmModeConnectorSetProperty(drmFd, connectorId,
						prop->prop_id, nd);

                                }
                        }
                }
        }

	return;
}

int getContentProtectionProp(int drmFd, drmModeResPtr res) {

        uint32_t i;
        drmModePropertyPtr prop;
        drmModeObjectPropertiesPtr props;

        for (i = 0; i < (uint32_t)res->count_connectors; i++) {
                pDrmModeConnectorPtr = drmModeGetConnector(drmFd, res->connectors[i]);
                if(!pDrmModeConnectorPtr)
                        continue;
                if(strcmp(util_lookup_connector_status_name(pDrmModeConnectorPtr->connection), "connected") == 0 ) {
			std::cout << pDrmModeConnectorPtr->connector_id << " " <<
                        util_lookup_connector_type_name(
                        pDrmModeConnectorPtr->connector_type)
                        << std::endl;

                        props = drmModeObjectGetProperties(drmFd,
                                                pDrmModeConnectorPtr->connector_id,
                                                DRM_MODE_OBJECT_CONNECTOR);

                        for (i = 0; i < props->count_props; i++) {
        			prop = drmModeGetProperty(drmFd, props->props[i]);

			        if (strcmp(prop->name, util_lookup_cp_type_name(CONTENT_PROTECTION)) == 0)
        			{
                			std::cout << prop->name << std::endl;
				        std::cout << prop->prop_id << std::endl;
	
        				if (!(drm_property_type_is(prop, DRM_MODE_PROP_BLOB)) ||
				   	(drm_property_type_is(prop, DRM_MODE_PROP_SIGNED_RANGE))) {
					std::cout << "Value: " << props->prop_values[i] << std::endl;
        				}
				}
			}
		}
	}
        
        drmModeFreeProperty(prop);
	return 0;
}

int getConnectorConnectionStatus(int drmFd, drmModeResPtr res) {
	int ret = 0;
	int i;

	drmModeObjectPropertiesPtr props;

	for (i = 0; i < res->count_connectors; i++) {
		pDrmModeConnectorPtr = drmModeGetConnector(drmFd, res->connectors[i]);
		if(!pDrmModeConnectorPtr)
			continue;
		if(strcmp(util_lookup_connector_status_name(pDrmModeConnectorPtr->connection), "connected") == 0 ) {
			std::cout << pDrmModeConnectorPtr->connector_id << " "<< 
			util_lookup_connector_type_name(
			pDrmModeConnectorPtr->connector_type)
		  	<< std::endl;
		}
	}
	return ret;
}

int getdrmModeGetRes(int drmFd) {
	int ret = 0;
	pDrmModeResPtr = drmModeGetResources(drmFd);
        if (NULL == pDrmModeResPtr) {
		ret = -ENODEV;
        }
	return ret;
}

int getDrmFd() {
	int drmFd;
	
	drmFd = util_open(NULL, I915_DEV);
	if (drmFd < 0) {
		std::cout << "Failed to open " << I915_DEV << std::endl;
	}
	return drmFd;
}
