CXX         = g++
STATIC      = libhcidecoder.a
LIB_NAME    = hcidecoder
EXEC        = hci-decoder-test
MAIN_PATH   = test/main.cpp
CFLAGS      = -O3 -g -Werror -std=c++11
LDFLAGS     = -lbtsnoop
SRC_DIR     = src
OBJECT_DIR  = bin
OBJECTS     = jsoncpp.o hcidecoder.o command_complete.o
VPATH       = src
HEADERS     = -Isrc -Iexternal_libs/include
VERSION     = 0.1
RELEASE_DIR = build-hcidecoder-${VERSION}
INCLUDE_DIR = include
OBJS        = $(patsubst %,$(OBJECT_DIR)/%,$(OBJECTS))

default: init ${STATIC}
	$(CXX) -Iexternal_libs/include -I${RELEASE_DIR}/include -L${RELEASE_DIR} -Lexternal_libs ${MAIN_PATH} $(CFLAGS) -o ${RELEASE_DIR}/${EXEC} -l${LIB_NAME} $(LDFLAGS)

init:
	mkdir -p ${OBJECT_DIR}

$(OBJECT_DIR)/%.o: ${SRC_DIR}/%.cpp
	$(CXX) $(HEADERS) -c  $< $(CFLAGS) -o $@ $(LDFLAGS)

${STATIC}: $(OBJS)
	mkdir -p $(RELEASE_DIR)
	mkdir -p ${RELEASE_DIR}/${INCLUDE_DIR}
	cd src; rsync -avm --include=*/ --include=*.h --exclude=* . ../$(RELEASE_DIR)/include
	cd external_libs/include; rsync -avm --include=*/ --include=*.h --exclude=* . ../../$(RELEASE_DIR)/include
	ar rcs ${RELEASE_DIR}/$@ $(OBJS)

clean:
	rm -fr ${OBJECT_DIR}/*.o

mrproper:
	rm -fr ${OBJECT_DIR}/*.o ${RELEASE_DIR}
