#!/bin/bash

#WORKDIR=$(cd `dirname $0`; pwd)
WORKDIR=$(pwd)

ICEPREFIX=/home/apps/cpplibs/Ice-3.6.4
export PATH=${ICEPREFIX}/bin:$PATH
export LD_LIBRARY_PATH=$ICEPREFIX/lib64:$ICEPREFIX/lib64/c++11:$LD_LIBRARY_PATH

CPPLIBS=/home/apps/cpplibs
export LD_LIBRARY_PATH=$CPPLIBS/db-5.3.28/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$CPPLIBS/libconfig-1.7.2/lib:$LD_LIBRARY_PATH

