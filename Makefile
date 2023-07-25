ifdef CROSS_COMPILE
	CROSS_COMPILE	=	$(CROSS_PATH)
	LIBRARY_PATH	=	$(CROSS_PREFIX)lib
	INCLUDE_PATH	=	$(CROSS_PREFIX)include
	LDFLAGS			=	-L$(LIBRARY_PATH) \
						-l:libjansson.a

	CFLAGS 			=	-I$(INCLUDE_PATH) 		\
						-g 						\
						-I./inc 				\
						-DUSE_DEBUG_LINE 		\
						-DUSE_COLOR 			\
						-DUSE_TIMESTAMP 		
endif

ifdef LOCAL
	LIBRARY_PATH	=	$(LOCAL_PREFIX)lib
	INCLUDE_PATH	=	$(LOCAL_PREFIX)include
	LDFLAGS 		=	-L$(LIBRARY_PATH) 		\
						-ljansson

	CFLAGS 			=	-g 						\
						-I$(INCLUDE_PATH) 		\
						-I./inc 				\
						-DUSE_DEBUG_LINE		\
						-DUSE_COLOR 			\
						-DUSE_TIMESTAMP 		
endif

.PHONY: clean, flash

CC 			= 	$(CROSS_COMPILE)gcc
CXX			= 	$(CROSS_COMPILE)g++
LD			=	$(CROSS_COMPILE)ld 
AR 			= 	$(CROSS_COMPILE)ar
AS 			= 	$(CROSS_COMPILE)as

STD 		= 	gnu99

VPATH		+=	./src/

EXEDIR		=	./bin
OBJDIR 		= 	./obj
OBJ 		+= 	$(OBJDIR)/main.o 			\
				$(OBJDIR)/logger.o	

DEP 		= 	$(OBJ:%.o=%.d)
EXEC 		= 	logger
RM 			= 	rm

# Builds the app
$(EXEDIR)/$(EXEC): $(OBJ)
	@echo =============EXE PROCESS=============
	[ -d "./bin" ] && echo "Directory /path/to/dir exists." || mkdir ./bin	
	$(CC) $(CFLAGS) $^ -o $@  $(LDFLAGS)

-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: %.c 
	@echo =============OBJ PROCESS=============
	[ -d "./obj" ] && echo "Directory /path/to/dir exists." || mkdir ./obj	
	$(CC)  $(CFLAGS) -MMD -c $< -o  $@  

# Cleans complete project

clean:
	$(RM) $(EXEDIR)/$(EXEC) $(OBJ) $(DEP)
 
flash: clean all
	@./$(EXEDIR)/$(EXEC)

