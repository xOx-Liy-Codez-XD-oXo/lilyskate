#ifndef PRIMITIVES_H
#define PRIMITIVES_H

void drawArrConfig(uint16_t config, long tricount, uint8_t vtxfmt, 
                   float *vertpos, uint8_t *vpindex8, uint16_t *vpindex16, 
                   float *vertnor, uint8_t *vnindex8, uint16_t *vnindex16, 
                   float *texcord, uint8_t *tcindex8, uint16_t *tcindex16, 
                   uint8_t *mtxidx, 
                   uint8_t *vertcol) {
	GX_Begin(GX_TRIANGLES, vtxfmt, tricount * 3);
	for(int i = 0; i < tricount * 3; i++) {
		int i3 = i * 3;
		if(config & LV_MX)     { GX_MatrixIndex1x8( mtxidx[i] * 3); }
		if(config & LV_VP)     { GX_Position3f32(vertpos[i3], vertpos[i3+1], vertpos[i3+2]); }
		if(config & LV_VPIDX8) { GX_Position1x8( vpindex8[i] ); }
		if(config & LV_VPIDX16){ GX_Position1x16( vpindex16[i] ); }
		if(config & LV_VN)     { GX_Normal3f32(vertnor[i3], vertnor[i3+1], vertnor[i3+2]); }
		if(config & LV_VNIDX8) { GX_Normal1x8( vnindex8[i] ); }
		if(config & LV_VNIDX16){ GX_Normal1x16( vnindex16[i] ); }
		if(config & LV_VC)     { GX_Color3u8(vertcol[i3], vertcol[i3+1], vertcol[i3+2]); }
		if(config & LV_TC)     { int i2 = i*2; GX_TexCoord2f32(texcord[i2], texcord[i2+1]); }
		if(config & LV_TCIDX8) { GX_TexCoord1x8( tcindex8[i] ); }
		if(config & LV_TCIDX16){ GX_TexCoord1x16( tcindex16[i] ); }
	}
}

#endif