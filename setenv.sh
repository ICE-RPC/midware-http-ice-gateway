#!/bin/bash

#WORKDIR=$(cd `dirname $0`; pwd)
WORKDIR=$(pwd)

ICEPREFIX=/home/apps/cpplibs/Ice-3.6.4
export PATH=${ICEPREFIX}/bin:$PATH
export LD_LIBRARY_PATH=$ICEPREFIX/lib64:$ICEPREFIX/lib64/c++11:$LD_LIBRARY_PATH

CPPLIBS=/home/apps/cpplibs
export LD_LIBRARY_PATH=$CPPLIBS/boost-1.61.0/lib:$CPPLIBS/boost_1_61_0/lib:$LD_LIBRARY_PATH
export LD_LIBRARY_PATH=$CPPLIBS/db-5.3.28/lib:$LD_LIBRARY_PATH

