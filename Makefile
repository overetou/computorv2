.PHONY : clean, fclean, all

NAME = computor

objdir = obj

obj = $(addprefix $(objdir)/, main.o exec_tracks.o trigger_name_funcs.o trigger_sign_funcs.o parenthesis.o operations.o)

VPATH = src

CC = gcc

CFLAGS = -Wextra -Werror -Wall -I header/

$(objdir)/%.o : %.c header/computor.h
	@$(COMPILE.c) $(OUTPUT_OPTION) $<

all: $(NAME)

$(NAME): $(obj)
	@$(CC) $(CFLAGS) -o $@ ../tools/libft/libft.a $^

$(obj): | $(objdir)

$(objdir):
	mkdir $(objdir)

clean:
	rm -f $(obj)

fclean: clean
	rm -f $(NAME)

re: fclean all

test: $(NAME)
	@$(CC) test_main.c -I header/ ./$(NAME)
	@./a.out
