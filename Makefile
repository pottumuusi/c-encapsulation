SRC_LENIENT := $(shell find ./lenient -name "*.c")
INCLUDE_LENIENT := -Ilenient/include

out:
	mkdir out

out/lenient : $(SRC_LENIENT) out
	gcc $(INCLUDE_LENIENT) $(SRC_LENIENT) -o $@

out/lenient_memory_error_detector: $(SRC_LENIENT) out
	gcc $(INCLUDE_LENIENT) -fsanitize=address $(SRC_LENIENT) -o $@
