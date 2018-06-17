#!/bin/bash
#created by wangfengliang

#Source function library.
. /etc/rc.d/init.d/functions

#Check that networking is up.
if [ "$NETWORKING" = "no" ]
then
    exit 0
fi

WORKDIR=$(cd `dirname $0`; pwd)

ulimit -c unlimited

PROGRAME="midware-http-ice-gateway"
CONFIG="server.cfg"

RETVAL=0
SELF_SCRIPT="$0"
SERVER_WC=`ps -ef | fgrep ${PROGRAME} | fgrep "${CONFIG}" | fgrep -v ' grep ' | fgrep -v "$SELF_SCRIPT" | wc -l`
SERVER_PIDS=`ps -ef | fgrep ${PROGRAME} | fgrep "${CONFIG}" | fgrep -v ' grep ' | fgrep -v "$SELF_SCRIPT" | awk '{print $2}'`

. ./setenv.sh
#mkdir -p logs/

start(){
    if [ ${SERVER_WC} -ge 1 ];then
        echo "${PROGRAME} already is running......"
    else
        cd ${WORKDIR}; nohup ./${PROGRAME} ${CONFIG} >/dev/null 2>&1 &
        RETVAL=$?
        if [ $RETVAL -ne 0 ] ; then
            action "start $PROGRAME fail......"  /bin/false
            exit 1
        else
            action "start $PROGRAME success......"  /bin/true
        fi
    fi
}

stop(){
    if [ ${SERVER_WC} -ge 1 ];then
        #kill -9 ${SERVER_PIDS}
        kill ${SERVER_PIDS}
        RETVAL=$?
        if [ $RETVAL -eq 0 ] ; then
            action "stop $PROGRAME ok......" /bin/true
        else
            action "stop $PROGRAME fail......" /bin/false
            exit 1 
        fi
    else
        echo "${PROGRAME} is not running......"
    fi
}

status(){
    ps -ef | fgrep ${PROGRAME} | fgrep "${CONFIG}" | fgrep -v ' grep ' | fgrep -v "$SELF_SCRIPT"
}

#See how we were called.
case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  info)
    status 
    ;;
  status)
    status 
    ;;
  *)
    echo "Usage: $0 {start|stop|status}"
    exit 1
esac

exit 0
