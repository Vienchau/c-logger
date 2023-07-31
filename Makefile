ifdef CROSS_COMPILE
	CROSS_COMPILER	=	$(CROSS_PATH)
	LIBRARY_PATH	=	$(CROSS_PREFIX)lib
	INCLUDE_PATH	=	$(CROSS_PREFIX)include
	LDFLAGS			=	-L$(LIBRARY_PATH)

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
	LDFLAGS 		=	-L$(LIBRARY_PATH)

	CFLAGS 			=	-g 						\
						-I$(INCLUDE_PATH) 		\
						-I./inc 				\
						-DUSE_DEBUG_LINE		\
						-DUSE_COLOR 			\
						-DUSE_TIMESTAMP 		\
						-fsanitize=address 		\
						-fsanitize=leak 

endif

.PHONY: clean, flash

CC 			= 	$(CROSS_COMPILER)gcc
CXX			= 	$(CROSS_COMPILER)g++
LD			=	$(CROSS_COMPILER)ld 
AR 			= 	$(CROSS_COMPILER)ar
AS 			= 	$(CROSS_COMPILER)as
STD 		= 	gnu99



VPATH		+=	./src/

EXEDIR		=	./bin
OBJDIR 		= 	./obj
LIBDIR		=	./lib

OBJ 		+= 	$(OBJDIR)/test.o 			\
				$(OBJDIR)/logger.o	

DEP 		= 	$(OBJ:%.o=%.d)
EXEC 		= 	logger
LIB			= 	libclogger.a

RM 			= 	rm

all: test libs install

# Builds the app
test: $(OBJ)
	@echo =============EXE PROCESS=============
	@[ -d "./bin" ] && echo "Directory /path/to/dir exists." || mkdir ./bin	
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $(EXEDIR)/$@  

-include $(DEP)

# Building rule for .o files and its .c/.cpp in combination with all .h
$(OBJDIR)/%.o: %.c 
	@echo =============OBJ PROCESS=============
	@[ -d "./obj" ] && echo "Directory /path/to/dir exists." || mkdir ./obj	
	$(CC)  $(CFLAGS) $(LDFLAGS) -c $< -o  $@  


$(LIBDIR)/$(LIB): $(OBJDIR)/logger.o
	@echo =============CLOGGER LIBRARY STATIC BUILD=============
	@[ -d "./lib" ] && echo "Directory lib exists." || mkdir lib	
	$(AR) rcs $(LIBDIR)/$(LIB) $(OBJDIR)/logger.o

libs: $(LIBDIR)/$(LIB)

install:
	@echo moving lib into "$(LIBRARY_PATH)"
	mv $(LIBDIR)/$(LIB) $(LIBRARY_PATH)
	cp ./inc/logger.h $(INCLUDE_PATH)

clean:
	$(RM) $(EXEDIR)/$(EXEC) $(OBJ) $(DEP) $(LIBDIR)/$(LIB)
 


