#ifndef     __OES_LIBRARY_MANAGE_HEADER__
#define    __OES_LIBRARY_MANAGE_HEADER__

class COES_LibraryManage
{
public:
    COES_LibraryManage();
    virtual ~COES_LibraryManage();

public:
    bool    				Load();
    FX_INT32			CountItem();
    CXML_Element*		GetItem(FX_INT32 index);

private:
    CXML_Element*		m_pRoot;
};

#endif //__OES_LIBRARY_MANAGE_HEADER__
