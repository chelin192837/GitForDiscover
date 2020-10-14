#include "../include/oes_dll.h"
#include "../include/OES_LibraryManage.h"

COES_LibraryManage::COES_LibraryManage():
    m_pRoot(NULL)
{

}

COES_LibraryManage::~COES_LibraryManage()
{
    delete m_pRoot;
}

bool COES_LibraryManage::Load()
{
    CFX_WideString wsSealListPath = L"/.oes_lib/htfoxit/seallist.xml";
    IFX_FileRead* pFileRead = FX_CreateFileRead(wsSealListPath);
    if (pFileRead)
    {
        int nCount = pFileRead->GetSize();
        FX_BYTE * pBuf = new FX_BYTE[nCount];
        int data_size = pFileRead->ReadBlock(pBuf, nCount);
        if( data_size > 0)
        {
            std::string strKey = COFD_Utils::GetSeedSHA256();
            CRYPT_ArcFourCryptBlock(pBuf, data_size, (FX_LPCBYTE)strKey.data(), strKey.length());
            m_pRoot = CXML_Element::Parse(pBuf,data_size);
        }
        delete[] pBuf;

        pFileRead->Release();
    }

    return m_pRoot != NULL;
}

FX_INT32 COES_LibraryManage::CountItem()
{
    if (!m_pRoot) return 0;

    CFX_ByteString csTagName = m_pRoot->GetTagName();
    CFX_WideString wsTagName = CFX_WideString::FromLocal(csTagName);
    if (!wsTagName.Equal(OES_LIBRARY_SEALS_NODE)) return 0;

    return m_pRoot->CountChildren();
}

CXML_Element* COES_LibraryManage::GetItem(FX_INT32 index)
{
    if (CountItem() > index)
    {
        return m_pRoot->GetElement(index);
    }

    return NULL;
}
