//#include "../../include/fs_pdfpage.h"
#include "fr_ofdannotImp.h"
//#include "fs_annoticonprovider.h"
#include "../fr_ofdpageImp.h"
#include "../../inc/fs_renderImp.h"
#include "../fr_ofddocImp.h"

#include "../../fs_util.h"

enum FLIP_TYPE {
    FLIP_NO = 0,
    FLIP_UP_DOWN,
    FLEP_LEFT_RIGHT,
};

FROFDInkImp::FROFDInkImp(COFD_Annot *pAnnotDict, FSPDFPage *page) {
    FROFDMarkupImp::init(pAnnotDict, page);

    m_inkList = NULL;
    createAppearanceStream();
}

FS_BOOL FROFDInkImp::createAppearanceStream() {
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
//    CFX_RectF boundary = m_pAnnotDict->GetBoundary();
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
    } else {
        m_inkList = getInkList();
    }

    return TRUE;
}

FS_BOOL FROFDInkImp::move(FSRectF rect)
{
/*    FSRectF ofdRect;
    ofdRect.left 	= PT_2_MM(rect.left);
    ofdRect.right 	= PT_2_MM(rect.right);
    ofdRect.top 	= PT_2_MM(m_pFSPDFPage->getHeight() - rect.top);
    ofdRect.bottom = PT_2_MM(m_pFSPDFPage->getHeight() - rect.bottom);

    FS_FLOAT ofdWidth = ofdRect.right - ofdRect.left;
    FS_FLOAT ofdHeight = ofdRect.bottom - ofdRect.top;
    LogOut("DSS - move boundary old: l:%f, t:%f, w:%f, h:%f", ofdRect.left, ofdRect.top, ofdWidth, ofdHeight);

    CFX_RectF boundary;
    boundary.Set(ofdRect.left, ofdRect.top, ofdWidth, ofdHeight);
    LogOut("DSS - move boundary new: l:%f, t:%f, w:%f, h:%f", boundary.left, boundary.top, boundary.width, boundary.height);
    // 两行LogOut完全一致，只只有平移、缩放，没有翻转

    FR_OFDPageImp* page = (FR_OFDPageImp*)m_pFSPDFPage;
    IOFD_Document* doc = page->getPage()->GetDocument();
    IOFD_WriteDocument* writeDoc = OFD_WriteDoc_Create(doc);

    COFD_Annot* pAnnot = m_pWriteAnnot->GetReadAnnot();
    m_pWriteAnnot = OFD_WriteAnnot_Create(writeDoc, OFD_ANNOTTYPE_Path, pAnnot);

    m_pWriteAnnot->SetBoundary(boundary);
*/
    if (NULL == m_inkList)
        return FALSE;

    FSPDFPath* pNewInkList = FSPDFPath::create();
    if (!pNewInkList)
        return FALSE;

/*    FSRectF pointRect;
    pointRect.left = m_pFSPDFPage->getWidth();
    pointRect.right = 0;
    pointRect.top = 0;
    pointRect.bottom = m_pFSPDFPage->getHeight();

    FS_INT32 pointCount = m_inkList->getPointCount();

    for(FS_INT32 i=0; i < pointCount; i++) {
        FSPointF pt = m_inkList->getPoint(i);

        pointRect.left = FX_MIN(pointRect.left, pt.x);
        pointRect.right = FX_MAX(pointRect.right, pt.x);

        pointRect.top = FX_MAX(pointRect.top, pt.y);
        pointRect.bottom = FX_MIN(pointRect.bottom, pt.y);
    }
*/
    CFX_RectF boundary = m_pAnnotDict->GetBoundary();
    FS_FLOAT heightPT = m_pFSPDFPage->getHeight();

    FSRectF pointRect;
    pointRect.left = MM_2_PT(boundary.left);
    pointRect.right = MM_2_PT(boundary.right());
    pointRect.top = heightPT - MM_2_PT(boundary.top);
    pointRect.bottom = heightPT - MM_2_PT(boundary.bottom());

    FS_FLOAT pointRectW = pointRect.right - pointRect.left;
    FS_FLOAT pointRectH = pointRect.top - pointRect.bottom;
    FS_FLOAT rectW = rect.right - rect.left;
    FS_FLOAT rectH = rect.top - rect.bottom;

    FS_INT32 pointCount = m_inkList->getPointCount();
    for(FS_INT32 j=0; j < pointCount; j++)
    {
        FSPointF point = m_inkList->getPoint(j);
        FSPointF pointNew;
        pointNew.x = (point.x-pointRect.left) * rectW / pointRectW + rect.left;
        pointNew.y = (point.y-pointRect.bottom) * rectH / pointRectH + rect.bottom;

        FS_PATHPOINTTYPE iFxPtType = m_inkList->getPointType(j);
        if (iFxPtType == FSCommonDefines::e_pointTypeMoveTo)
            pNewInkList->moveTo(pointNew);
        else
            pNewInkList->lineTo(pointNew);
    }

    if(pNewInkList->getPointCount()==0)
    {
        pNewInkList->release();
        pNewInkList = NULL;
        return FALSE;
    }

    m_inkList->release();
    m_inkList = pNewInkList;

    FSRectF ofdRect;
    ofdRect.left 	= PT_2_MM(rect.left);
    ofdRect.right 	= PT_2_MM(rect.right);
    ofdRect.top 	= PT_2_MM(m_pFSPDFPage->getHeight() - rect.top);
    ofdRect.bottom = PT_2_MM(m_pFSPDFPage->getHeight() - rect.bottom);

    boundary.Set(ofdRect.left, ofdRect.right, (ofdRect.right-ofdRect.left), (ofdRect.bottom-ofdRect.top));

    FR_OFDPageImp* page = (FR_OFDPageImp*)m_pFSPDFPage;
    IOFD_Document* doc = page->getPage()->GetDocument();
    IOFD_WriteDocument* writeDoc = OFD_WriteDoc_Create(doc);

    COFD_Annot* pAnnot = m_pWriteAnnot->GetReadAnnot();
    m_pWriteAnnot = OFD_WriteAnnot_Create(writeDoc, OFD_ANNOTTYPE_Path, pAnnot);

    m_pWriteAnnot->SetBoundary(boundary);

    resetAppearanceStream();    // ???
    // 没移动一次放大一次，需要知道Java端的Rect如何计算的

    return TRUE;
}

FS_BOOL FROFDInkImp::resetAppearanceStream() {
    if (NULL == m_inkList)
        return FALSE;

    FS_INT32 pointCount = m_inkList->getPointCount();
    if (pointCount < 1)
        return FALSE;

    FS_FLOAT widthMM = PT_2_MM(m_pFSPDFPage->getWidth());
    FS_FLOAT heightMM = PT_2_MM(m_pFSPDFPage->getHeight());
    FS_FLOAT rtL = widthMM;
    FS_FLOAT rtR = 0.0;
    FS_FLOAT rtT = heightMM;
    FS_FLOAT rtB = 0.0;

    for(FS_INT32 i=0; i < pointCount; i++)
    {
        FSPointF point = m_inkList->getPoint(i);

        FSPointF ptMM;
        ptMM.x = PT_2_MM(point.x);
        ptMM.y = heightMM - PT_2_MM(point.y);

        rtL = min(rtL, ptMM.x);
        rtR = max(rtR, ptMM.x);
        rtT = min(rtT, ptMM.y);
        rtB = max(rtB, ptMM.y);
    }
    CFX_RectF boundary;
    boundary.Set(rtL, rtT, rtR-rtL, rtB-rtT);
    boundary.Inflate(5.0f, 5.0f);
//
//    CFX_RectF boundary = m_pAnnotDict->GetBoundary();
//
//    FS_FLOAT heightMM = PT_2_MM(m_pFSPDFPage->getHeight());

    COFD_Path path;
    path.CreatePath();
    for(FS_INT32 j=0; j < pointCount; j++)
    {
        FSPointF point = m_inkList->getPoint(j);

        FSPointF ptMM;
        ptMM.x = PT_2_MM(point.x);
        ptMM.y = heightMM - PT_2_MM(point.y);

        FS_PATHPOINTTYPE iFxPtType = m_inkList->getPointType(j);
        if (FSCommonDefines::e_pointTypeMoveTo == iFxPtType)
            path.MoveTo(ptMM.x-boundary.left, ptMM.y-boundary.top);
        else
            path.LineTo(ptMM.x-boundary.left, ptMM.y-boundary.top);
    }

    FX_DWORD color = getBorderColor();
    FX_BYTE opacity = static_cast<FX_BYTE>(getOpacity() * 255 + 0.5f);
    FSBorderInfo borderInfo= getBorderInfo();
    FS_FLOAT lineWidth = PT_2_MM(borderInfo.width);

    IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
    IOFD_Document *doc = page->GetDocument();
    IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
    COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();
    m_pWriteAnnot = OFD_WriteAnnot_Create(ofdWriteDoc, OFD_ANNOTTYPE_Path, pAnnot);
    COFD_WriteAnnot *ofdWriteAnnot = m_pWriteAnnot;

    COFD_WriteBaseColor *pwriteColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(OFD_COLORTYPE_Base);
    pwriteColor->SetColorValue(color);
    pwriteColor->SetAlpha(opacity);

    COFD_WriteDrawParam *pdrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
    pdrawParam->SetStrokeColor(pwriteColor);
//    pdrawParam->SetFillColor(pwriteColor);  // no mater
    pdrawParam->SetLineWidth(lineWidth);

    COFD_WritePathObject *writePathObj = (COFD_WritePathObject *) OFD_WriteContentObject_Create( ofdWriteDoc, OFD_CONTENTTYPE_Path);

    CFX_RectF annotRect;
    annotRect.Set(0.0, 0.0, boundary.width, boundary.height);
    writePathObj->SetBoundary(annotRect);

    writePathObj->SetDrawParam(pdrawParam);
    writePathObj->SetPath(path);
    writePathObj->SetStrokeState(TRUE);

    COFD_WriteBlockObject *pAnnotAp = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create( ofdWriteDoc, OFD_CONTENTTYPE_Block);
    pAnnotAp->InsertObject(writePathObj);

    ofdWriteAnnot->SetBoundary(boundary);

    ofdWriteAnnot->SetAppearance(pAnnotAp);

    return TRUE;
}

FSPDFPath *FROFDInkImp::getInkList() {
    if (NULL == m_inkList)
        m_inkList = FSPDFPath::create();
    m_inkList->clear();

    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (ap) {
        CFX_RectF bound = m_pAnnotDict->GetBoundary();

        int count = ap->CountObjects();
        for (int i=0; i<count; ++i) {
            const COFD_ContentObject*obj = ap->GetContentObject(i);

            if (!obj || OFD_CONTENTTYPE_Path != obj->GetContentType()) {
                continue;
            }

            COFD_PathObject* pathObj = (COFD_PathObject*)obj;
            const COFD_Path* path = pathObj->GetPath();
            if (!path) {
                continue;
            }

            CFX_RectF rect;
            obj->GetBoundary(rect);
            rect.left += bound.left;
            rect.top += bound.top;

            int count = path->CountPathPoints();
            for (int j=0; j<count; ++j) {
                const OFD_PATHPOINT* pathPoint = path->GetPathPoint(j);

                FSPointF pt;
                pt.x = MM_2_PT(pathPoint->x + rect.left);
                FS_FLOAT height = m_pFSPDFPage->getHeight();
                pt.y = height - MM_2_PT(pathPoint->y + rect.top);

                if (pathPoint->t == OFD_PATHPOINTTYPE_MoveTo || pathPoint->t == OFD_PATHPOINTTYPE_Start) {
                    m_inkList->moveTo(pt);
                } else {
                    m_inkList->lineTo(pt);
                }
            }
        }
    }

    return m_inkList;
};

void FROFDInkImp::setInkList(FSPDFPath *inkList) {
//    m_inkList = inkList;
    if (m_inkList != inkList) {
        if (m_inkList)
        {
            m_inkList->release();
            m_inkList = NULL;
        }
        m_inkList = inkList;
    }
};
