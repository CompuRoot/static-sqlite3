#!/bin/sh

SQLITE_ZIP_URL='https://www.sqlite.org/2021/sqlite-amalgamation-3370000.zip'
SQLite_compressor='upx'  # Programm to use for compressing compiled sqlite
                         # Keep it empty as "" to disable compression

DockerImage='static-sqlite'

### No user intervention below this point ######################################

onErr(){
  local msg errNum
  msg="${1}"
  errNum=$2
  printf "Error[%i]: %s\n" ${errNum} "${msg}"
  exit 1
}

errMsgDep="Cannot continue due to absents of required dependancy"

ID=$(type id); [ $? -eq 0 ] && ID="/${ID#*/}" || onErr "${errMsgDep}" 1

if [ $($ID -u) -eq 0 ]; then
  SUDO=''
else
  SUDO=$(type sudo); [ $? -eq 0 ] && SUDO="/${SUDO#*/}" || SUDO=''
fi

DOCKER=$(type docker);
[ $? -eq 0 ] && DOCKER="/${DOCKER#*/}" || onErr "Install please docker first..." 2

$SUDO $DOCKER version  >/dev/null 2>&1
[ $? -ne 0 ] && {
  errMsg="$(printf '\n  You are not authorized to run docker,')"
  errMsg="${errMsg}$(printf '\n  try to "su -" into root account and try again.\n\n')"
  onErr  "${errMsg}" 3
}



cd ./docker

#$SUDO $DOCKER build --no-cache -t static-sqlite .       \
$SUDO $DOCKER build --rm -t "${DockerImage}" .          \
  --build-arg URL_SQLITE_SOURCE_ZIP="${SQLITE_ZIP_URL}" \
  --build-arg COMPRESS_SQLITE3="${SQLite_compressor}"
cd ..

printf "\n\n===== Taking ready to use static sqlite3 =======================================\n\n"

arch="${SQLITE_ZIP_URL##*/}"
workdir="${arch%.*}"

[ ! -d release ] && mkdir release

$SUDO $DOCKER run --rm -v $(pwd)/release:/release/  \
  -it "${DockerImage}"                              \
  cp -fv /app/${workdir}/sqlite3 /release/

$SUDO $DOCKER stop "${DockerImage}"

cd   release
echo "=============================="
ldd  sqlite3
echo "------------------------------"
file sqlite3
echo "------------------------------"
echo '.version' | ./sqlite3
echo "=============================="

cd ..

# Cleanup the built image
$SUDO $DOCKER image rm "${DockerImage}" >/dev/null 2>&1

