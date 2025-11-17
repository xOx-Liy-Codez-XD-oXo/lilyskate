#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <math.h>
#include <gccore.h>
#include <wiiuse/wpad.h>
#include <asndlib.h>

#include "globals.h"
#include "rendermgr.h"
#include "puff.h"
#include "graphicsutililiys.h"
#include "init.h"
#include "primitives.h"
#include "liyt.h"
#include "oggplayer.h"

#include "data/mdl/lily_liym3q.h"
#include "data/mdl/sign_liym3q.h"
#include "data/mdl/skateboar_liym3q.h"
#include "data/mdl/skatepark_liym3q.h"
#include "data/mdl/wheels_liym3q.h"
#include "data/mdl/gameoverwhite_liym3q.h"
#include "data/mdl/gameoverblack_liym3q.h"
#include "data/mdl/pushoff_liym3q.h"

#include "concrete.h"
#include "concrete_tpl.h"
#include "skateparkshadowmap_doodle.h"
#include "camera_liyabc.h"
#include "propanim_liyabc.h"
#include "lilytex.h"
#include "lilytex_tpl.h"
#include "game_ogg.h"
#include "title_ogg.h"
#include "titlecard.h"
#include "titlecard_tpl.h"

int main(int argc, char **argv) {
	GXColor background = {0, 0, 0, 0xff};
	TPLFile crateTPL;

	inits();

	SYS_STDIO_Report(true);
	CON_EnableGecko(1, 0);

	GX_SetCullMode(GX_CULL_NONE);

	liy_VtxDescConfig(LV_VP | LV_VN | LV_VC, 0);

	GX_InvalidateTexAll();

	if(widescreen) {
		guPerspective(perspective, 20.0f, aspect, 0.1F, 300.0F);
		guOrtho(overlaypersp, 1.0f, -1.0f, -1.0f, 1.0f, 0.1f, 50.0f);
	} else {
		guPerspective(perspective, 23.0f, aspect, 0.1F, 300.0F);
		guOrtho(overlaypersp, 1.0f, -1.0f, -0.8f, 0.8f, 0.1f, 50.0f);
	}

	VIDEO_SetBlack(false);

	GXTexObj concreteTexObj, skateparkshadowmapTexObj, lilytexTexObj, titlecardTexObj;
	TPLFile tplfileFromMem;
	liy_unpackDoodle(skateparkshadowmap, 1024, 1024);
	liy_unpackTPL(concrete);
	liy_unpackTPL(lilytex);
	liy_unpackTPL(titlecard);

	liy_unpack_liyabc(camera, 6868);
	liy_unpack_liyabc(propanim, 6868);

	GX_LoadTexObj(&skateparkshadowmapTexObj, GX_TEXMAP0);
	GX_LoadTexObj(&concreteTexObj, GX_TEXMAP1);
	GX_LoadTexObj(&lilytexTexObj, GX_TEXMAP2);
	GX_LoadTexObj(&titlecardTexObj, GX_TEXMAP3);

	u8 animationadvance = 0;
	u32 aframe = 0;
	u8 allowStart = 0;
	u8 displayGameOverandAllowReset = 0;
	u8 flip = 0;
	u8 flipctr = 0;
	u8 ontitlecard = 1;
	#define FLIPCTR_MAX 20

	WPAD_SetDataFormat(WPAD_CHAN_ALL, WPAD_FMT_BTNS_ACC);

	PlayOgg(title_ogg, title_ogg_size, 0, OGG_INFINITE_TIME);

	while(1) {
		WPAD_ScanPads();
		PAD_ScanPads();
		int wpaddown = WPAD_ButtonsDown(0);
		int paddown = PAD_ButtonsDown(0);
		if(wpaddown & WPAD_BUTTON_HOME) shouldquit = 1;
		if(paddown & PAD_BUTTON_START && !ontitlecard) shouldquit = 1;
		if(paddown & PAD_TRIGGER_R) flip |= 1;
		if((wpaddown & WPAD_BUTTON_A || paddown & PAD_BUTTON_A) && allowStart) {
			animationadvance = 1;
			allowStart = 0;
			PlayOgg(game_ogg, game_ogg_size, 0, OGG_ONE_TIME);
		} 
		if((wpaddown & WPAD_BUTTON_B || paddown & PAD_BUTTON_B) && displayGameOverandAllowReset) {
			allowStart = 1;
			aframe = 0;
			flip = 0;
			flipctr = 0;
			animationadvance = 0;
			StopOgg();
		}
		if(shouldquit) break;
		struct gforce_t gforce;
		WPAD_GForce(0, &gforce);
		if(gforce.z > 3.0f) flip |= 1;
		if(aframe > 262) flip &= ~1;

		if(flip & 2) { flip = 2; flipctr++; }
		if(flip & 1) flip = 3;
		if(flipctr == FLIPCTR_MAX) {flip = 0; flipctr = 0;}

		if(((wpaddown & WPAD_BUTTON_PLUS) || (paddown & PAD_BUTTON_START)) && ontitlecard) {
			StopOgg();
			ontitlecard = 0;
			allowStart = 1;
		}


		float * camframe;
		if(aframe <= 285) {
			camframe = &cameraData[aframe * cameraPrimcount];
		} else {
			camframe = &cameraData[285 * cameraPrimcount];
		}
		liyt_genMtxPosRotZyx(view, camframe[0], camframe[1], camframe[2], 
		                           camframe[3], camframe[4], camframe[5]);
		guMtxInverse(view, view);

		GXLightObj lobj;
		guVector lpos = {457.1f, -558.3f, 670.1f};

		guVecMultiply(view, &lpos, &lpos);
		GX_InitLightPos(&lobj, lpos.x, lpos.y, lpos.z);
		GX_InitLightColor(&lobj, LC_WHITE);
		GX_LoadLightObj(&lobj, GX_LIGHT0);
		GX_SetNumChans(1);
		GX_SetChanAmbColor(GX_COLOR0A0, LC_SKY);
		GX_SetChanMatColor(GX_COLOR0A0, LC_RED);
		GX_SetChanCtrl(GX_COLOR0A0, GX_ENABLE, GX_SRC_REG, GX_SRC_REG, GX_LIGHT0, GX_DF_CLAMP, GX_AF_NONE);

		GX_LoadProjectionMtx(perspective, GX_PERSPECTIVE);
		
		GX_SetNumTevStages(1);
		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP2, GX_COLOR0A0);

		if(ontitlecard) {
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
			GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP3, GX_COLOR0A0);

			GX_LoadProjectionMtx(overlaypersp, GX_ORTHOGRAPHIC);

			GX_LoadPosMtxImm(fullQuadMv, GX_PNMTX0);
			liy_VtxDescConfig(LV_VP | LV_TC, 0);
			GX_Begin(GX_QUADS, GX_VTXFMT0, 4);
				GX_Position3f32(-1.0f, 1.0f, 0.0f);
				GX_TexCoord2f32(0.0f, 1.0f);	
				GX_Position3f32(1.0f, 1.0f, 0.0f);
				GX_TexCoord2f32(1.0f, 1.0f);
				GX_Position3f32(1.0f, -1.0f, 0.0f);
				GX_TexCoord2f32(1.0f, 0.0f);
				GX_Position3f32(-1.0f, -1.0f, 0.0f);
				GX_TexCoord2f32(0.0f, 0.0f);
		} else {

		if(aframe <= 285) {
			float * skateboardframe = &propanimData[aframe * propanimPrimcount];
			liyt_genMtxPosRotZyx(model, skateboardframe[0], skateboardframe[1], skateboardframe[2],
			                                 skateboardframe[3], skateboardframe[4], skateboardframe[5]);
			Mtx model2, model3;
			guMtxCopy(model, model2);
			guMtxRotRad(model3, 'y', (6.28f * ((float)flipctr / (float)FLIPCTR_MAX)));
			guMtxConcat(model3, model, model);
			guMtxTransApply(model, model, 0.0f, 0.0f, (0.2f * sqrt(fabs(sin(3.14f * ((float)flipctr / (float)FLIPCTR_MAX))))));
			guMtxConcat(view, model, model);
			GX_LoadPosMtxImm(model, GX_PNMTX0);
			GX_LoadNrmMtxImm(model, GX_PNMTX0);

			ag_config_skateboar(0);
			ag_draw_skateboar(0);
			GX_SetChanMatColor(GX_COLOR0A0, LC_DARKDARK);
			ag_config_wheels(0);
			ag_draw_wheels(0);

			GX_SetBlendMode(GX_BM_NONE, GX_BL_ONE, GX_BL_ZERO, GX_LO_NOOP);
			GX_SetAlphaCompare(GX_GEQUAL, 192, GX_AOP_AND, GX_ALWAYS, 0);
			GX_SetZCompLoc(GX_FALSE);

			GX_SetChanMatColor(GX_COLOR0A0, LC_WHITE);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
			guMtxTransApply(model2, model2, 0.0f, 0.0f, 0.3f * sqrt(fabs(sin(3.14f * ((float)flipctr / (float)FLIPCTR_MAX)))));
			guMtxConcat(view, model2, model2);
			GX_LoadPosMtxImm(model2, GX_PNMTX0);
			GX_LoadNrmMtxImm(model2, GX_PNMTX0);
			ag_config_lily(0);
			ag_draw_lily(0);
			
			GX_SetAlphaCompare(GX_ALWAYS, 0, GX_AOP_AND, GX_ALWAYS, 0);
			GX_SetZCompLoc(GX_TRUE);
			GX_SetBlendMode(GX_BM_NONE, GX_BL_SRCALPHA, GX_BL_INVSRCALPHA, GX_LO_CLEAR);
		}

		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ONE, GX_CC_RASC, GX_CC_ZERO);
		GX_SetChanCtrl(GX_COLOR0A0, GX_DISABLE, GX_SRC_REG, GX_SRC_VTX, GX_LIGHTNULL, GX_DF_CLAMP, GX_AF_NONE);

		guMtxIdentity(model);
		guMtxConcat(view, model, model);
		GX_LoadPosMtxImm(model, GX_PNMTX0);
		ag_config_sign(0);
		ag_draw_sign(0);

		if(displayGameOverandAllowReset) {
			GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
			if(widescreen) {
				guMtxScaleApply(fullQuadMv, model, 0.012f, 0.05f, 0.1f);
			} else {
				guMtxScaleApply(fullQuadMv, model, 0.011f, 0.055f, 0.1f);
			}
			guMtxTransApply(model, model, 0.0f, -0.0095f, 0.0f);
			GX_LoadPosMtxImm(model, GX_PNMTX0);
			ag_config_gameoverblack(0);
			ag_draw_gameoverblack(0);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ONE);
			ag_draw_gameoverwhite(0);
			GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		}

		if(aframe == 0) {
			GX_SetZMode(GX_TRUE, GX_ALWAYS, GX_TRUE);
			GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO, GX_CC_ZERO);
			guMtxScaleApply(fullQuadMv, model, 0.004f, 0.004f, 0.1f);
			guMtxTransApply(model, model, 0.0f, -0.01f, 0.0f);
			GX_LoadPosMtxImm(model, GX_PNMTX0);
			ag_config_pushoff(0);
			ag_draw_pushoff(0);
			GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		}

		GX_SetTevKColorSel(GX_TEVSTAGE0, GX_TEV_KCSEL_K0);
		GX_SetTevKColor(GX_KCOLOR0, LC_SKY);
		GX_SetNumTevStages(3);
		GX_SetTevColorIn(GX_TEVSTAGE0, GX_CC_KONST, GX_CC_ONE, GX_CC_TEXC, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE0, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG0);
		GX_SetTevColorIn(GX_TEVSTAGE1, GX_CC_ZERO, GX_CC_RASC, GX_CC_TEXC, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE1, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVREG1);
		GX_SetTevColorIn(GX_TEVSTAGE2, GX_CC_ZERO, GX_CC_C0, GX_CC_C1, GX_CC_ZERO);
		GX_SetTevColorOp(GX_TEVSTAGE2, GX_TEV_ADD, GX_TB_ZERO, GX_CS_SCALE_1, GX_TRUE, GX_TEVPREV);
		GX_SetTevOrder(GX_TEVSTAGE0, GX_TEXCOORD0, GX_TEXMAP0, GX_COLOR0A0);
		GX_SetTevOrder(GX_TEVSTAGE1, GX_TEXCOORD1, GX_TEXMAP1, GX_COLOR0A0);
		GX_SetTevOrder(GX_TEVSTAGE2, GX_TEXCOORDNULL, GX_TEXMAP_NULL, GX_COLOR0A0);

		guMtxScale(model, 10.0f, 10.0f, 10.0f);
		GX_LoadTexMtxImm(model, GX_TEXMTX0, GX_MTX2x4);

		GX_SetTexCoordGen(GX_TEXCOORD0, GX_TG_MTX2x4, GX_TG_TEX0, GX_IDENTITY);
		GX_SetTexCoordGen(GX_TEXCOORD1, GX_TG_MTX2x4, GX_TG_TEX0, GX_TEXMTX0);
		GX_SetNumTexGens(2);

		guMtxIdentity(model);
		guMtxConcat(view, model, model);
		GX_LoadPosMtxImm(model, GX_PNMTX0);
		GX_LoadNrmMtxImm(model, GX_PNMTX0);
		GX_SetCurrentMtx(GX_PNMTX0);
		ag_config_skatepark(0);
		ag_draw_skatepark(0);
	
		}
		
		if(aframe > 341) {
			displayGameOverandAllowReset = 1;
		} else {
			displayGameOverandAllowReset = 0;
		}



		GX_SetZMode(GX_TRUE, GX_LEQUAL, GX_TRUE);
		GX_SetColorUpdate(GX_TRUE);
		GX_CopyDisp(xfb, GX_TRUE);

		GX_DrawDone();

		VIDEO_SetNextFramebuffer(xfb);
		
		if(animationadvance && aframe < 864) aframe++;

		rframe++;
		VIDEO_Flush();
		VIDEO_WaitVSync();
	}
}
