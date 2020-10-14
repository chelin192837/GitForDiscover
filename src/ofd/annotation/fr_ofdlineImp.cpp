//#include "../../include/fs_pdfpage.h"
#include "fr_ofdannotImp.h"
//#include "fs_annoticonprovider.h"
#include "../fr_ofdpageImp.h"
#include "../../inc/fs_renderImp.h"
#include "../fr_ofddocImp.h"

#include "../../fs_util.h"

FROFDLineImp::FROFDLineImp(COFD_Annot *pAnnotDict, FSPDFPage *page) {
    FROFDMarkupImp::init(pAnnotDict, page);

    createAppearanceStream();
}

FS_BOOL FROFDLineImp::move(FSRectF rect) {
    return TRUE;
}

FSString FROFDLineImp::getLineStartingStyle() const {
    CFX_ByteString startStyle, endStyle;
    getAnnotImp().getLineEndingStyles(startStyle, endStyle);
    FSString retStyle(startStyle.GetBuffer(startStyle.GetLength()), startStyle.GetLength());
    startStyle.ReleaseBuffer();
    return retStyle;
}

void FROFDLineImp::setLineStartingStyle(const char *startingStyle) {
    setLineEndingStyles(startingStyle, getLineEndingStyle().getBuffer());
}

FSString FROFDLineImp::getLineEndingStyle() const {
    CFX_ByteString startStyle, endStyle;
    getAnnotImp().getLineEndingStyles(startStyle, endStyle);
    FSString retStyle(endStyle.GetBuffer(endStyle.GetLength()), endStyle.GetLength());
    endStyle.ReleaseBuffer();
    return retStyle;
}

void FROFDLineImp::setLineEndingStyle(const char *endingStyle) {
    getAnnotImp().setLineEndingStyles(getLineStartingStyle().getBuffer(), endingStyle);
}

FS_ARGB FROFDLineImp::getStyleFillColor() const {
    FS_ARGB color = 0x00000000;
    getFillColor(color);
    return color;
}

void FROFDLineImp::setStyleFillColor(FS_ARGB color) {
    setFillColor(color);
}

FSPointF FROFDLineImp::getStartPoint() const {
    FSPointF result;
    result.x = 0.0f;
    result.y = 0.0f;

    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        return result;
    }

    const COFD_PathObject *objPath = NULL;
    int count = ap->CountObjects();
    for (int i = 0; i < count; ++i) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj || OFD_CONTENTTYPE_Path != obj->GetContentType()) {
            continue;
        }
        objPath = (const COFD_PathObject *) obj;
        break;
    }
    if (!objPath) {
        return result;
    }
    const COFD_Path *path = objPath->GetPath();
    if (path && path->CountPathPoints() > 0) {
        CFX_RectF rect;
        objPath->GetBoundary(rect);

        CFX_RectF bound = m_pAnnotDict->GetBoundary();
        rect.left += bound.left;
        rect.top += bound.top;

        const OFD_PATHPOINT *point = path->GetPathPoint(0);

        result.x = MM_2_PT(point->x + rect.left);
        FS_FLOAT height = m_pFSPDFPage->getHeight();
        result.y = height - MM_2_PT(point->y + rect.top);
    }
    LogOut("FROFDLineImp::getStartPoint, get result: x:%f, y:%f", result.x,
           result.y);

    return result;
}

void FROFDLineImp::setStartPoint(FSPointF point) {
//    CPDF_Array *pArray = CPDF_Array::Create();
//    if (!pArray) return;
//    FSPointF endPoint = getEndPoint();
//
//    pArray->AddNumber(point.x);
//    pArray->AddNumber(point.y);
//    pArray->AddNumber(endPoint.x);
//    pArray->AddNumber(endPoint.y);
//    setArray(FSANNOT_ARRAYKEY_LINEPOINTS, pArray);

    m_startPoint = point;
    LogOut("FROFDLineImp::setStartPoint, get point: x:%f, y:%f", point.x,
           point.y);
//    m_startPoint.x 	= PT_2_MM(point.x);
//    FR_OFDPageImp* page = (FR_OFDPageImp*)m_pFSPDFPage;
//    m_startPoint.y 	= PT_2_MM(page->getHeight() - point.y);
}

FSPointF FROFDLineImp::getEndPoint() const {
    FSPointF result;
    result.x = 0.0f;
    result.y = 0.0f;

    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        return result;
    }

    const COFD_PathObject *objPath = NULL;
    int count = ap->CountObjects();
    for (int i = 0; i < count; ++i) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj || OFD_CONTENTTYPE_Path != obj->GetContentType()) {
            continue;
        }
        objPath = (const COFD_PathObject *) obj;
        break;
    }
    if (!objPath) {
        return result;
    }
    const COFD_Path *path = objPath->GetPath();
    if (path && path->CountPathPoints() > 1) {
        CFX_RectF rect;
        objPath->GetBoundary(rect);

        CFX_RectF bound = m_pAnnotDict->GetBoundary();
        rect.left += bound.left;
        rect.top += bound.top;

        const OFD_PATHPOINT *point = path->GetPathPoint(1);

        result.x = MM_2_PT(point->x + rect.left);
        FS_FLOAT height = m_pFSPDFPage->getHeight();
        result.y = height - MM_2_PT(point->y + rect.top);
    }
    LogOut("FROFDLineImp::getEndPoint, get result: x:%f, y:%f", result.x,
           result.y);

    return result;
}

void FROFDLineImp::setEndPoint(FSPointF point) {
//    CPDF_Array *pArray = CPDF_Array::Create();
//    if (!pArray) return;
//    FSPointF startPoint = getStartPoint();
//
//    pArray->AddNumber(startPoint.x);
//    pArray->AddNumber(startPoint.y);
//    pArray->AddNumber(point.x);
//    pArray->AddNumber(point.y);
//
//    setArray(FSANNOT_ARRAYKEY_LINEPOINTS, pArray);

    m_endPoint = point;
    LogOut("FROFDLineImp::setEndPoint, get point: x:%f, y:%f", point.x,
           point.y);
//    m_endPoint.x 	= PT_2_MM(point.x);
//    FR_OFDPageImp* page = (FR_OFDPageImp*)m_pFSPDFPage;
//    m_endPoint.y 	= PT_2_MM(page->getHeight() - point.y);
}


//FS_ARGB FROFDLineImp::getBorderColor() const {
//
//    FS_ARGB color = 0;
//    if (!m_pAnnotDict)
//        return 0;
//    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
//    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
//    if (!ap) {
//        return 0;
//    }
//    int size = ap->CountObjects();
//    for (int i = 0; i < size; i++) {
//        const COFD_ContentObject *obj = ap->GetContentObject(i);
//        if (!obj) {
//            continue;
//        }
//
//        const COFD_DrawParam *drawParm = obj->GetDrawParam(
//                pPage->GetDocument()->GetPublicResources());
//        if (!drawParm) {
//            continue;
//        }
//
//        const COFD_Color *pColor = NULL;
//
//         pColor = drawParm->GetStrokeColor();
//
//        if (pColor) {
//            color = pColor->GetARGB();
//        }
//    }
//    return color;
//
//};

//void FROFDLineImp::setBorderColor(FS_ARGB color) {
//    m_color = color;
//};


//FS_FLOAT FROFDLineImp::getOpacity() const {
//
//    if (!m_pAnnotDict) {
//        return 1.0f;
//    }
//    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
//    if (!ap) {
//        LogOut("FOV_Annot::GetOpacity, 111 opacity=%f", 1.0f);
//        return 1.0f;
//    }
//
//    FX_DWORD alpha = 255;
//    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
//    int size = ap->CountObjects();
//    for (int i = 0; i < size; ++i) {
//        const COFD_ContentObject *obj = ap->GetContentObject(i);
//        if (!obj) {
//            continue;
//        }
//
//        const COFD_DrawParam *drawParm = obj->GetDrawParam(
//                pPage->GetDocument()->GetPublicResources());
//        if (!drawParm) {
//            continue;
//        }
//        const COFD_Color *strokeColor = drawParm->GetStrokeColor();
//        if (strokeColor) {
//            LogOut("FOV_Annot::GetOpacity, stroke opacity=%f", strokeColor->GetAlpha() / 255.0f);
//            alpha = strokeColor->GetAlpha();
//            break;
//        }
//        const COFD_Color *fillColor = drawParm->GetFillColor();
//        if (fillColor) {
//            LogOut("FOV_Annot::GetOpacity, fill opacity=%f", fillColor->GetAlpha() / 255.0f);
//            alpha = fillColor->GetAlpha();
//            break;
//        }
//    }
//    LogOut("FOV_Annot::GetOpacity, 333 opacity=%f", alpha / 255.0f);
//    return alpha / 255.0f;
//
//};
//
//void FROFDLineImp::setOpacity(FS_FLOAT opacity) {
//    m_opacity = opacity;
//};
//
//FSBorderInfo FROFDLineImp::getBorderInfo() const {
//    m_borderInfo;
//};
//
//void FROFDLineImp::setBorderInfo(FSBorderInfo border) {
//    m_borderInfo = border;
//};

FS_BOOL FROFDLineImp::hasCaption() const {
    if (!hasProperty(FSANNOT_BOOLEANKEY_CAP))
        return FALSE;
    return getBoolean(FSANNOT_BOOLEANKEY_CAP);
}

void FROFDLineImp::enableCaption(FS_BOOL cap) {
    setBoolean(FSANNOT_BOOLEANKEY_CAP, cap);
}

FSString FROFDLineImp::getCaptionPositionType() const {
    //CP is meaningful only when Cap exists and true.
    //BUT CP can exist separated no matter it's meaningful or not.
    return getName(FSANNOT_NAMEKEY_CAPPOSITIONING);
}

void FROFDLineImp::setCaptionPositionType(const char *captionPos) {
    //CP is meaningful only when Cap exists and true.
    //BUT CP can be set separated no matter it will be meaningful or not.
    setName(FSANNOT_NAMEKEY_CAPPOSITIONING, captionPos);
}

FSOffset FROFDLineImp::getCaptionOffset() const {
    FSOffset offset;
    offset.set(0, 0);
    if (!hasProperty(FSANNOT_ARRAYKEY_CAPOFFSET))
        return offset;
    CPDF_Array *pArray = getArray(FSANNOT_ARRAYKEY_CAPOFFSET);
    if (!pArray || pArray->GetCount() != 2)
        return offset;

    offset.x = pArray->GetNumber(0);
    offset.y = pArray->GetNumber(1);
    return offset;
}

void FROFDLineImp::setCaptionOffset(FSOffset offset) {
    CPDF_Array *pArray = CPDF_Array::Create();
    if (!pArray)
        return;
    pArray->AddNumber(offset.x);
    pArray->AddNumber(offset.y);
    setArray(FSANNOT_ARRAYKEY_CAPOFFSET, pArray);
}

FS_BOOL FROFDLineImp::createAppearanceStream() {
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (NULL == ap) {
        CFX_RectF boundary;
        boundary.Set(0.0, 0.0, 0.0, 0.0);
        boundary.Inflate(5.0f, 5.0f);

        COFD_Path path;
        path.CreatePath();
        path.MoveTo(0.0, 0.0);
        path.LineTo(0.0, 0.0);

        IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
        IOFD_Document *doc = page->GetDocument();
        IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
        COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();
        m_pWriteAnnot = OFD_WriteAnnot_Create(ofdWriteDoc, OFD_ANNOTTYPE_Path, pAnnot);
        COFD_WriteAnnot *ofdWriteAnnot = m_pWriteAnnot;

        COFD_WriteBaseColor *pwriteColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create( OFD_COLORTYPE_Base);
        pwriteColor->SetAlpha(255);
        pwriteColor->SetColorValue(0xffff0000);

        COFD_WriteDrawParam *pdrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
        pdrawParam->SetStrokeColor(pwriteColor);
        pdrawParam->SetLineWidth(1.0f);

        COFD_WritePathObject *writePathObj = (COFD_WritePathObject *) OFD_WriteContentObject_Create( ofdWriteDoc, OFD_CONTENTTYPE_Path);

        CFX_RectF annotRect;
        annotRect.Set(0.0, 0.0, boundary.width, boundary.height);

        writePathObj->SetDrawParam(pdrawParam);
        writePathObj->SetPath(path);
        writePathObj->SetBoundary(annotRect);
        writePathObj->SetStrokeState(TRUE);

        COFD_WriteBlockObject *pAnnotAp = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create( ofdWriteDoc, OFD_CONTENTTYPE_Block);
        pAnnotAp->InsertObject(writePathObj);

        ofdWriteAnnot->SetBoundary(boundary);

        ofdWriteAnnot->SetAppearance(pAnnotAp);
    } else{
        m_startPoint=getStartPoint();
        m_endPoint=getEndPoint();
    }

    return TRUE;
}

//////////////
// Methods for resetAp
//void		FSAnnot_Util_GetUserSpaceLength(long lSize, FX_FLOAT& fSize, FX_FLOAT fUserUnit)
//{
//    int x = 96;
//    int y = 96;
//    fSize = (FX_FLOAT)lSize / (FX_FLOAT)FX_MAX(x, y) / fUserUnit;
//}
//
//void		FSAnnot_Util_GetUserSpaceLength(long xSize, long ySize, FX_FLOAT& fxSize, FX_FLOAT& fySize, FX_FLOAT fUserUnit)
//{
//    int x = 96;
//    int y = 96;
//
//    fxSize = (FX_FLOAT)xSize / (FX_FLOAT)x / fUserUnit;
//    fySize = (FX_FLOAT)ySize / (FX_FLOAT)y / fUserUnit;
//
//}
//
//CPDF_Rect	FSAnnot_Util_RotateRectangle(CPDF_Rect crRect, FX_FLOAT fRotateAngle)
//{
//    CPDF_Point centerPoint((crRect.left + crRect.right) / 2.0f, (crRect.bottom + crRect.top) / 2.0f);
//    CPDF_Point point0(0.0f,0.0f),point1(0.0f,0.0f),point2(0.0f,0.0f),point3(0.0f,0.0f);
//    point0 = CPDF_Point(crRect.left, crRect.top);
//    point1 = CPDF_Point(crRect.left, crRect.bottom);
//    point2 = CPDF_Point(crRect.right, crRect.bottom);
//    point3 = CPDF_Point(crRect.right, crRect.top);
//    CPDF_Point v(0.0f,0.0f);
//    CPDF_Rect newRect;
//    newRect.left = newRect.right = centerPoint.x;
//    newRect.bottom = newRect.top = centerPoint.y;
//
//    for (int i = 0; i < 4; i ++)
//    {
//
//        CPDF_Point tempPoint = CPDF_Point(0.0f,0.0f);
//        if(i == 0) tempPoint = point0;
//        else if(i == 1) tempPoint = point1;
//        else if(i == 2) tempPoint = point2;
//        else tempPoint = point3;
//
//        v.x = tempPoint.x - centerPoint.x; v.y = tempPoint.y - centerPoint.y;
//        v = FSAnnot_Util_Rotate(v,fRotateAngle);
//        tempPoint.x = v.x + centerPoint.x; tempPoint.y = v.y + centerPoint.y;
//
//        if (newRect.left > tempPoint.x) newRect.left = tempPoint.x;
//        if (newRect.bottom > tempPoint.y) newRect.bottom = tempPoint.y;
//        if (newRect.right < tempPoint.x) newRect.right = tempPoint.x;
//        if (newRect.top < tempPoint.y) newRect.top = tempPoint.y;
//    }
//
//    return newRect;
//}

//////////////
// resetAp
FS_BOOL FROFDLineImp::resetAppearanceStream() {
    CFX_RectF rect = m_pAnnotDict->GetBoundary();
    LogOut("FROFDLineImp::resetAppearanceStream, get rect: l:%f, t:%f, w:%f, h:%f", rect.left,
           rect.top, rect.width, rect.height);

    FSPointF start, end;
    FSPointF startPoint  = getStartPoint();
    FSPointF endPoint= getEndPoint();
    start.x = PT_2_MM(m_startPoint.x);
    start.y = PT_2_MM(m_pFSPDFPage->getHeight() - m_startPoint.y);
    end.x = PT_2_MM(m_endPoint.x);
    end.y = PT_2_MM(m_pFSPDFPage->getHeight() - m_endPoint.y);
    LogOut("FROFDLineImp::resetAppearanceStream, get m_startPoint: x:%f, y:%f", m_startPoint.x,
           m_startPoint.y);
    LogOut("FROFDLineImp::resetAppearanceStream, get m_endPoint: x:%f, y:%f", m_endPoint.x,
           m_endPoint.y);
    LogOut("FROFDLineImp::resetAppearanceStream, get start: x:%f, y:%f", start.x,
           start.y);
    LogOut("FROFDLineImp::resetAppearanceStream, get end: x:%f, y:%f", end.x,
           end.y);


    CFX_RectF boundary;
    boundary.Set(FX_MIN(start.x, end.x), FX_MIN(start.y, end.y), FXSYS_fabs(start.x - end.x),
                 FXSYS_fabs(start.y - end.y));
    boundary.Inflate(5.0f, 5.0f);
    LogOut("FROFDLineImp::resetAppearanceStream, get boundary1: l:%f, t:%f, w:%f, h:%f",
           boundary.left,
           boundary.top, boundary.width, boundary.height);

    COFD_Path path;
    path.CreatePath();
    path.MoveTo(start.x - boundary.left, start.y - boundary.top);
    path.LineTo(end.x - boundary.left, end.y - boundary.top);

    IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
    IOFD_Document *doc = page->GetDocument();
    IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
    COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();
    m_pWriteAnnot = OFD_WriteAnnot_Create(ofdWriteDoc, OFD_ANNOTTYPE_Path, pAnnot);
    COFD_WriteAnnot *ofdWriteAnnot = m_pWriteAnnot;

//    COFD_BlockObject *pReadAp = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
//    if (NULL == pReadAp) {
//    } else {
//    }

    COFD_WriteDrawParam *pdrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(ofdWriteDoc,
                                                                                       OFD_RESOURCETYPE_DrawParam);

    COFD_WriteBaseColor *pwriteColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
            OFD_COLORTYPE_Base);
//    pwriteColor->SetAlpha(255);
//    pwriteColor->SetColorValue(0xffff0000);
//
//    pdrawParam->SetStrokeColor(pwriteColor);
//    pdrawParam->SetLineWidth(1.0f);
    FX_BYTE m_Opacity = static_cast<FX_BYTE>(getOpacity() * 255 + 0.5f);
    LogOut("FROFDLineImp::resetAppearanceStream,m_Opacity: %f ", getOpacity());
    pwriteColor->SetAlpha(m_Opacity);
    FX_DWORD m_Color = getBorderColor();
    pwriteColor->SetColorValue(m_Color);
    LogOut("FROFDLineImp::resetAppearanceStream,getBorderColor: %ld", getBorderColor());

    pdrawParam->SetStrokeColor(pwriteColor);
//    pdrawParam->SetLineWidth(m_borderInfo.width);
    FSBorderInfo borderInfo= getBorderInfo();
    FS_FLOAT  borderWidth= getBorderWidth();
    pdrawParam->SetLineWidth(PT_2_MM(borderInfo.width));
    LogOut("FROFDLineImp::resetAppearanceStream,borderInfo.width: %f", borderInfo.width);
    LogOut("FROFDLineImp::resetAppearanceStream,borderWidth: %f",borderWidth);

    COFD_WritePathObject *writePathObj = (COFD_WritePathObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Path);

//    //by  20170225 begin
//    m_WriteAnnot->SetBoundary(annotRect);
//    annotRect.left = 0.0f;
//    annotRect.top = 0.0f;
//    //by  20170225 end
    CFX_RectF annotRect;
    annotRect.Set(0.0, 0.0, boundary.width, boundary.height);

    writePathObj->SetDrawParam(pdrawParam);
    writePathObj->SetPath(path);
    writePathObj->SetBoundary(annotRect);
    writePathObj->SetStrokeState(TRUE);
    COFD_WriteBlockObject *pAnnotAp = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Block);
    pAnnotAp->InsertObject(writePathObj);

    ofdWriteAnnot->SetBoundary(boundary);
    LogOut("FROFDLineImp::resetAppearanceStream, get boundary2: l:%f, t:%f, w:%f, h:%f",
           boundary.left,
           boundary.top, boundary.width, boundary.height);

    ofdWriteAnnot->SetAppearance(pAnnotAp);

    return TRUE;
}

