
tfs: src/tfs.c src/libtfs.c
	gcc src/tfs.c src/libtfs.h src/libtfs.c -o ./build/tfs `pkg-config fuse --libs --cflags`



mkfs.tfs: mkfs.c libtfs.h libtfs.c
	gcc mkfs.c libtfs.h libtfs.c -o ./build/mkfs.tfs

clean:
	rm build/* -r
