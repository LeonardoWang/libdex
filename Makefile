SRC = capi.cpp			\
      analyze.cpp 		\
      dex.cpp 			\
      inst.cpp 			\
      insttype.cpp		\
      reader.cpp 		\
      ustring.cpp 		\
      ustring_encoding.cpp	\
      repackage.cpp

CXX = g++ -std=c++17 -Wall -Wextra -fmax-errors=1 -O2

OBJ = $(SRC:%.cpp=build/%.o)
DEP = $(OBJ:%.o=%.d)

all : libdex.so test

libdex.so : $(OBJ)
	$(CXX) $^ -shared -o $@

test : $(OBJ) main.cpp
	$(CXX) $^ -o test

doc : .FORCE
	doxygen

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

.PHONY : .FORCE
.FORCE:
