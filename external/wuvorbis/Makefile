
CC = i686-w64-mingw32-gcc
CXX = i686-w64-mingw32-g++
AR = i686-w64-mingw32-ar
ASM = nasm
WINDRES = i686-w64-mingw32-windres
#CFLAGS += -O0 -g
# GIT_HASH = $(shell git rev-parse --short HEAD)
GIT_HASH = nothing
CUR_TIME = $(shell date +%s)
ASMFLAGS += -fwin32 -DWIN32
CFLAGS += -O3 -march=ivybridge -flto
CFLAGS += -Wall -Wno-unused-value -Wno-format -fpermissive -I. -I.. -Ivorbis/include -Iogg/include -DGIT_HASH=L\"$(GIT_HASH)\" -DCUR_TIME=L\"$(CUR_TIME)\" -DNDEBUG -DWIN32 -D_WIN32 -D_WINDOWS 
CFLAGS += -DWUVORBIS_EXPORTS -DDECODE_ONLY -D_USRDLL -DMINGW_HAS_SECURE_API -DUNICODE -D_UNICODE -DNO_STRICT -DCMAKE_INTDIR=\"Release\"   
LDFLAGS += -static -static-libstdc++ -static-libgcc -shared -Wl,--kill-at
LDLIBS += -lodbc32 -lodbccp32 -luuid

%.o: %.c
	echo -e "\tCC  $<"
	$(CC) -c $(CFLAGS) -o $@ $<

%.o: %.cpp
	echo -e "\tCXX  $<"
	$(CXX) -c $(CFLAGS) -o $@ $<

%.o: %.asm
	echo -e "\ASM  $<"
	$(ASM) $(ASMFLAGS) $< -o$@ 

%.o: %.rc
	echo -e "\tWINDRES  $<"
	$(WINDRES) --codepage=65001 $< $@

SOURCES :=  detect_cpu.cpp ogg/src/bitwise.c vorbis/lib/bitrate.c vorbis/lib/block.c vorbis/lib/codebook.c vorbis/lib/floor0.c vorbis/lib/floor1.c vorbis/lib/smallft.c vorbis/lib/psy.c ogg/src/framing.c vorbis/lib/info.c vorbis/lib/lpc.c vorbis/lib/lsp.c vorbis/lib/mapping0.c vorbis/lib/mdct.c vorbis/lib/registry.c vorbis/lib/res0.c vorbis/lib/sharedbook.c vorbis/lib/synthesis.c ../tp_stub.cpp tvpsnd.c vorbis/lib/vorbisfile.c vorbis/lib/window.c WuVorbisMainUnit.cpp
OBJECTS := $(SOURCES:.c=.o)
OBJECTS := $(OBJECTS:.cpp=.o)
OBJECTS := $(OBJECTS:.asm=.o)
OBJECTS := $(OBJECTS:.rc=.o)

BINARY ?= wuvorbis.dll
ARCHIVE ?= wuvorbis.$(GIT_HASH).7z

all: $(BINARY)

archive: $(ARCHIVE)

clean:
	rm -f $(OBJECTS) $(BINARY) $(ARCHIVE)

$(ARCHIVE): $(BINARY) 
	rm -f $(ARCHIVE)
	7z a $@ $^

$(BINARY): $(OBJECTS) 
	@echo -e "\tLNK $@"
	$(CXX) $(CFLAGS) $(LDFLAGS)  -o $@ $^  $(LDLIBS)
