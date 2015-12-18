COMPILER = gcc
FLAGS= -O3 --std=gnu99 -Wall  -funsigned-char -Wundef -Wsign-compare  -Wstrict-prototypes 
LDFLAGS=

FLAGS+= `sdl2-config --cflags`
LDFLAGS+= `sdl2-config --libs`

all: 2048 

clean:
	rm -f 2048
	rm -f 2048.exe
	rm -f 2048_win.zip
	rm -f 2048.rc
	rm -f 2048.ico
	rm -f 2048.app/Contents/MacOS/2048_osx
	rm -f 2048.app/Contents/Resources/iconfile.icns
	rm -f 2048rc.o

2048: main.c sdl_util.c sdl_util.h 2048.h 2048.c Makefile 
	@$(COMPILER) $(FLAGS) main.c 2048.c sdl_util.c $(LDFLAGS) -o 2048 

2048.app/Contents/MacOS/2048_osx: main.c sdl_util.c sdl_util.h 2048.h 2048.c Makefile 
	$(COMPILER) $(FLAGS) main.c 2048.c sdl_util.c $(LDFLAGS) -o 2048_osx
	@install_name_tool -change /usr/local/opt/sdl2/lib/libSDL2-2.0.0.dylib @executable_path/../Frameworks/SDL2.framework/libSDL2-2.0.0.dylib 2048_osx
	@makeicns -32 2048_36x36x4.png -out 2048.app/Contents/Resources/iconfile.icns 
	@mv 2048_osx 2048.app/Contents/MacOS
	cp -f /usr/local/lib/libSDL2-2.0.0.dylib 2048.app/Contents/Frameworks/SDL2.framework/libSDL2-2.0.0.dylib
	@touch 2048.app

2048.ico: 2048_36x36x4.png Makefile
	icotool -c -o 2048.ico 2048_36x36x4.png

2048.exe: 2048_36x36x4.png 2048.ico main.c sdl_util.c sdl_util.h 2048.h 2048.c Makefile SDL2-2.0.4
	echo "0 ICON 2048.ico" > 2048.rc
	i686-w64-mingw32-windres 2048.rc 2048rc.o
	i686-w64-mingw32-gcc -static -std=gnu99 -ISDL2-2.0.4/i686-w64-mingw32/include/SDL2 -D_GNU_SOURCE=1 -Dmain=SDL_main -LSDL2-2.0.4/i686-w64-mingw32/lib  main.c 2048.c sdl_util.c -lmingw32 -lSDL2main -lSDL2 -mwindows -Wl,--no-undefined -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lversion -luuid -static-libgcc  2048rc.o -o 2048.exe

2048_win.zip: 2048.exe
	zip -j 2048_win.zip 2048.exe 

2048_osx.zip: 2048.app/Contents/MacOS/2048_osx
	@zip -r 2048_osx.zip 2048.app
	

SDL2-2.0.4:
	wget https://www.libsdl.org/tmp/release/SDL2-devel-2.0.4-mingw.tar.gz
	tar -xzf SDL2-devel-2.0.4-mingw.tar.gz
	rm -f SDL2-devel-2.0.4-mingw.tar.gz


.PHONY : clean all

