SRC_D = d1_files.cpp d2_streams.cpp d3_format.cpp
SRC_E = e1_force.cpp
SRC_P = p1_set.cpp p2_fixnum.cpp p3_tmpfile.cpp p4_parse.cpp p5_grep.cpp p6_csv.cpp
SRC_R = r5_json.cpp r6_cpp.cpp
SRC_T =

CXX      = g++
TIDY     = clang-tidy.exe
VALGRIND = valgrind
CXXFLAGS = -std=c++20 -Wall -Wextra -Werror
TDYFLAGS = -std=c++20

-include ../local.mk

BIN_T    = ${SRC_T:.cpp=}
BIN_D    = ${SRC_D:.cpp=}
BIN_E    = ${SRC_E:.cpp=}
BIN_P    = ${SRC_P:.cpp=}
BIN_R    = ${SRC_R:.cpp=}

BIN      = $(BIN_T) $(BIN_D) $(BIN_E) $(BIN_P) $(BIN_R)
SRC      = $(SRC_T) $(SRC_D) $(SRC_E) $(SRC_P) $(SRC_R)
DEP      = ${BIN_D:%=.%.d} ${BIN_E:%=.%.d} ${BIN_T:%=.%.d} ${BIN_E:%=.sol.%.d}

_ENV   = CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" TDYFLAGS="$(TDYFLAGS)" TIDY="$(TIDY)" VALGRIND="$(VALGRIND)"

all: $(BIN)

clean:
	@rm -f $(BIN) $(DEP)
	@rm -f *.core core *~ a.out .check.*.out valgrind.out a.out

.cpp: makefile .helper.sh
	@env $(_ENV) sh .helper.sh cxx    $< -o $@
	@env $(_ENV) sh .helper.sh tidy   $<
	@env $(_ENV) sh .helper.sh grind ./$@

$(BIN_T): makefile .helper.sh
	@echo building and testing $@
	@env $(_ENV) sh .helper.sh cxx    $@.cpp -o $@
	@env $(_ENV) sh .helper.sh tidy   $@.cpp
	@env $(_ENV) sh .helper.sh grind ./$@

-include $(DEP)

.PHONY: all clean
