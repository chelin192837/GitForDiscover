//#include "../fxcore.h"
#include "../../../include/fs_annot.h"
#include "fr_ofdannotImp.h"
#include "../../annotation/fs_annot_util.h"
//#include "../fs_util.h"
//#include "fs_annoticonprovider.h"
#include "../fr_ofdpageImp.h"
#include "../fr_ofddocImp.h"

///////////////////////////////////////
// Class FROFDAnnotImp
FROFDAnnotImp::FROFDAnnotImp()
//:m_bSetFreeTextBorderColor(FALSE)
{
    init(NULL, NULL);
}

FROFDAnnotImp::FROFDAnnotImp(COFD_Annot *pAnnotDict, FSPDFPage *page)
//:m_bSetFreeTextBorderColor(FALSE)
{
    init(pAnnotDict, page);
}

FROFDAnnotImp::~FROFDAnnotImp() {
    //clearCachedAppearance();// temp comment by gm
}

void FROFDAnnotImp::init(COFD_Annot *pAnnotDict, FSPDFPage *page) {
    m_pAnnotDict = pAnnotDict;
    m_pWriteAnnot = (COFD_WriteAnnot *) pAnnotDict;
    m_pFSPDFPage = page;
}

FS_BOOL FROFDAnnotImp::isMarkup() const {
    CFX_ByteString bsSubType = getStringImpl(FSANNOT_NAMEKEY_SUBTYPE);
    return FSANNOT_Util_CheckAnnotIsMarkup(bsSubType);
}

FSM_ANNOTTYPE FROFDAnnotImp::getType() const {
    CFX_ByteString bsSubType = getStringImpl(FSANNOT_NAMEKEY_SUBTYPE);
    return FSANNOT_Util_ConvertAnnotTypeFromStrToEnum(bsSubType);
}

FS_INT32 FROFDAnnotImp::getIndex() const {
    if (!m_pAnnotDict || !m_pFSPDFPage || !((FR_OFDPageImp *) m_pFSPDFPage)->getPage())
        return -1;
#if 0 // temp comment by gm 20170721, getDict is NOT COFD_Annot
    FR_OFDPageImp* pPageImp = (FR_OFDPageImp*)m_pFSPDFPage;
    FSCRT_LOCKOBJECT(lock, &pPageImp->m_lockAnnots);
    FS_INT32 iAnnotCount = pPageImp->getAnnotCount();
    for(FS_INT32 i=0; i<iAnnotCount; i++)
    {
        FSAnnot* pFSAnnot = pPageImp->getAnnot(i);
        if(!pFSAnnot) continue;

        if ((CPDF_Dictionary*)pFSAnnot->getDict() == m_pAnnotDict)
            return i;
    }
#endif
    return -1;
}

FSString FROFDAnnotImp::getContent() const {
    return getString(FSANNOT_STRINGKEY_CONTENT);
}

void FROFDAnnotImp::setContent(const char *content) {
    setString(FSANNOT_STRINGKEY_CONTENT, content);
}

FSDateTime FROFDAnnotImp::getModifiedDateTime() const {
    return getDateTime(FSANNOT_DATETIMEKEY_MODIFY);
}

void FROFDAnnotImp::setModifiedDateTime(FSDateTime dataTime) {
    setDateTime(FSANNOT_DATETIMEKEY_MODIFY, dataTime);
}

FS_DWORD FROFDAnnotImp::getFlags() const {
    // temp comment by gm 20170721, need convert ofd rights to pdf flag
    //return getInteger(FSANNOT_INTEGERKEY_FLAGS);
    FS_DWORD result = 0;
    if (m_pAnnotDict->IsInvisible()) {
        result |= FSAnnot::e_annotFlagInvisible;
    }
    if (m_pAnnotDict->IsPrintable()) {
        result |= FSAnnot::e_annotFlagPrint;
    }
    if (m_pAnnotDict->IsReadOnly()) {
        result |= FSAnnot::e_annotFlagReadOnly;
    }
    if (m_pAnnotDict->IsNoZoom()) {
        result |= FSAnnot::e_annotFlagNoZoom;
    }
    if (m_pAnnotDict->IsNoRotate()) {
        result |= FSAnnot::e_annotFlagNoRotate;
    }
    return result;
}

void FROFDAnnotImp::setFlags(FS_DWORD flags) {
    // temp comment by gm 20170721
    //setInteger(FSANNOT_INTEGERKEY_FLAGS, flags);
    bool invisible = flags & FSAnnot::e_annotFlagInvisible;
    bool printable = flags & FSAnnot::e_annotFlagPrint;
    bool readonly = flags & FSAnnot::e_annotFlagReadOnly;
    bool nozoom = flags & FSAnnot::e_annotFlagNoZoom;
    bool norotate = flags & FSAnnot::e_annotFlagNoRotate;
    m_pWriteAnnot->SetInvisible(invisible);
    m_pWriteAnnot->SetPrintable(printable);
    m_pWriteAnnot->SetReadOnly(readonly);
    m_pWriteAnnot->SetNoZoom(nozoom);
    m_pWriteAnnot->SetNoRotate(norotate);
}

FSString FROFDAnnotImp::getUniqueID() const {
    return getString(FSANNOT_STRINGKEY_UNIQUEID);
}

void FROFDAnnotImp::setUniqueID(const char *uniqueID) {
    // note by gm, diff, cannot set id in ofd
    //return setString(FSANNOT_STRINGKEY_UNIQUEID, uniqueID);
}

FSRectF FROFDAnnotImp::getRect() const {
    return getRect(FSANNOT_RECTKEY_RECT);
}

FS_ARGB FROFDAnnotImp::getColor(FS_BOOL bFill) const {
    // This value will be returned if no color property is found or error occurs.
    // Actually, if success, alpha value will be set 0xFF.
    FS_ARGB color = 0x00000000;

    if (bFill)
        getFillColor(color);
    else
        getBorderColorImp(color);

    return color;
}

void FROFDAnnotImp::setColor(FS_ARGB color, FS_BOOL bFill) {
    if (bFill)
        setFillColor(color);
    else
        setBorderColorImp(color);
}

FS_INT32 FROFDAnnotImp::getQuadPointsCount() const {
    /* temp comment by gm
    if (NULL == m_pAnnotDict)
        return 0;
    CPDF_Array* pArrQuad = m_pAnnotDict->GetArray(FSANNOT_ARRAYKEY_QUADPOINTS);
    if (!pArrQuad) return 0;
    return pArrQuad->GetCount() / 8;
    */
    return 0;
}

FSQuadPoints FROFDAnnotImp::getQuadpoints(FS_INT32 index) const {
    /* temp comment by gm
    FSQuadPoints retQuadpoints;
    FSPointF ptFirst, ptSecond, ptThird, ptFourth;
    ptFirst.set(0, 0); ptSecond.set(0, 0); ptThird.set(0, 0); ptFourth.set(0, 0);
    retQuadpoints.set(ptFirst, ptSecond, ptThird, ptFourth);
    FS_INT32 nCount = getQuadPointsCount();
    if (NULL == m_pAnnotDict || index < 0 || index >= nCount) return retQuadpoints;
    CFX_PointsF quadPoints;
    if (!getQuadPointsImpl(quadPoints)) return retQuadpoints;

    FS_INT32 nbaseIndex = index * 4;
    ptFirst = FSPDF_Util_CFXPointF2FSPointF(quadPoints.GetAt(nbaseIndex++));
    ptSecond = FSPDF_Util_CFXPointF2FSPointF(quadPoints.GetAt(nbaseIndex++));
    ptThird = FSPDF_Util_CFXPointF2FSPointF(quadPoints.GetAt(nbaseIndex++));
    ptFourth = FSPDF_Util_CFXPointF2FSPointF(quadPoints.GetAt(nbaseIndex));
    FSQuadPoints fsQuadPoints;
    fsQuadPoints.set(ptFirst, ptSecond, ptThird, ptFourth);
    return fsQuadPoints;
    // */
    return FSQuadPoints();
}

FS_BOOL FROFDAnnotImp::getQuadPointsImpl(CFX_PointsF &quadPoints) const {
    /* temp comment by gm
    CPDF_Array* pPoints = getArray(FSANNOT_ARRAYKEY_QUADPOINTS);
    if (!pPoints) return FALSE;

    FX_INT32 nCount = pPoints->GetCount() / 8 * 8;
    for (FX_INT32 i = 0; i < nCount; i += 2)
    {
        CFX_PointF pt;
        pt.x = pPoints->GetNumber(i);
        pt.y = pPoints->GetNumber(i + 1);
        quadPoints.Add(pt);
    }
    // */
    return TRUE;
}

void FROFDAnnotImp::setQuadPoints(const FSQuadPoints *quadPoints, FS_INT32 count) {
    /* temp comment by gm
    // TODO: for link annot, if any point in quadoints is outside rect, needs to update rect
    if (NULL == m_pAnnotDict || !quadPoints || count<1)
        return ;
    CFX_PointsF fxPoints;
    for (FS_INT32 index = 0;index < count;index++)
    {
        fxPoints.Add(FSPDF_Util_FSPointF2CFXPointF(quadPoints[index].first));
        fxPoints.Add(FSPDF_Util_FSPointF2CFXPointF(quadPoints[index].second));
        fxPoints.Add(FSPDF_Util_FSPointF2CFXPointF(quadPoints[index].third));
        fxPoints.Add(FSPDF_Util_FSPointF2CFXPointF(quadPoints[index].fourth));
    }
    setQuadPointsImpl(fxPoints);
    // */
}

FS_BOOL FROFDAnnotImp::setQuadPointsImpl(const CFX_PointsF &quadPoints) {
    /* temp comment by gm/*
    FX_INT32 nCount = quadPoints.GetSize() / 4 * 4;
    if (nCount < 1) return FALSE;

    CPDF_Array* pArray = CPDF_Array::Create();
    if (!pArray) return FALSE;

    if (FALSE == setArray(FSANNOT_ARRAYKEY_QUADPOINTS, pArray))
    {
        pArray->Release();
        return FALSE;
    }

    for (FX_INT32 i = 0; i < nCount; i ++)
    {
        pArray->AddNumber(quadPoints[i].x);
        pArray->AddNumber(quadPoints[i].y);
    }

    CFX_ByteString bstrAnnotType = m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE);
    if (0 == bstrAnnotType.Compare(FSPDF_ANNOTTYPE_HIGHLIGHT)
        || 0 == bstrAnnotType.Compare(FSPDF_ANNOTTYPE_UNDERLINE)
        || 0 == bstrAnnotType.Compare(FSPDF_ANNOTTYPE_STRIKEOUT)
        || 0 == bstrAnnotType.Compare(FSPDF_ANNOTTYPE_SQUIGGLY))
    {
        CPDF_Rect newAnnotRect(0, 0, 0, 0);
        FX_INT32 iQuadCount = quadPoints.GetSize();
        if (iQuadCount > 1)
        {
            newAnnotRect.left = newAnnotRect.right = quadPoints[0].x;
            newAnnotRect.top = newAnnotRect.bottom = quadPoints[0].y;

            for (FX_INT32 i=1; i<iQuadCount; i++)
            {
                newAnnotRect.left = (newAnnotRect.left < quadPoints[i].x)? newAnnotRect.left : quadPoints[i].x;//left
                newAnnotRect.right = (newAnnotRect.right > quadPoints[i].x)? newAnnotRect.right : quadPoints[i].x;//right
                newAnnotRect.top = (newAnnotRect.top > quadPoints[i].y)? newAnnotRect.top : quadPoints[i].y;//top
                newAnnotRect.bottom = (newAnnotRect.bottom < quadPoints[i].y)? newAnnotRect.bottom : quadPoints[i].y;//bottom
            }
        }
        m_pAnnotDict->SetAtRect(FSANNOT_RECTKEY_RECT, newAnnotRect);
    }

    setModified();
    */
    return TRUE;
}

void FROFDAnnotImp::setModified() {
    if (!m_pFSPDFPage)
        return;
    /* temp comment by gm
    //((FR_OFDPageImp*)m_pFSPDFPage)->setModified();
    // */
}

void FROFDAnnotImp::clearCachedAppearance() {
    /* temp comment by gm
    FX_POSITION pos = m_APMap.GetStartPosition();
    while (pos) {
        void* pForm; void* pObjects;
        m_APMap.GetNextAssoc(pos, pForm, pObjects);
        delete (CPDF_PageObjects*)pObjects;
    }
    m_APMap.RemoveAll();
    // */
}

static CPDF_Stream *FPDFAnnot_GetAnnotAP(CPDF_Dictionary *pAnnotDict, FX_BSTR ap_entry) {
    /* temp comment by gm
    CPDF_Dictionary* pAP = pAnnotDict->GetDict(FX_BSTRC("AP"));
    if (pAP == NULL) return NULL;

    CPDF_Object* psub = pAP->GetElementValue(ap_entry);
    if (psub == NULL) return NULL;

    CPDF_Stream* pStream = NULL;
    if (psub->GetType() == PDFOBJ_STREAM)
    {
        pStream = (CPDF_Stream*)psub;
    }
    else if (psub->GetType() == PDFOBJ_DICTIONARY)
    {
        CFX_ByteString as = pAnnotDict->GetString("AS");
        if (as.IsEmpty())
        {
            CFX_ByteString value = pAnnotDict->GetString(FX_BSTRC("V"));
            if (value.IsEmpty())
                value = pAnnotDict->GetDict(FX_BSTRC("Parent"))->GetString(FX_BSTRC("V"));
            if (value.IsEmpty() || !((CPDF_Dictionary*)psub)->KeyExist(value))
                as = FX_BSTRC("Off");
            else
                as = value;
        }
        pStream = ((CPDF_Dictionary*)psub)->GetStream(as);
    }
    return pStream;
    // */
    return NULL;
}


CPDF_Form *FROFDAnnotImp::getAPForm(FX_BSTR mode) {
    /* temp comment by gm
    CPDF_Stream* pStream = FPDFAnnot_GetAnnotAP(m_pAnnotDict, mode);
    if (pStream == NULL) return NULL;

    // Check for cached form
    CPDF_Form* pForm;
    if (m_APMap.Lookup(pStream, (void*&)pForm))
        return pForm;

    CPDF_Page* pPDFPage = ((FSPDFPageImp*)m_pFSPDFPage)->getPage();
    pForm = FX_NEW CPDF_Form(pPDFPage->m_pDocument, pPDFPage->m_pResources, pStream);
    pForm->ParseContent(NULL, NULL, NULL, NULL);

    m_APMap.SetAt(pStream, pForm);

    return pForm;
    // */
    return NULL;
}

FX_BOOL FROFDAnnotImp::drawAppearance(CFX_RenderDevice *pDevice, const CFX_Matrix *pUser2Device,
                                      FX_BSTR bsMode, CPDF_RenderOptions *pOptions) {
    /* temp comment by gm
    CPDF_Form* pForm = getAPForm(bsMode);
    if (pForm == NULL) return FALSE;

    CFX_FloatRect form_bbox = pForm->m_pFormDict->GetRect(FX_BSTRC("BBox"));
    CFX_AffineMatrix form_matrix = pForm->m_pFormDict->GetMatrix(FX_BSTRC("Matrix"));
    form_matrix.TransformRect(form_bbox);

    CPDF_Rect arect = m_pAnnotDict->GetRect(FX_BSTRC("Rect"));
    arect.Normalize();
    CFX_AffineMatrix matrix;
    matrix.MatchRect(arect, form_bbox);
    matrix.Concat(*pUser2Device);

    CPDF_RenderContext context;
    context.Create(((FSPDFPageImp*)m_pFSPDFPage)->getPage());
    context.DrawObjectList(pDevice, pForm, &matrix, pOptions);
    // */
    return TRUE;
}

FX_BOOL FROFDAnnotImp::drawInContext(const CPDF_Page *pPage, const CPDF_RenderContext *pContext,
                                     const CFX_AffineMatrix *pUser2Device,
                                     CPDF_Annot::AppearanceMode mode) {
    /* temp comment by gm
    CFX_ByteString bsMode;
    switch(mode)
    {
    case 0:
        bsMode = "N";
        break;
    case 1:
        bsMode = "R";
        break;
    case 2:
        bsMode = "D";
        break;
    }
    CPDF_Form* pForm = getAPForm(bsMode);
    if (pForm == NULL) return FALSE;

    CFX_FloatRect form_bbox = pForm->m_pFormDict->GetRect(FX_BSTRC("BBox"));
    CFX_AffineMatrix form_matrix = pForm->m_pFormDict->GetMatrix(FX_BSTRC("Matrix"));
    form_matrix.TransformRect(form_bbox);

    CPDF_Rect arect = m_pAnnotDict->GetRect(FX_BSTRC("Rect"));
    CFX_AffineMatrix matrix;
    matrix.MatchRect(arect, form_bbox);
    matrix.Concat(*pUser2Device);
    ((CPDF_RenderContext*)pContext)->AppendObjectList(pForm, &matrix);

    // */
    return TRUE;
}


CPDF_Stream *FROFDAnnotImp::writeAppearance(FX_BSTR bsAPType, const CFX_FloatRect &rcBBox,
                                            const CFX_Matrix &matrix, CFX_ByteTextBuf &bsContents,
                                            FX_BSTR bsAPState,
                                            FSPDFANNOT_APPPARAM *pParam/* = NULL*/) {
    /* temp comment by gm
    if (NULL == m_pAnnotDict || !m_pFSPDFPage || !((FSPDFPageImp*)m_pFSPDFPage)->getPage())
        return NULL;
    CPDF_Dictionary* pAPDict = m_pAnnotDict->GetDict(FX_BSTRC("AP"));
    if (!pAPDict)
    {
        pAPDict = CPDF_Dictionary::Create();
        if (!pAPDict) return NULL;
        m_pAnnotDict->SetAt(FSANNOT_DICTKEY_AP, pAPDict);
    }

    CPDF_Stream* pStream = NULL;
    CPDF_Dictionary* pParentDict = NULL;
    CFX_ByteString bsStreamKey = bsAPType;
    if (bsAPState.IsEmpty())
    {
        pParentDict = pAPDict;
        pStream = pAPDict->GetStream(bsAPType);
    }
    else
    {
        CPDF_Dictionary* pAPTypeDict = pAPDict->GetDict(bsAPType);
        if (!pAPTypeDict)
        {
            pAPTypeDict = CPDF_Dictionary::Create();
            if (!pAPTypeDict) return NULL;
            pAPDict->SetAt(bsAPType, pAPTypeDict);
        }
        pParentDict = pAPTypeDict;
        pStream = pAPTypeDict->GetStream(bsAPState);
        bsStreamKey = bsAPState;
    }

    CPDF_Document* pDoc = ((FSPDFPageImp*)m_pFSPDFPage)->getPage()->m_pDocument;
    if (!pDoc) return NULL;

    if (!pStream)
    {
        pStream = FX_NEW CPDF_Stream(NULL, 0, NULL);
        if (!pStream) return NULL;
        pDoc->AddIndirectObject(pStream);
        pParentDict->SetAtReference(bsStreamKey, pDoc, pStream);
    }

    CPDF_Dictionary * pStreamDict = pStream->GetDict();
    if (!pStreamDict)
    {
        pStreamDict = FX_NEW CPDF_Dictionary;
        if (!pStreamDict)return NULL;
        pStreamDict->SetAtName(FX_BSTRC("Type"), FX_BSTRC("XObject"));
        pStreamDict->SetAtName(FX_BSTRC("Subtype"), FX_BSTRC("Form"));
        pStreamDict->SetAtInteger(FX_BSTRC("FormType"), 1);
        pStream->InitStream(NULL, 0, pStreamDict);
    }
    pStreamDict->SetAtMatrix(FX_BSTRC("Matrix"), matrix);
    pStreamDict->SetAtRect(FX_BSTRC("BBox"), rcBBox);

    if (pParam && pParam->bUseOpacity)
    {
        CPDF_Dictionary* pStreamResList = pStreamDict->GetDict(FX_BSTRC("Resources"));
        if (!pStreamResList)
        {
            pStreamResList = FX_NEW CPDF_Dictionary;
            if (!pStreamResList)return NULL;
            pStreamDict->SetAt(FX_BSTRC("Resources"), pStreamResList);
        }

        CPDF_Dictionary* pExtGState = pStreamResList->GetDict(FX_BSTRC("ExtGState"));
        if (!pExtGState)
        {
            pExtGState = FX_NEW CPDF_Dictionary;
            if (!pExtGState)return NULL;
            pStreamResList->SetAt(FX_BSTRC("ExtGState"), pExtGState);
        }
        CPDF_Dictionary * pOpacity = pExtGState->GetDict(pParam->bsExtGState);
        if (!pOpacity)
        {
            pOpacity = FX_NEW CPDF_Dictionary;
            if (!pOpacity)return NULL;
            pExtGState->SetAtReference(pParam->bsExtGState, pDoc, pDoc->AddIndirectObject(pOpacity));
            pOpacity->SetAtName(FX_BSTRC("Type"), FX_BSTRC("ExtGState"));
        }

        pOpacity->SetAtNumber(FX_BSTRC("CA"), pParam->fOpacity);
        pOpacity->SetAtNumber(FX_BSTRC("ca"), pParam->fOpacity);
        pOpacity->SetAtBoolean(FX_BSTRC("AIS"), FALSE);
        if (!pParam->bsBlenMode.IsEmpty())
            pOpacity->SetAtName(FX_BSTRC("BM"), pParam->bsBlenMode);
    }

    pStream->SetData(bsContents.GetBuffer(), bsContents.GetLength(), FALSE, TRUE);
    bsContents.DetachBuffer();
    setModified();
    return pStream;
    // */
    return NULL;
}

FSBorderInfo FROFDAnnotImp::getBorderInfo() const {
    FSBorderInfo borderInfo;
    borderInfo.width = 1;
    borderInfo.style = e_borderStyleSolid;
    borderInfo.cloudIntensity = 0;
    borderInfo.dashPhase = 0;

    if (!m_pAnnotDict) {
        return borderInfo;
    }
    COFD_BlockObject *ap = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
    if (!ap) {
        return borderInfo;
    }
    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
    int size = ap->CountObjects();
    for (int i = 0; i < size; i++) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }
        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (drawParm) {
            borderInfo.width = MM_2_PT(drawParm->GetLineWidth());
            borderInfo.style = e_borderStyleSolid;
            borderInfo.cloudIntensity = 0;
            borderInfo.dashPhase = 0;

        }
    }
    /* temp comment by gm
    for (FS_INT32 i=0; i<16; i++)
        borderInfo.dashes[i] = -1.0;

    if (FALSE == hasProperty(FSANNOT_ARRAYKEY_BORDER) && FALSE == hasProperty(FSANNOT_DICTKEY_BORDERSTYLE)
        && FALSE == hasProperty(FSANNOT_DICTKEY_BORDEREFFECT))
        return borderInfo;

    borderInfo.width = getBorderWidth();
    borderInfo.style = getBorderStyle();
    borderInfo.cloudIntensity = 0.0f;
    if (FSAnnot::e_borderStyleCloudy == borderInfo.style)
        borderInfo.cloudIntensity = getCloudyIntensity();

    FX_FLOAT fDashOffset = 0;
    CFX_FloatArray dashPatternArray;
    getBorderDash(fDashOffset, dashPatternArray);
    FS_INT32 iDashCount = dashPatternArray.GetSize();
    FS_INT32 iUsedCount = iDashCount>16? 16:iDashCount;
    borderInfo.dashPhase = fDashOffset;
    FS_INT32 i=0;
    for (i=0; i<iUsedCount; i++)
        borderInfo.dashes[i] = dashPatternArray[i];
    for (;i<16; i++)
        borderInfo.dashes[i] = -1.0;
    // */
    return borderInfo;
}

void FROFDAnnotImp::getBorderDash(FX_FLOAT &dashOffset, CFX_FloatArray &dashPatternArray) const {
    dashOffset = 0;
    /* temp comment by gm
    CPDF_Array* pBorder = NULL;
    CPDF_Dictionary* pBS = NULL;
    if (pBS = getSubDict(FSANNOT_DICTKEY_BORDERSTYLE))
    {
        CPDF_Array* pD = pBS->GetArray(FX_BSTRC("D"));
        if (!pD || pD->GetCount() < 1) return ;
        CPDF_Object* pElement = pD->GetElement(0);
        if (!pElement) return ;
        if(pElement->GetType() != PDFOBJ_ARRAY)
        { // eg. /BS <</D [2 3]>>
            for(FX_DWORD i=0; i<pD->GetCount(); i++)
                dashPatternArray.Add(pD->GetElement(i)->GetNumber());
        }
        else
        {// eg. /BS <</D [[2 3] 2]>>
            for (FX_DWORD i = 0; i < ((CPDF_Array*)pElement)->GetCount(); i++)
                dashPatternArray.Add(((CPDF_Array*)pElement)->GetNumber(i));
            if (pElement = pD->GetElement(1))
                dashOffset = pElement->GetNumber();
        }
    }
    else if (pBorder = getArray(FSANNOT_ARRAYKEY_BORDER))
    {// eg. /Border [0 0 1 [2 3]]
        CPDF_Array* pD = pBorder->GetArray(3);
        if (!pD || pD->GetCount() < 1) return ;
        for(FX_DWORD i=0; i<pD->GetCount(); i++)
            dashPatternArray.Add(pD->GetElement(i)->GetNumber());
        if(pBorder->GetCount()>=5) // eg. /Border [0 0 1 [2 3] 1]
            dashOffset = pBorder->GetNumber(4);
    }
    // */
}

FSM_BORDERSTYLE FROFDAnnotImp::getBorderStyle() const {
    /* temp comment by gm
    // First, check /BE <</S >>.
    CPDF_Dictionary* pBE = getSubDict(FSANNOT_DICTKEY_BORDEREFFECT);
    if (pBE)
    {
        FS_BOOL bTemp = pBE->KeyExist("S");
        CFX_ByteString sTemp = pBE->GetString("S");
        if (pBE->KeyExist("S") && pBE->GetString("S").Equal("C"))
            return FSAnnot::e_borderStyleCloudy;
    }
    // Then check /BS << /S >>
    CPDF_Dictionary* pBS = getSubDict(FSANNOT_DICTKEY_BORDERSTYLE);
    if (pBS)
    {
        CFX_ByteString bsBorderStyle = pBS->GetString("S");
        if (bsBorderStyle.GetLength() != 1)
            return FSAnnot::e_borderStyleSolid;

        FX_BYTE single = bsBorderStyle[0];
        if (single == 'D')
            return FSAnnot::e_borderStyleDashed;
        if (single == 'B')
            return FSAnnot::e_borderStyleBeveled;
        if (single == 'I')
            return FSAnnot::e_borderStyleInset;
        if (single == 'U')
            return FSAnnot::e_borderStyleUnderLine;

        return FSAnnot::e_borderStyleSolid;
    }
    // Finally, check /Border []
    CPDF_Array* pBorder = getArray(FSANNOT_ARRAYKEY_BORDER);
    if (pBorder && pBorder->GetCount() > 3)
    {
        CPDF_Array* pDP = pBorder->GetArray(3) ;
        if (pDP != NULL && pDP->GetCount() > 0)
            return FSAnnot::e_borderStyleDashed;
    }
    // */
    return FSAnnot::e_borderStyleSolid;
}


FS_FLOAT FROFDAnnotImp::getBorderWidth() const {
    FX_FLOAT fWidth = 1.0;

    if (!m_pAnnotDict) {
        return FALSE;
    }
    COFD_BlockObject *ap = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
    if (!ap) {
        return FALSE;
    }
    IOFD_Page *pPage = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
    int size = ap->CountObjects();
    for (int i = 0; i < size; i++) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }
        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (drawParm) {
            fWidth = MM_2_PT(drawParm->GetLineWidth());
            return fWidth;
        }
    }
    return fWidth;
}

FS_INT32 FROFDAnnotImp::getBorderEffect() const {
    /* temp comment by gm 20170721
    CPDF_Dictionary* pBE = getSubDict(FSANNOT_DICTKEY_BORDEREFFECT);
    if (!pBE) return FSANNOT_BORDEREFFECT_STYLE;

    CFX_ByteString bsStyle = pBE->GetString(FX_BSTRC("S"));
    return bsStyle == FX_BSTRC("C") ? FSANNOT_BORDEREFFECT_CLOUDY : FSANNOT_BORDEREFFECT_STYLE;
    // */
    return FSANNOT_BORDEREFFECT_STYLE;
}

FX_FLOAT FROFDAnnotImp::getCloudyIntensity() const {
    /* temp comment by gm 20170721
    CPDF_Dictionary* pBE = getSubDict("BE");
    if (pBE)
        return pBE->GetNumber(FX_BSTRC("I"));
    // */
    return 0;
}

void FROFDAnnotImp::setBorderDash(FX_FLOAT dashOffset, const CFX_FloatArray &dashPatternArray) {
    /* temp comment by gm 20170721
    CPDF_Dictionary* pBS = getSubDict(FSANNOT_DICTKEY_BORDERSTYLE, TRUE);
    if (!pBS) return;
    CPDF_Array* pD = CPDF_Array::Create();
    if (!pD) return;
    pBS->SetAt(FX_BSTRC("D"), pD);

    if(FXSYS_fabs(dashOffset) <= FLOAT_ZERO_CMP)
    {
        FX_INT32 nCount = dashPatternArray.GetSize();
        for (FX_INT32 i = 0; i < nCount; i ++)
            pD->AddNumber(dashPatternArray.GetAt(i));
    }
    else
    {
        CPDF_Array* pPattern = CPDF_Array::Create();
        if (!pPattern) return;
        //FX_INT32 nCount = aDashPattern.GetSize();
        //for (FX_INT32 i = 0; i < nCount; i ++)
        //	pPattern->AddNumber(aDashPattern.GetAt(i));

        FX_INT32 nCount = dashPatternArray.GetSize();
        for (FX_INT32 i = 0; i < nCount; i ++)
            pPattern->AddNumber(dashPatternArray.GetAt(i));
        pD->Add(pPattern);
        pD->AddNumber(dashOffset);
    }
    setModified();
    // */
}

void FROFDAnnotImp::setBorderInfo(FSBorderInfo border) {
    // temp comment by gm 20170721
    if (!m_pAnnotDict)
        return;

    FS_BOOL bSetBorderEntry = TRUE; //FALSE means to set BS and BE
    switch (getType()) {
        case FSAnnot::e_annotLink: {
            if (FSAnnot::e_borderStyleUnderLine == border.style) {
                bSetBorderEntry = FALSE;
            } else
                bSetBorderEntry = TRUE;
        }
            break;
        case FSAnnot::e_annotSquare:
        case FSAnnot::e_annotCircle:
        case FSAnnot::e_annotFreeText:
        case FSAnnot::e_annotPolygon: {
            bSetBorderEntry = FALSE;
            if (FSAnnot::e_borderStyleDashed != border.style
                && FSAnnot::e_borderStyleCloudy != border.style
                && FSAnnot::e_borderStyleSolid != border.style)
                border.style = FSAnnot::e_borderStyleSolid;
        }
            break;
        case FSAnnot::e_annotPolyLine:
        case FSAnnot::e_annotLine: {
            bSetBorderEntry = FALSE;
            if (FSAnnot::e_borderStyleSolid != border.style
                && FSAnnot::e_borderStyleDashed != border.style)
                border.style = FSAnnot::e_borderStyleSolid;
        }
            break;
        default:
            //case FSAnnot::e_annotNote:
            //case FSAnnot::e_annotHighlight:
            //case FSAnnot::e_annotUnderline:
            //case FSAnnot::e_annotStrikeOut:
            //case FSAnnot::e_annotSquiggly:
            //case FSAnnot::e_annotStamp:
            //case FSAnnot::e_annotCaret:
        {//Other annotations can only support solid border style.
            bSetBorderEntry = TRUE;
            border.style = FSAnnot::e_borderStyleSolid;
        }
            break;
    }

    if (bSetBorderEntry) {
        if (!m_pAnnotDict) {
            return;
        }

        COFD_BlockObject *ap = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
        if (!ap) {
            return;
        }
        IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
        int size = ap->CountObjects();
        for (int i = 0; i < size; i++) {
            const COFD_ContentObject *obj = ap->GetContentObject(i);
            if (!obj) {
                continue;
            }
            const COFD_DrawParam *drawParm = obj->GetDrawParam(
                    pPage->GetDocument()->GetPublicResources());
            if (!drawParm) {
                continue;
            }
//			COFD_WriteDrawParam* pWritedrawParam = getWriteDrawParam(drawParm);
//			IOFD_WriteDocument* writeDoc = m_Page->getDocument()->getOFDWriteDoc();
            IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
            IOFD_Document *doc = page->GetDocument();
            IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
            COFD_WriteDrawParam *pWritedrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(
                    ofdWriteDoc, OFD_RESOURCETYPE_DrawParam,
                    const_cast<COFD_DrawParam *>(drawParm));
            if (!pWritedrawParam) {
                continue;
            }
            pWritedrawParam->SetLineWidth(PT_2_MM(border.width));
        }
    } else {
        if (!m_pAnnotDict) {
            return;
        }

        COFD_BlockObject *ap = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
        if (!ap) {
            return;
        }
        IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
        int size = ap->CountObjects();
        for (int i = 0; i < size; i++) {
            const COFD_ContentObject *obj = ap->GetContentObject(i);
            if (!obj) {
                continue;
            }
            const COFD_DrawParam *drawParm = obj->GetDrawParam(
                    pPage->GetDocument()->GetPublicResources());
            if (!drawParm) {
                continue;
            }
//			COFD_WriteDrawParam* pWritedrawParam = getWriteDrawParam(drawParm);
//			IOFD_WriteDocument* writeDoc = m_Page->getDocument()->getOFDWriteDoc();
            IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
            IOFD_Document *doc = page->GetDocument();
            IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
            COFD_WriteDrawParam *pWritedrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(
                    ofdWriteDoc, OFD_RESOURCETYPE_DrawParam,
                    const_cast<COFD_DrawParam *>(drawParm));
            if (!pWritedrawParam) {
                continue;
            }
            pWritedrawParam->SetLineWidth(PT_2_MM(border.width));
        }

    }

    setModified();
    // */
}

FS_BOOL FROFDAnnotImp::setBorderInfo2Border(FSBorderInfo border) {
    /* temp comment by gm 20170721
    // Create new border array and set border info to it.
    CPDF_Array* pNewBorderArray = CPDF_Array::Create();
    if (!pNewBorderArray) return FALSE;

    pNewBorderArray->AddInteger(0);
    pNewBorderArray->AddInteger(0);
    pNewBorderArray->AddNumber(border.width);
    if (FSAnnot::e_borderStyleDashed == border.style)
    {
        CPDF_Array* pNewDashArray = CPDF_Array::Create();
        if (!pNewDashArray)
        {
            pNewBorderArray->Release();
            return FALSE;
        }
        for (FS_INT32 i=0; i<16; i++)
        {
            if (border.dashes[i]<0)
                break;
            pNewDashArray->AddNumber(border.dashes[i]);
        }
        pNewDashArray->AddNumber(border.style);
        pNewBorderArray->Add(pNewDashArray);
        //if (border.dashPhase>0)
            pNewBorderArray->AddNumber(border.dashPhase);
    }
    // Set the new border array to /Border.
    m_pAnnotDict->SetAt(FSANNOT_ARRAYKEY_BORDER, pNewBorderArray);
    setModified();
    // */
    return TRUE;
}

FS_BOOL FROFDAnnotImp::setBorderInfo2BorderStyleEffect(FSBorderInfo border) {
    /* temp comment by gm 20170721
    // Create a new BS dictionary and set it.
    CPDF_Dictionary* pNewBSDict = CPDF_Dictionary::Create();
    if (!pNewBSDict) return FALSE;

    pNewBSDict->SetAtName("Type", CFX_ByteString("Border"));
    pNewBSDict->SetAtNumber("W", border.width);

    FX_LPCSTR borderStyle_Chars = "SDUBI";
    FS_INT32 iBorderType = border.style;
    // Currently, e_borderStyleCloudy means /BE <</S /C>> with /BS <</S /S>>.
    if (FSAnnot::e_borderStyleCloudy == border.style)
        iBorderType = (FS_INT32)FSAnnot::e_borderStyleSolid;
    pNewBSDict->SetAtName(FX_BSTRC("S"), CFX_ByteString(borderStyle_Chars[iBorderType]));

    if (FSAnnot::e_borderStyleCloudy == border.style)
    {
        // For e_borderStyleCloudy, create new BE dictionary and set it.
        CPDF_Dictionary* pNewBEDict = CPDF_Dictionary::Create();
        if (!pNewBEDict)
        {
            pNewBSDict->Release();
            return FALSE;
        }
        pNewBEDict->SetAtName("S", "C");
        pNewBEDict->SetAtNumber("I", border.cloudIntensity);
        // Set the new BE dictionary to /BE of annotation dictionary.
        m_pAnnotDict->SetAt("BE", pNewBEDict);
        // Set the new BS dictionary to /BS of annotation dictionary.
        m_pAnnotDict->SetAt("BS", pNewBSDict);
    }
    else
    {
        // Set the new BS dictionary to /BS of annotation dictionary.
        m_pAnnotDict->SetAt("BS", pNewBSDict);
        // Remove old /BE dictionary when style is not e_borderStyleCloudy.
        m_pAnnotDict->RemoveAt("BE");
        if (FSAnnot::e_borderStyleDashed == border.style)
        {
            CFX_FloatArray dashPatternArray;
            for(FS_INT32 i = 0; i < 16; i ++)
            {
                if(border.dashes[i]<0)
                    break;
                dashPatternArray.Add(border.dashes[i]);
            }
            setBorderDash(border.dashPhase, dashPatternArray);
        }
    }
    setModified();
    // */
    return TRUE;
}

FSRectI FROFDAnnotImp::getDeviceRect(FS_BOOL bTransformIcon, const FSMatrix *matrix) {
    return FSRectI();
#if 0    //* temp comment by gm 20170721
    FSRectI emptyRect;
    emptyRect.set(0, 0, 0, 0);
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(FX_BSTRC("Rect")) || !matrix)
        return emptyRect;

    FS_FLOAT fIconWidth = 0.0f;
    FS_FLOAT fIconHeight = 0.0f;
    FS_BOOL bIcon = FALSE;
    FSM_ANNOTTYPE annotType = getType();
    if( FSAnnot::e_annotPopup == annotType) return emptyRect;
    if ( FALSE == bTransformIcon && (FSAnnot::e_annotFileAttachment == annotType || FSAnnot::e_annotNote == annotType))
    {
        bIcon = TRUE;
        CFX_ByteString bsIconName = getStringImpl(FSANNOT_NAMEKEY_ICONNAME);
        if (FALSE == CFSPDF_AnnotIconProvider::getDisplaySize(annotType, bsIconName, fIconWidth, fIconHeight))
            fIconWidth = fIconHeight = 0.0f;
    }

    CPDF_Rect annotRect = m_pAnnotDict->GetRect("Rect");
    CPDF_Matrix pdfMatrix;
    pdfMatrix.a = matrix->a;
    pdfMatrix.b = matrix->b;
    pdfMatrix.c = matrix->c;
    pdfMatrix.d = matrix->d;
    pdfMatrix.e = matrix->e;
    pdfMatrix.f = matrix->f;

    if (FALSE == bTransformIcon && bIcon)
    {
        FS_FLOAT scale_x = 1.0f, scale_y = 1.0f;
        if (fIconWidth != 0 && fIconHeight != 0)
        {//Icon provider is used.
            FS_FLOAT fWidth = annotRect.Width();
            FS_FLOAT fHeight = annotRect.Height();

            scale_x = fIconWidth / fWidth;
            scale_y = fIconHeight / fHeight;
        }
        float factor_x = 0;
        float factor_y = 0;
        if(pdfMatrix.Is90Rotated())
        {
            factor_x = 1.0f/FXSYS_fabs(pdfMatrix.c) * scale_y;
            factor_y = 1.0f/FXSYS_fabs(pdfMatrix.b) * scale_x;
        }
        else
        {
            factor_x = 1.0f/FXSYS_fabs(pdfMatrix.a) * scale_x;
            factor_y = 1.0f/FXSYS_fabs(pdfMatrix.d) * scale_y;
        }
        float x1, x2;
        float y1, y2;
        FS_ROTATION nRotate = this->getPage()->getRotation();// getRotation();
        switch(nRotate)
        {
        case FSCommonDefines::e_rotationUnknown:
        case FSCommonDefines::e_rotation0:
            {
                x1 = x2 = annotRect.left;
                y1 = y2 = annotRect.top;
            }
            break;
        case FSCommonDefines::e_rotation90:
            {
                x1 = x2 = annotRect.left;
                y1 = y2 = annotRect.bottom;
            }
            break;
        case FSCommonDefines::e_rotation180:
            {
                x1 = x2 = annotRect.right;
                y1 = y2 = annotRect.bottom;
            }
            break;
        case FSCommonDefines::e_rotation270:
            {
                x1 = x2 = annotRect.right;
                y1 = y2 = annotRect.top;
            }
            break;
        }
        pdfMatrix.TransformPoint(x1, y1);
        pdfMatrix.Scale(factor_x, factor_y);
        pdfMatrix.TransformPoint(x2, y2);
        pdfMatrix.Translate(x1-x2, y1-y2);
    }

    annotRect.Transform(&pdfMatrix);

    /*FX_RECT tempRect = annotRect.GetClosestRect();
    FSRectI deviceRect;
    deviceRect.set(tempRect.left, tempRect.top, tempRect.right, tempRect.bottom);*/

    FSRectI deviceRect;
    deviceRect.set(FXSYS_ceil(annotRect.left), FXSYS_floor(annotRect.top), FXSYS_floor(annotRect.right), FXSYS_ceil(annotRect.bottom));
    if (deviceRect.left > deviceRect.right) {
        int temp = deviceRect.left;
        deviceRect.left = deviceRect.right;
        deviceRect.right = temp;
    }

    if (deviceRect.top > deviceRect.bottom) {
        int temp = deviceRect.top;
        deviceRect.top = deviceRect.bottom;
        deviceRect.bottom = temp;
    }
    return deviceRect;
#endif
}

CFX_WideString FROFDAnnotImp::getUnicodeString(FX_BSTR key) const {
    /* modify by gm
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return CFX_WideString();

    CPDF_Object* pObj = m_pAnnotDict->GetElementValue(key);
    if (!pObj) return CFX_WideString();
    return pObj->GetUnicodeText();
    // */
    if (!m_pAnnotDict) {
        return CFX_WideString();
    }

    if (key == FSANNOT_NAMEKEY_SUBTYPE) {
        return m_pAnnotDict->GetSubType();
    } else if (key == FSANNOT_STRINGKEY_CONTENT) {
        return m_pAnnotDict->GetRemark();
    } else if (key == FSANNOT_STRINGKEY_UNIQUEID) {
        CFX_WideString ws;
        ws.Format((FX_LPCWSTR) L"%d", m_pAnnotDict->GetAnnotID());
        return ws;
    } else if (key == FSANNOT_STRINGKEY_TITLE) {
        return m_pAnnotDict->GetCreator();
    } else if (key == FSANNOT_STRINGKEY_SUBJECT) {
        return L"";
    }
    return CFX_WideString();
}

CFX_ByteString FROFDAnnotImp::getStringImpl(FX_BSTR key) const {
    if (!m_pAnnotDict /*|| !m_pAnnotDict->KeyExist(key)*/)
        return CFX_ByteString();

    CFX_WideString wsString = getUnicodeString(key);
    return FSCRT_UTF8_FromWideString(wsString);
}

FSString FROFDAnnotImp::getString(FX_BSTR key) const {
    CFX_ByteString bsByteStringUTF8 = getStringImpl(key);
    FSString fsRetString(bsByteStringUTF8.GetBuffer(bsByteStringUTF8.GetLength()));
    bsByteStringUTF8.ReleaseBuffer();
    return fsRetString;
}

void FROFDAnnotImp::setString(FX_BSTR key, const char *value) {
    // note by gm, FSANNOT_Util_CheckAnnotIsSupport?????
    //if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
    if (!m_pWriteAnnot)
        return;

    CFX_WideString wsString;
    if (!FSCRT_Util_IsStringEmpty(
            value)) // TODO: maybe some key doesn't support empty string value.
        wsString = FSCRT_UTF8_ToWideString(value);
    // modify by gm 20170721
    //m_pAnnotDict->SetAtString(key, wsString);
    bool found = true;
    if (key == FSANNOT_STRINGKEY_CONTENT) {
        m_pWriteAnnot->SetRemark(wsString);
    } else if (key == FSANNOT_STRINGKEY_TITLE) {
        m_pWriteAnnot->SetCreator(wsString);
    } else {
        found = false;
    }
    if (found) {
#if _FX_OS_ == _FX_IOS_
        //A temporary trick to make sure unique id will not modify the document, for ios rdk will always change unique id.
            if(key != FSANNOT_STRINGKEY_UNIQUEID)
                setModified();
#else
        setModified();
#endif
    }
}

void FROFDAnnotImp::setName(FX_BSTR key, const char *nameValue) {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return;
    // Input "nameValue" must be UTF-8 and CPDF_Dictioanry::SetAtName() only supports CFX_ByteString values.
    // So just set UTF-8 "nameValue" directly.
    if (FSCRT_Util_IsStringEmpty(nameValue))
        m_pAnnotDict->RemoveAt(key);
    else
    {
        CFX_ByteString bstr(nameValue);
        m_pAnnotDict->SetAtName(key, bstr);
    }
    setModified();
    // */
}

CFX_Matrix FROFDAnnotImp::getMatrix(FX_BSTR key) const {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return CFX_Matrix();

    return m_pAnnotDict->GetMatrix(key);
    // */
    return CFX_Matrix();
}

void FROFDAnnotImp::setMatrix(FX_BSTR key, const CFX_Matrix &mt) {
    /* temp comment by gm 20170721
    m_pAnnotDict->SetAtMatrix(key, mt);
    // */
}

FSString FROFDAnnotImp::getName(FX_BSTR key) const {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return FSString("");

    // When setting an entry with Name object as value, we need to use CPDF_Dictioanry::SetAtName() -- which only supports CFX_ByteString values.
    // That means UTF-8 string will be set into dictionary directly.
    // So when getting the value (Name object) of the entry, the value is already in UTF-8 encoding.
    CFX_ByteString bsName = m_pAnnotDict->GetString(key);
    FSString fsRetString(bsName.GetBuffer(bsName.GetLength()));
    bsName.ReleaseBuffer();
    return fsRetString;
    // */
    return FSString("");
}

FSDateTime FROFDAnnotImp::getDateTime(FX_BSTR key) const {
    FSDateTime fsDateTime;
    fsDateTime.set(0, 0, 0, 0, 0, 0, 0, 0, 0);
    if (!m_pAnnotDict /*|| !m_pAnnotDict->KeyExist(key)*/)
        return fsDateTime;
    fxutil::CFX_Unitime docTime;
    if (key == FSANNOT_DATETIMEKEY_MODIFY) {
        docTime = m_pAnnotDict->GetLastModDate();
    } else {
        return fsDateTime;
    }

    fsDateTime.year = docTime.GetYear();
    fsDateTime.month = docTime.GetMonth();
    fsDateTime.day = docTime.GetDay();
    fsDateTime.hour = docTime.GetHour();
    fsDateTime.minute = docTime.GetMinute();
    fsDateTime.second = docTime.GetSecond();
    fsDateTime.milliseconds = docTime.GetMillisecond();
    fsDateTime.UTHourOffset = 0;
    fsDateTime.UTMinuteOffset = 0;

    return fsDateTime;
}

void FROFDAnnotImp::setDateTime(FX_BSTR key, FSDateTime dateTime) {
    /*if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE))
        || !FSCRT_Util_CheckDateTime(dateTime))
        return;
    // */
    if (!m_pAnnotDict) {
        return;
    }
    fxutil::CFX_Unitime docTime;
    // note by gm 20170721, need to deal with timezone
    docTime.Set(dateTime.year, dateTime.month, dateTime.day, dateTime.hour, dateTime.minute,
                dateTime.second, dateTime.milliseconds);

    if (key == FSANNOT_DATETIMEKEY_MODIFY) {
        m_pWriteAnnot->SetLastModDate(docTime);
        setModified();
    }
}

FS_INT32 FROFDAnnotImp::getInteger(FX_BSTR key) const {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return 0;

    return m_pAnnotDict->GetInteger(key);
    // */
    return 0;
}

void FROFDAnnotImp::setInteger(FX_BSTR key, FS_INT32 value) {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return;

    m_pAnnotDict->SetAtInteger(key, value);
    setModified();
    // */
}

FS_FLOAT FROFDAnnotImp::getFloat(FX_BSTR key) const {

    if (!m_pAnnotDict) {
        return 1.0f;
    }
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        LogOut("FOV_Annot::GetOpacity, 111 opacity=%f", 1.0f);
        return 1.0f;
    }

    FX_DWORD alpha = 255;
    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
    int size = ap->CountObjects();
    for (int i = 0; i < size; ++i) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }

        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (!drawParm) {
            continue;
        }
        const COFD_Color *strokeColor = drawParm->GetStrokeColor();
        if (strokeColor) {
            LogOut("FOV_Annot::GetOpacity, stroke opacity=%f", strokeColor->GetAlpha() / 255.0f);
            alpha = strokeColor->GetAlpha();
            break;
        }
        const COFD_Color *fillColor = drawParm->GetFillColor();
        if (fillColor) {
            LogOut("FOV_Annot::GetOpacity, fill opacity=%f", fillColor->GetAlpha() / 255.0f);
            alpha = fillColor->GetAlpha();
            break;
        }
    }
    LogOut("FOV_Annot::GetOpacity, 333 opacity=%f", alpha / 255.0f);
    return alpha / 255.0f;
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return 0.0f;

    return m_pAnnotDict->GetNumber(key);
    */
    return 1.0f;
}

void FROFDAnnotImp::setFloat(FX_BSTR key, FS_FLOAT value) {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return;

    setModified();
    return m_pAnnotDict->SetAtNumber(key, value);
    // */

    LogOut("FOV_Annot::SetOpacity, opacity=%f", value);
    if (!m_pAnnotDict) {
        return;
    }
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        return;
    }
    FX_BYTE alpha = value * 255.0f + 0.5f;
    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();

    int count = ap->CountObjects();
    for (int i = 0; i < count; i++) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }

        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (!drawParm) {
            continue;
        }

        const COFD_Color *strokeColor = drawParm->GetStrokeColor();
        if (strokeColor) {
            COFD_WriteColor *color = OFD_WriteColor_Create(OFD_COLORTYPE_Base,
                                                           const_cast<COFD_Color *>(strokeColor));
//            COFD_WriteBaseColor* color = getWriteColor(strokeColor);
            if (color) {
                color->SetAlpha(alpha);
            }
        }

        const COFD_Color *fillColor = drawParm->GetFillColor();
        if (fillColor) {
            COFD_WriteColor *color = OFD_WriteColor_Create(OFD_COLORTYPE_Base,
                                                           const_cast<COFD_Color *>(fillColor));
//            COFD_WriteBaseColor* color = getWriteColor(fillColor);
            if (color) {
                color->SetAlpha(alpha);
            }
        }
    }
}

FSRectF FROFDAnnotImp::getRect(FX_BSTR key) const {
    FSRectF rectF;
    rectF.set(0, 0, 0, 0);
    CFX_FloatRect fxRect = getRectImp(key);
    if (FALSE == fxRect.IsEmpty())
        rectF.set(fxRect.left, fxRect.bottom, fxRect.right, fxRect.top);
    return rectF;
}

CFX_FloatRect FROFDAnnotImp::getRectImp(FX_BSTR key) const {
    /*
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return CFX_FloatRect();
    return m_pAnnotDict->GetRect(key);
    // */
    if (!m_pAnnotDict) {
        return CFX_FloatRect();
    }
    CFX_FloatRect result;
    if (key == FSANNOT_RECTKEY_RECT) {
        CFX_RectF rect = m_pAnnotDict->GetBoundary();
        CFX_Matrix ofd_2_pdf = ((FR_OFDPageImp *) m_pFSPDFPage)->getFormatMatrix();
        ofd_2_pdf.TransformRect(rect);
        result.left = rect.left;
        result.top = rect.bottom();
        result.right = rect.right();
        result.bottom = rect.top;
        // note by gm need convert to PDF coordinator
    }
    return result;
}

void FROFDAnnotImp::setRect(FX_BSTR key, FSRectF rectF) {
    CFX_FloatRect fxRectF(rectF.left, rectF.bottom, rectF.right, rectF.top);
    setRectImp(key, fxRectF);
}

void FROFDAnnotImp::setRectImp(FX_BSTR key, CFX_FloatRect fxRectF) {
    if (!m_pAnnotDict/* || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE))*/)
        return;

    // moidfy by gm 20170721
    //m_pAnnotDict->SetAtRect(key, fxRectF);
    // note by gm need to convert PDF coordinator to OFD
    CFX_RectF rect;
    rect.Set(fxRectF.left, fxRectF.top, fxRectF.Width(), fxRectF.Height());
    m_pWriteAnnot->SetBoundary(rect);

    setModified();
}

FSPDFDictionary *FROFDAnnotImp::getDict() const {
    /* comment by gm 20170721
    if (!m_pAnnotDict)
        return NULL;

    return (FSPDFDictionary*)m_pAnnotDict;
    */
    return NULL;
}

FS_BOOL FROFDAnnotImp::removeProperty(FSM_ANNOTPROPERTY property) {
    /* comment by gm 20170721
    if (!m_pAnnotDict
        || property<FSAnnot::e_annotPropertyModifiedDate || property>e_annotPropertyFillColor)
        return FALSE;
    FSM_ANNOTTYPE annotType = getType();
    if (!FSANNOT_Util_CheckAnnotIsSupport(annotType))
        return FALSE;

    CFX_ByteString bsKey;
    switch(property)
    {
    case FSAnnot::e_annotPropertyModifiedDate:
        {
            bsKey = FSANNOT_DATETIMEKEY_MODIFY;
        }
        break;
    case FSAnnot::e_annotPropertyCreationDate:
        {
            if(!FSANNOT_Util_CheckAnnotIsMarkup(annotType))
                return FALSE;
            bsKey = FSANNOT_DATETIMEKEY_CREATION;
        }
        break;
    case FSAnnot::e_annotPropertyBorderColor:
        {
            if (FSAnnot::e_annotFreeText == annotType)
            {
                CFX_ByteString bsDA = m_pAnnotDict->GetString(FSANNOT_STRINGKEY_DEFAULTAP);
                if (!bsDA.IsEmpty())
                {
                    CFSAnnot_DefaultAppearance da(bsDA);
                    if (da.HasColor(FALSE))
                    {
                        da.RemoveColor(FALSE);
                        setString(FSANNOT_STRINGKEY_DEFAULTAP,(CFX_ByteString)da);
                        setModified();
                    }
                }
                return TRUE;
            }
            else
                bsKey = FSANNOT_COLORKEY_BORDERCOLOR;
        }
        break;
    case FSAnnot::e_annotPropertyFillColor:
        {
            if(FSAnnot::e_annotSquare != annotType && FSAnnot::e_annotCircle != annotType && FSAnnot::e_annotFreeText != annotType
                && FSAnnot::e_annotLine != annotType && FSAnnot::e_annotPolygon != annotType && FSAnnot::e_annotPolyLine != annotType)
                return FALSE;
            if (FSAnnot::e_annotFreeText == annotType)
                bsKey = FSANNOT_COLORKEY_BORDERCOLOR;
            else
                bsKey = FSANNOT_COLORKEY_FILLCOLOR;
        }
        break;
    default:
        return FALSE;
    }
    if (hasProperty(bsKey))
    {
        m_pAnnotDict->RemoveAt(bsKey);
        setModified();
    }
    return TRUE;
    // */
    return TRUE;
}

CPDF_Dictionary *FROFDAnnotImp::getSubDict(FX_BSTR key, FS_BOOL bCreate) const {
    /* comment by gm 20170721
    if (!m_pAnnotDict)
        return NULL;

    CPDF_Dictionary* pDict = m_pAnnotDict->GetDict(key);
    if (!pDict && bCreate)
    {
        pDict = FX_NEW CPDF_Dictionary;
        if (!pDict)
            return NULL;

        m_pAnnotDict->SetAt(key, pDict);
    }
    return pDict;
    // */
    return NULL;
}

CPDF_Array *FROFDAnnotImp::getArray(FX_BSTR key, FS_BOOL bCreate) const {
    /* comment by gm 20170721
    if (!m_pAnnotDict)
        return NULL;

    CPDF_Array* pArray = m_pAnnotDict->GetArray(key);
    if (!pArray && bCreate)
    {
        pArray = FX_NEW CPDF_Array;
        if (!pArray)
            return NULL;

        m_pAnnotDict->SetAt(key, pArray);
    }
    return pArray;
    // */
    return NULL;
}

FS_BOOL FROFDAnnotImp::setArray(FX_BSTR key, CPDF_Array *pArray) {
    /* comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return FALSE;

    m_pAnnotDict->SetAt(key, pArray);
    setModified();
    return TRUE;
    // */
    return TRUE;
}

FS_BOOL FROFDAnnotImp::getBoolean(FX_BSTR key) const {
    /* comment by gm 20170721
    if (!m_pAnnotDict || !m_pAnnotDict->KeyExist(key))
        return FALSE;

    return m_pAnnotDict->GetBoolean(key);
    // */
    return TRUE;
}

void FROFDAnnotImp::setBoolean(FX_BSTR key, FS_BOOL bStatus) {
    /* comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return;

    m_pAnnotDict->SetAtBoolean(key, bStatus);
    setModified();
    // */
}

FS_BOOL FROFDAnnotImp::getBorderColorImp(FS_ARGB &color/*, FX_BOOL isFromAP*/) const {
    /* comment by gm 20170721
    if (getType() == FSAnnot::e_annotFreeText)
    {
        CFX_ByteString bsDA = m_pAnnotDict->GetString(FSANNOT_STRINGKEY_DEFAULTAP);
        //don't use default color when getting. Just get color from dictionary.
        //color = ArgbEncode(255, 255, 0, 0);

        // Common out following code which is used to get border color from AP stream.
        // This may be an adjustment for fixing a side-effect of rich text in GSDK. So it may not be useful here.
        //CPDF_Dictionary* pAPDict = m_pAnnotDict->GetDict(FSANNOT_DICTKEY_AP);
        //if(pAPDict && isFromAP)
        //{
        //	CPDF_Stream *pStream = pAPDict->GetStream(FSANNOT_STREAMKEY_NORMALAP);
        //	if(pStream)
        //	{
        //		CPDF_StreamAcc acc;
        //		acc.LoadAllData(pStream);

        //		FX_LPCBYTE lpDataOld = acc.GetData();
        //		CFX_ByteString bsStreamOld(lpDataOld, acc.GetSize());

        //		if(!bsStreamOld.IsEmpty())
        //		{
        //			FX_STRSIZE sizeRG = -1;
        //			if((sizeRG = bsStreamOld.Find("RG"))!=-1)
        //			{
        //				CFX_ByteString bsRGbuf = bsStreamOld.Mid(0, sizeRG+2);
        //				CFX_ByteString bsTmpBuf = bsRGbuf;
        //				FX_STRSIZE tmpSize = -1;
        //				FX_INT32 count=0, iOldPos=-1;
        //				for(FX_INT32 i=0; i < bsTmpBuf.GetLength();i++)
        //				{
        //					tmpSize = bsTmpBuf.ReverseFind(' ');
        //					if(tmpSize!=-1)
        //					{
        //						bsTmpBuf = bsTmpBuf.Mid(0,tmpSize);
        //						if(FXSYS_fabs(tmpSize - iOldPos) > 1)
        //							count++;

        //						iOldPos = tmpSize;
        //						if(count>3)
        //						{
        //							bsRGbuf = bsRGbuf.Mid(tmpSize, sizeRG + 2 - tmpSize);
        //							count=3;
        //							break;
        //						}
        //					}
        //					else
        //						break;
        //				}

        //				for(FX_INT32 i=0; i<bsRGbuf.GetLength();i++)
        //				{
        //					FX_CHAR nChar= bsRGbuf.GetAt(i);
        //					if(nChar>='0'&& nChar<='9')
        //					{
        //						bsRGbuf = bsRGbuf.Mid(i, bsRGbuf.GetLength()-i);
        //						break;
        //					}
        //				}
        //				if(count==3)
        //				{
        //					CFSAnnot_DefaultAppearance da(bsRGbuf);
        //					if (da.HasColor(TRUE))
        //					{
        //						FS_INT32 iColorType = 0;// as output parameter
        //						da.GetColor((FX_ARGB&)color, iColorType, TRUE);
        //						return TRUE;
        //					}
        //				}
        //			}
        //		}
        //	}
        //}

        if (!bsDA.IsEmpty())
        {
            CFSAnnot_DefaultAppearance da(bsDA);
            if (da.HasColor(FALSE))
            {
                FS_INT32 iColorType = 0;// as output parameter
                da.GetColor((FX_ARGB&)color, iColorType, FALSE);
                return TRUE;
            }
        }
        return FALSE;
    }
    // */
    return getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, color);
}

void FROFDAnnotImp::setBorderColorImp(FS_ARGB color) {
/*	if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
		return;

	if (getType() == FSAnnot::e_annotFreeText)
	{// For freetext, border color is stored in /DA ( rg) --> rg means non-stroking operation.
		CFX_ByteString sDA = m_pAnnotDict->GetString(FSANNOT_STRINGKEY_DEFAULTAP);

		CFSAnnot_DefaultAppearance da(sDA);
		da.SetColor(color, COLORTYPE_RGB, FALSE);

		m_pAnnotDict->SetAtString(FSANNOT_STRINGKEY_DEFAULTAP, (CFX_ByteString)da);
		//m_bSetFreeTextBorderColor = TRUE; 
	}
	else // For rest annotations, border color is stored in /C.
	// */
    setColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, color);
    setModified();
}

FS_BOOL FROFDAnnotImp::getFillColor(FS_ARGB &color) const {
    color = 0;
    //Fill color of freetext annot is stored in /C, which is mainly used as border color for annots of other types.
    if (getType() == FSAnnot::e_annotFreeText)
        return getColorImpl(FSANNOT_COLORKEY_BORDERCOLOR, color);

    // For rest annotations which have fill color, fill color is stored in /IC
    return getColorImpl(FSANNOT_COLORKEY_FILLCOLOR, color);
}

void FROFDAnnotImp::setFillColor(FS_ARGB color) {
    // modify by gm
    if (!m_pAnnotDict /*|| !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE))*/)
        return;

    FSM_ANNOTTYPE annotType = getType();
    if (annotType != FSAnnot::e_annotFreeText
        && annotType != FSAnnot::e_annotSquare
        && annotType != FSAnnot::e_annotCircle
        && annotType != FSAnnot::e_annotPolyLine
        && annotType != FSAnnot::e_annotPolygon
        && annotType != FSAnnot::e_annotLine)
        return;

    CFX_ByteString bsKey = FSANNOT_COLORKEY_FILLCOLOR;
    // /C of free text is used as its fill color.
    if (FSAnnot::e_annotFreeText == annotType)
        bsKey = FSANNOT_COLORKEY_BORDERCOLOR;

    setColorImpl(bsKey, color);
}

FS_BOOL FROFDAnnotImp::getColorImpl(FX_BSTR key, FS_ARGB &color) const {
    color = 0;
    if (!m_pAnnotDict)
        return FALSE;
    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        return FALSE;
    }
    int size = ap->CountObjects();
    for (int i = 0; i < size; i++) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }

        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (!drawParm) {
            continue;
        }

        const COFD_Color *pColor = NULL;
        if (key == FSANNOT_COLORKEY_FILLCOLOR) {
            pColor = drawParm->GetFillColor();
        } else {
            pColor = drawParm->GetStrokeColor();
        }

        if (pColor) {
            color = pColor->GetARGB();
            return TRUE;
        }
    }
    return FALSE;
}

void FROFDAnnotImp::setColorImpl(FX_BSTR bsKey, FS_ARGB color) {
    if (!m_pAnnotDict)
        return;

    FX_FLOAT r = ((color >> 16) & 0xFF) / 255.0f;
    FX_FLOAT g = ((color >> 8) & 0xFF) / 255.0f;
    FX_FLOAT b = (color & 0xFF) / 255.0f;

    setModified();
    IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (!ap) {
        LogOut("FROFDAnnotImp::setColorImpl,ap: NULL");
        return;
    }
    int size = ap->CountObjects();
    for (int i = 0; i < size; i++) {
        const COFD_ContentObject *obj = ap->GetContentObject(i);
        if (!obj) {
            continue;
        }

        const COFD_DrawParam *drawParm = obj->GetDrawParam(
                pPage->GetDocument()->GetPublicResources());
        if (!drawParm) {
            continue;
        }

        const COFD_Color *pColor = NULL;
        if (bsKey == FSANNOT_COLORKEY_FILLCOLOR) {
            pColor = drawParm->GetFillColor();
        } else {
            pColor = drawParm->GetStrokeColor();
        }

        if (!pColor) {
            continue;
        }

        COFD_WriteBaseColor *writeColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
                OFD_COLORTYPE_Base, const_cast<COFD_Color *>(pColor));
        if (!writeColor) {
            continue;
        }
        switch (pColor->GetColorSpaceType()) {
            case OFD_COLORSPACE_Gray: {
                int r = FXARGB_R(color);
                int g = FXARGB_G(color);
                int b = FXARGB_B(color);
                int gray = FXRGB2GRAY(r, g, b);
                writeColor->SetColorValue(gray);
                break;
            }
            case OFD_COLORSPACE_CMYK: {
                int a = 0xff;
                FX_ARGB argb = ArgbEncode(a, color);
                FX_BYTE c = 255 - FXARGB_R(argb);
                FX_BYTE m = 255 - FXARGB_G(argb);
                FX_BYTE y = 255 - FXARGB_B(argb);
                FX_BYTE k = FX_MIN(c, FX_MIN(m, y));
                FX_CMYK cmyk = FXSYS_CMYK(c, m, y, k);
                writeColor->SetColorValue(cmyk);
                break;
            }
            case OFD_COLORSPACE_RGB:
            default: {
                FX_ARGB argb = color;
                writeColor->SetColorValue(argb);
                break;
            }
        }
    }
    return;
}

FSRectF FROFDAnnotImp::getRotateRect() {
    // modify by gm
    //if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
    if (!m_pAnnotDict)
        return FSRectF();

    FSRectF annotOrgRect = this->getRect();
    FS_FLOAT fOrgWidth = annotOrgRect.right - annotOrgRect.left;
    FS_FLOAT fOrgHeight = annotOrgRect.top - annotOrgRect.bottom;
    FS_ROTATION rotate = getPage()->getRotation();
    FS_FLOAT fRetWidth = 0.0, fRetHeight = 0.0;
    switch (FXSYS_abs(rotate % 360)) {
        case FSCommonDefines::e_rotation0 :
        case FSCommonDefines::e_rotation180 :
        default :
            fRetWidth = fOrgWidth;
            fRetHeight = fOrgHeight;
            break;
        case FSCommonDefines::e_rotation90 :
        case FSCommonDefines::e_rotation270 :
            fRetWidth = fOrgHeight;
            fRetHeight = fOrgWidth;
            break;
    }
    FSRectF retRect;

    retRect.set(0, 0, fRetWidth, fRetHeight);
    return retRect;
}

FS_INT32 FROFDAnnotImp::getRotation() {
    /* comment by gm 20170721
    if (!m_pAnnotDict) return 0;
    CPDF_Dictionary* pMK = m_pAnnotDict->GetDict(FSANNOT_DICTKEY_MK);
    if (!pMK) return 0;
    FS_INT32 rotate = pMK->GetInteger(FX_BSTRC("R"), 0);
    while (rotate < 0)
        rotate += 360;

    return (rotate % 360 / 90) * 90;
    // */
    return 0;
}

FS_ARGB FROFDAnnotImp::getBorderColor() const {
    return getColor(FALSE);
}

void FROFDAnnotImp::setBorderColor(FS_ARGB color) {
    return setColor(color, FALSE);
}

FS_BOOL FROFDAnnotImp::hasProperty(FX_BSTR key) const {
    /* temp comment by gm 20170721
    if(!m_pAnnotDict)
        return FALSE;
    return m_pAnnotDict->KeyExist(key);
    // */
    return TRUE;
}

CPDF_Rect FROFDAnnotImp::getInnerRectImp() {
    CPDF_Rect crRect = getRectImp(FSANNOT_RECTKEY_RECT);
    // note by gm20170721: what is RD ??
    CPDF_Rect crRD = getRectImp(FSANNOT_RECTKEY_RD);
    crRect.Deflate(crRD);
    return crRect;
}

void FROFDAnnotImp::setRectMargin(FSRectF innerRect) {
    /* temp comment by gm 20170721
    if (!m_pAnnotDict || !FSANNOT_Util_CheckAnnotIsSupport(m_pAnnotDict->GetString(FSANNOT_NAMEKEY_SUBTYPE)))
        return;
    CPDF_Rect annotRect = getRectImp(FSANNOT_RECTKEY_RECT);
    CPDF_Rect newInnerRect(innerRect.left, innerRect.bottom, innerRect.right, innerRect.top);

    if (annotRect.Contains(newInnerRect))
    {
        CPDF_Rect rdRect;
        rdRect.left = FXSYS_fabs(annotRect.left - innerRect.left);
        rdRect.bottom = FXSYS_fabs(annotRect.bottom - innerRect.bottom);
        rdRect.right = FXSYS_fabs(annotRect.right - innerRect.right);
        rdRect.top = FXSYS_fabs(annotRect.top - innerRect.top);
        setRectImp(FSANNOT_RECTKEY_RD, rdRect);
    }
    // */
}

FS_BOOL FROFDAnnotImp::getLineEndingStyles(CFX_ByteString &startStyle, CFX_ByteString &endStyle) {
    /* temp comment by gm 20170721
    startStyle = "None";
    endStyle = "None";
    if (!m_pAnnotDict || !hasProperty(FSANNOT_KEY_LINEENDINGSTYLES))
        return FALSE;
    if (FSAnnot::e_annotFreeText == getType())
    {
        CFX_ByteString sIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
        if (0 == sIntent.Compare("FreeTextCallout"))
        {// For callout freetext, try to get data from format "/LE /style" first.
            CFX_ByteString bsLE = m_pAnnotDict->GetString(FSANNOT_KEY_LINEENDINGSTYLES);
            // If /LE value is a name object, get the string and return;
            // If not, try to get data from format "/LE [/style1 /style2]" then.
            if (!bsLE.IsEmpty())
            {
                startStyle = bsLE;
                return TRUE;
            }
        }
    }
    // Get data from format "/LE [/style1 /style2]"
    CPDF_Array* pLEArray = getArray(FSANNOT_KEY_LINEENDINGSTYLES);
    if (!pLEArray || 2 != pLEArray->GetCount())
        return FALSE;
    startStyle = pLEArray->GetString(0);
    endStyle = pLEArray->GetString(1);
    // */
    return TRUE;
}

void FROFDAnnotImp::setLineEndingStyles(const char *newStartStyle, const char *newEndStyle) {
    /* temp comment by gm 20170721
    CFX_ByteString tempStartStyle;
    if (!newStartStyle || FXSYS_strlen(newStartStyle)<1)
        tempStartStyle = "None";
    else
        tempStartStyle = newStartStyle;
    CFX_ByteString tempEndStyle;
    if (!newEndStyle || FXSYS_strlen(newEndStyle)<1)
        tempEndStyle = "None";
    else
        tempEndStyle = newEndStyle;
    if (FSAnnot::e_annotFreeText == getType())
    {
        CFX_ByteString sIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
        if (0 == sIntent.Compare("FreeTextCallout"))
        {// For callout freetext, set as format "/LE /style" only.
            // First remove old data.
            m_pAnnotDict->RemoveAt(FSANNOT_KEY_LINEENDINGSTYLES);
            m_pAnnotDict->SetAtName(FSANNOT_KEY_LINEENDINGSTYLES, tempStartStyle);
            setModified();
            return ;
        }
    }
    // Set as format "/LE [/style1 /style2]"
    CPDF_Array* pLEArray = getArray(FSANNOT_KEY_LINEENDINGSTYLES, TRUE);
    switch(pLEArray->GetCount())
    {
    case 0:
        {
            pLEArray->AddName("None");
            pLEArray->AddName("None");
        }
        break;
    case 1:
        {
            pLEArray->AddName("None");
        }
        break;
    }
    if (newStartStyle)
    {
        CPDF_Name* tempStr = FX_NEW CPDF_Name(tempStartStyle);
        pLEArray->SetAt(0, tempStr);
    }
    if (newEndStyle)
    {
        CPDF_Name* tempStr = FX_NEW CPDF_Name(tempEndStyle);
        pLEArray->SetAt(1, tempStr);
    }
    setModified();
    // */
}


CPDF_Matrix FROFDAnnotImp::getRotateMatrix(FS_INT32 rotate, CFX_FloatRect rect) {
    FX_FLOAT flWidth = rect.right - rect.left;
    FX_FLOAT flHeight = rect.top - rect.bottom;

    CPDF_Matrix matrix;
    switch (rotate) {
        case 90:
            matrix = CPDF_Matrix(0, 1, -1, 0, flWidth, 0);
            break;
        case 180:
            matrix = CPDF_Matrix(-1, 0, 0, -1, flWidth, flHeight);
            break;
        case 270:
            matrix = CPDF_Matrix(0, -1, 1, 0, 0, flHeight);
            break;
        default:
            matrix = CPDF_Matrix(1, 0, 0, 1, 0, 0);
    }
    return matrix;
}

CPDF_Matrix FROFDAnnotImp::getRotateMatrix(/*FX_BOOL bConsiderAnnotRotate*/) {
    if (!getPage() || !getPage()->getDict())
        return CPDF_Matrix(1, 0, 0, 1, 0, 0);
    CFX_FloatRect rect = getRectImp(FSANNOT_RECTKEY_RECT);

    return getRotateMatrix(getPage()->getRotation() * 90, rect);
}

CPDF_Rect
FROFDAnnotImp::getLineEndingStyleStream(FS_BOOL bStartStyle, CFX_ByteString &csAP, FSPointF &point,
                                        FSPointF &direction, FX_FLOAT &fWidth,
                                        FS_BOOL &bNeedFill) {//Only support: Line, Polyline, FreeText(callout)
    /* temp comment by gm 20170721
    bNeedFill = FALSE;
    FSM_ANNOTTYPE annotType = getType();
    FS_FLOAT fHalfWidth = fWidth/2.0f;
    if (FSAnnot::e_annotFreeText == annotType)
    {
        CFX_ByteString bsIntent = getStringImpl(FSANNOT_NAMEKEY_INTENT);
        if (0 != bsIntent.Compare("FreeTextCallout"))
            return CPDF_Rect(0, 0, 0, 0);
        fHalfWidth = fWidth;
    }
    else if (FSAnnot::e_annotLine != annotType && FSAnnot::e_annotPolyLine != annotType)
        return CPDF_Rect(0, 0, 0, 0);

    CFX_ByteString bsStartStyle, bsEndStyle;
    if (FALSE == getLineEndingStyles(bsStartStyle, bsEndStyle))
        return CPDF_Rect(point.x, point.y, point.x, point.y);
    CFX_ByteString bsTempStyle = bsEndStyle;
    if (bStartStyle)
        bsTempStyle = bsStartStyle;

    CPDF_Point tempPoint(point.x, point.y);
    CPDF_Point tempDirection(direction.x, direction.y);
    CPDF_Rect retRect;
    if (0 == bsTempStyle.Compare("Square")){
        retRect = FSAnnot_CreateLineEndingAP_Square(csAP, tempPoint, tempDirection, fWidth);
        bNeedFill = TRUE;
    }
    else if (0 == bsTempStyle.Compare("Circle")){
        retRect = FSAnnot_CreateLineEndingAP_Circle(csAP, tempPoint, fHalfWidth);
        bNeedFill = TRUE;
    }
    else if (0 == bsTempStyle.Compare("Diamond")){
        retRect = FSAnnot_CreateLineEndingAP_Diamond(csAP, tempPoint, tempDirection, fWidth);
        bNeedFill = TRUE;
    }
    else if (0 == bsTempStyle.Compare("OpenArrow"))
        retRect = FSAnnot_CreateLineEndingAP_OpenArrow(csAP, tempPoint, tempDirection, fWidth);
    else if (0 == bsTempStyle.Compare("ClosedArrow")){
        retRect = FSAnnot_CreateLineEndingAP_ClosedArrow(csAP, tempPoint, tempDirection, fWidth);
        bNeedFill = TRUE;
    }
    else if (0 == bsTempStyle.Compare("Butt"))
        retRect = FSAnnot_CreateLineEndingAP_Butt(csAP, tempPoint, tempDirection, fWidth);
    else if (0 == bsTempStyle.Compare("ROpenArrow"))
        retRect = FSAnnot_CreateLineEndingAP_ROpenArrow(csAP, tempPoint, tempDirection, fWidth);
    else if (0 == bsTempStyle.Compare("RClosedArrow")){
        retRect = FSAnnot_CreateLineEndingAP_RClosedArrow(csAP, tempPoint, tempDirection, fWidth);
        bNeedFill = TRUE;
    }
    else if (0 == bsTempStyle.Compare("Slash"))
        retRect = FSAnnot_CreateLineEndingAP_Slash(csAP, tempPoint, tempDirection, fWidth);

    return retRect;
    // */
    return CPDF_Rect();
}

CPDF_Action FROFDAnnotImp::GetAction() const {
    /* temp comment by gm 20170721
    ASSERT(m_pAnnotDict != NULL);

    return m_pAnnotDict->GetDict("A");
    // */
    return CPDF_Action();
}

CPDF_AAction FROFDAnnotImp::GetAAction() const {
    /* temp comment by gm 20170721
    ASSERT(m_pAnnotDict != NULL);

    return m_pAnnotDict->GetDict("AA");
    // */
    return CPDF_AAction();
}

CPDF_Action FROFDAnnotImp::GetAAction(CPDF_AAction::AActionType eAAT) {
    /* temp comment by gm 20170721
    CPDF_AAction AAction = GetAAction();

    if (AAction.ActionExist(eAAT))
    {
        return AAction.GetAction(eAAT);
    }
    else if (eAAT == CPDF_AAction::ButtonUp)
    {
        return GetAction();
    }
    // */
    return NULL;
}

FS_BOOL FROFDAnnotImp::getVertexesImp(CFX_PointsF &vertexes) {
    /* temp comment by gm 20170721
    CPDF_Array* pArray = getArray(FSANNOT_ARRAYKEY_VERTICES);
    if (!pArray) return FALSE;

    FS_DWORD nCount = pArray->GetCount();
    for (FS_DWORD i = 0; i < nCount/2; i++)
    {
        CFX_PointF pt;
        pt.x = pArray->GetNumber(2*i);
        pt.y = pArray->GetNumber(2*i + 1);
        vertexes.Add(pt);
    }
    // */
    return TRUE;
}

FS_BOOL FROFDAnnotImp::setVerticesImp(CFX_PointsF vertexes) {
    /* temp comment by gm 20170721
    FX_INT32 nCount = vertexes.GetSize();
    if (nCount < 1) return FALSE;
    CPDF_Array* pArray = CPDF_Array::Create();
    if (!pArray) return FALSE;
    for (FX_INT32 i = 0; i < nCount; i ++)
    {
        pArray->AddNumber(vertexes[i].x);
        pArray->AddNumber(vertexes[i].y);
    }
    setArray(FSANNOT_ARRAYKEY_VERTICES, pArray);
    // */
    return TRUE;
}

// static method
FS_BOOL FROFDAnnotImp::moveImp(FSAnnot *pAnnot, FSRectF rect, FS_BOOL isResetAp) {
    LogOut("FROFDAnnotImp::moveImp  pAnnot: %x, rect: %x, isResetAp:%d ", pAnnot, rect, isResetAp);
    /* // temp comment by gm
    if (!pAnnot->getDict() || !pAnnot->getPage()
        || !CHECKVALID_FLOATRECT_PDF(rect)) {
            return FALSE;
        }
    FSM_ANNOTTYPE annotType = pAnnot->getType();
    if (!FSANNOT_Util_CheckAnnotIsSupport(annotType)) {
        return FALSE;
    }

    CPDF_Rect rtNew(rect.left, rect.bottom, rect.right, rect.top);
    ((CPDF_Dictionary*)pAnnot->getDict())->SetAtRect(FSANNOT_RECTKEY_RECT, rtNew);
    FS_BOOL bRet = TRUE;
    if (isResetAp)
        bRet = pAnnot->resetAppearanceStream();
    ((FR_OFDPageImp*)pAnnot->getPage())->setModified();
    return bRet;
    // */
    return FALSE;
}

FS_BOOL FROFDAnnotImp::getTransformMatrix(FSRectF newRect, CPDF_Matrix &matrix) {
    if (!CHECKVALID_FLOATRECT_PDF(newRect))
        return FALSE;

    matrix = CPDF_Matrix(1, 0, 0, 1, 0, 0);
    CPDF_Rect rtOld = getRectImp(FSANNOT_RECTKEY_RECT);
    CPDF_Rect rtNew(newRect.left, newRect.bottom, newRect.right, newRect.top);
    FX_FLOAT a = 1.0, b = 1.0;
    if (!rtOld.IsEmpty()) {
        a = rtNew.Width() / rtOld.Width();
        b = rtNew.Height() / rtOld.Height();
    }

    matrix = CPDF_Matrix(a, 0, 0, b, rtNew.left - rtOld.left * a, rtNew.bottom - rtOld.bottom * b);
    return TRUE;
}

FS_BOOL FROFDAnnotImp::transformVertices(FSRectF newRect) {
    /* temp comment by gm
    CPDF_Matrix matrix;
    if (!getTransformMatrix(newRect, matrix))
        return FALSE;
    CFX_PointsF vertices;
    FX_BOOL bRet = getVertexesImp(vertices);
    if (FALSE == bRet) return FALSE;

    FS_INT32 iCount = vertices.GetSize();
    for (FS_INT32 i = 0; i < iCount; i++)
    {
        CFX_PointF &pt = vertices.ElementAt(i);
        matrix.TransformPoint(pt.x, pt.y);
    }
    setVerticesImp(vertices);
    // */
    return TRUE;
}

FS_BOOL FROFDAnnotImp::transformQuadPoints(FSRectF newRect) {
    /* temp comment by gm
    CPDF_Matrix matrix;
    if (!getTransformMatrix(newRect, matrix))
        return FALSE;
    CFX_PointsF quadPoints;
    if (!getQuadPointsImpl(quadPoints))
        return FALSE;

    FX_INT32 iCount = quadPoints.GetSize() / 4 * 4;
    for (FX_INT32 i = 0; i < iCount; i++)
    {
        CFX_PointF &pt = quadPoints.ElementAt(i);
        matrix.TransformPoint(pt.x, pt.y);
    }
    setQuadPointsImpl(quadPoints);
    // */
    return TRUE;
}

FS_INT32 FROFDAnnotImp::getVertexCount() {
    /* temp comment by gm
    CPDF_Array* pArray = getArray(FSANNOT_ARRAYKEY_VERTICES);

    if(!pArray)
        return 0;
    return (FS_INT32)(pArray->GetCount()/2);
    // */
    return 0;
}

FSPointF FROFDAnnotImp::getVertex(FS_INT32 index) {
    FSPointF point;
    /* temp comment by gm
    point.set(0,0);
    FS_INT32 vertexCount = getVertexCount();
    if (!m_pAnnotDict->KeyExist(FSANNOT_ARRAYKEY_VERTICES) || index < 0 || index > vertexCount - 1)
        return point;
    FSPDFAnnot_Path* pVertices = this->getVerticesImp();
    if (pVertices == NULL)
    {
        return point;
    }
    else if (pVertices != NULL && pVertices->GetSize() == 0)
    {
        delete pVertices;
        return point;
    }
    CPDF_Point pPoint = pVertices->GetAt(index);
    point.set(pPoint.x, pPoint.y);

    delete pVertices;
    // */
    return point;
}

void FROFDAnnotImp::setVertexes(const FSPointF *vertexes, FS_INT32 count) {
    /* temp comment by gm
    if (!vertexes || count < 2)
    {
        return;
    }
    CPDF_Array* pArray = CPDF_Array::Create();
    if(!pArray)
        return;
    for(int i=0;i<count; i++)
    {
        pArray->AddNumber(vertexes[i].x);
        pArray->AddNumber(vertexes[i].y);
    }
     this->setArray(FSANNOT_ARRAYKEY_VERTICES, pArray);
     // */
}
/*
FSPDFAnnot_Path* FROFDAnnotImp::getVerticesImp()
{
	FSPDFAnnot_Path* arrayVertices = FX_NEW FSPDFAnnot_Path;
	if (!arrayVertices) 
		return NULL;

	CPDF_Array* pVerticesArray = getArray(FSANNOT_ARRAYKEY_VERTICES); 
	if (pVerticesArray != NULL)
	{
		FX_DWORD dwCount = pVerticesArray->GetCount();
		FX_DWORD i = 0;
		FX_FLOAT fx, fy;
		CPDF_Object* pElement;
		do
		{
			if (i < dwCount)
			{
				pElement = pVerticesArray->GetElementValue(i ++);
				if (pElement->GetType() == PDFOBJ_NUMBER)
					fx = pElement->GetNumber();
				else
					break;
			}
			if (i < dwCount)
			{
				pElement = pVerticesArray->GetElementValue(i ++);
				if (pElement->GetType() == PDFOBJ_NUMBER)
					fy = pElement->GetNumber();
				else
					break;
			}
			arrayVertices->Add(CPDF_Point(fx, fy));
		} while (i < dwCount);

		return arrayVertices;
	}
		return NULL;
}
// */