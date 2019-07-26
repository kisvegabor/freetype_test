#include <stdint.h>
#include <ft2build.h>
#include FT_FREETYPE_H

FT_Face     face;      /* handle to face object */

int render_init(void * font_buf, uint32_t font_buf_size);

int render_glyph(uint32_t glyph_code, 
                 uint8_t * out_buf, uint32_t * g_w, uint32_t * g_h, uint32_t * g_x, uint32_t * g_y);

void test(uint32_t glyph_id);

int main(int argc, char ** argv)
{

    FILE *fp;

    fp = fopen("./arial.ttf", "r"); 

    fseek(fp, 0L, SEEK_END);
    uint32_t sz = ftell(fp);
    rewind(fp);

    uint8_t * buf = malloc(sz);

    fread(buf, sz, 1, fp);

    fclose(fp);

    int error; 

    render_init(buf, sz);


    test('A');
    test('B');
}

int render_init(void * font_buf, uint32_t font_buf_size) 
{
    FT_Library  library;
    
    int error; 

    error = FT_Init_FreeType( &library );
    if ( error )
    {
        printf("Error in FT_Init_FreeType: %d\n", error);
        return -1;
    }
    
    error = FT_New_Memory_Face( library,
            font_buf,    /* first byte in memory */
            font_buf_size,      /* size in bytes        */
            0,         /* face_index           */
            &face );
    if ( error ) 
    {
        printf("Error in FT_New_Face: %d\n", error);
        return error;
    }

    error = FT_Set_Char_Size(
            face,    /* handle to face object           */
            0,       /* char_width in 1/64th of points  */
            16*64,   /* char_height in 1/64th of points */
            300,     /* horizontal device resolution    */
            300 );   /* vertical device resolution      */

    if ( error )
    {
        printf("Error in FT_Set_Char_Size: %d\n", error);
        return error;
    }

    error = FT_Set_Pixel_Sizes(
            face,   /* handle to face object */
            0,      /* pixel_width           */
            16 );   /* pixel_height          */

    if ( error )
    {
        printf("Error in FT_Set_Char_Size: %d\n", error);
        return error;
    }
}

int render_glyph(uint32_t glyph_code, 
                 uint8_t * out_buf, uint32_t * g_w, uint32_t * g_h, uint32_t * g_x, uint32_t * g_y)
{
    int error; 

    FT_UInt glyph_index = FT_Get_Char_Index( face, glyph_code );

    error = FT_Load_Glyph(
            face,          /* handle to face object */
            glyph_index,   /* glyph index           */
            FT_LOAD_DEFAULT );  /* load flags, see below */
    if ( error )
    {
        printf("Error in FT_Load_Glyph: %d\n", error);
        return error;
    }

    error = FT_Render_Glyph( face->glyph,   /* glyph slot  */
            FT_RENDER_MODE_NORMAL ); /* render mode */

    if ( error )
    {
        printf("Error in FT_Render_Glyph: %d\n", error);
        return error;
    }

    *g_w = face->glyph->bitmap.width;
    *g_h = face->glyph->bitmap.rows;
    *g_x = face->glyph->bitmap_left;
    *g_y = face->glyph->bitmap_top;

    memcpy(out_buf, face->glyph->bitmap.buffer, (*g_w) * (*g_h));
}

void test(uint32_t glyph_id)
{
    uint32_t w;
    uint32_t h;
    uint32_t x;
    uint32_t y;

    uint8_t out_buf[1024];
    
    render_glyph(glyph_id, out_buf, &w, &h, &x, &y);

    printf("w:%d, h:%d, x:%d, y:%d\n\n", w, h, x, y);
    
    uint32_t i,j;
    for(i = 0; i < h; i++) {
        for(j = 0; j < w; j++) {
            uint8_t p = out_buf[i * w + j];
            if(p) printf("%02x ", p);
            else printf("    ");
        }
        printf("\n");
    }
    
   printf("\n");
   printf("\n");
   
}
