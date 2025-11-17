void lightcap(void * outbuf) {
	Mtx capview, capmodel;
	Mtx44 matpersp;

	u16 matcapsize = 128;
	GX_SetViewport(0, 0, matcapsize, matcapsize, 0, 1);
	GX_SetScissor(0, 0, matcapsize, matcapsize);
	GX_SetTexCopySrc(0, 0, matcapsize, matcapsize);
	GX_SetTexCopyDst(matcapsize, matcapsize, GX_TF_RGBA8, GX_FALSE);

	guVector cam = {0.0F, -2.0F, 0.0F},
			up = {0.0F, 0.0F, 1.0F},
		  look = {0.0F, 0.0F, 0.0F};

	guLookAt(capview, &cam, &up, &look);
	guOrtho(matpersp, 1.0f, -1.0f, -1.0f, 1.0f, 0.1f, 50.0f);
	GX_LoadProjectionMtx(matpersp, GX_ORTHOGRAPHIC);

	guMtxIdentity(capmodel);
	guMtxConcat(view, capmodel, capmodel);
	GX_LoadPosMtxImm(capmodel, GX_PNMTX0);
	GX_LoadNrmMtxImm(capmodel, GX_PNMTX0);
	GX_SetCurrentMtx(GX_PNMTX0);

	GX_SetCullMode(GX_CULL_FRONT);

	ag_config_Matcapcone(0);
	ag_draw_Matcapcone(0);

	GX_CopyTex(outbuf, GX_FALSE); //TRUE to clear .. false just looks cool
}