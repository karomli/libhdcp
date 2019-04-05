This is a very basic libraries to allow hdcp user space to enable or disable hdcp.

This libraries wrap libdrm call and specifically narrow down to only Content Protection display property.

Android:
This libraries will make use of libhwcservice API due to SetProperty call that only permit drmMaster to perform the operation.

Linux:
X server will need to be stop.

Build instruction:

Android:
cd $(ANDROID_TOP)/project/<this_project>
mm -jN

Linux:
mkdir build
cd build
cmake ../
make -jN


Dependencies:
Android:
libdrm
libhwcservice
libbinder

Linux:
libdrm

