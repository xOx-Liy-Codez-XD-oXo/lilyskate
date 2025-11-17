#ifndef UTILILIYS_H
#define UTILILIYS_H

#include "colors.h"

#define printMtx(mtx) \
	printf("Mtx\r");\
	printf("%f, %f, %f\r", mtx[0][0], mtx[1][0], mtx[2][0]);\
	printf("%f, %f, %f\r", mtx[0][1], mtx[1][1], mtx[2][1]);\
	printf("%f, %f, %f\r", mtx[0][2], mtx[1][2], mtx[2][2]);\
	printf("%f, %f, %f\r", mtx[0][3], mtx[1][3], mtx[2][3]);\

//liy vert
#define LV_VP 1	        //0000 0000  0000 0001
#define LV_VN 2	        //0000 0000  0000 0010
#define LV_TC 4	        //0000 0000  0000 0100
#define LV_VC 8	        //0000 0000  0000 1000
#define LV_MX 16        //0000 0000  0001 0000
#define LV_VPIDX8 32    //0000 0000  0010 0000
#define LV_VPIDX16 64   //0000 0000  0100 0000
#define LV_VNIDX8 128   //0000 0000  1000 0000
#define LV_VNIDX16 256  //0000 0001  0000 0000
#define LV_TCIDX8  512  //0000 0010  0000 0000
#define LV_TCIDX16 1024 //0000 0100  0000 0000
#define LV_TC1	 2048   //0000 1000  0000 0000

void liy_VtxDescConfig(uint16_t config, int vtxfmt) {
	GX_ClearVtxDesc();
	if(config & LV_MX) { GX_SetVtxDesc(GX_VA_PTNMTXIDX, GX_DIRECT); }
	if(config & LV_VP) { GX_SetVtxDesc(GX_VA_POS, GX_DIRECT); }
	if(config & LV_VPIDX8) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX8); }
	if(config & LV_VPIDX16) { GX_SetVtxDesc(GX_VA_POS, GX_INDEX16); }
	if(config & LV_VN) { GX_SetVtxDesc(GX_VA_NRM, GX_DIRECT); }
	if(config & LV_VNIDX8) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX8); }
	if(config & LV_VNIDX16) { GX_SetVtxDesc(GX_VA_NRM, GX_INDEX16); }
	if(config & LV_VC) { GX_SetVtxDesc(GX_VA_CLR0, GX_DIRECT); }
	if(config & LV_TC) { GX_SetVtxDesc(GX_VA_TEX0, GX_DIRECT); }
	if(config & LV_TC1){ GX_SetVtxDesc(GX_VA_TEX1, GX_DIRECT); }
	if(config & LV_TCIDX8) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX8); }
	if(config & LV_TCIDX16) { GX_SetVtxDesc(GX_VA_TEX0, GX_INDEX16); }
	//attrfmts
	if(config & LV_MX) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_PTNMTXIDX, GX_INDEX8, GX_U8, 0); }
	if(config & LV_VP) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VPIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_POS, GX_POS_XYZ, GX_F32, 0); }
	if(config & LV_VN) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VNIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_NRM, GX_NRM_XYZ, GX_F32, 0); }
	if(config & LV_VC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_CLR0, GX_CLR_RGB, GX_RGB8, 0); }
	if(config & LV_TC) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TC1){ GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX1, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX8) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
	if(config & LV_TCIDX16) { GX_SetVtxAttrFmt(vtxfmt, GX_VA_TEX0, GX_TEX_ST, GX_F32, 0); }
}

static void liy_ConvertI4(uint8_t *in, uint8_t *out, int w, int h) {
	int tilesX = w / 8;
	int tilesY = h / 8;
	int outpos = 0;

	for (int ty = 0; ty < tilesY; ty++) {
		for (int tx = 0; tx < tilesX; tx++) {
			for (int y = 0; y < 8; y++) {
				for (int x = 0; x < 8; x += 2) {
					int srcX0 = tx * 8 + x;
					int srcY0 = ty * 8 + y;
					int srcX1 = srcX0 + 1;

					int pixelIndex0 = srcY0 * w + srcX0;
					int pixelIndex1 = srcY0 * w + srcX1;

					uint8_t p0 = (in[pixelIndex0 >> 1] >> ((~pixelIndex0 & 1) * 4)) & 0xF;
					uint8_t p1 = (in[pixelIndex1 >> 1] >> ((~pixelIndex1 & 1) * 4)) & 0xF;

					out[outpos++] = (p0 << 4) | p1;
				}
			}
		}
	}
}

#define liy_unpack_liyabc(title, destlen) \
u8 * title##Uncompressedbytes = malloc(destlen); \
unsigned long title##Destlen = destlen; \
unsigned long title##Sourcelen = (unsigned long)title##_liyabc_size; \
u16 title##Primcount, title##Framecount; \
int title##puffret = puff( \
	title##Uncompressedbytes, &title##Destlen, \
	title##_liyabc, &title##Sourcelen \
); \
if(title##puffret == 0) { \
	memcpy(&title##Primcount, &title##Uncompressedbytes[0], 2); \
	memcpy(&title##Framecount, &title##Uncompressedbytes[2], 2); \
} \
float * title##Data = (float *)&title##Uncompressedbytes[4];


#define liy_unpackTPL(tex) \
TPL_OpenTPLFromMemory(&tplfileFromMem, (void *)tex##_tpl, tex##_tpl_size);\
TPL_GetTexture(&tplfileFromMem, tex, &tex##TexObj); \
TPL_CloseTPLFile(&tplfileFromMem);

#define liy_unpackDoodle(tex, xres, yres) \
unsigned long tex##Destlen = (xres*yres)/8; \
unsigned long tex##Sourcelen = (unsigned long)tex##_doodle_size; \
u8 * tex##Uncompressedbytes = malloc(tex##Destlen); \
s8 tex##puffret = puff( \
	tex##Uncompressedbytes, &tex##Destlen, \
	tex##_doodle, &tex##Sourcelen \
); \
u8 * tex##Quadruple; \
u8 * tex##QuadrupleConv; \
unsigned long tex##Quadlen = tex##Destlen * 4; \
if(tex##puffret == 0) { \
	tex##Quadruple = malloc(tex##Quadlen); \
	memset(tex##Quadruple, 0, tex##Quadlen); \
	for(int i = 0; i < tex##Quadlen * 2; i++) { \
		int bit = (tex##Uncompressedbytes[i / 8] >> (7 - (i % 8))) & 1; \
       		tex##Quadruple[i * 4 / 8] |= bit << (7 - ((i * 4) % 8)); \
        	tex##Quadruple[(i * 4 + 1) / 8] |= bit << (7 - ((i * 4 + 1) % 8)); \
        	tex##Quadruple[(i * 4 + 2) / 8] |= bit << (7 - ((i * 4 + 2) % 8)); \
        	tex##Quadruple[(i * 4 + 3) / 8] |= bit << (7 - ((i * 4 + 3) % 8)); \
	} \
	free(tex##Uncompressedbytes); \
	tex##QuadrupleConv = memalign(32, tex##Quadlen); \
	liy_ConvertI4(tex##Quadruple, tex##QuadrupleConv, xres, yres); \
	DCFlushRange(tex##QuadrupleConv, tex##Quadlen); \
	free(tex##Quadruple); \
	GX_InitTexObj(&tex##TexObj, tex##QuadrupleConv, xres, yres, GX_TF_I4, GX_CLAMP, GX_CLAMP, GX_FALSE); \
}

#endif