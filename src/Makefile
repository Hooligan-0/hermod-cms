##
 # hermod-cms - An Hermod based Content Management System
 #
 # Copyright (c) 2016 Cowlab
 #
 # Hermod is free software: you can redistribute it and/or modify
 # it under the terms of the GNU Lesser General Public License 
 # version 3 as published by the Free Software Foundation. You
 # should have received a copy of the GNU Lesser General Public
 # License along with this program, see LICENSE file for more details.
 # This program is distributed WITHOUT ANY WARRANTY see README file.
 #
 # Authors: Saint-Genest Gwenael <gwen@hooligan0.net>
 #
TARGET=app_cms.so
SRC=App.cpp PageIndex.cpp PageInstall.cpp PageArticle.cpp PageNewArticle.cpp Article.cpp
MODEL=Article.hpp

HERMOD_PATH = /usr/local
HERMOD_INC ?= $(HERMOD_PATH)/include/hermod

CC=g++
CP=cp
RM=rm -f

CFLAGS   = -fPIC -I$(HERMOD_INC)

ORMMODEL =  $(MODEL:%.hpp=%-odb.cxx)
ORMOBJ = $(ORMMODEL:%.cxx=%.o)

_COBJ =  $(SRC:.cpp=.o)
COBJ = $(patsubst %, %,$(_COBJ))

all: $(TARGET)
	@echo "  [CP] $(TARGET) => ../$(TARGET)"
	@$(CP) $(TARGET) ..

$(TARGET): $(ORMMODEL) $(ORMOBJ) $(COBJ)
	@echo "  [LD] $(TARGET)"
	@$(CC) $(CFLAGS) -shared  -Wl,-E,-soname,$@ -o $(TARGET) $(COBJ) $(ORMOBJ)

clean:
	$(RM) $(TARGET) *.o *~
	$(RM) -f *.cxx *.hxx *.ixx
	$(RM) -f ../$(TARGET)

install:
	cp $(TARGET) /usr/local/lib/hermod/

$(COBJ) : %.o : %.cpp
	@echo "  [CC] $@"
	@$(CC) $(CFLAGS) -c $< -o $@

################################################################################

$(ORMMODEL) : %-odb.cxx : %.hpp
	@echo " [ODB] $@"
	@odb -d pgsql --generate-query --generate-schema -I$(HERMOD_INC) $<

$(ORMOBJ) : %.o : %.cxx
	@echo "  [CC] $<"
	@$(CC) $(CFLAGS) -c $< -o $@
