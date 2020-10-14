#ifndef _FS_FILE_H_
#define _FS_FILE_H_

#include "fs_util.h"
#include "fxcore.h"
#include "../include/common.h"

class CFSCRT_FileStream : public IFX_FileStream, public CFX_Object
{
public:
	CFSCRT_FileStream(FSFileRead* pFS);

	virtual IFX_FileStream*		Retain();
	virtual void				Release();

	virtual FX_FILESIZE			GetSize();
	virtual FX_BOOL				IsEOF();
	virtual FX_FILESIZE			GetPosition() {FSCRT_LOCKOBJECT(lock, &m_lock) return m_nCurPos;}
	virtual void				SetPosition(FX_FILESIZE pos) {FSCRT_LOCKOBJECT(lock, &m_lock) m_nCurPos = pos; }
	virtual FX_BOOL				SetRange(FX_FILESIZE offset, FX_FILESIZE size);
	virtual void				ClearRange() {m_bUseRange = FALSE; m_nOffset = 0;}
	virtual	FX_BOOL				WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size);
	virtual FX_BOOL				Flush();
	virtual FX_BOOL				ReadBlock(void* buffer, FX_FILESIZE offset, size_t size);
	virtual size_t				ReadBlock(void* buffer, size_t size);
protected:
	virtual ~CFSCRT_FileStream() {}

	FSFileRead*	m_pFS;

	FX_FILESIZE		m_nCurPos; // the position of source file, regardless the Range(m_nOffset to m_nOffset + m_nSize)

	FX_FILESIZE		m_nOffset;
	FX_FILESIZE		m_nSize;

	FX_BOOL			m_bUseRange;
public:
	FSCRT_DEFINELOCK(m_lock)
};

#define FSCRT_MEMORY_READ 0
#define FSCRT_MEMORY_WRITE 1

class CFSCRT_MemoryFile: public IFX_FileStream, public CFX_Object
{
public:
	CFSCRT_MemoryFile(FX_LPVOID buffer, FX_FILESIZE length, FX_INT32 mode = FSCRT_MEMORY_READ):
	  m_buffer(buffer), m_bufLen(length),m_offset(0),m_wroteSize(0) 
	  {
		  m_mode = mode;
	  }
	  
	  virtual IFX_FileStream*	Retain() {return new CFSCRT_MemoryFile(m_buffer, m_bufLen);}
	  virtual void				Release() {delete this;}

	  virtual FX_FILESIZE			GetSize() 
	  {
		  switch(m_mode)
		  {
		  case FSCRT_MEMORY_READ:
			  return m_bufLen;
		  case FSCRT_MEMORY_WRITE:
			  return m_wroteSize;
		  default:
			  return 0;
		  }
		  return 0;
	  }

	  virtual FX_BOOL				IsEOF() {return m_offset == m_bufLen;}
	  virtual FX_FILESIZE			GetPosition() {return m_offset;}

	  virtual FX_BOOL				ReadBlock(void* buffer, FX_FILESIZE offset, size_t size) 
	  {
		  if(offset<0 || offset>m_bufLen-1 || offset+(FX_FILESIZE)size>m_bufLen) return FALSE;
		  m_mode = FSCRT_MEMORY_READ;

		  FXSYS_memcpy(buffer, (FX_BYTE*)m_buffer+offset, size);
		  return TRUE;
	  }

	  virtual size_t				ReadBlock(void* buffer, size_t size) 
	  {
		  if(IsEOF()) return 0;
		  m_mode = FSCRT_MEMORY_READ;

		  if(m_offset+(FX_FILESIZE)size>m_bufLen) size =(size_t) (m_bufLen-m_offset);
		  FXSYS_memcpy(buffer, (FX_BYTE*)m_buffer+m_offset, size);
		  m_offset += size;
		  return size;
	  }

	  virtual	FX_BOOL				WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size)
	  {
		  if(offset<0 || offset+(FX_FILESIZE)size>m_bufLen) return FALSE;
		  m_mode = FSCRT_MEMORY_WRITE;

		  FXSYS_memcpy((FX_BYTE*)m_buffer+offset, buffer, size);
		  m_wroteSize = m_wroteSize>(offset+(FX_FILESIZE)size)?m_wroteSize:(offset+(FX_FILESIZE)size);
		  return TRUE;
	  }

	  virtual	FX_BOOL				WriteBlock(const void* buffer, size_t size)
	  {
		  if(m_offset<0 || m_offset+(FX_FILESIZE)size>m_bufLen) return FALSE;
		  m_mode = FSCRT_MEMORY_WRITE;

		  FXSYS_memcpy((FX_BYTE*)m_buffer+m_offset, buffer, size);
		  m_wroteSize = m_wroteSize>(m_offset+(FX_FILESIZE)size)?m_wroteSize:(m_offset+(FX_FILESIZE)size);
		  m_offset += size;
		  return TRUE;
	  }


	  virtual FX_BOOL				Flush() {return TRUE;}

private:
	FX_FILESIZE m_offset;
	FX_FILESIZE	m_wroteSize;
	FX_FILESIZE m_bufLen;
	FX_LPVOID m_buffer;

	FX_INT32 m_mode;
};

#endif