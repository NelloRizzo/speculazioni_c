# ─── Compilatore e standard ───────────────────────────────────────────────────
CC       := gcc
STD      := -std=c17

# ─── Flag ─────────────────────────────────────────────────────────────────────
WFLAGS   := -Wall -Wextra -Wpedantic -Wshadow -Wconversion -Wstrict-prototypes
CFLAGS   := $(STD) $(WFLAGS)
LDFLAGS  := -lm
DBGFLAGS := -g3 -O0 -DDEBUG -fsanitize=address,undefined
RELFLAGS := -O2 -DNDEBUG

# ─── File ─────────────────────────────────────────────────────────────────────
out     ?= a.out
OUT_DIR ?= .

TARGET  := $(OUT_DIR)/$(out)
OBJ_DIR := $(OUT_DIR)/obj

# ─── Sorgenti e Oggetti ───────────────────────────────────────────────────────
SRCS := $(wildcard $(OUT_DIR)/*.c)
OBJS := $(patsubst $(OUT_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))
DEPS := $(OBJS:.o=.d)

# ─── Default: debug ───────────────────────────────────────────────────────────
all: debug

# ─── Target debug / release ───────────────────────────────────────────────────
debug:   CFLAGS += $(DBGFLAGS)
debug:   $(TARGET)

release: CFLAGS += $(RELFLAGS)
release: $(TARGET)

# ─── Link ─────────────────────────────────────────────────────────────────────
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

# ─── Compilazione con dipendenze automatiche ──────────────────────────────────
$(OBJ_DIR)/%.o: $(OUT_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

# ─── Crea la cartella obj se non esiste ───────────────────────────────────────
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(info SRCS  = $(SRCS))
$(info OBJS  = $(OBJS))
$(info TARGET= $(TARGET))
# ─── Includi dipendenze (header) ──────────────────────────────────────────────
-include $(DEPS)

# ─── Pulizia ──────────────────────────────────────────────────────────────────
.PHONY: all debug release clean fclean re

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -f $(TARGET)

re: fclean all