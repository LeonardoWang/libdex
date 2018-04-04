SRC = capi.cpp dex.cpp inst.cpp insttype.cpp reader.cpp ustring.cpp ustring_encoding.cpp

CXX = g++ -std=c++17 -Wall -Wextra

OBJ = $(SRC:%.cpp=build/%.o)
DEP = $(OBJ:%.o=%.d)

all : libdex.so test

libdex.so : $(OBJ)
	$(CXX) $^ -shared -o $@

test : $(OBJ) libdex.so src/main.cpp
	$(CXX) src/main.cpp -L. -ldex -o $@

.PHONY : run-test
run-test : test
	LD_LIBRARY_PATH=${PWD} ./test

-include $(DEP)

build/%.o : src/%.cpp
	mkdir -p $(@D)
	$(CXX) -MMD -c $< -fPIC -o $@

.PHONY : clean
clean :
	-rm -r build
	-rm test
