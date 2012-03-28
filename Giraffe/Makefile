###############################################################################
###############################################################################

CROSS_COMPILE =
OPTIMIZE := -O0
WARNINGS := -Wall -Wno-unused -Wno-format
DEFS     := -DMYDEF=1 -UMYDEF2
EXTRA_CFLAGS := 

INC_DIR   =  include include/network_header  
#/opt/boost/include
BASE_SRC_DIR   = src src/Base src/Base/ToolKit src/Packets src/Packets/Giraffe src/Packets/Giraffe/paxos
GIRAFFE_SRC_DIR 	= src/Giraffe src/Giraffe/logic
GIRAFFE_LIB_SRC_DIR 	= src/GiraffeLib src/GiraffeLib/logic src/GiraffeLib/config
OBJ_DIR   = obj/base 
GIRAFFE_OBJ_DIR   = obj/giraffe 
GIRAFFE_LIB_OBJ_DIR   = obj/giraffelib 
EXTRA_SRC = 
EXCLUDE_FILES = src/Giraffe/BerkelyFileSystemTest.cpp src/Giraffe/logic/GPaxosPktParser.cpp 
#src/common/Checksum.cpp src/common/Header.cpp src/common/IAppTerminate.cpp src/common/IPacketParser.cpp src/common/ISerializble.cpp src/common/MD5.cpp src/common/PPlatTransThreadFunc.cpp src/common/PTaskQueue.cpp src/common/PTime.cpp 
#LIB_DIR = lib /opt/boost/lib
SUFFIX       = c cpp cc cxx
Base       := ./base.so
NETWORK 		:= bin/libnetlet.so
GIRAFFE 	:=./giraffe
GIRAFFELIB 	:=./giraffeclient
#TARGET_TYPE  := ar
#TARGET_TYPE  := app
#TARGET_TYPE  := so

LIB = /opt/log4cplus/lib/liblog4cplus.a 

#####################################################################################
#  Do not change any part of them unless you have understood this script very well  #
#  This is a kind remind.                                                           #
#####################################################################################

#FUNC#  Add a new line to the input stream.
define add_newline
$1

endef

#FUNC# set the variable `src-x' according to the input $1
define set_giraffe_lib_src_x
giraffe-lib-src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_giraffe_lib_obj_x
giraffe-lib-obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef
#FUNC# set the variable `src-x' according to the input $1
define set_giraffe_src_x
giraffe-src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_giraffe_obj_x
giraffe-obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef
#FUNC# set the variable `src-x' according to the input $1
define set_base_src_x
base-src-$1 = $(filter-out $4,$(foreach d,$2,$(wildcard $d/*.$1)) $(filter %.$1,$3))

endef

#FUNC# set the variable `obj-x' according to the input $1
define set_base_obj_x
base-obj-$1 = $(patsubst %.$1,$3%.o,$(notdir $2))

endef
$(warning $(OBJ_DIR) $(prefix_objdir ))
#VAR# Get the uniform representation of the object directory path name
ifneq ($(OBJ_DIR),)
prefix_objdir  = $(shell echo $(OBJ_DIR)|sed 's:\(\./*\)*::')
prefix_objdir := $(filter-out /,$(prefix_objdir)/)
#$(error $(prefix_objdir))
endif

ifneq ($(GIRAFFE_OBJ_DIR),)
giraffe_prefix_objdir  = $(shell echo $(GIRAFFE_OBJ_DIR)|sed 's:\(\./*\)*::')
giraffe_prefix_objdir := $(filter-out /,$(giraffe_prefix_objdir)/)
endif

ifneq ($(GIRAFFE_LIB_OBJ_DIR),)
giraffe_lib_objdir  = $(shell echo $(GIRAFFE_LIB_OBJ_DIR)|sed 's:\(\./*\)*::')
giraffe_lib_objdir := $(filter-out /,$(giraffe_lib_objdir)/)
endif
GCC      := $(CROSS_COMPILE)gcc
G++      := $(CROSS_COMPILE)g++
BASE_SRC_DIR := $(sort . $(BASE_SRC_DIR))
inc_dir = $(foreach d,$(sort $(INC_DIR) $(BASE_SRC_DIR)),-I$d)
giraffe_inc_dir := $(inc_dir)
giraffe_lib_inc_dir := $(inc_dir)

giraffe_inc_dir +=  $(foreach d,$(GIRAFFE_SRC_DIR),-I$d)
giraffe_lib_inc_dir +=  $(foreach d,$(GIRAFFE_LIB_SRC_DIR),-I$d)
#$(error $(giraffe_inc_dir))

#--# Do smart deduction automatically
$(eval $(foreach i,$(SUFFIX),$(call set_giraffe_lib_src_x,$i,$(GIRAFFE_LIB_SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_giraffe_lib_obj_x,$i,$(giraffe-lib-src-$i),$(giraffe_lib_objdir))))
$(eval $(foreach i,$(SUFFIX),$(call set_giraffe_src_x,$i,$(GIRAFFE_SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_giraffe_obj_x,$i,$(giraffe-src-$i),$(giraffe_prefix_objdir))))
$(eval $(foreach i,$(SUFFIX),$(call set_base_src_x,$i,$(BASE_SRC_DIR),$(EXTRA_SRC),$(EXCLUDE_FILES))))
$(eval $(foreach i,$(SUFFIX),$(call set_base_obj_x,$i,$(base-src-$i),$(prefix_objdir))))
$(eval $(foreach f,$(EXTRA_SRC),$(call add_newline,vpath $(notdir $f) $(dir $f))))
$(eval $(foreach d,$(BASE_SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))

$(eval $(foreach d,$(GIRAFFE_LIB_SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))
$(eval $(foreach d,$(GIRAFFE_SRC_DIR),$(foreach i,$(SUFFIX),$(call add_newline,vpath %.$i $d))))
base_all_objs = $(foreach i,$(SUFFIX),$(base-obj-$i))
base_all_srcs = $(foreach i,$(SUFFIX),$(base-src-$i))

giraffe_lib_objs = $(foreach i,$(SUFFIX),$(giraffe-lib-obj-$i))
giraffe_lib_srcs = $(foreach i,$(SUFFIX),$(giraffe-lib-src-$i))
giraffe_objs = $(foreach i,$(SUFFIX),$(giraffe-obj-$i))
giraffe_srcs = $(foreach i,$(SUFFIX),$(giraffe-src-$i))
CFLAGS       = $(EXTRA_CFLAGS) $(WARNINGS) $(OPTIMIZE) $(DEFS)
#TARGET_TYPE := $(strip $(TARGET_TYPE))

#ifeq ($(filter $(TARGET_TYPE),so ar app),)
#$(error Unexpected TARGET_TYPE `$(TARGET_TYPE)')
#endif

#ifeq ($(TARGET_TYPE),so)
 CFLAGS  += -fpic -g 
 LDFLAGS += -shared
#endif

PHONY = all .mkdir clean
all:
	@echo "please choose target!"

client: .mkdir $(GIRAFFELIB)

$(GIRAFFELIB):	base $(giraffe_lib_objs)
	@echo "giraffe client building!!!"
	$(G++) $(CFLAGS) $(giraffe_lib_objs) $(Base) ./libnetlet.so -o $@ ./lib/liblog4cplus.a ./lib/libboost_thread.a -lpthread 
	mv $@ bin/linux
 
giraffe: .mkdir $(GIRAFFE)

$(GIRAFFE):	base $(giraffe_objs)
	@echo "giraffe building!!!"
	$(G++) $(CFLAGS) $(giraffe_objs) $(Base) ./libnetlet.so -o $@ ./lib/liblog4cplus.a ./lib/libboost_thread.a ./lib/libdb_cxx.a -lpthread
	mv $@ bin/linux
 

base: .mkdir $(Base) 

define cmd_giraffe_lib_o
$(giraffe-lib-obj-$1): $2%.o: %.$1  $(MAKEFILE_LIST)
	$(GCC) $(giraffe_lib_inc_dir) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<

endef
$(eval $(foreach i,$(SUFFIX),$(call cmd_giraffe_lib_o,$i,$(giraffe_lib_objdir))))
define cmd_giraffe_o
$(giraffe-obj-$1): $2%.o: %.$1  $(MAKEFILE_LIST)
	$(GCC) $(giraffe_inc_dir) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<

endef
$(eval $(foreach i,$(SUFFIX),$(call cmd_giraffe_o,$i,$(giraffe_prefix_objdir))))

#ifeq ($(TARGET_TYPE),ar)

define cmd_o
$(base-obj-$1): $2%.o: %.$1  $(MAKEFILE_LIST)
	 $(GCC) $(inc_dir) -Wp,-MT,$$@ -Wp,-MMD,$$@.d $(CFLAGS) -c -o $$@ $$<

endef
#	rm -f $(prefix_objdir)*.o $(Base)
$(eval $(foreach i,$(SUFFIX),$(call cmd_o,$i,$(prefix_objdir))))

#ifeq ($(TARGET_TYPE),ar)
#$(TARGET): AR := $(CROSS_COMPILE)ar
#$(TARGET): $(all_objs)
#	rm -f $@
#	$(AR) rcvs $@ $(all_objs)
#else
$(Base): LD = $(if $(strip $(src-cpp) $(src-cc) $(src-cxx)),$(G++),$(GCC))
$(Base): $(base_all_objs)
	$(LD) $(LDFLAGS) -shared $(base_all_objs) ./libnetlet.so -o $@  -lpthread 
	cp $@ bin/linux/
	
#endif

.mkdir:
	@if [ ! -d $(OBJ_DIR) ]; then mkdir -p $(OBJ_DIR); fi
	@if [ ! -d $(GIRAFFE_OBJ_DIR) ]; then mkdir -p $(GIRAFFE_OBJ_DIR); fi
	@if [ ! -d $(GIRAFFE_LIB_OBJ_DIR) ]; then mkdir -p $(GIRAFFE_LIB_OBJ_DIR); fi

clean:
	rm -f $(prefix_objdir)*.o $(prefix_objdir)*.o.d $(Base)
	rm -f $(giraffe_lib_objdir)*.o $(giraffe_lib_objdir)*.o.d $(GIRAFFELIB)
	rm -f $(giraffe_prefix_objdir)*.o $(giraffe_prefix_objdir)*.o.d $(GIRAFFE)

-include $(patsubst %.o,%.o.d,$(base_all_objs))
-include $(patsubst %.o,%.o.d,$(giraffe_objs))
-include $(patsubst %.o,%.o.d,$(giraffe_lib_objs))

.PHONY: $(PHONY)

