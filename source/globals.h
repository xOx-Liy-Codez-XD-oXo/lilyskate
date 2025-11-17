u16 fbwidth, fbheight;
u32 rframe = 0;
u8 shouldquit = 0;

static GXRModeObj *rmode = NULL;
static void *xfb = NULL;
void *gpfifo = NULL;

float aspect;
u8 widescreen;

Mtx view, model, viewreflec;
Mtx44 perspective, overlaypersp; 

Mtx fullQuadMv;