#include "bmp.h"
#include <math.h>



int BOXIN( int x, int y, int rad, int xp, int yp ){
    if(
        (xp >= x) &&
        (yp >= y) &&
        (xp <= x+rad) &&
        (yp <= y+rad)
    ) return 1;
    return 0;
}

unsigned int inline SHADERPIXEL( unsigned int BUFF, unsigned int BUFFB ){
    return (((BUFF & 0x00FF00FFu) * BUFFB) & 0xFF00FF00u | (((BUFF & 0x0000FF00u) * BUFFB) & 0x00FF0000u)) >> 8;
}

#define COLOR_A 0xffffffff
#define COLOR_B 0xffadfd3f
#define COLOR_C 0x00000000

int __fastcall Distancia( int x1, int y1, int x2, int y2 ){
    return sqrt( pow(x1-x2,2) + pow(y1-y2,2) );
}


int main(void){
	typedef unsigned int U32;
	int W,H,Z;
    unsigned int *IMG0 = (unsigned int*)LoadBMPConverter( "0.bmp", &W, &H, &Z );
	unsigned int *IMG1 = (unsigned int*)LoadBMPConverter( "1.bmp", &W, &H, &Z );
    unsigned int *SCAN0 = IMG0;
    unsigned int *SCAN1 = IMG1;

    Z = W*H;

    unsigned int MAXVAL = 0;

    unsigned int Step = 0;
    while(Z--){
        unsigned char MAX = (IMG1[Step]&0xFF);
        if (MAX>MAXVAL){
            MAXVAL = MAX;
        }
        Step++;
    }
    printf("Value Z-Buffer max: %d\n", MAXVAL );

    int Radio   =        (0xff); // Radio de la luz.
    int Z_      =               17; // Z de la luz al mover.
    int ZOOM    =               16; // radio de alcanse en profundidad Z.
    int POSL[3] = { W/2, H/2, Z_ }; // Posicion de la luz en X,Y.

    for (int Y = 0; Y < H; ++Y)
    {
        for (int X = 0; X < W; ++X)
        {
            int INDICE = Y * W + X; // Indice de los dos buferes.
            // int DIST = Distancia( POSL[0]+128, POSL[1]+128, X,Y );
            int DIST = Distancia( X,Y, POSL[0], POSL[1] );
            // if ( BOXIN( POSL[0],POSL[1], Radio, X,Y )/*BOXIN(109,97,64,X,Y)*/ )
            if ( DIST <= Radio )
            {
                int ZL = (SCAN1[ INDICE ]&0xff);
                if ( (ZL >= POSL[2]) && (ZL <= (POSL[2]+ZOOM) )){
                    SCAN0[ INDICE ] = SHADERPIXEL( SCAN0[ INDICE ], ((MAXVAL * -DIST) / Radio)&0xff );
                } else SCAN0[ INDICE ] = COLOR_C; // Color negro
            }  else SCAN0[ INDICE ] = COLOR_C;    // Color negro

        }
    }





    // {
    //     for (int Y = 0; Y < H; ++Y)
    //     {
    //         for (int X = 0; X < W; ++X)
    //         {
    //             SHADERPIXEL( &SCAN0[ Y * W + X ], &SCAN1[ Y * W + X ] );
    //         }
    //     }
    // }

    // Z = W*H;
    // while(Z--)SHADERPIXEL(SCAN0++,SCAN1++);
	SaveBMPConverter("VIDEO.BMP",IMG0,W,H,32);
    free(IMG0),IMG0;
    free(IMG1),IMG1;
	return 0;
}