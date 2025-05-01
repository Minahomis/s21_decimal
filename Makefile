GCC=-Wall -Werror -Wextra

all: clean s21_decimal

s21_decimal:
	gcc $(GCC) s21_print_decimal.c -o s21_decimal


style:
	clang-format -i -style=Google $(SRC_DIR)/*.c $(SRC_DIR)/*.h
	
clean:
	rm -rf $(TARGET) *.o *.a *.gch *.gcda *.gcno *.info report $(SRC_DIR)/*.o s21_string.a
