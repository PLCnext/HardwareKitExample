#!/bin/bash

TARGET="axcf3152"
VERSION="22.0"
RELEASE_TYPE="Release"

# Get the directory of this script
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" >/dev/null 2>&1 && pwd )"

###
### RFID_DRIVER
###
echo -e "\n##### Generate"
plcncli.exe generate all -p "${DIR}"/RFID_DRIVER/

echo -e "\n##### Set Target"
plcncli.exe set target --add -p "${DIR}"/RFID_DRIVER/ -n $TARGET -v $VERSION

echo -e "\n##### build"
plcncli.exe build -p "${DIR}"/RFID_DRIVER/ -b $RELEASE_TYPE

echo -e "\n##### deploy"
plcncli.exe deploy -p "${DIR}"/RFID_DRIVER/  --verbose

echo -e "\n##### build for external"
plcncli.exe build -p "${DIR}"/RFID_DRIVER/  -b Release --output "${DIR}/RFID_READER/external/"

echo -e "\n##### remove Target"
plcncli.exe set target --remove -p "${DIR}"/RFID_DRIVER/ -n $TARGET -v $VERSION

#plcncli.exe deploy -p "${DIR}"/RFID_DRIVER/ --output "${DIR}"/RFID_READER/external/  --verbose --files "bin/*|. "

###
###RFID_READER
###
echo -e "\n##### Generate"
plcncli.exe generate all -p "${DIR}/RFID_READER/"

echo -e "\n##### Add Target"
plcncli.exe set target --add -p "${DIR}/RFID_READER/" -n $TARGET -v $VERSION

echo -e "\n##### build"
plcncli.exe build -p "${DIR}"/RFID_READER/ --configure -b $RELEASE_TYPE

echo -e "\n##### deploy"
plcncli.exe deploy -p "${DIR}"/RFID_READER/ --verbose

echo -e "\n##### remove Target"
plcncli.exe set target --remove -p "${DIR}/RFID_READER/" -n $TARGET -v $VERSION

###
###
###

#PW=""
#IP="192.168.2.10"
#echo $PW | ssh -tt admin@${IP} -C "mkdir /opt/plcnext/projects/RFID"
#echo $PW | scp -r "${DIR}"/XT_KIT_DEMO/bin/AXCF3152_22.0.5.102/Release/* admin@${IP}:~/projects/RFID/
#echo $PW | scp -r "${DIR}"/XT_KIT_DEMO/external/AXCF3152_22.0.5.102/Release/*/*/lib*.so admin@${IP}:~/projects/RFID/lib/
#echo $PW | scp "${DIR}"/XT_KIT_DEMO/src/RFID.acf.config admin@${IP}:~/projects/Default/
#echo $PW | ssh -tt admin@${IP} -C "sudo /etc/init.d/plcnext restart"