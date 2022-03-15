##########################################################################
# targets:                                                               #
#  all (default) : build sap2 and sapfs                                  #
#  install       : build and install sap2 and sapfs in $(HOME)/emu/sap   #
#  clean         : delete all rebuildable files                          #
########################################################################## 


################
# object files #
################ 

OBJ_DIR =
OBJ_SAP = $(OBJ_DIR)sap2.o $(OBJ_DIR)libsap.o $(OBJ_DIR)lfloppy.o
OBJ_SAPFS = $(OBJ_DIR)sapfs.o $(OBJ_DIR)libsap.o


##################
# compiler flags #
##################

CC = gcc

WFLAGS = -Wall -W -Werror

OFLAGS = -O2 

CFLAGS = $(WFLAGS) $(OFLAGS)


################
# target rules #
################

.PHONY: all install clean

all: sap2 sapfs fd2sap

clean:
	rm -f sap2 sapfs fd2sap $(OBJ_DIR)*.o

sap2: $(OBJ_SAP) 
	$(CC) -s -o sap2 $(OBJ_SAP)

sapfs: $(OBJ_SAPFS)
	$(CC) -s -o sapfs $(OBJ_SAPFS)

fd2sap: 
	$(CC) -s -o fd2sap fd2sap.c

$(OBJ_DIR)%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $<

