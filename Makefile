build/out: src/*.c src/headers/*.h
	gcc -o build/out src/*.c -lSDL3 -lSDL3_image -lSDL3_ttf

.PHONY: clean
clean:
	rm build/out
