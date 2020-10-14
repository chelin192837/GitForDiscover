//#include "../../include/fs_pdfpage.h"
#include "fr_ofdannotImp.h"
//#include "fs_annoticonprovider.h"
#include "../fr_ofdpageImp.h"
#include "../../inc/fs_renderImp.h"
#include "../fr_ofddocImp.h"

#include "../../fs_util.h"
#include "../../../include/common.h"
#include "../../inc/commonImp.h"


CFX_Font *GetFontByName(const char *pszFontName) {
    // Courier Helvetica Times | DroidSansFallback
    CFX_ByteString bsFontName(pszFontName);        // 应使用字体名 中文->英文 映射

    if (bsFontName.EqualNoCase("DroidSansFallback")) {
        CFX_Font *pFont = FX_NEW CFX_Font;
        FX_BOOL bLoad = pFont->LoadSubst(bsFontName, TRUE, 0, 0, 0, FXFONT_GB2312_CHARSET);
        if (bLoad)
            return pFont;

        delete pFont;
    } else {
        CFX_Font *pFont = FX_NEW CFX_Font;
        FX_BOOL bLoad = pFont->LoadSubst(bsFontName, TRUE, 0, 0, 0, FXFONT_ANSI_CHARSET);
        if (bLoad)
            return pFont;

        delete pFont;
    }

    return NULL;
}

FX_FLOAT
GlyphWidthFromCharCode(CFX_Font *pFont, IFX_FontEncoding *pFontEncoding, FX_DWORD charcode) {
    if (NULL == pFont)
        return 0.0;

    if (NULL == pFontEncoding)
        return 0.0;

//    FX_DWORD dwGlyph = pFontEncoding->GlyphFromCharCode(charcode);
    FX_DWORD dwGlyph = pFontEncoding->GlyphFromCharCodeEx(charcode);
    FX_FLOAT fGlyphWidth = 0.0;
    if (dwGlyph != 0) {
        fGlyphWidth = (FX_FLOAT) (pFont->GetGlyphWidth(dwGlyph) / 1000.0);
    } else {
//        dwGlyph = pFontEncoding->GlyphFromCharCode(0x31);
        dwGlyph = pFontEncoding->GlyphFromCharCodeEx(0x31);
        FX_FLOAT fGlyphWidth31En = (FX_FLOAT) (pFont->GetGlyphWidth(dwGlyph) / 1000.0);
        fGlyphWidth = fGlyphWidth31En * 2;
    }

    return fGlyphWidth;
//    fWidth += fGlyphWidth * fFontSize;
}

FX_FLOAT
GlyphHeightFromCharCode(CFX_Font *pFont, IFX_FontEncoding *pFontEncoding, FX_DWORD charcode) {
    if (NULL == pFont)
        return 0.0;

    if (NULL == pFontEncoding)
        return 0.0;

    FX_FLOAT fGlyphHeight = 0.0;
    int nAscent = pFont->GetAscent();       // 正值
    int nDescent = pFont->GetDescent();     // 负值
    fGlyphHeight = (FX_FLOAT) ((nAscent - nDescent) / 1000.0);

    return fGlyphHeight;
//    fHeight += fGlyphHeight * fFontSize;
}

FX_FLOAT GetFontAscent(CFX_Font *pFont) {
    if (NULL == pFont)
        return 0.0;

    FX_FLOAT fAscent = 0.0;
    int nAscent = pFont->GetAscent();
    fAscent = (FX_FLOAT) (nAscent / 1000.0);

    return fAscent;
//    fRealAscent = fAscent * fFontSize;
}

FX_FLOAT GetFontDescent(CFX_Font *pFont) {
    // 已转换成正值

    if (NULL == pFont)
        return 0.0;

    FX_FLOAT fDescent = 0.0;
    int nDescent = pFont->GetDescent();
    fDescent = (FX_FLOAT) ((0 - nDescent) / 1000.0);

    return fDescent;
//    fRealDescent = fDescent * fFontSize;
}

FOV_TextHandler::FOV_TextHandler(vector<string> textStringVet, const char *pszFontEn,
                                 const char *pszFontCn, float fontSize) {
    CFX_Font *pFontEn = GetFontByName(pszFontEn);
    CFX_Font *pFontCn = GetFontByName(pszFontCn);

//    IFX_FontEncoding* pFontEncodingEn = FXGE_CreateUnicodeEncoding(pFontEn);
//    IFX_FontEncoding* pFontEncodingCn = FXGE_CreateUnicodeEncoding(pFontCn);

    IFX_FontEncoding *pFontEncodingEn = NULL;
    if (pFontEn)
        pFontEncodingEn = FXGE_CreateUnicodeEncoding(pFontEn);

    IFX_FontEncoding *pFontEncodingCn = NULL;
    if (pFontCn)
        pFontEncodingCn = FXGE_CreateUnicodeEncoding(pFontCn);

//    FX_FLOAT fGlyphWidth = GlyphWidthFromCharCode(pFontEn, pFontEncodingEn, wsText[i]);
//    FX_FLOAT fGlyphHeight = GlyphHeightFromCharCode(pFontEn, pFontEncodingEn, wsText[i]);
//
//    FX_FLOAT fGlyphWidth = GlyphWidthFromCharCode(pFontCn, pFontEncodingCn, wsText[i]);
//    FX_FLOAT fGlyphHeight = GlyphHeightFromCharCode(pFontCn, pFontEncodingCn, wsText[i]);

////    FX_FLOAT fGlypDescentCn = GetFontDescent(pFontCn);
////    FX_FLOAT fGlypDescentMax = max(fGlypDescentEn, fGlypDescentCn);
////    FX_FLOAT fDescent = fGlypDescentMax * m_fontSize;
//    FX_FLOAT fDescent = fGlypDescentEn * fontSize;

    FX_FLOAT fGlypAscentEn = GetFontAscent(pFontEn);
    FX_FLOAT fGlypDescentEn = GetFontDescent(pFontEn);
    FX_FLOAT fAscentEn = fGlypAscentEn * fontSize;
    FX_FLOAT fDescentEn = fGlypDescentEn * fontSize;

    FX_FLOAT fGlypAscentCn = GetFontAscent(pFontCn);
    FX_FLOAT fGlypDescentCn = GetFontDescent(pFontCn);
    FX_FLOAT fAscentCn = fGlypAscentCn * fontSize;
    FX_FLOAT fDescentCn = fGlypDescentCn * fontSize;

    FX_FLOAT fYPos = 0.0;
    FX_FLOAT fLastDescent = 0.0;

    m_bChinese = false;
    m_fontSize = fontSize;

    FX_FLOAT maxLineWidth = 0.0;
    FX_FLOAT allLineHeight = 0.0;

    int nCount = textStringVet.size();
    for (int i = 0; i < nCount; i++) {
        string textString = textStringVet[i];
        CFX_WideString wsTxt = CFX_WideString::FromUTF8(textString.c_str());

        CFX_WideString wsTxtEn;
        CFX_WideString wsTxtCn;

        CFX_FloatArray fArrayDeltaXEn;    // DeltaX
        CFX_FloatArray fArrayDeltaXCn;    // DeltaX

        FX_FLOAT fLastPosEn = 0.0;
        FX_FLOAT fLastPosCn = 0.0;
        FX_FLOAT fLastPos = 0.0;

        bool bFirstEn = true;
        bool bFirstCn = true;

        FX_FLOAT currentLineWidth = 0.0;
        FX_FLOAT maxCurrentLineHeight = 0.0;

        int len = wsTxt.GetLength();
        for (int j = 0; j < len; j++) {
            if (wsTxt[j] < 255)        // 英文字符
            {
                wsTxtEn += wsTxt[j];

                if (bFirstEn) {
                    bFirstEn = false;
                    m_fXStartPosEnVet.push_back(fLastPos);
                } else {
                    FX_FLOAT fDeltaX = fLastPos - fLastPosEn;
                    fArrayDeltaXEn.Add(fDeltaX);
                }
                fLastPosEn = fLastPos;
//                fLastPos += fontSize / 2;
                FX_FLOAT fGlyphWidth = GlyphWidthFromCharCode(pFontEn, pFontEncodingEn, wsTxt[j]);
                FX_FLOAT fGlyphHeight = GlyphHeightFromCharCode(pFontEn, pFontEncodingEn, wsTxt[j]);
                FX_FLOAT fWidth = fGlyphWidth * m_fontSize;
                FX_FLOAT fHeight = fGlyphHeight * m_fontSize;
                currentLineWidth += fWidth;
                maxCurrentLineHeight = max(maxCurrentLineHeight, fHeight);
                fLastPos += fWidth;
            } else {
                if (!m_bChinese)
                    m_bChinese = true;

                wsTxtCn += wsTxt[j];

                if (bFirstCn) {
                    bFirstCn = false;
                    m_fXStartPosCnVet.push_back(fLastPos);
                } else {
                    FX_FLOAT fDeltaX = fLastPos - fLastPosCn;
                    fArrayDeltaXCn.Add(fDeltaX);
                }
                fLastPosCn = fLastPos;
//                fLastPos += fontSize;
                FX_FLOAT fGlyphWidth = GlyphWidthFromCharCode(pFontCn, pFontEncodingCn, wsTxt[j]);
                FX_FLOAT fGlyphHeight = GlyphHeightFromCharCode(pFontCn, pFontEncodingCn, wsTxt[j]);
                FX_FLOAT fWidth = fGlyphWidth * m_fontSize;
                FX_FLOAT fHeight = fGlyphHeight * m_fontSize;
                currentLineWidth += fWidth;
                maxCurrentLineHeight = max(maxCurrentLineHeight, fHeight);
                fLastPos += fWidth;
            }
        }
        if (bFirstEn)
            m_fXStartPosEnVet.push_back(0.0);
        if (bFirstCn)
            m_fXStartPosCnVet.push_back(0.0);

        FX_FLOAT fAscentMax = fAscentEn;
        FX_FLOAT fDescentMax = fDescentEn;
//        if (false == bFirstCn) {
//            fAscentMax = max(fAscentMax, fAscentCn);        // 是用 max，还是用 Ascent+Descent 来比较大小
//            fDescentMax = max(fDescentMax, fDescentCn);
//        }
        if (0 == i) {
            fYPos = fAscentMax;
            fLastDescent = fDescentMax;
        } else {
            fYPos += (fLastDescent + fAscentMax);
            fLastDescent = fDescentMax;
        }

        maxLineWidth = max(maxLineWidth, currentLineWidth);
        allLineHeight += maxCurrentLineHeight;

//        FX_FLOAT fy = fontSize * (i + 1);   // DLYT
//        FX_FLOAT fy = allLineHeight;
//        FX_FLOAT fy = allLineHeight - fDescent;
//        if (0 == i)
//            fYPos = fontSize;
//        else
//            fYPos += maxCurrentLineHeight;
//        FX_FLOAT fy = fYPos;
        FX_FLOAT fy = fYPos;
        m_fYEnVet.push_back(fy);
        m_fYCnVet.push_back(fy);

        m_textEnVet.push_back(wsTxtEn);
        m_textCnVet.push_back(wsTxtCn);

        m_fArrayDeltaXEnVet.push_back(fArrayDeltaXEn);
        m_fArrayDeltaXCnVet.push_back(fArrayDeltaXCn);
    }

    m_newRect.Set(0, 0, maxLineWidth, allLineHeight);

    if (pFontEncodingEn)
        delete pFontEncodingEn;
    if (pFontEncodingCn)
        delete pFontEncodingCn;

    if (pFontEn)
        delete pFontEn;
    if (pFontCn)
        delete pFontCn;
}

FOV_TextHandler::~FOV_TextHandler() {
}


//FROFDFreeTextImp::~FROFDFreeTextImp() {
//    SAFE_DELETE(m_pImage);
//}


FS_BOOL FROFDFreeTextImp::createAppearanceStream() {
    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();

    if (NULL == ap) {
/*        IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
        IOFD_Document *doc = page->GetDocument();
        IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
        COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();
        m_pWriteAnnot = OFD_WriteAnnot_Create(ofdWriteDoc, OFD_ANNOTTYPE_Path, pAnnot);

        // Appearance
        COFD_WriteBlockObject* pAppearance = (COFD_WriteBlockObject*)OFD_WriteContentObject_Create(ofdWriteDoc, OFD_CONTENTTYPE_Block);
//        pWriteAnnot->SetBoundary(ofdRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置


        // ---- PathObject
        COFD_WritePathObject* pPathObj = (COFD_WritePathObject*)OFD_WriteContentObject_Create(ofdWriteDoc, OFD_CONTENTTYPE_Path);
        pAppearance->InsertObject(pPathObj);

//        CFX_RectF pathRect;
//        pathRect.Set(0, 0, ofdRect.width, ofdRect.height);
//        pPathObj->SetBoundary(pathRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

        COFD_WriteDrawParam* pPathWriteDrawParam = (COFD_WriteDrawParam*)OFD_WriteResource_Create(ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
        pPathObj->SetDrawParam(pPathWriteDrawParam);

        pPathWriteDrawParam->SetLineWidth(fLineWidth);

        pPathObj->SetFillState(FALSE);
        COFD_WriteBaseColor* pFillColor = (COFD_WriteBaseColor*)OFD_WriteColor_Create(OFD_COLORTYPE_Base);
        pPathWriteDrawParam->SetFillColor(pFillColor);
        FX_ARGB fillColor = 0x0FFFFFF;
        pFillColor->SetColorValue(fillColor);
        pFillColor->SetAlpha(opacity);      // DSS

        pPathObj->SetStrokeState(FALSE);
        COFD_WriteBaseColor* pStrokeColor = (COFD_WriteBaseColor*)OFD_WriteColor_Create(OFD_COLORTYPE_Base);
        pPathWriteDrawParam->SetStrokeColor(pStrokeColor);
        FX_ARGB strokeColor = 0x0FF0000;
        pStrokeColor->SetColorValue(strokeColor);
        pStrokeColor->SetAlpha(opacity);    // DSS

        // ---- ---- AbbreviatedData
//        pPathObj->SetPath(path);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置


        // ---- PageBlock
        COFD_WriteBlockObject* pPageBlock = (COFD_WriteBlockObject*)OFD_WriteContentObject_Create(ofdWriteDoc, OFD_CONTENTTYPE_Block);
        pAppearance->InsertObject(pPageBlock);


        FOV_TextHandler textHandler(textStringVet, pszFontEn, pszFontCn, fontSize);

        bool bChinese = textHandler.GetIsChinese();

        vector<CFX_WideString > textEnVet = textHandler.GetTextEn();
        vector<FX_FLOAT > fXStartPosEnVet = textHandler.GetXStartPosEn();
        vector<FX_FLOAT > fYEnVet = textHandler.GetYPosEn();
        vector<CFX_FloatArray > fArrayDeltaXEnVet = textHandler.GetDeltaXEn();

        vector<CFX_WideString > textCnVet = textHandler.GetTextCn();
        vector<FX_FLOAT > fXStartPosCnVet = textHandler.GetXStartPosCn();
        vector<FX_FLOAT > fYCnVet = textHandler.GetYPosCn();
        vector<CFX_FloatArray > fArrayDeltaXCnVet = textHandler.GetDeltaXCn();

        CFX_RectF newRect = textHandler.GetNewRect();


        // ----  ---- TextObject EN
        COFD_WriteTextObject* pTextObj = (COFD_WriteTextObject*)OFD_WriteContentObject_Create(ofdWriteDoc, OFD_CONTENTTYPE_Text);
        pPageBlock->InsertObject(pTextObj);

//        CFX_RectF textRect;
//        textRect.Set(0, 0, ofdRect.width, ofdRect.height);
//        pTextObj->SetBoundary(textRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

        pTextObj->SetFont(dwFontIDEn);
        pTextObj->SetFontSize(fontSize);

        COFD_WriteDrawParam* pWDrawParam = (COFD_WriteDrawParam*)OFD_WriteResource_Create(ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
        pTextObj->SetDrawParam(pWDrawParam);

        pTextObj->SetFillState(TRUE);
        COFD_WriteBaseColor* pWColor = (COFD_WriteBaseColor*)OFD_WriteColor_Create(OFD_COLORTYPE_Base);
        pWDrawParam->SetFillColor(pWColor);
        pWColor->SetColorValue(FX_COLORREFTOARGB(color));
        pWColor->SetAlpha(opacity);    // DSS

        int nCount = textEnVet.size();
        for (int i = 0; i < nCount; i++)
        {
            CFX_WideString wsTxt = textEnVet[i];

            if(wsTxt.GetLength() == 1)
                if(wsTxt == L"\n")
                    continue;

            COFD_WriteTextPiece* pTextCode = OFD_WriteTextPiece_Create();

            FX_FLOAT fx = fXStartPosEnVet[i];
            FX_FLOAT fy = fYEnVet[i];

            pTextCode->SetCodes(wsTxt);
            pTextCode->SetStartPosition(fx, fy);

            CFX_FloatArray fArrayDeltaX = fArrayDeltaXEnVet[i];
            int count = fArrayDeltaX.GetSize();
            for (int j = 0; j < count; j ++)
            {
                FX_FLOAT fDeltaX = fArrayDeltaX.GetAt(j);

                pTextCode->InsertDeltaX(fDeltaX);
            }

            FX_INT32 nRet = pTextObj->InsertTextPiece(pTextCode);
        }


        // ----  ---- TextObject CN
        COFD_WriteTextObject* pTextObjCn = NULL;
        if (bChinese)
        {
            pTextObjCn = (COFD_WriteTextObject*)OFD_WriteContentObject_Create(ofdWriteDoc, OFD_CONTENTTYPE_Text);
            pPageBlock->InsertObject(pTextObjCn);

//            CFX_RectF textRect;
//            textRect.Set(0, 0, ofdRect.width, ofdRect.height);
//            pTextObjCn->SetBoundary(textRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

            pTextObjCn->SetFont(dwFontIDCn);
            pTextObjCn->SetFontSize(fontSize);

            COFD_WriteDrawParam* pWDrawParam = (COFD_WriteDrawParam*)OFD_WriteResource_Create(ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
            pTextObjCn->SetDrawParam(pWDrawParam);

            pTextObjCn->SetFillState(TRUE);
            COFD_WriteBaseColor* pWColor = (COFD_WriteBaseColor*)OFD_WriteColor_Create(OFD_COLORTYPE_Base);
            pWDrawParam->SetFillColor(pWColor);
            pWColor->SetColorValue(FX_COLORREFTOARGB(color));
            pWColor->SetAlpha(opacity);    // DSS

            int nCount = textCnVet.size();
            for (int i = 0; i < nCount; i++)
            {
                CFX_WideString wsTxt = textCnVet[i];

                if(wsTxt.GetLength() == 1)
                    if(wsTxt == L"\n")
                        continue;

                COFD_WriteTextPiece* pTextCode = OFD_WriteTextPiece_Create();

                FX_FLOAT fx = fXStartPosCnVet[i];
                FX_FLOAT fy = fYCnVet[i];

                pTextCode->SetCodes(wsTxt);
                pTextCode->SetStartPosition(fx, fy);

                CFX_FloatArray fArrayDeltaX = fArrayDeltaXCnVet[i];
                int count = fArrayDeltaX.GetSize();
                for (int j = 0; j < count; j ++)
                {
                    FX_FLOAT fDeltaX = fArrayDeltaX.GetAt(j);

                    pTextCode->InsertDeltaX(fDeltaX);
                }

                FX_INT32 nRet = pTextObjCn->InsertTextPiece(pTextCode);
            }
        }

        // 设置 新的范围 Boundary
        CFX_RectF newOfdRect;
        newOfdRect.Set(ofdRect.left, ofdRect.top, newRect.width, newRect.height);
        // Appearance
        m_pWriteAnnot->SetBoundary(newOfdRect);       // 根据文字实际宽度调整范围

        // ---- PathObject
        CFX_RectF pathRect;
        pathRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
        pPathObj->SetBoundary(pathRect);            // 根据文字实际宽度调整范围

        // ---- ---- AbbreviatedData
        COFD_Path path;
        path.CreatePath();
        path.MoveTo(pathRect.left, pathRect.top);
        path.LineTo(pathRect.left, pathRect.top + pathRect.height);
        path.LineTo(pathRect.left + pathRect.width, pathRect.top + pathRect.height);
        path.LineTo(pathRect.left + pathRect.width, pathRect.top);
        path.CloseFigure();

        pPathObj->SetPath(path);                    // 根据文字实际宽度调整范围

        // ----  ---- TextObject EN
        CFX_RectF textRect;
        textRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
        pTextObj->SetBoundary(textRect);            // 根据文字实际宽度调整范围

        // ----  ---- TextObject CN
        if (bChinese)
        {
            if (pTextObjCn)
            {
                CFX_RectF textRect;
                textRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
                pTextObjCn->SetBoundary(textRect);  // 根据文字实际宽度调整范围
            }
        }

        m_pWriteAnnot->SetAppearance(pAppearance);
*/
    } else {
//        m_inkList = getInkList();
    }

    return TRUE;
}

FS_BOOL FROFDFreeTextImp::move(FSRectF rect) {
    LogOut("FROFDFreeTextImp::move, rect: l:%f, b:%f, r:%f, t:%f", rect.left, rect.bottom,
           rect.right,
           rect.top);
    FS_BOOL flag = TRUE;

    COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();

    FR_OFDPageImp *page = (FR_OFDPageImp *) m_pFSPDFPage;
    IOFD_Document *doc = page->getPage()->GetDocument();
    IOFD_WriteDocument *writeDoc = OFD_WriteDoc_Create(doc);
    m_pWriteAnnot = OFD_WriteAnnot_Create(writeDoc, OFD_ANNOTTYPE_Path, pAnnot);

    FSRectF temp;
    temp.left = PT_2_MM(rect.left);
    temp.top = PT_2_MM(page->getHeight() - rect.top);
    temp.right = PT_2_MM(rect.right);
    temp.bottom = PT_2_MM(page->getHeight() - rect.bottom);

    CFX_RectF boundary;
    boundary.left = temp.left;
    boundary.top = FX_MIN(temp.top, temp.bottom);
    boundary.width = FXSYS_fabs(temp.right - temp.left);
    boundary.height = FXSYS_fabs(temp.bottom - temp.top);

    LogOut("FROFDFreeTextImp::move, set boundary: l:%f, t:%f, w:%f, h:%f", boundary.left,
           boundary.top,
           boundary.width, boundary.height);

    m_pWriteAnnot->SetBoundary(boundary);

    return flag;


}

FS_BOOL FROFDFreeTextImp::resetAppearanceStream() {
//    CFX_RectF ofdRect;
//    ofdRect.Set(40, 30, 60, 50);
    
    printf("this is a resetAppearanceStream");

//    CFX_RectF boundary = m_pAnnotDict->GetBoundary();
    CFX_RectF ofdRect = m_pAnnotDict->GetBoundary();

    FX_FLOAT fLineWidth = 1.0;
//    FX_BYTE opacity = 255;
    FX_BYTE opacity = static_cast<FX_BYTE>(getOpacity() * 255 + 0.5f);

    // "Courier" "Helvetica" "Times" | "DroidSansFallback"
//    const char * pszFontEn = "Courier";
//    const char * pszFontCn = "DroidSansFallback";
//    float fontSize = 18.0;
//    FX_DWORD color = 0x0FFFF00;

    FSFont *pFont = m_defaultAP.font;
//    FSString fsFont = pFont->getName();

//    CFX_WideString fsFont = ((FSFontImp*)m_defaultAP.font)->getPsName();
//    const char *pszFontEn = fsFont.getBuffer();
    const char *pszFontEn = "Courier";

//    CFX_WideString wsFontNameEn = ((FSFontImp*)pFont)->getPsName();
//    CFX_ByteString bsFontNameEn = wsFontNameEn.UTF8Encode();
//    const char *pszFontEn = (FX_LPCSTR)bsFontNameEn;

//    const char *pszFontCn = "DroidSansFallback";
    
    const char *pszFontCn = "Courier";
    
    float fontSize = PT_2_MM(m_defaultAP.fontSize);
    LogOut("FROFDFreeTextImp::resetAppearanceStream, fontSize=%f",PT_2_MM(m_defaultAP.fontSize));
    FX_DWORD color = m_defaultAP.textColor;
    LogOut("FROFDFreeTextImp::resetAppearanceStream, color=%d",m_defaultAP.textColor);

    vector<string> textStringVet;
    
//    textStringVet.push_back(string("abcd efg"));
//    textStringVet.push_back(string("中国人"));

    FSString textContent = getContent();
    string strText(textContent.getBuffer());

    string strTemp;
    int nTextSize = strText.size();
    for (int i = 0; i < nTextSize; ++i) {
        char ch = strText.at(i);

        if (ch == '\n' || ch == '\r') {
            textStringVet.push_back(strTemp);
            strTemp.clear();
            continue;
        }

        strTemp.push_back(ch);
    }
    textStringVet.push_back(strTemp);

    IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
    IOFD_Document *doc = page->GetDocument();
    IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
    COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();
    m_pWriteAnnot = OFD_WriteAnnot_Create(ofdWriteDoc, OFD_ANNOTTYPE_Path, pAnnot);
    
//    m_pAnnotDict = m_pWriteAnnot->GetReadAnnot();

//    FX_DWORD dwFontIDEn = GetFontIDByName(pWriteDoc, pAnnot, pszFontEn);        // 获取 英文字体资源ID
//    FX_DWORD dwFontIDCn = GetFontIDByName(pWriteDoc, pAnnot, pszFontCn);
//    CFX_WideString wsFontNameEn = CFX_WideString::FromUTF8(pszFontName);
    
    CFX_WideString wsFontNameEn = CFX_WideString::FromUTF8(pszFontEn);
    COFD_WriteFont *pFontResEn = (COFD_WriteFont *) OFD_WriteResource_Create(ofdWriteDoc,
                                                                             OFD_RESOURCETYPE_Font);
    pFontResEn->SetFontName(wsFontNameEn);
    LogOut("FROFDFreeTextImp::resetAppearanceStream, wsFontNameEn=%s",
           (FX_LPCSTR) wsFontNameEn.UTF8Encode());
    pFontResEn->SetCharset(L"unicode");
    FX_DWORD dwFontIDEn = pFontResEn->GetReadResource()->GetID();

    CFX_WideString wsFontNameCn = CFX_WideString::FromUTF8(pszFontCn);
    COFD_WriteFont *pFontResCn = (COFD_WriteFont *) OFD_WriteResource_Create(ofdWriteDoc,
                                                                             OFD_RESOURCETYPE_Font);
    pFontResCn->SetFontName(wsFontNameCn);
    LogOut("FROFDFreeTextImp::resetAppearanceStream, wsFontNameCn=%s",
           (FX_LPCSTR) wsFontNameCn.UTF8Encode());
    pFontResCn->SetCharset(L"unicode");
    FX_DWORD dwFontIDCn = pFontResCn->GetReadResource()->GetID();

    // Appearance
    COFD_WriteBlockObject *pAppearance = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Block);
//  pWriteAnnot->SetBoundary(ofdRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置


    // ---- PathObject
    COFD_WritePathObject *pPathObj = (COFD_WritePathObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Path);
    pAppearance->InsertObject(pPathObj);

//  CFX_RectF pathRect;
//  pathRect.Set(0, 0, ofdRect.width, ofdRect.height);
//  pPathObj->SetBoundary(pathRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

    COFD_WriteDrawParam *pPathWriteDrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(
            ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
    pPathObj->SetDrawParam(pPathWriteDrawParam);

    pPathWriteDrawParam->SetLineWidth(fLineWidth);

    pPathObj->SetFillState(FALSE);
    COFD_WriteBaseColor *pFillColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
            OFD_COLORTYPE_Base);
    pPathWriteDrawParam->SetFillColor(pFillColor);
    FX_ARGB fillColor = 0x0FFFFFF;
    pFillColor->SetColorValue(fillColor);
    pFillColor->SetAlpha(opacity);      // DSS

    pPathObj->SetStrokeState(FALSE);
    COFD_WriteBaseColor *pStrokeColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
            OFD_COLORTYPE_Base);
    pPathWriteDrawParam->SetStrokeColor(pStrokeColor);
    FX_ARGB strokeColor = 0x0FF0000;
    pStrokeColor->SetColorValue(strokeColor);
    pStrokeColor->SetAlpha(opacity);    // DSS

    // ---- ---- AbbreviatedData
//  pPathObj->SetPath(path);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置


    // ---- PageBlock
    COFD_WriteBlockObject *pPageBlock = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Block);
    pAppearance->InsertObject(pPageBlock);


    FOV_TextHandler textHandler(textStringVet, pszFontEn, pszFontCn, fontSize);

    bool bChinese = textHandler.GetIsChinese();

    vector<CFX_WideString> textEnVet = textHandler.GetTextEn();
    vector<FX_FLOAT> fXStartPosEnVet = textHandler.GetXStartPosEn();
    vector<FX_FLOAT> fYEnVet = textHandler.GetYPosEn();
    vector<CFX_FloatArray> fArrayDeltaXEnVet = textHandler.GetDeltaXEn();

    vector<CFX_WideString> textCnVet = textHandler.GetTextCn();
    vector<FX_FLOAT> fXStartPosCnVet = textHandler.GetXStartPosCn();
    vector<FX_FLOAT> fYCnVet = textHandler.GetYPosCn();
    vector<CFX_FloatArray> fArrayDeltaXCnVet = textHandler.GetDeltaXCn();

    CFX_RectF newRect = textHandler.GetNewRect();


    // ----  ---- TextObject EN
    COFD_WriteTextObject *pTextObj = (COFD_WriteTextObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Text);
    pPageBlock->InsertObject(pTextObj);

//  CFX_RectF textRect;
//  textRect.Set(0, 0, ofdRect.width, ofdRect.height);
//  pTextObj->SetBoundary(textRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

    pTextObj->SetFont(dwFontIDEn);
    pTextObj->SetFontSize(fontSize);

    COFD_WriteDrawParam *pWDrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(ofdWriteDoc,
                                                                                        OFD_RESOURCETYPE_DrawParam);
    pTextObj->SetDrawParam(pWDrawParam);

    pTextObj->SetFillState(TRUE);
    COFD_WriteBaseColor *pWColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
            OFD_COLORTYPE_Base);
    pWDrawParam->SetFillColor(pWColor);
//    pWColor->SetColorValue(FX_COLORREFTOARGB(color));
    pWColor->SetColorValue(color);
    pWColor->SetAlpha(opacity);    // DSS

    int nCount = textEnVet.size();
    for (int i = 0; i < nCount; i++) {
        CFX_WideString wsTxt = textEnVet[i];

        if (wsTxt.GetLength() == 1)
            if (wsTxt == L"\n")
                continue;

        COFD_WriteTextPiece *pTextCode = OFD_WriteTextPiece_Create();

        FX_FLOAT fx = fXStartPosEnVet[i];
        FX_FLOAT fy = fYEnVet[i];

        pTextCode->SetCodes(wsTxt);
        pTextCode->SetStartPosition(fx, fy);

        CFX_FloatArray fArrayDeltaX = fArrayDeltaXEnVet[i];
        int count = fArrayDeltaX.GetSize();
        for (int j = 0; j < count; j++) {
            FX_FLOAT fDeltaX = fArrayDeltaX.GetAt(j);

            pTextCode->InsertDeltaX(fDeltaX);
        }

        FX_INT32 nRet = pTextObj->InsertTextPiece(pTextCode);
    }


    // ----  ---- TextObject CN
    COFD_WriteTextObject *pTextObjCn = NULL;
    if (bChinese) {
        pTextObjCn = (COFD_WriteTextObject *) OFD_WriteContentObject_Create(ofdWriteDoc,
                                                                            OFD_CONTENTTYPE_Text);
        pPageBlock->InsertObject(pTextObjCn);

//        CFX_RectF textRect;
//        textRect.Set(0, 0, ofdRect.width, ofdRect.height);
//        pTextObjCn->SetBoundary(textRect);    // 需要注释掉，等根据文字实际宽度调整好范围后，再设置

        pTextObjCn->SetFont(dwFontIDCn);
        pTextObjCn->SetFontSize(fontSize);

        COFD_WriteDrawParam *pWDrawParam = (COFD_WriteDrawParam *) OFD_WriteResource_Create(
                ofdWriteDoc, OFD_RESOURCETYPE_DrawParam);
        pTextObjCn->SetDrawParam(pWDrawParam);

        pTextObjCn->SetFillState(TRUE);
        COFD_WriteBaseColor *pWColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
                OFD_COLORTYPE_Base);
        pWDrawParam->SetFillColor(pWColor);
//        pWColor->SetColorValue(FX_COLORREFTOARGB(color));
        pWColor->SetColorValue(color);
        pWColor->SetAlpha(opacity);    // DSS

        int nCount = textCnVet.size();
        for (int i = 0; i < nCount; i++) {
            CFX_WideString wsTxt = textCnVet[i];

            if (wsTxt.GetLength() == 1)
                if (wsTxt == L"\n")
                    continue;

            COFD_WriteTextPiece *pTextCode = OFD_WriteTextPiece_Create();

            FX_FLOAT fx = fXStartPosCnVet[i];
            FX_FLOAT fy = fYCnVet[i];

            pTextCode->SetCodes(wsTxt);
            pTextCode->SetStartPosition(fx, fy);

            CFX_FloatArray fArrayDeltaX = fArrayDeltaXCnVet[i];
            int count = fArrayDeltaX.GetSize();
            for (int j = 0; j < count; j++) {
                FX_FLOAT fDeltaX = fArrayDeltaX.GetAt(j);

                pTextCode->InsertDeltaX(fDeltaX);
            }

            FX_INT32 nRet = pTextObjCn->InsertTextPiece(pTextCode);
        }
    }

    // 设置 新的范围 Boundary  newRect -->
//    newOfdRect.Set(ofdRect.left, ofdRect.top, newRect.width, newRect.height);

    CFX_RectF newOfdRect;
    newOfdRect.Set(ofdRect.left, ofdRect.top, newRect.width, newRect.height);
//    newOfdRect.Set(ofdRect.left, ofdRect.top, ofdRect.width, ofdRect.height);
    // Appearance
    m_pWriteAnnot->SetBoundary(newOfdRect);       // 根据文字实际宽度调整范围

    // ---- PathObject
    CFX_RectF pathRect;
    pathRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
    pPathObj->SetBoundary(pathRect);            // 根据文字实际宽度调整范围

    // ---- ---- AbbreviatedData
    COFD_Path path;
    path.CreatePath();
    path.MoveTo(pathRect.left, pathRect.top);
    path.LineTo(pathRect.left, pathRect.top + pathRect.height);
    path.LineTo(pathRect.left + pathRect.width, pathRect.top + pathRect.height);
    path.LineTo(pathRect.left + pathRect.width, pathRect.top);
    path.CloseFigure();

    pPathObj->SetPath(path);                    // 根据文字实际宽度调整范围

    // ----  ---- TextObject EN
    CFX_RectF textRect;
    textRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
    pTextObj->SetBoundary(textRect);            // 根据文字实际宽度调整范围

    // ----  ---- TextObject CN
    if (bChinese) {
        if (pTextObjCn) {
            CFX_RectF textRect;
            textRect.Set(0, 0, newOfdRect.width, newOfdRect.height);
            pTextObjCn->SetBoundary(textRect);  // 根据文字实际宽度调整范围
        }
    }

    m_pWriteAnnot->SetAppearance(pAppearance);

    return TRUE;
}

// Function for release in FSFreeText
FS_ARGB FROFDFreeTextImp::getFillColor() const {
    return 0;
}

void FROFDFreeTextImp::setFillColor(FS_ARGB fillColor) {

}

FS_INT32 FROFDFreeTextImp::getAlignment() const {
    return 0;
}

void FROFDFreeTextImp::setAlignment(FS_INT32 alignment) {}

FSRectF FROFDFreeTextImp::getInnerRect() const {
    FSRectF ret;
    return ret;
};

void FROFDFreeTextImp::setInnerRect(FSRectF innerRect) {}

FSDefaultAppearance FROFDFreeTextImp::getDefaultAppearance() {

    FSDefaultAppearance defaultAP;
    defaultAP.set(0, NULL, 0, 0);

    const COFD_BlockObject *ap = m_pAnnotDict->GetAppearance();
    if (ap) {
        IOFD_Page *pPage = ((FR_OFDPageImp *) getPage())->getPage();


        CFX_RectF bound = m_pAnnotDict->GetBoundary();
        int count = ap->CountObjects();
        for (int i = 0; i < count; i++) {
            const COFD_ContentObject *obj = ap->GetContentObject(i);

            OFD_CONTENTTYPE nType = obj->GetContentType();
            if (nType != 1 && nType != 2)
                continue;

            FX_INT32 nCount2 = ((const COFD_ContentLayer *) obj)->CountObjects();
            const COFD_ContentObject *pTextObj = ((const COFD_ContentLayer *) obj)->GetContentObject(
                    0);

            COFD_TextObject *pTextObj2 = (COFD_TextObject *) pTextObj;


            const COFD_DrawParam *drawParm = pTextObj2->GetDrawParam(pPage->GetDocument()->GetPublicResources());
            if (!drawParm) {
                continue;
            }
            const COFD_Color *pColor = NULL;
            pColor = drawParm->GetFillColor();
            if (pColor) {
                FS_ARGB color;
                color = pColor->GetARGB();
                defaultAP.textColor = color;
                LogOut("FROFDFreeTextImp::getDefaultAppearance, color=%d", color);
            } else{
                LogOut("FROFDFreeTextImp::getDefaultAppearance, color=NULL");
            }

            FX_DWORD dwID = pTextObj2->GetFontID();


            COFD_Font *pFont = (COFD_Font *) pPage->GetDocument()->GetPublicResources()->GetResourceByID(
                    dwID);

            FS_STANDARDFONTID wsFont;
            CFX_WideString wsFontName = pFont->GetFontName();

            if (wsFontName.IsEmpty())
                wsFontName = pFont->GetFamilyName();
            if (wsFontName.Equal(CFX_WideString::FromUTF8("Courier"))) {
                wsFont = FSCommonDefines::e_fontStandardIDCourier;
            } else if (wsFontName.Equal(CFX_WideString::FromUTF8("Helvetica"))) {
                wsFont = FSCommonDefines::e_fontStandardIDHelvetica;
            } else if (wsFontName.Equal(CFX_WideString::FromUTF8("Times"))) {
                wsFont = FSCommonDefines::e_fontStandardIDTimes;
            }
            FSFont *font = (FSFont *) FSFont::createStandard(wsFont);
            defaultAP.font = font;
            defaultAP.fontSize = MM_2_PT(pTextObj2->GetFontSize());


        }
    } else{
        defaultAP=m_defaultAP;
        LogOut("FROFDFreeTextImp::getDefaultAppearance, defaultAP==NULL");
    }

    return defaultAP;
};

FS_BOOL FROFDFreeTextImp::setDefaultAppearance(FSDefaultAppearance defaultAP) {
//    if (0 == defaultAP.flags)
//        return TRUE;
//
//    if (defaultAP.flags & FSCommonDefines::e_defaultAPFont && defaultAP.font == NULL)
//        return FALSE;
//
//    if (defaultAP.flags & FSCommonDefines::e_defaultAPFontSize &&
//        (FLOAT_ISBIGGER(0, defaultAP.fontSize) || FLOAT_ISZERO(defaultAP.fontSize)))
//        return FALSE;
//    FSString fsFont = defaultAP.font->getName();
//    const char *pszFontEn = fsFont.getBuffer();
//
//    const char *pszFontCn = "DroidSansFallback";
//
//    float fontSize = PT_2_MM(m_defaultAP.fontSize);
//    FX_DWORD color = m_defaultAP.textColor;
//
//    IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
//    IOFD_Document *doc = page->GetDocument();
//    IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
//
//    COFD_WriteTextObject *pTextObjCn = (COFD_WriteTextObject *) OFD_WriteContentObject_Create(
//            ofdWriteDoc, OFD_CONTENTTYPE_Text);
//    pTextObjCn->SetFontSize(fontSize);
//
//    CFX_WideString wsFontNameCn = CFX_WideString::FromUTF8(pszFontCn);
//    COFD_WriteFont *pFontResCn = (COFD_WriteFont *) OFD_WriteResource_Create(ofdWriteDoc,
//                                                                             OFD_RESOURCETYPE_Font);
//    pFontResCn->SetFontName(wsFontNameCn);
//
//    COFD_WriteBaseColor *pWColor = (COFD_WriteBaseColor *) OFD_WriteColor_Create(
//            OFD_COLORTYPE_Base);
//    pWColor->SetColorValue(color);

    m_defaultAP = defaultAP;

    return TRUE;
};

FSString FROFDFreeTextImp::getCalloutLineEndingStyle() const {
    return NULL;
}

void FROFDFreeTextImp::setCalloutLineEndingStyle(const char *endingStyle) {};

FS_INT32 FROFDFreeTextImp::getCalloutLinePointCount() const {
    return 0;
}

FSPointF FROFDFreeTextImp::getCalloutLinePoint(FS_INT32 index) const {
    FSPointF point ;
    return point;
}

void
FROFDFreeTextImp::setCalloutLinePoints(FSPointF point1, FSPointF point2, FSPointF point3) {}

// Internal use
FROFDFreeTextImp::FROFDFreeTextImp(COFD_Annot *pAnnotDict, FSPDFPage *page) : m_pImage(NULL) {
    FROFDMarkupImp::init(pAnnotDict, page);

    createAppearanceStream();
}

CPDF_Font *FROFDFreeTextImp::getPDFFont(CFX_ByteString sFontName) {
    return NULL;
}

FS_BOOL FROFDFreeTextImp::resetAppearance_TW(FX_BOOL bTextOverflow) {
    return TRUE;
}

void FROFDFreeTextImp::readyAppearance_CO(FX_BOOL bTextOverflow) {

}

FS_BOOL FROFDFreeTextImp::resetAppearance_CO(FX_BOOL bTextOverflow) {
    return TRUE;
}

FS_BOOL FROFDFreeTextImp::resetAppearance_TB(FX_BOOL bTextOverflow) {
    return TRUE;
}

void FROFDFreeTextImp::setStreamExtGState(CPDF_Stream *pStream, const CFX_ByteString &sExtAlias,
                                          FX_FLOAT fOpacity) {}

void FROFDFreeTextImp::transformRectWithTextMatrix(CPDF_Rect &rcText) {}

FS_BOOL FROFDFreeTextImp::hasKneePoint() {
    return TRUE;
};

void
FROFDFreeTextImp::updateCalloutPoints(CPDF_Rect rectBody, FSPointF startPoint,
                                      FX_FLOAT fKneeLength,
                                      FSPointF &pKneePoint, FSPointF &endPoint) {

};

FS_FLOAT FROFDFreeTextImp::getKneeLength() const {
    return 0.0;
}

void FROFDFreeTextImp::adjustPosition(const CPDF_Matrix &mt, const CPDF_Rect &rcBBoxOld,
                                      const CPDF_Rect &rcBBoxNew) {

}

FS_BOOL
FROFDFreeTextImp::correctCallOutTextArea(const CPDF_Rect &rcBBox, const CPDF_Rect &rcRD) {
    return TRUE;
}

CPDF_Rect FROFDFreeTextImp::getBBoxByTextRect(const CPDF_Rect &rcText, FX_BOOL bCalculateKnee) {

    CPDF_Rect rect;
    return rect;
}

CPDF_Rect FROFDFreeTextImp::getTextRect() const {
    CPDF_Rect rect;
       return rect;
}

FS_BOOL FROFDFreeTextImp::getTextColor(FS_ARGB &textColor) {
    return TRUE;
}

FS_BOOL FROFDFreeTextImp::getFontInfo(CFX_ByteString &fontName, FS_FLOAT &fontSize) {
    return TRUE;
}

