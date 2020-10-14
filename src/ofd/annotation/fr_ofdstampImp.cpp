//#include "../../include/fs_pdfpage.h"
#include "fr_ofdannotImp.h"
//#include "fs_annoticonprovider.h"
#include "../fr_ofdpageImp.h"
#include "../../inc/fs_renderImp.h"
#include "../fr_ofddocImp.h"

#include "../../fs_util.h"


///////////////////////////////////////
FROFDStampImp::FROFDStampImp(COFD_Annot *pAnnotDict, FSPDFPage *page)
        : m_pImage(NULL) {
    FROFDMarkupImp::init(pAnnotDict, page);
}

FROFDStampImp::~FROFDStampImp() {
    SAFE_DELETE(m_pImage);
}

//////////////
// Methods inherited from FSAnnot
FS_BOOL FROFDStampImp::move(FSRectF rect) {
    LogOut("FROFDStampImp::move, rect: l:%f, b:%f, r:%f, t:%f", rect.left, rect.bottom, rect.right,
           rect.top);
    FS_BOOL flag = TRUE;

    COFD_Annot *pAnnot = m_pWriteAnnot->GetReadAnnot();

    FR_OFDPageImp *page = (FR_OFDPageImp *) m_pFSPDFPage;
    IOFD_Document *doc = page->getPage()->GetDocument();
    IOFD_WriteDocument *writeDoc = OFD_WriteDoc_Create(doc);
    m_pWriteAnnot = OFD_WriteAnnot_Create(writeDoc, OFD_ANNOTTYPE_Stamp, pAnnot);

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

    LogOut("FROFDStampImp::move, set boundary: l:%f, t:%f, w:%f, h:%f", boundary.left, boundary.top,
           boundary.width, boundary.height);

    m_pWriteAnnot->SetBoundary(boundary);

    return flag;
}

FS_BOOL FROFDStampImp::resetAppearanceStream() {
    CFX_RectF rect = m_pAnnotDict->GetBoundary();
    LogOut("FROFDStampImp::resetAppearanceStream, get boundary: l:%f, t:%f, w:%f, h:%f", rect.left,
           rect.top, rect.width, rect.height);

//	FOV_Document* doc = page->getDocument();
    IOFD_Page *page = ((FR_OFDPageImp *) m_pFSPDFPage)->getPage();
    IOFD_Document *doc = page->GetDocument();
    IOFD_WriteDocument *ofdWriteDoc = OFD_WriteDoc_Create(doc);
//	IOFD_WritePageAnnots* ofdWritePageAnnots = ofdWriteDoc->CreatePageAnnots(pageindex);
    COFD_WriteAnnot *ofdWriteAnnot = m_pWriteAnnot;
    //ofdWriteAnnot->SetSubType(L"Screen");
    //ofdWriteAnnot->SetCreator((FX_LPCWSTR)L"HTFX");
    //ofdWriteAnnot->SetPrintable(TRUE);
    //fxutil::CFX_Unitime md;
    //md.Now();
    //ofdWriteAnnot->SetLastModDate(md);

    //ofdWritePageAnnots->InsertAnnot(ofdWriteAnnot);
    COFD_BlockObject *pReadAp = (COFD_BlockObject *) m_pAnnotDict->GetAppearance();
    COFD_ImageObject *pReadImageObj = NULL;
    COFD_Multimedia *pReadImageRes = NULL;
    FX_DWORD dwImage;
    if (pReadAp) {
        for (int i = 0; i < pReadAp->CountObjects(); ++i) {
            COFD_ContentObject *pReadObj = (COFD_ContentObject *) pReadAp->GetContentObject(i);
            if (!pReadObj || pReadObj->GetContentType() != OFD_CONTENTTYPE_Image) { continue; }

            pReadImageObj = (COFD_ImageObject *) pReadObj;
            dwImage = pReadImageObj->GetImageResourceID();
            if (dwImage <= 0 || !doc->GetPublicResources()) { continue; }

            COFD_Resource *res = (COFD_Resource *) doc->GetPublicResources()->GetResourceByID(
                    dwImage);
            if (!res || res->GetResourceType() != OFD_RESOURCETYPE_Multimedia) { continue; }

            pReadImageRes = (COFD_Multimedia *) res;
            break;
        }
    }

    CFX_WideString wsImagepath = CFX_WideString::FromUTF8(m_ImagePath.c_str());
    IOFD_FileStream *fileread = OFD_CreateFileStream((FX_LPCWSTR) wsImagepath,
                                                     OFD_FILEMODE_ReadOnly);
    LogOut("FROFDStampImp::resetAppearanceStream, after new fileread=%x", fileread);
    LogOut("FROFDStampImp::resetAppearanceStream, filepath=%s", m_ImagePath.c_str());
    LogOut("FROFDStampImp::resetAppearanceStream, filepath=%s",
           (FX_LPCSTR) wsImagepath.UTF8Encode());
    if (!fileread) {
        LogOut("FROFDStampImp::resetAppearanceStream, OFD_CreateFileStream return=0x0");
        fileread = pReadImageRes->GetMediaFile();
    }
    CFX_WideString path = fileread->GetCurrentFileName();
    LogOut("FROFDStampImp::resetAppearanceStream, fileread path: %s",
           (FX_LPCSTR) path.UTF8Encode());

    COFD_WriteBlockObject *pAnnotAp = (COFD_WriteBlockObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Block, pReadAp);

    LogOut("FROFDStampImp::resetAppearanceStream, before create resource");
    COFD_WriteMultimedia *imageRes = (COFD_WriteMultimedia *) OFD_WriteResource_Create(ofdWriteDoc,
                                                                                       OFD_RESOURCETYPE_Multimedia,
                                                                                       pReadImageRes);
    imageRes->SetMultimediaType((FX_LPCWSTR) L"Image");
    imageRes->SetMultimediaFormat((FX_LPCWSTR) L"PNG");
    //IFX_FileRead* fileread = FX_CreateFileRead(imagepath.c_str());

    imageRes->SetMediaFile(ofdWriteDoc, fileread);
    if (dwImage) {
        dwImage = imageRes->GetReadResource()->GetID();
    }

    LogOut("FROFDStampImp::resetAppearanceStream, image id=%d", dwImage);

    COFD_WriteImageObject *writeImageObj = (COFD_WriteImageObject *) OFD_WriteContentObject_Create(
            ofdWriteDoc, OFD_CONTENTTYPE_Image, pReadImageObj);
    writeImageObj->SetImageResourceID(dwImage);
    CFX_RectF boundary = rect;
    //ofdWriteAnnot->SetBoundary(boundary);

    boundary.left = 0;
    boundary.top = 0;
    writeImageObj->SetBoundary(boundary);
    CFX_Matrix matrix;
    matrix.Set(boundary.width, 0, 0, boundary.height, 0, 0);
    writeImageObj->SetMatrix(matrix);

    if (!pReadImageObj) {
        pAnnotAp->InsertObject(writeImageObj);
    }

    if (!pReadAp) {
        ofdWriteAnnot->SetAppearance(pAnnotAp);
    }

    LogOut("FROFDStampImp::resetAppearanceStream, end");
    return TRUE;
}

FSString FROFDStampImp::getIconName() const {
    return getName(FSANNOT_NAMEKEY_ICONNAME);
}

void FROFDStampImp::setIconName(const char *iconName) {
    setName(FSANNOT_NAMEKEY_ICONNAME, iconName);
}

void FROFDStampImp::setBitmap(FSBitmap *bitmap) {
    /* comment by gm
    if (!bitmap || !((FSBitmapImp*)bitmap)->getBitmap()
        || !((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument())
        return ;

    if (m_pImage)
        SAFE_DELETE(m_pImage);
    CPDF_Document* pDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
    m_pImage = FX_NEW CPDF_Image(pDoc);
    if (!m_pImage) return ;
    m_pImage->SetImage(((FSBitmapImp*)bitmap)->getBitmap(), PDF_IMAGE_NO_COMPRESS);
    // */
}

void FROFDStampImp::setImagePath(const char *path) {
    LogOut("FROFDStampImp::setImagePath, path=%s", path);
    m_ImagePath = path;
}

//if dwOldNStreamObjNum = 0, it means to XObject of AP stream should be created, currently only support Image.
//else use old N stream object as new AP stream's XObject.
FS_BOOL FROFDStampImp::createStampAPFromImage() {
#if 0    // comment by gm
    FSPDFAnnotImp annotImp = getAnnotImp();
    CPDF_Document* pPDFDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
    CFX_FloatRect fRect =  annotImp.getRectImp(FSANNOT_RECTKEY_RECT);

    FX_FLOAT fWidth = fRect.Width();
    FX_FLOAT fHeight = fRect.Height();

    CPDF_Matrix mt = CPDF_Matrix(1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f);
    FX_INT32 iPageRotate = 	((CPDF_Dictionary*)(getPage()->getDict()))->GetInteger("Rotate");

    CPDF_Rect newBBox;
    newBBox.left = 0.0f;
    newBBox.right = fWidth;
    newBBox.bottom = 0.0f;
    newBBox.top = fHeight;

    CPDF_Rect newBBox90;
    newBBox90.left = newBBox.left;
    newBBox90.bottom = newBBox.bottom;
    newBBox90.right = newBBox.top;
    newBBox90.top = newBBox.right;

    //setIconName("Foxit_RDK_Stamp");

    CPDF_Dictionary* pApDict = annotImp.getSubDict(FSANNOT_DICTKEY_AP, TRUE);
    if (!pApDict) return FALSE;
    //Set /N AP stream dictionary
    CPDF_Stream* pNApStream = FX_NEW CPDF_Stream(NULL, 0, NULL);
    if (!pNApStream) return FALSE;
    CPDF_Dictionary* pNApStreamDict = pNApStream->GetDict();
    if (!pNApStreamDict)
        pNApStreamDict = FX_NEW CPDF_Dictionary;
    if (!pNApStreamDict)
    {
        pNApStream->Release();
        return FALSE;
    }
    pNApStream->InitStream(NULL, 0, pNApStreamDict);

    pNApStreamDict->SetAtName("Type", "XObject");
    pNApStreamDict->SetAtName("Subtype", "Form");
    pNApStreamDict->SetAtInteger("FormType", 1);
    //pNApStreamDict->SetAtName("Name", "Foxit_SDK_Stamp");
    pNApStreamDict->SetAtMatrix("Matrix", mt);
    if(iPageRotate == 90 || iPageRotate == 270)
        pNApStreamDict->SetAtRect("BBox",newBBox90);
    else
        pNApStreamDict->SetAtRect("BBox", newBBox);

    //Set resource of /N ap stream dictionary
    CPDF_Dictionary* pNApResDict = FX_NEW CPDF_Dictionary;
    if (!pNApResDict)
    {
        pNApStream->Release();
        return FALSE;
    }
    pNApStreamDict->SetAt(FX_BSTRC("Resources"), pNApResDict);
    // XObject in Resource of /N
    CPDF_Dictionary* pNApResXObjDict = FX_NEW CPDF_Dictionary;
    if (!pNApResXObjDict)
    {
        pNApStream->Release();
        return FALSE;
    }
    pNApResDict->SetAt(FX_BSTRC("XObject"), pNApResXObjDict);

    //if (dwOldNStreamObjNum == 0)
    //{
    if (m_pImage)
    {
        CPDF_Stream* pNewImgStream = ((CPDF_Image*)m_pImage)->GetStream();
        if (pNewImgStream)
        {
            FX_DWORD dwImgStreamObjNum = pNewImgStream->GetObjNum();
            if (0 == dwImgStreamObjNum)
                dwImgStreamObjNum = pPDFDoc->AddIndirectObject(pNewImgStream);
            pNApResXObjDict->SetAtReference("Im0",pPDFDoc,dwImgStreamObjNum);
        }
        delete (CPDF_Image*)m_pImage;
        m_pImage = NULL;
    }
    //}
    //else
    //{
    //	pNApResXObjDict->SetAtReference("Fm0", pPDFDoc, dwOldNStreamObjNum);
    //}

    // ExtGState in Resource of /N
    CPDF_Dictionary* pNApResExtGSDict = FX_NEW CPDF_Dictionary;
    if (!pNApResExtGSDict)
    {
        pNApStream->Release();
        return FALSE;
    }
    pNApResDict->SetAt(FX_BSTRC("ExtGState"), pNApResExtGSDict);
    // /TransGs of ExtGState
    CPDF_Dictionary* pTransGs = FX_NEW CPDF_Dictionary;
    if (!pTransGs)
    {
        pNApStream->Release();
        return FALSE;
    }
    FX_DWORD dwTransGs = pPDFDoc->AddIndirectObject(pTransGs);
    pNApResExtGSDict->SetAtReference(FX_BSTRC("TransGs"), pPDFDoc, dwTransGs);
    //Set dictionary of TransGs
    pTransGs->SetAtName("Type", "ExtGState");
    // Use 1 as opacity in the new image ap stream, because stamp's AP structure will be changed by FROFDStampImp::updateStampAP() then and the opacity will be use there.
    pTransGs->SetAtNumber("CA", 1);
    pTransGs->SetAtNumber("ca", 1);

    CFX_ByteString csCm= "";
    if(iPageRotate == 90)
    {
        csCm.Format("0 1 -1 0 %.3f 0 cm\n",newBBox90.right);
    }
    else if(iPageRotate == 180)
    {
        csCm.Format("-1 0 0 -1 %.3f %.3f cm\n",newBBox.right,newBBox.top);
    }
    else if(iPageRotate == 270)
    {
        csCm.Format("-0 -1 1 -0 0 %.3f cm\n",newBBox90.top);
    }

    CFX_ByteString cs = "/TransGs gs\n";
    CFX_ByteString sStream;
    CFX_FloatRect rcBBox = pNApStreamDict->GetRect("BBox");
    if (iPageRotate == 90 || iPageRotate == 270)
        sStream.Format("q %f 0 0 %f 0 0 cm", rcBBox.Height(), rcBBox.Width());
    else
        sStream.Format("q %f 0 0 %f 0 0 cm", rcBBox.Width(), rcBBox.Height());
    sStream += " /Im0 Do Q\n";
    /*if (dwOldNStreamObjNum == 0)
        sStream += " /Im0 Do Q\n";
    else if (TRUE == m_bChangeSize)
        sStream += " /Fm0 Do Q\n";
    else
        sStream = "/Fm0 Do \n";*/
    sStream = csCm + cs + sStream;
    pNApStream->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);

    FX_DWORD dwNApStream = pPDFDoc->AddIndirectObject(pNApStream);
    pApDict->SetAtReference("N", pPDFDoc, dwNApStream);
    ((CPDF_Dictionary*)getDict())->SetAt(FX_BSTRC("AP"), pApDict, pPDFDoc);
#endif
    return TRUE;
}

FS_BOOL FROFDStampImp::updateStampAP() {
#if 0
    // When update stamp ap, currently only update opacity information. (Maybe, need to take care of BBox and Matrix later).
    // First, Check if current /N stream has "MWFOForm" XObject.
    // If yes, update /ExtGState of current /N stream directly.
    // If no, need to create a new N stream, which contains "MWFOForm" XObject, and "MWFOForm" XObject is a stream which contains current /N stream as XObject.
    // Any way, finally the /N stream should be like:
    // <</N 100 0 R>> -- annot's /AP dict.
    // 100 0 obj << /Resources << /XObject << /MWFOForm 110 0 R>> >> >> -- a new stream object, as the new /N stream.
    // 110 0 obj << /Resources << /XObject << /Form 36 0 R>> >> >> -- a new stream object, as /MWFOForm XObject in 100 0 obj.
    // 36 0 obj << >> -- old /N stream
    CPDF_Document* pPDFDoc = ((FSPDFDocImp*)getPage()->getDocument())->getPDFDocument();
    if (!pPDFDoc) return FALSE;
    CPDF_Dictionary* pAnnotDict = (CPDF_Dictionary*)getDict();
    CPDF_Dictionary* pApDict = pAnnotDict->GetDict(FSANNOT_DICTKEY_AP);
    if (!pApDict) return FALSE;
    CPDF_Stream* pOldNStream = pApDict->GetStream(FSANNOT_STREAMKEY_NORMALAP);
    if (!pOldNStream) return FALSE;
    CPDF_Dictionary* pOldNDict = pOldNStream->GetDict();
    if (!pOldNDict) return FALSE;

    // Get /CA value in current annot dictionary.
    FS_FLOAT fOpacity = getOpacity();

    // Check if /MWFOForm exists in current N stream dictionary.
    if (!FSAnnot_Util_HasMWFOFormInNAP(pOldNDict))
    {// No /MWFOForm, so change the N stream structure --  to add 2 extra XObject stream between annot dictionary and old N stream.
        // Following code will try to create the similar new N stream structure as phantom.

        // Get old N stream's BBox
        CPDF_Rect oldBBox = pOldNDict->GetRect(FSANNOT_RECTKEY_BBOX);

        // Prepare new N ap stream, which contains "MWFOForm" XObject in Resources.
        CPDF_Stream* pNewNStream = FSUT_CreateFormXObject();
        if (!pNewNStream) return FALSE;
        CPDF_Dictionary* pNewNStreamDict = pNewNStream->GetDict();
        // Set matrix and bbox in new N ap dictionary.
        CPDF_Matrix matrix = pOldNStream->GetDict()->GetMatrix(FSANNOT_ARRAYKEY_MATRIX);
        pNewNStreamDict->SetAtMatrix(FSANNOT_ARRAYKEY_MATRIX, matrix);
        pNewNStreamDict->SetAtRect(FSANNOT_RECTKEY_BBOX, oldBBox);
        // Set /Resources in new N ap dictionary.
        // To create "/Resources << /XObject << >> /ExtGState << >> >>"
        CPDF_Dictionary* pNewResDict = FX_NEW CPDF_Dictionary;
        if (!pNewResDict)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pNewNStreamDict->SetAt(FSANNOT_DICTKEY_RESOURCES, pNewResDict);
        // Set /ExtGState in /Resources of new N ap dictionary.
        // To create "/ExtGState << /GS0 x x R>>"
        CFX_ByteString bsExtGStateName = "GS0";
        CPDF_Dictionary* pExtGStateDict = FX_NEW CPDF_Dictionary;
        if (!pExtGStateDict)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pNewResDict->SetAt(FSANNOT_DICTKEY_EXTGSTATE, pExtGStateDict);
        CPDF_Dictionary* pNewExt = FSUT_CreateExtGStateDict(fOpacity);
        FS_DWORD dwNewExtObjNum = pPDFDoc->AddIndirectObject(pNewExt);
        pExtGStateDict->SetAtReference(bsExtGStateName, pPDFDoc, dwNewExtObjNum);
        // Set /XObject (with name "MWFOForm") in /Resources of new N ap dictionary
        // To create "/XObject << /MWFOForm x x R>>"
        CFX_ByteString bsXObjName = "MWFOForm";
        CPDF_Dictionary* pXObjDict = FX_NEW CPDF_Dictionary;
        if (!pXObjDict)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pNewResDict->SetAt("XObject", pXObjDict);
        CPDF_Stream* pMWFOFormXObjStream = FSUT_CreateFormXObject();
        FS_DWORD dwNewXObjNum = pPDFDoc->AddIndirectObject(pMWFOFormXObjStream);
        pXObjDict->SetAtReference(bsXObjName, pPDFDoc, dwNewXObjNum);
        // Set new XObject, as "MWFOForm" in new N ap dictionary's /Resources
        CPDF_Dictionary* pMWFOFormStreamDict = pMWFOFormXObjStream->GetDict();
        CPDF_Matrix matrix_2 = CPDF_Matrix(1, 0, 0, 1, 0, 0);
        pMWFOFormStreamDict->SetAtMatrix(FSANNOT_ARRAYKEY_MATRIX, matrix_2);
        pMWFOFormStreamDict->SetAtRect(FSANNOT_RECTKEY_BBOX, oldBBox);
        //Set "/Group << /Type /Group /S /Transparency >>" in the XObject.
        CPDF_Dictionary* pGroupDict = FX_NEW CPDF_Dictionary;
        if (!pGroupDict)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pGroupDict->SetAtName("Type", "Group");
        pGroupDict->SetAtName("S", "Transparency");
        pMWFOFormStreamDict->SetAt("Group", pGroupDict);

        CPDF_Dictionary* pNewResDict_2 = FX_NEW CPDF_Dictionary;
        if (!pNewResDict_2)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pMWFOFormStreamDict->SetAt(FSANNOT_DICTKEY_RESOURCES, pNewResDict_2);
        // Set /XObject for /Resources in N ap dict -- old N stream will be this XObject.
        CFX_ByteString bsXObjName_2 = "Form";
        CPDF_Dictionary* pXObjDict_2 = FX_NEW CPDF_Dictionary;
        if (!pXObjDict_2)
        {
            pNewNStream->Release();
            return FALSE;
        }
        pNewResDict_2->SetAt("XObject", pXObjDict_2);
        // Change matrix of old N stream to [1 0 0 1 0 0]
        pOldNStream->GetDict()->SetAtMatrix(FSANNOT_ARRAYKEY_MATRIX, matrix_2);
        FS_DWORD dwOldNStreamObjNum = pOldNStream->GetObjNum();
        pXObjDict_2->SetAtReference(bsXObjName_2, pPDFDoc, dwOldNStreamObjNum);
        // Set stream data
        CFX_ByteString sStream_2;
        sStream_2.Format("/%s Do\n", (FX_LPCSTR)bsXObjName_2);
        pMWFOFormXObjStream->SetData((FX_LPCBYTE)sStream_2, sStream_2.GetLength(), FALSE, FALSE);

        // Set stream data, for new N ap stream.
        CFX_ByteString sStream;
        sStream.Format("/%s gs\n/%s Do\n", (FX_LPCSTR)bsExtGStateName, (FX_LPCSTR)bsXObjName);
        pNewNStream->SetData((FX_LPCBYTE)sStream, sStream.GetLength(), FALSE, FALSE);

        // Set new /N.
        pApDict->RemoveAt(FSANNOT_STREAMKEY_NORMALAP);
        FS_DWORD dwNewNObjNum = pPDFDoc->AddIndirectObject(pNewNStream);
        pApDict->SetAtReference(FSANNOT_STREAMKEY_NORMALAP, pPDFDoc, dwNewNObjNum);
    }
    else
    {// /N stream already has "MWFOForm" /XObject, so update opacity information. Currently no need to update /BBox and /Matrix yet.
        CPDF_Dictionary* pNResDict = pOldNStream->GetDict()->GetDict(FSANNOT_DICTKEY_RESOURCES);
        CPDF_Dictionary* pExtGStateDict = pNResDict->GetDict(FSANNOT_DICTKEY_EXTGSTATE);
        FX_POSITION pos = pExtGStateDict->GetStartPos();
        while(pos)
        {
            CFX_ByteString bsKey;
            CPDF_Object* pObj = pExtGStateDict->GetNextElement(pos, bsKey);
            if (pObj && PDFOBJ_REFERENCE == pObj->GetType())
            {
                CPDF_Object* pDirect = (CPDF_Dictionary*)pObj->GetDirect();
                if (pDirect && PDFOBJ_DICTIONARY == pDirect->GetType()
                    && ((CPDF_Dictionary*)pDirect)->GetString("Type").Equal("ExtGState"))
                {
                    ((CPDF_Dictionary*)pDirect)->SetAtNumber("CA", fOpacity);
                    ((CPDF_Dictionary*)pDirect)->SetAtNumber("ca", fOpacity);
                }
            }
        }
    }
#endif
    return TRUE;
}
