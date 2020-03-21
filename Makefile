# ======================================================= #
#                                                         #
#                      Definitions                        #
#                                                         #
# ======================================================= #

CC 	   	= 	g++
CFLAGS 	= 	-Wall
TARGET 	= 	ROUTER.exe

SRC_DIR = 	./src
OBJ_DIR = 	./obj

SRC 	= 	${wildcard $(SRC_DIR)/*.cpp}
OBJ 	= 	${patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC)}
LIB 	= 	-lpthread -lm

# ======================================================= #
#                                                         #
#                      Commands                           #
#                                                         #
# ======================================================= #

.PHONY: all clean

all : clean init $(TARGET)

init :
	if [ ! -d "obj" ]; then mkdir obj; fi

clean :
	rm -rf obj
	if [ -f $(TARGET) ]; then rm $(TARGET); fi

test : clean init $(TARGET)
	sh ./testing/route.sh
	sh ./testing/evaluate.sh

evaluate : clean init $(TARGET)
	sh ./testing/evaluate.sh

toy : clean init $(TARGET)
	./ROUTER.exe benchmarks/input benchmarks/output


$(TARGET) 		: 	$(OBJ)
	$(CC) $(CFLAGS) -o $@ $? $(LIB)

${OBJ_DIR}/%.o	:	${SRC_DIR}/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

