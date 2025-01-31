#---------------------------------------------------------------------------------------------------------------------
# TARGET is the name of the output.
# BUILD is the directory where object files & intermediate files will be placed.
# LIBBUTANO is the main directory of butano library (https://github.com/GValiente/butano).
# PYTHON is the path to the python interpreter.
# SOURCES is a list of directories containing source code.
# INCLUDES is a list of directories containing extra header files.
# DATA is a list of directories containing binary data.
# GRAPHICS is a list of files and directories containing files to be processed by grit.
# AUDIO is a list of files and directories containing files to be processed by mmutil.
# DMGAUDIO is a list of files and directories containing files to be processed by mod2gbt and s3m2gbt.
# ROMTITLE is a uppercase ASCII, max 12 characters text string containing the output ROM title.
# ROMCODE is a uppercase ASCII, max 4 characters text string containing the output ROM code.
# USERFLAGS is a list of additional compiler flags:
#     Pass -flto to enable link-time optimization.
#     Pass -O0 or -Og to try to make debugging work.
# USERCXXFLAGS is a list of additional compiler flags for C++ code only.
# USERASFLAGS is a list of additional assembler flags.
# USERLDFLAGS is a list of additional linker flags:
#     Pass -flto=<number_of_cpu_cores> to enable parallel link-time optimization.
# USERLIBDIRS is a list of additional directories containing libraries.
#     Each libraries directory must contains include and lib subdirectories.
# USERLIBS is a list of additional libraries to link with the project.
# DEFAULTLIBS links standard system libraries when it is not empty.
# STACKTRACE enables stack trace logging when it is not empty.
# USERBUILD is a list of additional directories to remove when cleaning the project.
# EXTTOOL is an optional command executed before processing audio, graphics and code files.
#
# All directories are specified relative to the project directory where the makefile is found.
#---------------------------------------------------------------------------------------------------------------------

TARGET      	:=  $(notdir $(CURDIR))
BUILD       	:=  build
LIBBUTANO   	:=  /home/d9k/repos/_gba/butano/butano
PYTHON      	:=  python
SOURCES     	:=  src common/src
INCLUDES    	:=  include common/include include/rapidjson
DATA        	:=
GRAPHICS    	:=  graphics common/graphics
AUDIO       	:=  audio
DMGAUDIO    	:=  dmg_audio
ROMTITLE    	:=  BUTANO SAVEGAME JSON
ROMCODE     	:=  SBTP
USERFLAGS   	:= -DBN_CFG_LOG_MAX_SIZE=1024 -DBN_CFG_SPRITES_MAX_ITEMS=256 -DBN_CFG_SPRITE_TILES_MAX_ITEMS=256
# USERFLAGS   	:= -DBN_CFG_LOG_MAX_SIZE=1024 -DBN_CFG_SPRITES_MAX_ITEMS=512 -DBN_CFG_SPRITE_TILES_MAX_ITEMS=512 -DBN_CFG_SPRITE_TILES_SANITY_CHECK_ENABLED=true -DBN_CFG_SPRITE_TILES_LOG_ENABLED=true -DBN_CFG_LOG_ENABLED=true
USERCXXFLAGS	:=
USERASFLAGS 	:=
USERLDFLAGS 	:=
USERLIBDIRS 	:=
USERLIBS    	:=

# [Frequently asked questions (FAQ) | Butano Docs](https://gvaliente.github.io/butano/faq.html#faq_standard)
# You need to link the standard system libraries to use standard functions like strlen and standard classes like std::shared_ptr. Since you are now linking the standard system libraries, you must comply with their license.
DEFAULTLIBS 	:= true
STACKTRACE		:= true
USERBUILD   	:=
EXTTOOL     	:=
# RAPIDJSON_UNSAFE_IGNORE_STACK_POP_ASSERT := true
# Doesn't work?! Added to main.cpp
# BN_CFG_LOG_MAX_SIZE := 1024


#---------------------------------------------------------------------------------------------------------------------
# Export absolute butano path:
#---------------------------------------------------------------------------------------------------------------------
ifndef LIBBUTANOABS
	export LIBBUTANOABS	:=	$(realpath $(LIBBUTANO))
endif

#---------------------------------------------------------------------------------------------------------------------
# Include main makefile:
#---------------------------------------------------------------------------------------------------------------------
include $(LIBBUTANOABS)/butano.mak

generate-pseudo-random-array-build :
	g++ utils/generate-pseudo-random-array.cpp -o utils/generate-pseudo-random-array

generate-pseudo-random-array : generate-pseudo-random-array-build
	utils/generate-pseudo-random-array

# https://stackoverflow.com/questions/4219255/how-do-you-get-the-list-of-targets-in-a-makefile/26339924#26339924
.PHONY: list
list:
	@LC_ALL=C $(MAKE) -pRrq -f $(firstword $(MAKEFILE_LIST)) : 2>/dev/null | awk -v RS= -F: '/(^|\n)# Files(\n|$$)/,/(^|\n)# Finished Make data base/ {if ($$1 !~ "^[#.]") {print $$1}}' | sort | grep -E -v -e '^[^[:alnum:]]' -e '^$@$$'

start : build
	mgba-qt savegame-json.gba
