#ifndef         __OFD_BASEHANDLER_HEADER__
#define         __OFD_BASEHANDLER_HEADER__

class COFD_BaseHandler
{
public:
	COFD_BaseHandler(){}
	virtual ~COFD_BaseHandler(){}

public:
    static err_t					Digest(unsigned char* pbToBeHash, FX_DWORD cbToBeHash,
                                                   unsigned char* pbHashedData, int* pcbHashedData,
                                                   const char* pDigestMethod);
};


#endif
