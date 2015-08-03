#
#


CXX = c++  -DSTRINGLESS  -march=native 
# -Wl,--icf=all
# CXXFLAGS = -g -Wall -O2 -fPIC 
GRAPHITE = -fgraphite -fgraphite-identity -floop-block -floop-flatten -floop-interchange -floop-strip-mine -ftree-loop-linear -floop-parallelize-all
CXXFLAGS = -DNDEBUG -Wall -Ofast -fvisibility-inlines-hidden -fopt-info-vec -fPIC -flto -fipa-icf -fno-fat-lto-objects -flto-odr-type-merging -Wodr --param vect-max-version-for-alias-checks=50  -funsafe-loop-optimizations -ftree-loop-distribution -ftree-loop-if-convert-stores 
# -fipa-pta -Wunsafe-loop-optimizations -fgcse-sm -fgcse-las --param max-completely-peel-times=1
# -funswitch-loops -funroll-loops
#-DSTRINGLESS


INCDIR = -Iinclude -Iconfigs

OBJECTS := $(patsubst src/%.cc,obj/%.o,$(wildcard src/*.cc))
PLOBJECTS := $(patsubst plugins/%.cc,plobj/%.o,$(wildcard plugins/*.cc))
TESTS := $(patsubst test/%.cpp,bin/%,$(wildcard test/*.cpp))



SHLIB = lib/libOctave.so
PLUGINS = lib/libPlugins.so

# header file
# HS = $(wildcard include/*.h)

STRESSTEST_ALGO = lib/libStressTestAlgo.so
STRESSTEST_CONFIG = lib/libStressTestConfig.so
STRESSTEST_Main = bin/stressTest

.PHONY : all clean dump



all:	$(SHLIB) $(PLUGINS)  $(TESTS)

dump :
	echo $(OBJECTS) $(PLOBJECTS) $(TESTS)


$(SHLIB) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) -shared -o $(SHLIB) $(OBJECTS)

$(PLUGINS) : $(PLOBJECTS) $(SHLIB)
	$(CXX) $(CXXFLAGS) -fvisibility=hidden -shared -o $(PLUGINS) $(PLOBJECTS)  -L./lib -lOctave -Wl,-rpath ./lib

bin/% : test/%.cpp $(SHLIB) $(PLUGINS)
	$(CXX) $(CXXFLAGS)  $(INCDIR) -MMD $< -o $@  -L./lib -lOctave -lPlugins -Wl,-rpath ./lib


obj/%.o : src/%.cc
	$(CXX) $(CXXFLAGS) $(INCDIR) -MMD -c $< -o $@

plobj/%.o : plugins/%.cc
	$(CXX) $(CXXFLAGS)  -fvisibility=hidden  $(INCDIR) -MMD -c $< -o $@

clean:
	rm -r lib/* bin/* plobj/* obj/*; touch  bin/d.d plobj/d.d obj/d.d

runtest:
	$(patsubst test/%.cpp, bin/%;  ,$(wildcard test/*.cpp))
#	for exe in bin/*; do echo "running " ${exe}; ${exe}; done;

include  bin/*.d plobj/*.d obj/*.d
