#

## where source files live
COMPUBPRINTF = ../src

CFLAGS += -I../include

CFLAGS += -g

CFLAGS += -std=gnu99

# treat warnings as errors -- a gentle push in the back to write clean, portable code.
CFLAGS += -Werror

#warnings
CFLAGS += -pedantic
CFLAGS += -Wall -Wextra
CFLAGS += -Wformat-extra-args -Wformat-zero-length -Wformat-nonliteral -Wformat-security

CFLAGS += -Wcast-align -Wimplicit 
CFLAGS += -Wpointer-arith -Wswitch
CFLAGS += -Wredundant-decls -Wreturn-type -Wshadow -Wunused
CFLAGS += -Wcast-qual
CFLAGS += -Wnested-externs 

CFLAGS += -Wmissing-prototypes 
CFLAGS += -Wstrict-prototypes
CFLAGS += -Wmissing-declarations

CPPFLAGS += -DTESTDEBUG

# Compiler flags to generate dependency files.
CPPFLAGS += -MD -MP


OBJPREFIX = obj/dot/dot/

SRC_TESTS += tests-double.c
SRC_TESTS += tests-nodoubles.c
SRC_TESTS += tests-pointers.c
SRC_TESTS += tests-iso.c
SRC_TESTS += tests_BrunoHaible.c
SRC_TESTS += test-runner.c

SRC_HOST = test-host.c $(SRC_TESTS)

test-host: $(addprefix $(OBJPREFIX),$(SRC_HOST:.c=.o))
	$(CC) $(LDFLAGS) -o $@ $^

SRC_COMMON_LIB += $(COMPUBPRINTF)/xprintf-core.c
#SRC_COMMON_LIB += $(COMPUBPRINTF)/xprintf-util.c
SRC_COMMON_LIB += $(COMPUBPRINTF)/xprintf-debug.c
SRC_COMMON_LIB += impl.c
SRC_COMMON_LIB += $(SRC_TESTS)

SRC_LIB_FLOAT += $(COMPUBPRINTF)/xprintf-double.c

SRC_PRINTF = test-xvprintf-protected.c
SRC_PRINTF += xprintf-core-hosttest.c

SRC_PRINTF_NODOUBLES = test-xvprintf-protected-nodoubles.c
SRC_PRINTF_NODOUBLES += xprintf-core-hosttest.c

SRC_SPRINTF = test-xvsnprintf.c
SRC_SPRINTF += $(COMPUBPRINTF)/xsnprintf.c

SRC_RSRC = test-xvprintf-protected.c
SRC_RSRC += xprintf-core-hosttest.c

OBJ_LIB_FLOAT = $(addprefix $(OBJPREFIX),$(SRC_LIB_FLOAT:.c=.o))
OBJ_COMMON_LIB = $(addprefix $(OBJPREFIX),$(SRC_COMMON_LIB:.c=.o))
OBJ_PRINTF = $(addprefix $(OBJPREFIX),$(SRC_PRINTF:.c=.o))
OBJ_PRINTF_NODOUBLES = $(addprefix $(OBJPREFIX),$(SRC_PRINTF_NODOUBLES:.c=.o))
OBJ_SPRINTF = $(addprefix $(OBJPREFIX),$(SRC_SPRINTF:.c=.o))
OBJ_RSRC = $(addprefix $(OBJPREFIX),$(SRC_RSRC:.c=.o))

TEST_EXECUTABLES = test-host test-xvsnprintf test-xvprintf-protected test-xvprintf-protected-nodouble test-xvprintf
ALL_EXECUTABLES = $(TEST_EXECUTABLES) printf-pointer

all: $(ALL_EXECUTABLES)

this: all

test: run-test-all

.PHONY: run-test-* 

run-test-all: $(addprefix run-,$(TEST_EXECUTABLES))

run-%: %
	./$<

.PHONY: diff-syms-doubles
diff-syms-doubles: test-xvprintf-protected-nodouble-globals.log test-xvprintf-protected-globals.log
	@echo "===================="
	@echo "Only in test-xvprintf-protected-nodouble:"
	@comm -23 test-xvprintf-protected-nodouble-globals.log test-xvprintf-protected-globals.log
	@echo "===================="
	@echo "Only in test-xvprintf-protected (with doubles):"
	@comm -13 test-xvprintf-protected-nodouble-globals.log test-xvprintf-protected-globals.log
	@echo "===================="

test-%-globals.log: test-%
	@echo "nm -g $< > $@"
	@nm -g $< | sed -e 's=^[0-9a-fA-F]*==' -e 's=^[ 	]*[A-Z][ 	]*_==' | sort > $@

LIBXPRINTF = libxprintf.a

DOUBLE_OBJ = $(OBJ_LIB_FLOAT)

$(LIBXPRINTF): $(OBJ_COMMON_LIB)
	$(AR) $(ARFLAGS) $@ $^

$(COMMON_LIB_FLOAT): $(OBJ_COMMON_LIB_FLOAT)
	$(AR) $(ARFLAGS) $@ $^

test-xvprintf-protected-nodouble: $(OBJ_PRINTF_NODOUBLES) $(LIBXPRINTF)
	$(CC) $(LDFLAGS) -o $@ $^
	
test-xvprintf-protected: $(OBJ_PRINTF) $(LIBXPRINTF) $(DOUBLE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(DOUBLE_TRIGGER) $^

test-xvsnprintf: $(OBJ_SPRINTF) $(LIBXPRINTF) $(DOUBLE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(DOUBLE_TRIGGER) $^

test-xvprintf: $(OBJ_RSRC) $(LIBXPRINTF) $(DOUBLE_OBJ)
	$(CC) $(LDFLAGS) -o $@ $(DOUBLE_TRIGGER) $^

printf-pointer: $(OBJPREFIX)printf-pointer.o
	$(CC) $(LDFLAGS) -o $@ $^

# exceptions for "%#s" format:
$(OBJPREFIX)printf-pointer.o: CFLAGS += -Wno-format
# redundant redeclaration of 'xxx':
$(OBJPREFIX)$(COMPUBPRINTF)/xprintf-double.o: CFLAGS += -Wno-redundant-decls


# general .c -> .o rule
$(OBJPREFIX)%.o: %.c
	@test -d $(dir $@) || mkdir -p $(dir $@)
	$(CC) -c $(CPPFLAGS) $(CFLAGS) $< -o $@

clean::
	-rm -rf obj
	-rm -f $(LIBXPRINTF) $(ALL_EXECUTABLES)

# Include the dependency files, but not during a clean.
ifeq ($(filter clean%,$(MAKECMDGOALS)),) 
-include $(OBJPREFIX)*.d
-include $(OBJPREFIX)$(COMPUBPRINTF)/*.d
endif 