# Written by Maximilian Gerhardt <maximilian.gerhardt@rub.de>
# 29th December 2020
# and Christian Baars, Johann Obermeier
# 2023 / 2024
# License: Apache
# Expanded from functionality provided by PlatformIO's espressif32 and espressif8266 platforms, credited below.
# This script provides functions to download the filesystem (LittleFS) from a running ESP32 / ESP8266
# over the serial bootloader using esptool.py, and mklittlefs for extracting.
# run by either using the VSCode task "Custom" -> "Download Filesystem"
# or by doing 'pio run -t downloadfs' (with optional '-e <environment>') from the commandline.
# output will be saved, by default, in the "unpacked_fs" of the project.
# this folder can be changed by writing 'custom_unpack_dir = some_other_dir' in the corresponding platformio.ini
# environment.
import re
import sys
from os.path import isfile, join
from enum import Enum
import os
import subprocess
import shutil

Import("env")
# Dump construction environment (for debug purpose)
#print(env.Dump())
platform = env.PioPlatform()
board = env.BoardConfig()
mcu = board.get("build.mcu", "esp32")


class FSType(Enum):
    LITTLEFS="littlefs"
    FATFS="fatfs"
    SPIFFS="spiffs"

class FSInfo:
    def __init__(self, fs_type, start, length, page_size, block_size):
        self.fs_type = fs_type
        self.start = start
        self.length = length
        self.page_size = page_size
        self.block_size = block_size
    def __repr__(self):
        return f"FS type {self.fs_type} Start {hex(self.start)} Len {self.length} Page size {self.page_size} Block size {self.block_size}"
    # extract command supposed to be implemented by subclasses
    def get_extract_cmd(self, input_file, output_dir):
        raise NotImplementedError()

class FS_Info(FSInfo):
    def __init__(self, start, length, page_size, block_size):
        print("env[\"MKFSTOOL\"]: " + env["MKFSTOOL"])
        mkfsToolPathPattern = env["MKFSTOOL"]
        print("env.subst(\"$MKFSTOOL\"): " + env.subst("$MKFSTOOL"))
        #print("env[\"MKSPIFFSTOOL\"]: " + env["MKSPIFFSTOOL","none"])
        print("env[\"PIOPLATFORM\"]: " + env["PIOPLATFORM"]) 
        pioPlatform = env["PIOPLATFORM"]
        print("env[\"PIOFRAMEWORK\"][0]: " + env["PIOFRAMEWORK"][0]) 
        pioFramework = env["PIOFRAMEWORK"][0]
        self.tool = env.subst("$MKFSTOOL")
        #self.tool = mkfsToolPathPattern
        #self.tool = self.tool.replace("${PIOPLATFORM}", pioPlatform)
        #self.tool = self.tool.replace("${PIOFRAMEWORK}", pioFramework)
        
        if mkfsToolPathPattern.startswith("mkspiffs"):
            self.tool = join(platform.get_package_dir("tool-mkspiffs"), self.tool)
        else:
            self.tool = join(platform.get_package_dir("tool-mklittlefs"), self.tool)
        print('self.tool: ' + self.tool)
        super().__init__(FSType.LITTLEFS, start, length, page_size, block_size)
    def __repr__(self):
        return f"{self.fs_type} Start {hex(self.start)} Len {hex(self.length)} Page size {hex(self.page_size)} Block size {hex(self.block_size)}"
    def get_extract_cmd(self, input_file, output_dir):
        return f'"{self.tool}" -b {self.block_size} -s {self.length} -p {self.page_size} --unpack "{output_dir}" "{input_file}"'

# SPIFFS helpers copied from ESP32, https://github.com/platformio/platform-espressif32/blob/develop/builder/main.py
# Copyright 2014-present PlatformIO <contact@platformio.org>
# Licensed under the Apache License, Version 2.0 (the "License");

def _parse_size(value):
    if isinstance(value, int):
        return value
    elif value.isdigit():
        return int(value)
    elif value.startswith("0x"):
        return int(value, 16)
    elif value[-1].upper() in ("K", "M"):
        base = 1024 if value[-1].upper() == "K" else 1024 * 1024
        return int(value[:-1]) * base
    return value

## FS helpers for ESP8266
# copied from https://github.com/platformio/platform-espressif8266/blob/develop/builder/main.py
# Copyright 2014-present PlatformIO <contact@platformio.org>
# Licensed under the Apache License, Version 2.0 (the "License");

def _parse_ld_sizes(ldscript_path):
    assert ldscript_path
    result = {}
    # get flash size from LD script path
    match = re.search(r"\.flash\.(\d+[mk]).*\.ld", ldscript_path)
    if match:
        result['flash_size'] = _parse_size(match.group(1))

    appsize_re = re.compile(
        r"irom0_0_seg\s*:.+len\s*=\s*(0x[\da-f]+)", flags=re.I)
    filesystem_re = re.compile(
        r"PROVIDE\s*\(\s*_%s_(\w+)\s*=\s*(0x[\da-f]+)\s*\)" % "FS"
        if "arduino" in env.subst("$PIOFRAMEWORK")
        else "SPIFFS",
        flags=re.I,
    )
    with open(ldscript_path) as fp:
        for line in fp.readlines():
            line = line.strip()
            if not line or line.startswith("/*"):
                continue
            match = appsize_re.search(line)
            if match:
                result['app_size'] = _parse_size(match.group(1))
                continue
            match = filesystem_re.search(line)
            if match:
                result['fs_%s' % match.group(1)] = _parse_size(
                    match.group(2))
    return result

def esp8266_fetch_fs_size(env):
    ldsizes = _parse_ld_sizes(env.GetActualLDScript())
    for key in ldsizes:
        if key.startswith("fs_"):
            env[key.upper()] = ldsizes[key]

    assert all([
        k in env
        for k in ["FS_START", "FS_END", "FS_PAGE", "FS_BLOCK"]
    ])

    # esptool flash starts from 0
    for k in ("FS_START", "FS_END"):
        _value = 0
        if env[k] < 0x40300000:
            _value = env[k] & 0xFFFFF
        elif env[k] < 0x411FB000:
            _value = env[k] & 0xFFFFFF
            _value -= 0x200000  # correction
        else:
            _value = env[k] & 0xFFFFFF
            _value += 0xE00000  # correction

        env[k] = _value

APP_TYPE = 0x00
DATA_TYPE = 0x01

TYPES = {
    'app': APP_TYPE,
    'data': DATA_TYPE,
}

def get_ptype_name(ptype):
    for name, value in TYPES.items():
        if value == ptype:
            return name
    raise KeyError("partition type value not found: " + hex(ptype))

def get_ptype_as_int(ptype):
    """ Convert a string which might be numeric or the name of a partition type to an integer """
    try:
        return TYPES[ptype]
    except KeyError:
        try:
            return int(ptype, 0)
        except TypeError:
            return ptype


# Keep this map in sync with esp_partition_subtype_t enum in esp_partition.h
MIN_PARTITION_SUBTYPE_APP_OTA = 0x10
NUM_PARTITION_SUBTYPE_APP_OTA = 16

SUBTYPE_OTA         = 0x00
SUBTYPE_NVS         = 0x02
SUBTYPE_FAT         = 0x81
SUBTYPE_SPIFFS      = 0x82
SUBTYPE_LITTLEFS    = 0x83

SUBTYPES = {
    APP_TYPE: {
        'factory': 0x00,
        'test': 0x20,
    },
    DATA_TYPE: {
        'ota': SUBTYPE_OTA,
        'phy': 0x01,
        'nvs': SUBTYPE_NVS,
        'coredump': 0x03,
        'nvs_keys': 0x04,
        'efuse': 0x05,
        'undefined': 0x06,
        'esphttpd': 0x80,
        'fat': SUBTYPE_FAT,
        'spiffs': SUBTYPE_SPIFFS,
        'littlefs': SUBTYPE_LITTLEFS,
    },
}

# add subtypes for the 16 OTA slot values ("ota_XX, etc.")
for ota_slot in range(NUM_PARTITION_SUBTYPE_APP_OTA):
    #print("adding partition type: ", "ota_%d" % ota_slot, "value: ", MIN_PARTITION_SUBTYPE_APP_OTA + ota_slot)
    SUBTYPES[TYPES["app"]]["ota_%d" % ota_slot] = MIN_PARTITION_SUBTYPE_APP_OTA + ota_slot

def get_subtype_as_int(ptype, subtype):
    """ Convert a string which might be numeric or the name of a partition subtype to an integer """
    try:
        return SUBTYPES[get_ptype_as_int(ptype)][subtype]
    except KeyError:
        try:
            return int(subtype, 0)
        except TypeError:
            return subtype

def get_ptype_names(ptype,subtype):
    typeName = get_ptype_name(ptype)
    for subtypeName, value in SUBTYPES[ptype].items():
        if value == subtype:
            return typeName, subtypeName
    raise KeyError("subpartition type value not found for type \"" + typeName + "\": " + hex(subtype))

def get_pstype_name(ptype,subtype):
    typeName = get_ptype_name(ptype)
    for subtypeName, value in SUBTYPES[ptype].items():
        if value == subtype:
            return subtypeName
    raise KeyError("subpartition type value not found for type \"" + typeName + "\": " + hex(subtype))

## Script interface functions
def parse_partition_table(content, subtypes=[SUBTYPE_SPIFFS,SUBTYPE_LITTLEFS]):
    entries = [e for e in content.split(b'\xaaP') if len(e) > 0]
    print("Partition data:")
    print("type:\t","\tsubtype:\t","\taddress:\t","\tsize:\t")
    for entry in entries:
        type = entry[0]
        subtype = entry[1]
        typeName,subtypeName = get_ptype_names(type,subtype)
        offset = int.from_bytes(entry[2:5], byteorder='little', signed=False)
        size = int.from_bytes(entry[6:9], byteorder='little', signed=False)
        #print("type:\t",typeName," (",hex(type),")\tsubtype:\t",subtypeName," (",hex(subtype),")\taddress:\t",hex(offset), "\tsize:\t",hex(size) )
        print(typeName," (",hex(type),")\t",subtypeName," (",hex(subtype),")\t",hex(offset), "\t",hex(size) )
        # print("type:",hex(type))
        # print("address:",hex(p_offset))
        # print("size:",hex(p_size))
        if subtype in subtypes: # SPIFFS or LITTLEFS
            offset = int.from_bytes(entry[2:5], byteorder='little', signed=False)
            size = int.from_bytes(entry[6:9], byteorder='little', signed=False)
            #print("type:",hex(type))
            #print("address:",hex(offset))
            #print("size:",hex(size))
            env["FS_START"] = offset
            env["FS_SIZE"] = size
            env["FS_PAGE"] = int("0x100", 16)
            env["FS_BLOCK"] = int("0x1000", 16)

def get_partition_table(subtypes=[SUBTYPE_SPIFFS,SUBTYPE_LITTLEFS]):
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    download_speed = join(str(board.get("download.speed", "115200")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    fs_file = join(env["PROJECT_DIR"], "partition_table_from_flash.bin")
    esptoolpy_flags = [
            "--chip", mcu,
            "--port", upload_port,
            "--baud",  download_speed,
            "--before", "default_reset",
            "--after", "hard_reset",
            "read_flash",
            "0x8000",
            "0x1000",
            fs_file
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    try:
        returncode = subprocess.call(esptoolpy_cmd, shell=False)
    except subprocess.CalledProcessError as exc:
        print("Downloading failed with " + str(exc))
    with open(fs_file, mode="rb") as file:
        content = file.read()
        parse_partition_table(content,subtypes)

def get_fs_type_start_and_length(subtypes=[SUBTYPE_SPIFFS,SUBTYPE_LITTLEFS]):
    platform = env["PIOPLATFORM"]
    print("env[\"PIOPLATFORM\"]: " + platform)
    if platform == "espressif32":
        print(f"Retrieving filesystem info for {mcu}.")
        get_partition_table(subtypes)
        return FS_Info(env["FS_START"], env["FS_SIZE"], env["FS_PAGE"], env["FS_BLOCK"])
    elif platform == "espressif8266":
        print("Retrieving filesystem info for ESP8266.")
        filesystem = board.get("build.filesystem", "littlefs")
        if filesystem not in ("littlefs"):
            print("Unrecognized board_build.filesystem option '" + str(filesystem) + "'.")
            env.Exit(1)
        # fetching sizes is the same for all filesystems
        esp8266_fetch_fs_size(env)
        #print("FS_START: " + hex(env["FS_START"]))
        #print("FS_SIZE: " + hex(env["FS_END"] - env["FS_START"]))
        #print("FS_PAGE: " + hex(env["FS_PAGE"]))
        #print("FS_BLOCK: " + hex(env["FS_BLOCK"]))
        if filesystem == "littlefs":
            print("Recognized LittleFS filesystem.")
            return FS_Info(env["FS_START"], env["FS_END"] - env["FS_START"], env["FS_PAGE"], env["FS_BLOCK"])
        else:
            print("Unrecongized configuration.")
    pass

def download_fs(fs_info: FSInfo):
    print(fs_info)
    esptoolpy = join(platform.get_package_dir("tool-esptoolpy") or "", "esptool.py")
    upload_port = join(env.get("UPLOAD_PORT", "none"))
    download_speed = join(str(board.get("download.speed", "115200")))
    if "none" in upload_port:
        env.AutodetectUploadPort()
        upload_port = join(env.get("UPLOAD_PORT", "none"))
    fs_file = join(env.subst("$BUILD_DIR"), f"downloaded_fs_{hex(fs_info.start)}_{hex(fs_info.length)}.bin")
    esptoolpy_flags = [
            "--chip", mcu,
            "--port", upload_port,
            "--baud",  download_speed,
            "--before", "default_reset",
            "--after", "hard_reset",
            "read_flash",
            hex(fs_info.start),
            hex(fs_info.length),
            fs_file
    ]
    esptoolpy_cmd = [env["PYTHONEXE"], esptoolpy] + esptoolpy_flags
    print("Download filesystem image")
    print("command: ", esptoolpy_cmd)
    try:
        returncode = subprocess.call(esptoolpy_cmd, shell=False)
        return (True, fs_file)
    except subprocess.CalledProcessError as exc:
        print("Downloading failed with " + str(exc))
        return (False, "")

def unpack_fs(fs_info: FSInfo, downloaded_file: str):
    # by writing custom_unpack_dir = some_dir in the platformio.ini, one can
    # control the unpack directory
    unpack_dir = env.GetProjectOption("custom_unpack_dir", "unpacked_fs")
    if not os.path.exists(downloaded_file):
        print(f"ERROR: {downloaded_file} with filesystem not found, maybe download failed due to download_speed setting being too high.")
        assert(0)
    try:
        if os.path.exists(unpack_dir):
            shutil.rmtree(unpack_dir)
    except Exception as exc:
        print("Exception while attempting to remove the folder '" + str(unpack_dir) + "': " + str(exc))
    if not os.path.exists(unpack_dir):
        os.makedirs(unpack_dir)

    cmd = fs_info.get_extract_cmd(downloaded_file, unpack_dir)
    print("Unpack files from filesystem image")
    print("downloaded_file: " + downloaded_file)
    print("unpack_dir: " + unpack_dir)
    print("cmd: " + cmd)
    try:
        returncode = subprocess.call(cmd, shell=True)
        return (True, unpack_dir)
    except subprocess.CalledProcessError as exc:
        print("Unpacking filesystem failed with " + str(exc))
        return (False, "")

def display_fs(extracted_dir):
    # extract command already nicely lists all extracted files.
    # no need to display that ourselves. just display a summary
    file_count = sum([len(files) for r, d, files in os.walk(extracted_dir)])
    print("Extracted " + str(file_count) + " file(s) from filesystem.")

def command_download_fs(*args, **kwargs):
    info = get_fs_type_start_and_length()
    download_ok, downloaded_file = download_fs(info)
    unpack_ok, unpacked_dir = unpack_fs(info, downloaded_file)
    if unpack_ok is True:
        display_fs(unpacked_dir)

env.AddCustomTarget(
    name="downloadfs",
    dependencies=None,
    actions=[
        command_download_fs
    ],
    title="Download Filesystem",
    description="Downloads and displays files stored in the target ESP32/ESP8266"
)

def unpack_nvs(downloaded_file):
    # by writing custom_nvs_unpack_dir = some_dir in the platformio.ini, one can
    # control the unpack directory
    unpack_dir = env.GetProjectOption("custom_nvs_unpack_dir", "unpacked_nvs")
    # file name (prefix) for upacked nvs content
    unpack_filename = env.GetProjectOption("custom_nvs_unpack_file", "nvs")
    
    if not os.path.exists(downloaded_file):
        print(f"ERROR: {downloaded_file} with filesystem not found, maybe download failed due to download_speed setting being too high.")
        assert(0)
    try:
        if os.path.exists(unpack_dir):
            shutil.rmtree(unpack_dir)
    except Exception as exc:
        print("Exception while attempting to remove the folder '" + str(unpack_dir) + "': " + str(exc))
    if not os.path.exists(unpack_dir):
        os.makedirs(unpack_dir)

    #PYTHONEXE
    pyth_exe = env.get("PYTHONEXE","python")
    #PROJECT_PACKAGES_DIR': '/home/adam/.platformio/packages'
    #env.get("PROJECT_PACKAGES_DIR")
    pacdir = env.get("PROJECT_PACKAGES_DIR","/home/adam/.platformio/packages")
    #/framework-espidf
    idfdir = pacdir + "/framework-espidf"
    #"python ${IDF_PATH}/components/nvs_flash/nvs_partition_tool/nvs_tool.py"
    nvs_tool = idfdir + "/components/nvs_flash/nvs_partition_tool/nvs_tool.py"
    #return f'"{self.tool}" -b {self.block_size} -s {self.length} -p {self.page_size} --unpack "{output_dir}" "{input_file}"'
    #cmd = fs_info.get_extract_cmd(downloaded_file, unpack_dir)
    print("Unpack NVS partition from filesystem image")
    print("downloaded_file: " + downloaded_file)
    print("unpack_dir: " + unpack_dir)
    for dump_format in ["text","json"]:
        if( dump_format == "text" ):
            unpack_file = join(unpack_dir,unpack_filename+".txt")
        else:
            unpack_file = join(unpack_dir,unpack_filename+"."+dump_format)
        cmd = f'"{pyth_exe}" "{nvs_tool}" --dump minimal --format {dump_format} "{downloaded_file}"'
        print("cmd: " + cmd, " > ", unpack_file)
        try:
            with open(unpack_file, 'w') as f:
                returncode = subprocess.call(cmd, shell=True, stdout=f)
        except subprocess.CalledProcessError as exc:
            print("Unpacking NVS partition failed with " + str(exc))
            return (False, "")
    return (True, unpack_dir)



def command_download_nvs(*args, **kwargs):
    info = get_fs_type_start_and_length([SUBTYPE_NVS])
    download_ok, downloaded_file = download_fs(info)
    unpack_ok, unpacked_dir = unpack_nvs(downloaded_file)
    if unpack_ok is True:
        display_fs(unpacked_dir)


env.AddCustomTarget(
    name="downloadnvs",
    dependencies=None,
    actions=[
        command_download_nvs
    ],
    title="Download NVS partition",
    description="Downloads and parses NVS partition stored on ESP32."
)

def command_dump_environment(*args, **kwargs):
    env_dump_file = env.GetProjectOption("custom_env_dump_file", "env_dump.txt")
    print("env_dump_file: ", env_dump_file)
    #print("os.path.dirname(env_dump_file): ", os.path.dirname(env_dump_file))
    #print("os.path.abspath(env_dump_file): ", os.path.abspath(env_dump_file))
    #print("os.path.dirname(os.path.abspath(env_dump_file)): ", os.path.dirname(os.path.abspath(env_dump_file)))
    unpack_dir=os.path.dirname(env_dump_file)
    #unpack_dir=os.path.dirname(os.path.abspath(env_dump_file))
    print("unpack_dir: ", unpack_dir)
    unpack_dir_empty = len(str(unpack_dir).strip())==0
    print("unpack_dir is empty: ", unpack_dir_empty)
    if not unpack_dir_empty:
        print("os.path.exists(unpack_dir): ",os.path.exists(unpack_dir))
        if not os.path.exists(unpack_dir):
            print("creating directory: ", unpack_dir)
            os.makedirs(unpack_dir,exist_ok=True)
    with open(env_dump_file, 'w') as f:
        print("writing env.Dump() to ", env_dump_file)
        print(env.Dump(), file=f)  # Python 3.x

env.AddCustomTarget(
    name="dumpEnv",
    dependencies=None,
    actions=[
        command_dump_environment
    ],
    title="dump environment",
    description="dumps/prints environment to file given as option 'env_dump_file', default: env_dump_file.txt"
)

