.RECIPEPREFIX := $() $()
# Personal Note:
# Make wants tabs to indicate command, naw.
# RECIPEPREFIX so dumb, just remember to use only 1 space before (' ') before
# command ( '$() $()' )

CXX = g++
RM = rm -f

CXXFLAGS = -Wall -std=c++20 -fopenmp

MAGICK_CXXFLAGS = $(shell Magick++-config --cppflags --cxxflags)

COMPILE_FLAGS = $(CXXFLAGS) $(MAGICK_CXXFLAGS)

MAGICK_LDLIBS = $(shell Magick++-config --libs)

LDFLAGS =

SRC_DIR = src
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIR)))
OBJS = $(subst .cpp,.o,$(SRCS))


all: RenderMarkdown

RenderMarkdown: $(OBJS)
 $(CXX) $(LDFLAGS) -o RenderMarkdown $(OBJS) $(MAGICK_LDLIBS)

%.o: %.cpp
 $(CXX) $(COMPILE_FLAGS) -c $< -o $@

depend: .depend

.depend: $(SRCS)
 $(RM) ./.depend
 $(CXX) $(COMPILE_FLAGS) -MM $^ >> ./.depend;

clean:
 $(RM) $(OBJS)

distclean: clean
 $(RM) RenderMarkdown *~ .depend

-include .depend

