#ifndef _FR_OFDUTIL_H_
#define _FR_OFDUTIL_H_




template <class Type>
class AutoPtr
{
public:
	AutoPtr(Type* p = 0) : m_ptr(0)
	{ attach(p);}
	~AutoPtr()
	{ reset();}

	Type*		get() const
	{ return m_ptr;}

	Type& 		operator*() const
	{ return *m_ptr;}
	Type*		operator->() const
	{ return m_ptr;}

	void		attach(Type* p)
	{
		reset();
		m_ptr = p;
	}
	Type*		detach()
	{
		Type* result = m_ptr;
		m_ptr = 0;
		return result;
	}

	void		reset()
	{
		if (m_ptr) {
			delete m_ptr;
			m_ptr = 0;
		}
	}

protected:
	Type*		m_ptr;
};

template <class Type>
class AutoReleasePtr
{
public:
	AutoReleasePtr(Type* p = 0) : m_ptr(0)
	{ attach(p);}
	~AutoReleasePtr()
	{ reset();}

	Type*		get() const
	{ return m_ptr;}

	Type& 		operator*() const
	{ return *m_ptr;}
	Type*		operator->() const
	{ return m_ptr;}
    
    operator Type*() const
    { return m_ptr;}

	void		attach(Type* p)
	{
		reset();
		m_ptr = p;
	}
	Type*		detach()
	{
		Type* result = m_ptr;
		m_ptr = 0;
		return result;
	}

	void		reset()
	{
		if (m_ptr) {
			m_ptr->Release();
			m_ptr = 0;
		}
	}

protected:
	Type*		m_ptr;

};

class AutoFxMemory
{
public:
	AutoFxMemory(void* p = 0) : m_ptr(p)
	{ }
	~AutoFxMemory()
	{ reset();}

	void*		get() const
	{ return m_ptr;}

	void		attach(void* p)
	{
		reset();
		m_ptr = p;
	}
	void*		detach()
	{
		void* result = m_ptr;
		m_ptr = 0;
		return result;
	}

	void		reset()
	{
		if (m_ptr) {
			FX_Free(m_ptr);
			m_ptr = 0;
		}
	}

protected:
	void*		m_ptr;
};

template <class Type>
class AutoArrayPtr
{
public:
	AutoArrayPtr(Type* p = 0) : m_ptr(0)
	{ attach(p);}
	~AutoArrayPtr()
	{ reset();}

	Type*		get() const
	{ return m_ptr;}

	Type& 		operator*() const
	{ return *m_ptr;}
	Type*		operator->() const
	{ return m_ptr;}

	void		attach(Type* p)
	{
		reset();
		m_ptr = p;
	}
	Type*		detach()
	{
		Type* result = m_ptr;
		m_ptr = 0;
		return result;
	}

	void		reset()
	{
		if (m_ptr) {
			delete[] m_ptr;
			m_ptr = 0;
		}
	}

protected:
	Type*		m_ptr;
};

class IOFD_Page;

class CoordinatorAssistant
{
public:
	CoordinatorAssistant(IOFD_Page* page);

	void			toPDF(const CFX_RectF& src, CFX_FloatRect& dst);
	void			fromPDF(const CFX_FloatRect& src, CFX_RectF& dst);

	void			toPDF(float& x, float& y);
	void			fromPDF(float& x, float& y);

	void			toPDF(float& len);
	void			fromPDF(float& len);

protected:
	float			GetHeight();

protected:
	IOFD_Page*		m_Page;
};

class COFD_SG_Function
{
public:
	static CFX_WideString	GetNoExitFilePath(CFX_WideString& oldFilePath);
	static bool				Ber_Decode_Sealdata(const unsigned char *buffer, const int& buf_size, FX_LPBYTE sealPic, FX_INT32& sealPicLen,FX_LPBYTE sealType = NULL);
	static bool				Ber_Decode_PicSize(const unsigned char *buffer, const int& buf_size, long& width, long& height);

	static void				JumpPtr(FX_LPBYTE  pSealData,FX_INT32 pSealDataLen, FX_INT32 _startpos ,FX_LPBYTE  _outPutData ,FX_INT32 &_inoutPutLen/*in  out*/,FX_INT32 &_pSealDataOffset /*in out*/);
	static void				JumpSealID(FX_LPBYTE  pSealData , FX_INT32 pSealDataLen, FX_INT32 _startpos  , FX_LPBYTE  _outPutData ,FX_INT32 &_inoutPutLen ,FX_INT32 &_pSealDataOffset /*in out*/);
	//this function never use picture type to parse picture
	static void				ParseSealData(FX_LPBYTE psealData/*in*/ ,FX_INT32 psealDataLen /*in*/,FX_INT32 _startpos /*in*/,FX_INT32 & _inoutPutLen/*in out*/,FX_INT32 &psealDataOffset/*in out*/,FX_LPBYTE pSealType = NULL/*in out*/, FX_INT32 nCount = 0);
};



#endif
