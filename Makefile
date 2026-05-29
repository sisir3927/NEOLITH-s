# --- Compiler and Flags Definition ---
CXX          := g++
CXXFLAGS     := -fPIC -Iinclude -I/home/daq/exp/anaroot_v4.6.1-install/include -pthread -std=c++17 -m64 -I/usr/local/root/root_v6.32.08-install/include
ROOTLIBS     := $(shell root-config --libs)

# --- Target Library Name ---
LIB          := libmyLib.so

# --- Automatic File Discovery ---
SOURCES      := $(wildcard src/*.cc)
OBJECTS      := $(SOURCES:.cc=.o)
HEADERS      := $(wildcard include/*.hh)
LINKDEF      := include/LinkDef.h

# Dictionary output locations
DICT_SRC     := src/myDict.cxx
DICT_OBJ     := src/myDict.o

# --- Build Rules ---

all: $(LIB)

# Rule 1: Link objects and COPY the PCM file to the current directory
$(LIB): $(OBJECTS) $(DICT_OBJ)
	$(CXX) -shared -m64 -o $@ $(OBJECTS) $(DICT_OBJ) $(ROOTLIBS)
	cp src/myDict_rdict.pcm .

# Rule 2: Compile source files
src/%.o: src/%.cc
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule 3: Generate dictionary
$(DICT_SRC): $(HEADERS) $(LINKDEF)
	rootcling -f $@ -Iinclude -I/home/daq/exp/anaroot_v4.6.1-install/include $(notdir $(HEADERS)) $(LINKDEF)

# Rule 4: Compile dictionary
$(DICT_OBJ): $(DICT_SRC)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# --- Utility Rules ---
clean:
	rm -f src/*.o $(DICT_SRC) src/myDict_rdict.pcm ./myDict_rdict.pcm $(LIB)

.PHONY: all clean
