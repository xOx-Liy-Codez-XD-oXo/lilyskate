#ifndef LIYT_H
#define LIYT_H

void liyt_genMtxPosRot(Mtx out, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot) {
	Mtx xrotmtx, yrotmtx, zrotmtx;
	guMtxRotRad(xrotmtx, 'x', xrot);
	guMtxRotRad(yrotmtx, 'y', yrot);
	guMtxRotRad(zrotmtx, 'z', zrot);
	guMtxConcat(xrotmtx, yrotmtx, yrotmtx);
	guMtxConcat(yrotmtx, zrotmtx, zrotmtx);
	guMtxTransApply(zrotmtx, out, xpos, ypos, zpos);
	return;
}

void liyt_genMtxPosRotZyx(Mtx out, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot) {
	Mtx xrotmtx, yrotmtx, zrotmtx;
	guMtxRotRad(xrotmtx, 'x', xrot);
	guMtxRotRad(yrotmtx, 'y', yrot);
	guMtxRotRad(zrotmtx, 'z', zrot);
	guMtxConcat(zrotmtx, yrotmtx, yrotmtx);
	guMtxConcat(yrotmtx, xrotmtx, xrotmtx);
	guMtxTransApply(xrotmtx, out, xpos, ypos, zpos);
	return;
}

void liyt_genMtxPosRotZyxScale(Mtx out, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot, float xscl, float yscl, float zscl) {
	Mtx xrotmtx, yrotmtx, zrotmtx;
	guMtxRotRad(xrotmtx, 'x', xrot);
	guMtxRotRad(yrotmtx, 'y', yrot);
	guMtxRotRad(zrotmtx, 'z', zrot);
	guMtxConcat(zrotmtx, yrotmtx, yrotmtx);
	guMtxConcat(yrotmtx, xrotmtx, xrotmtx);
	guMtxScaleApply(xrotmtx, xrotmtx, xscl, yscl, zscl);
	guMtxTransApply(xrotmtx, out, xpos, ypos, zpos);
	return;
}

void guMtxCopyNopos(const Mtx src,Mtx dst) {
	if(src==dst) return;

    dst[0][0] = src[0][0];    dst[0][1] = src[0][1];    dst[0][2] = src[0][2];    dst[0][3] = 0.0f;
    dst[1][0] = src[1][0];    dst[1][1] = src[1][1];    dst[1][2] = src[1][2];    dst[1][3] = 0.0f;
    dst[2][0] = src[2][0];    dst[2][1] = src[2][1];    dst[2][2] = src[2][2];    dst[2][3] = 0.0f;
}

#endif