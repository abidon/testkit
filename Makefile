CXXFILES=TestKit/TKHtmlWriter.cpp TestKit/TKJUnitWriter.cpp TestKit/TKMechanisms.cpp TestKit/TKTestResults.cpp TestKit/TKTestRunner.cpp TestKit/TKTestSuite.cpp TestKit/TKTextWriter.cpp
CXXOBJS=$(patsubst %.cpp, %.o, ${CXXFILES})
TARGET=libtestkit.a

CPPFLAGS=-std=c++11 -fPIC -Wall -Wextra -I.

all: libstatic

libstatic: $(TARGET)

$(TARGET): $(CXXOBJS)
	$(AR) rcs $@ $^

clean:
	$(RM) TestKit/*.o
