# from: http://make.mad-scientist.net/papers/advanced-auto-dependency-generation/
# TODO: https://stackoverflow.com/questions/145270/calling-c-c-from-python
CC = gcc
CFLAGS=-DLOG_INFO -DLOG_ERROR -DLOG_DEBUG -Wpedantic -Werror -fPIC -lm
LIBS=
SRC=logging.c $(wildcard walla_*.c) simple_mmap.c json.c
TEST_MAIN=main.c
OBJS=$(patsubst %.c,%.o,$(SRC))

DEPDIR := .d
$(shell mkdir -p $(DEPDIR) >/dev/null)
DEPFLAGS = -MT $@ -MMD -MP -MF $(DEPDIR)/$*.Td

COMPILE.c = $(CC) $(DEPFLAGS) $(CFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
COMPILE.cc = $(CXX) $(DEPFLAGS) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) -c
POSTCOMPILE = @mv -f $(DEPDIR)/$*.Td $(DEPDIR)/$*.d && touch $@

%.o : %.c
%.o : %.c $(DEPDIR)/%.d
		$(COMPILE.c) $(OUTPUT_OPTION) $<
		$(POSTCOMPILE)

%.o : %.cc
%.o : %.cc $(DEPDIR)/%.d
		$(COMPILE.cc) $(OUTPUT_OPTION) $<
		$(POSTCOMPILE)

%.o : %.cxx
%.o : %.cxx $(DEPDIR)/%.d
		$(COMPILE.cc) $(OUTPUT_OPTION) $<
		$(POSTCOMPILE)

$(DEPDIR)/%.d: ;
.PRECIOUS: $(DEPDIR)/%.d

include $(wildcard $(patsubst %,$(DEPDIR)/%.d,$(basename $(SRCS))))

all: $(OBJS) tests libwalladb.so
clean:
	rm tests.* *.o *.so

tests: $(OBJS)
	$(CC) $(OBJS) $(TEST_MAIN) $(CFLAGS) -o $@

libwalladb.so: $(OBJS)
	$(CC) -shared -Wl,-soname,$@ -o $@ $(OBJS)
	cp libwalladb.so libwalladb.dll
