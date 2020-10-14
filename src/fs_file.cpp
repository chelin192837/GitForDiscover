#include "fxcore.h"
#include "fs_file.h"

CFSCRT_FileStream::CFSCRT_FileStream(FSFileRead* pFS)
	: m_pFS(pFS)
	, m_nCurPos(0)
	, m_nOffset(0)
	, m_bUseRange(FALSE)
	, m_nSize(0)
{
}

IFX_FileStream* CFSCRT_FileStream::Retain()
{
	return NULL;
}

void CFSCRT_FileStream::Release()
{
	FSCRT_LOCK(m_lock);

	if (m_pFS)
	{
		m_pFS->release();
	}

	FSCRT_UNLOCK(m_lock);

	delete this;
}

FX_FILESIZE CFSCRT_FileStream::GetSize()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if(m_bUseRange)
		return m_nSize;

	if (m_pFS)
	{
		FS_UINT64 size = m_pFS->getSize();
		return (FX_FILESIZE)size;
	}
	return 0;
}

FX_BOOL CFSCRT_FileStream::IsEOF()
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	if (m_pFS)
		return m_nCurPos - m_nOffset >= GetSize();
	return TRUE;
}

FX_BOOL CFSCRT_FileStream::ReadBlock(void* buffer, FX_FILESIZE offset, size_t size)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	if (!buffer || !size) return FALSE;

	if (m_pFS && m_pFS->readBlock(buffer, (FS_UINT64)offset, (FS_DWORD)size))
	{
		m_nCurPos = offset + size;
		return TRUE;
	}

	return FALSE;
}

size_t CFSCRT_FileStream::ReadBlock(void* buffer, size_t size)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)
	if (!buffer || !size) return 0;

	FX_FILESIZE nSize = GetSize();
	if (m_nCurPos - m_nOffset >= nSize) return 0;// Already read m_nCurPos - m_nOffset, 
	FX_FILESIZE dwAvail = nSize - m_nCurPos + m_nOffset; //Available lengths is nSize - (m_nCurPos - m_nOffset)
	if (dwAvail < (FX_FILESIZE)size) size = (size_t)dwAvail;

	if(ReadBlock(buffer, m_nCurPos, size))
		return size;
	return 0;
}

FX_BOOL CFSCRT_FileStream::SetRange(FX_FILESIZE offset, FX_FILESIZE size)
{
	FSCRT_LOCKOBJECT(lock, &m_lock)

	FX_FILESIZE fileSize = 0;
	if (m_pFS)
		fileSize = (FX_FILESIZE)m_pFS->getSize();

	if (offset < 0 || offset + size > fileSize)
		return FALSE;

	m_nOffset = offset;
	m_nCurPos = offset;
	m_nSize = size;
	m_bUseRange = TRUE;
	return TRUE;
}

FX_BOOL CFSCRT_FileStream::WriteBlock(const void* buffer, FX_FILESIZE offset, size_t size)
{
	return FALSE;
}

FX_BOOL CFSCRT_FileStream::Flush()
{
	return FALSE;
}