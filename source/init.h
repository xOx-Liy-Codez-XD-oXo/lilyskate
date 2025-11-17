void quitplease() {
	shouldquit = 1;
}

void inits() {
	fullQuadMv[0][0] = 1.0f;
	fullQuadMv[1][0] = 0.0f;
	fullQuadMv[2][0] = 0.0f;

	fullQuadMv[0][1] = 0.0f;
	fullQuadMv[1][1] = 1.0f;
	fullQuadMv[2][1] = 0.0f;
	
	fullQuadMv[0][2] = 0.0f;
	fullQuadMv[1][2] = 0.0f;
	fullQuadMv[2][2] = 1.0f;

	fullQuadMv[0][3] = 0.0f;
	fullQuadMv[1][3] = 0.0f;
	fullQuadMv[2][3] = -1.0f;

	VIDEO_Init();
	WPAD_Init();
	PAD_Init();
	ASND_Init();

	SYS_SetPowerCallback(quitplease);

	#define DEFAULT_FIFO_SIZE 256*1024
	gpfifo = memalign(32, DEFAULT_FIFO_SIZE);
	memset(gpfifo, 0, DEFAULT_FIFO_SIZE);

	rmode = VIDEO_GetPreferredMode(NULL);
	xfb = SYS_AllocateFramebuffer(rmode);
	rmode->viWidth = 704;
	rmode->viXOrigin = 8;
	VIDEO_Configure(rmode);
	VIDEO_SetNextFramebuffer(xfb);
	VIDEO_Flush();
	VIDEO_WaitVSync();
	if(rmode->viTVMode&VI_NON_INTERLACE) VIDEO_WaitVSync();

	GX_Init(gpfifo, DEFAULT_FIFO_SIZE);
	GX_SetCopyClear(LC_SKY, 0x00ffffff);

	fbwidth = rmode->fbWidth;
	fbheight = rmode->efbHeight;

	resetViewport();
	GX_SetCopyFilter(GX_FALSE, NULL, GX_FALSE, NULL);
	GX_SetFieldMode(rmode->field_rendering,((rmode->viHeight==2*rmode->xfbHeight)?GX_ENABLE:GX_DISABLE));

	GX_SetPixelFmt(GX_PF_RGB8_Z24, GX_ZC_LINEAR);
	GX_SetDispCopyGamma(GX_GM_1_0);
	GX_CopyDisp(xfb, GX_TRUE);

	if (CONF_GetAspectRatio() == CONF_ASPECT_16_9) {
		aspect = 16.0f/9.0f;
		widescreen = 1;
	} else {
		aspect = 4.0f/3.0f;
	}
}