#include "ndsProject.h"
#include "stdio.h"

#include "gl2d.h"
#include "tiles.h"
#include "shuttle.h"

// This tileset won't make use of our texture packer generated coords.
// Messy, manual and prone to errors.
// BMP is 256x256 and tiles are 16x16 so.. (256/16) * (256 /16) = 16 * 16
glImage  Tiles[(256/16) * (256/16)];

// These sprites are single texture only so no need to
// do anything special
glImage  Shuttle[1];					// single image

	// Console fonts for both screen
	PrintConsole topScreen;
    PrintConsole bottomScreen;

void NDSProject::Init()
{
	fw::Framework::Init();

	//set mode 5, enable BG0 and set it to 3D
	videoSetMode(MODE_0_3D);
	videoSetModeSub(MODE_0_2D);

	// initialize gl2d
	glScreen2D();

	// Set up enough texture memory for our textures
	// Bank A is just 128kb and we are using 194 kb of
	// sprites
	vramSetBankA( VRAM_A_TEXTURE );
	vramSetBankB( VRAM_B_TEXTURE );

	vramSetBankF(VRAM_F_TEX_PALETTE);  // Allocate VRAM bank for all the palettes

	vramSetBankE(VRAM_E_MAIN_BG);	 // Main bg text/8bit bg. Bank E size == 64kb, exactly enough for 8bit * 256 * 192 + text layer
	// Load our custom font for the top screen
	consoleInit(&topScreen,1, BgType_Text4bpp, BgSize_T_256x256, 31,0, true, false);
	//put bg 0 at a lower priority than the text background
	bgSetPriority(0, 1);

	// Bottom screeen
	vramSetBankI( VRAM_I_SUB_BG_0x06208000 );
	consoleInit( &bottomScreen, 0, BgType_Text4bpp, BgSize_T_256x256, 20, 0, false, false );

	consoleDemoInit();

	iprintf("\x1b[0;0HAmongNDS\n");
	iprintf("By Coda\n");
	iprintf("github.com/cfazilleau");


	// Our texture handle for our tiles
	// I used glLoadTileSet since the texture
	// is just a bunch of 16x16 tiles in a 256x256
	// tileset so we don't need a texture packer for this.
	int TilesTextureID = glLoadTileSet(Tiles, 16, 16, 256, 256,						// glImage, width, height, bmp width, bmp height
										GL_RGB256,									// texture type for glTexImage2D() in videoGL.h
										TEXTURE_SIZE_256, TEXTURE_SIZE_256,			// sizeX & sizeY for glTexImage2D() in videoGL.h
										GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT, // param for glTexImage2D() in videoGL.h
										256, (u16*)tilesPal, (u8*)tilesBitmap);		// Length of the palette to use, palette, bmd data (generated by GRIT)

	// Shuttle
	// Since the shuttle is just a single 64x64 image,
	// We use glLoadTileSet() giving the right dimensions.
	int ShuttleTextureID =
		glLoadTileSet( Shuttle,			// pointer to glImage array
					   64,				// sprite width
					   64,				// sprite height
					   64,				// bitmap image width
					   64,				// bitmap image height
					   GL_RGB16,		// texture type for glTexImage2D() in videoGL.h
					   TEXTURE_SIZE_64,	// sizeX for glTexImage2D() in videoGL.h
					   TEXTURE_SIZE_64,	// sizeY for glTexImage2D() in videoGL.h
					   GL_TEXTURE_WRAP_S|GL_TEXTURE_WRAP_T|TEXGEN_OFF|GL_TEXTURE_COLOR0_TRANSPARENT,
					   16,					// Length of the palette to use (16 colors)
					   (u16*)shuttlePal,		// Load our 256 color tiles palette
					   (u8*)shuttleBitmap // image data generated by GRIT
					 );

}

void NDSProject::Update()
{
	fw::Framework::Update();

	keys = keysDown();
	if (keys & KEY_START)
		Stop();

	touchRead(&touchXY);
}

// Draws the background
void DrawBG( glImage *images )
{
	for(int y = 0; y < 256 / 16; y++ )
	for(int x = 0; x < 256 / 16; x++ )
		glSprite( x * 16, y * 16, GL_FLIP_NONE, &images[(y * 16 + x) & 255] );
}

void NDSProject::Render()
{
	fw::Framework::Render();

	// Start 2D mode
	glBegin2D();

	// Set our palette to our tiles (256 colors)
	// and draw our background
	DrawBG( Tiles );

	glEnd2D();


	glFlush( 0 );


	// print at using ansi escape sequence \x1b[line;columnH
	iprintf("\x1b[10;0HFrame = %d", GetFrame());
	iprintf("\x1b[16;0HTouch x = %04X, %04X\n", touchXY.rawx, touchXY.px);
	iprintf("Touch y = %04X, %04X\n", touchXY.rawy, touchXY.py);
}