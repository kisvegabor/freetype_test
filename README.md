# freetype_test


I've mainly followed [this tutorial](https://www.freetype.org/freetype2/docs/tutorial/step1.html).

Steps:
- Download Freetype from [here](https://sourceforge.net/projects/freetype/files/)
- `make`
- `sudo make install`
- In `freetype_test`: ` gcc render.c -I/usr/include/freetype2 -L/usr/local/lib -lfreetype`
- `./a.out`
