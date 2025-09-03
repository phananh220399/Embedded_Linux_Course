CUR_DIR := .
INC_DIR := $(CUR_DIR)/inc
SRC_DIR := $(CUR_DIR)/src
BIN_DIR := $(CUR_DIR)/bin
OBJ_DIR := $(CUR_DIR)/obj
LIB_DIR := $(CUR_DIR)/lib
LIB_STATIC_DIR := $(LIB_DIR)/static
LIB_SHARED_DIR := $(LIB_DIR)/shared

PROGRAM := main
LIB_NAME := strutils

create_obj:
	gcc -c $(SRC_DIR)/main.c -o $(OBJ_DIR)/main.o -I $(INC_DIR)
	gcc -c -fPIC $(SRC_DIR)/strutils.c -o $(OBJ_DIR)/strutils.o -I $(INC_DIR)

create_static:
	ar rcs $(LIB_STATIC_DIR)/lib$(LIB_NAME).a $(OBJ_DIR)/strutils.o
	gcc $(OBJ_DIR)/main.o -L$(LIB_STATIC_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/$(PROGRAM)_static

create_shared:
	gcc -shared $(OBJ_DIR)/strutils.o -o $(LIB_SHARED_DIR)/lib$(LIB_NAME).so
	gcc $(OBJ_DIR)/main.o -L$(LIB_SHARED_DIR) -l$(LIB_NAME) -o $(BIN_DIR)/$(PROGRAM)_shared

all: create_obj create_static create_shared

clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(LIB_STATIC_DIR)/*
	rm -rf $(LIB_SHARED_DIR)/*
	rm -rf $(BIN_DIR)/*