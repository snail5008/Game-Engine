
CC = clang
CARGS = -static -c -Wall -Wextra -pedantic -std=c2x -O2 -o
OBJS = build/mesh.o build/window.o build/camera.o build/utils.o build/libgame.a

$(shell mkdir -p build/)

all: run

build/camera.o: src/camera.c src/camera.h
	$(CC) $(CARGS) $@ $<

build/window.o: src/window.c src/window.h
	$(CC) $(CARGS) $@ $<

build/mesh.o: src/mesh.c src/mesh.h
	$(CC) $(CARGS) $@ $<

build/utils.o: src/utils.c src/utils.h
	$(CC) $(CARGS) $@ $<

build/libgame.a:
	(cd game && cargo build --release && mv target/release/libgame.a ../build/libgame.a)

a.out: $(OBJS)
	cp third_party/glad.o build/glad.o
	clang -o a.out build/*.o build/libgame.a -lglfw -lm

.PHONY: build
build: a.out

.PHONY: run
run: a.out
	./$<
