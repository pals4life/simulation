CXXFLAGS   = -O3 -Wall -Wextra -g -fstack-protector-all -std=c++14
LDFLAGS    =
EXECUTABLE = engine
#SOURCES    = $(basename $(shell find . -name '*.cc' -or -name '*.cpp'))
SOURCES    = $(basename $(shell find . \( -path ./cmake-build-debug -prune \) -o \( \( -name '*.cc' -o -name '*.cpp' \) -print \)))

.PHONY: all
all: $(EXECUTABLE)

$(EXECUTABLE): $(addsuffix .o,$(SOURCES))
	$(CXX) $(LDFLAGS) $^ -o $@

%.d: %.$(EXTENSION)
	$(CXX) $(CXXFLAGS) -MM $< -o $@
	echo sed -i 's/:/ $@:/' $@
	printf '\t$$(CC) $$(CXXFLAGS) -c $$< -o $$@\n' >>$@

-include $(addsuffix .d,$(SOURCES))

.PHONY: clean
clean:
	find . -name '*.o' -delete
	find . -name '*.d' -delete
	find . -name '*~'  -delete
