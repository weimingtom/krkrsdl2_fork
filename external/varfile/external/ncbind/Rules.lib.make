#############################################
##                                         ##
##    Copyright (C) 2020-2020 Julian Uy    ##
##  https://sites.google.com/site/awertyb  ##
##                                         ##
## See details of license at "license.txt" ##
##                                         ##
#############################################

INCFLAGS += -Iexternal -Iexternal/ncbind
SOURCES += ../ncbind/ncbind.cpp

TP_STUB_BASE = external/ncbind/external/tp_stubz/
include $(TP_STUB_BASE)/Rules.lib.make
