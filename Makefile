SRC_LENIENT := $(wildcard lenient/src/*.c)
SRC_LENIENT_WITHOUT_MAIN := $(filter-out lenient/src/main.c,$(SRC_LENIENT))
INCLUDE_LENIENT := -Ilenient/include

SRC_TEST := \
	$(SRC_LENIENT_WITHOUT_MAIN) \
	$(shell find ./test -name "*.c")
INCLUDE_TEST := $(INCLUDE_LENIENT)

LDFLAGS :=
LDFLAGS_TEST := $(LDFLAGS) -lcriterion

out:
	mkdir out

# If libcriterion shared object file is not found, try running
# `sudo /sbin/ldconfig`. Config of the executable should be found from:
# /etc/ld.so.conf
test: out/utest
	$<

out/utest: $(SRC_TEST) out
	$(CC) $(INCLUDE_TEST) $(SRC_TEST) $(LDFLAGS_TEST) -o $@

out/lenient: $(SRC_LENIENT) out
	$(CC) $(INCLUDE_LENIENT) $(SRC_LENIENT) -o $@

out/lenient_invalid_private_access: $(SRC_LENIENT) out
	$(CC) -DINVALID_ACCESS_TO_PRIVATE_DATA=1 $(INCLUDE_LENIENT) $(SRC_LENIENT) -o $@

out/lenient_memory_error_detector: $(SRC_LENIENT) out
	$(CC) $(INCLUDE_LENIENT) -fsanitize=address $(SRC_LENIENT) -o $@
