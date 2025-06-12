#!/usr/bin/env bash

export IDF_PATH=/home/adam/.platformio/packages/framework-espidf


PARTTOOL_PY="python ${IDF_PATH}/components/partition_table/parttool.py"
NVSTOOL_PY="python ${IDF_PATH}/components/nvs_flash/nvs_partition_tool/nvs_tool.py"

#python /home/adam/.platformio/packages/framework-espidf/components/partition_table/parttool.py read_partition --partition-name=nvs --output download_nvs_partition.bin
#$PARTTOOL_PY get_partition_info --partition-name=nvs --info offset

#echo "${@:1}" 

if [ $# -lt 1 ]
then
$NVSTOOL_PY --help
else
$NVSTOOL_PY "${@:1}"
fi

#${PARTTOOL_PY} read_partition --partition-name=nvs --output download_nvs_partition.bin
