#ifndef _FS_OFDBARCODE_H_
#define _FS_OFDBARCODE_H_

class CFS_OFDBarcode;

class CFS_OFDBarcode : public CFX_Object
{
public:
	CFS_OFDBarcode();
	~CFS_OFDBarcode();

	static int Encode(CFX_WideString wsContent, int bcFormat, OFD_LPCWSTR imgFile,int &width, int &height,CFX_WideString wsPassword);

	static int Decode(CFX_WideString wsImgFile, int& bcFormat, CFX_WideString wsPassword, OFD_WSTR* wContent);

protected:

};

#endif //_FS_OFDBARCODE_H_
