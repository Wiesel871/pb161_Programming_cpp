SRC_D = d1_queue.cpp d3_expr.cpp
SRC_E = e1_dynarray.cpp e2_list.cpp e3_iota.cpp
SRC_P = p1_unrolled.cpp p2_bittrie.cpp p3_solid.cpp p4_inttrie.cpp p5_bdd.cpp p6_rope.cpp
SRC_R = r1_circular.cpp r2_zipper.cpp r3_segment.cpp r4_diff.cpp
SRC_T =

CXX      = g++
TIDY     = clang-tidy
ifdef ON_AISA
	TIDY = /packages/run.64/llvm-15.0.7/bin/clang-tidy
endif
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
