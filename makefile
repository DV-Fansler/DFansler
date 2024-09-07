######################################################################
# This file compiles a single .c or .cpp file (with the basename 
# defined by TARGET) as well as any additional files specified by 
# ADD_FILES. Edit TARGET and ADD_FILES variables as needed.
#
# This file must be edited by adjusting directory path as well as
# uncommenting additional options depending on the platform.
# By default, Linux options are used.
#
# Edit the GLG_LIB line to use either GLG EVALUATION or PRODUCT 
# libraries.
######################################################################

# Default compiler 
CC = gcc
#CC = g++

# Place any additional compiler options here.
OPTS = -g

######################################################################
# Uncomment one of the GLG_EXT_LIB lines for linking with either the 
# intermediate or extended library in addition to the GLG Standard API.
# For C++, also uncomment a corresponding GLG_CPP_API_OPTS option below.
######################################################################

# Linking with the GLG Intermediate API.
#GLG_EXT_LIB = -lglg_int

# Linking with the GLG Extended API.
GLG_EXT_LIB = -lglg_ext

#####################################################################
# For C++ compiling, uncomment one of the GLG_CPP_API_OPTS lines 
# to match the GLG library used for linking (Standard, Intermediate or 
# Extended).
######################################################################
#GLG_CPP_API_OPTS = -DGLG_CPP_INTERMEDIATE_API

#GLG_CPP_API_OPTS = -DGLG_CPP_STANDARD_API
GLG_CPP_API_OPTS = -DGLG_CPP_EXTENDED_API

######################################################################
# Uncomment one of the GLG_MAP_LIBS lines for linking with or without
# a map server.
######################################################################

# Linking without map server.
GLG_MAP_LIBS = -lglg_map_stub 

# Linking with the map server.
#GLG_MAP_LIBS = -lglg_map -ltiff

######################################################################
# Uncomment OPENGL_LIBS line for linking with OpenGL libraries
# if the application uses OpenGL outside of the GLG drawing.
######################################################################
#OPENGL_LIBS=-lGLU -lGL

######################################################################

MOTIF_INC=-I/usr/include
X_INC=-I/usr/include
X_LIB= -L/usr/lib/aarch64-linux-gnu/libXdmf.so.3.0.0                #-L/usr/lib

GLG_DIR=/home/pi/glg
GLG_INC=-I$(GLG_DIR)/include

BCM_INC=-I/home/pi/bcm2835-1.73/src
BCM_LIB=-L/home/pi/bcm2835-1.73/src

BSP_INC =-I/home/pi/projects/BSH

######################################################################
# Uncomment one GLG_LIB line for linking with the GLG evaluation, 
# product or Community Edition libraries.
######################################################################

# To use GLG EVALUATION libraries
#GLG_LIB=-L$(GLG_DIR)/eval/lib

# To use GLG PRODUCT or COMMUNITY EDITION libraries
GLG_LIB=-L$(GLG_DIR)/lib
 
# On Linux, uncomment the line below for additional libraries.
# Due to a known bug in ldd/dlopen, -lpthread is required to avoid a crash 
# when OpenGL is used.
EXTRA_LIBS = -lpthread

# On Solaris, uncomment the next line for additional libraries
#EXTRA_LIBS = -lnsl -lsocket

# On AIX, uncomment the next line for additional libraries
#EXTRA_LIBS = -liconv

# To use static Motif library (libXm.a), uncomment the next line for 
# additional libraries
#EXTRA_LIBS_STATIC = -lXmu -lXext -lXp 

# Name of the main file to compile and link
#   (without the .c or .cpp extension)
TARGET = Lesson4


# Uncomment to list additional files here (.o)
ADD_FILES = BSP.o ADC.o

######## Don't edit below ############################################
 
CFLAGS = $(OPTS) $(GLG_CPP_API_OPTS) $(GLG_INC) $(MOTIF_INC) $(X_INC) $(BCM_INC) $(BSP_INC) $(PCA9685_INC)
 
LD_FLAGS = $(GLG_LIB) $(X_LIB) -L. $(BCM_LIB)
 
OBJECTS =  $(TARGET).o $(ADD_FILES)

INCLUDES := $(sort $(wildcard *.h))

all:   $(TARGET) 
 
%.o : %.c $(INCLUDES)
	$(CC) -c $(CFLAGS) \
		$< -o $@;

%.o : %.cpp $(INCLUDES)
	$(CC) -c $(CFLAGS) \
		$< -o $@;

$(TARGET) : $(OBJECTS)
	( $(CC) $(LD_FLAGS) -o $@ $(OBJECTS) \
		$(GLG_EXT_LIB) -lglg $(GLG_MAP_LIBS) \
		-lXm -lXt -lX11 $(EXTRA_LIBS_STATIC) \
                -lXft -lfontconfig -lfreetype -lz -ljpeg -lpng \
                $(OPENGL_LIBS) -lm -ldl $(EXTRA_LIBS) \
		|| ( rm $@ ; exit 1 ) )

