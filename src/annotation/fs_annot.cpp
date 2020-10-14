#include "../../include/fs_annot.h"
#include "../../include/fs_pdfobject.h"

#include "../fxcore.h"
#include "../fs_util.h"
#include "fs_annot_util.h"
#include "fs_annot_imp.h"


void FSShadingColor::set(FS_ARGB firstcolor, FS_ARGB secondcolor)
{
	firstColor = firstcolor;
	secondColor = secondcolor;
}

void FSBorderInfo::set(FS_FLOAT width, FSM_BORDERSTYLE style, FS_FLOAT intensity, FS_FLOAT dashPhase, const FS_FLOAT dashes[16])
{
	this->width = width;
	this->style = style;
	this->cloudIntensity = intensity;
	this->dashPhase = dashPhase;
	FXSYS_memcpy(this->dashes, dashes, 16*sizeof(FS_FLOAT));
}

void FSQuadPoints::set(FSPointF first, FSPointF second, FSPointF third, FSPointF fourth)
{
	this->first = first;
	this->second = second;
	this->third = third;
	this->fourth = fourth;
}
