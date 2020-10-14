// test
int OFD_SEAL_Sign_KT(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCWSTR pcwOESName
				, OFD_LPCSTR pPassWD
				, float fXPos
				, float fYPos
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, int nPageSealType
				, int nStartPage
				, int nEndPage
				, int nDoublePrint
				, int nFirstPercent
				, int nPageSealDirection
				, int nPageSealCoordinates
				, OFD_LPCWSTR pcwSignFile		//B64
				); 

//第一种情况，x、y坐标
int OFD_SEAL_Sign_KT_Pos(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, float fXPos
				, float fYPos
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwSignFile		//B64
				);

//第二种情况，关键字（非骑缝章）
int OFD_SEAL_Sign_KT_Key(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, OFD_LPCSTR pRange
				, OFD_LPCSTR pSubRange
				, OFD_LPCWSTR pcwKeyWord
				, int nMatchCase
				, int nMatchWord
				, int nKeyIndex
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				);

//第三种情况，骑缝章
int OFD_SEAL_Sign_KT_Straddle(OFD_LPCWSTR pcwFileName	//B64
				, void* hSeal
				, int nPageSealType
				, int nStartPage
				, int nEndPage
				, int nDuplexPrint
				, int nFirstPagePercent
				, int nPageSealDirection
				, int nPageSealCoordinates
				, int nSealSignType
				, OFD_LPCWSTR pcwSignFile		//B64
				);