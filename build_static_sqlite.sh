#!/bin/sh

SQLITE_ZIP_URL='https://sqlite.org/2024/sqlite-amalgamation-3470000.zip'
SQLite_compressor='upx'  # Program to use for compressing compiled sqlite
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

errMsgDep="Cannot continue due to absence of required dependency"

ID=$(command -v id); [ $? -ne 0 ] && onErr "${errMsgDep}" 1

if [ $($ID -u) -eq 0 ]; then
  SUDO=''
else
  SUDO=$(command -v sudo); [ $? -ne 0 ] && SUDO=''
fi

DOCKER=$(command -v docker);
[ $? -eq 0 ] || onErr "Please install docker first..." 2

$SUDO $DOCKER version  >/dev/null 2>&1
[ $? -ne 0 ] && {
  errMsg="$(printf '\n  You are not authorized to run docker,')"
  errMsg="${errMsg}$(printf '\n  try to "su -" into root account and try again.\n\n')"
  onErr  "${errMsg}" 3
}



cd ./docker

$SUDO $DOCKER build --rm --no-cache=true -t "${DockerImage}" .  \
  --build-arg URL_SQLITE_SOURCE_ZIP="${SQLITE_ZIP_URL}"         \
  --build-arg COMPRESS_SQLITE3="${SQLite_compressor}"
cd ..

printf "\n\n===== Taking ready to use static sqlite3 =======================================\n\n"

arch="${SQLITE_ZIP_URL##*/}"
workdir="${arch%.*}"

[ ! -d release ] && mkdir release

$SUDO $DOCKER run --rm -v $(pwd)/release:/release/  \
  -it "${DockerImage}"                              \
  cp -fv /app/${workdir}/sqlite3 /release/

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

