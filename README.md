# rtMessage

# Building

## Desktop
* mkdir build
* cd build
* cmake -DBUILD_RTMESSAGE_LIB=ON -DBUILD_RTMESSAGE_SAMPLE_APP=ON -DBUILD_FOR_DESKTOP=ON BUILD_DATAPROVIDER_LIB=ON ..
* RELEASE BUILD -- Add -DCMAKE_BUILD_TYPE=release 
* DEBUG BUILD -- Add -DCMAKE_BUILD_TYPE=debug
* make
