#ifndef _FR_OFD_ACTION_IMP_H_
#define _FR_OFD_ACTION_IMP_H_

#include "../../include/common.h"
#include "../../include/fs_action.h"
#include "fr_ofdutil.h"

class FR_OFDDocImp;
class COFD_Dest;

class FR_OFDDestImp : public FSDestination, public CFX_Object
{
public:

	static FR_OFDDestImp*		create(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor);
	static FSDestination*		createFromOFDDest(FR_OFDDocImp* doc, COFD_Dest* dest);

public:
#ifndef _SWIG_IOS_
	/** 
	 * @brief	Release current destination which is created by FSDestination::createXXX functions.
	 *
	 * @details	This can only be called for a newly created destination object by FSDestination::createXXX functions.
	 *			If current destination object is retrieved from other object, please do not call this function.
	 *
	 * @return	None.
	 */
	virtual void release();
#endif

	/** 
	 * @brief	Get the index of the destination page.
	 * 
	 * @return	The page index, starting from 0.
	 *			If there is any error, this function will return -1.
	 */
	virtual FS_INT32 getPageIndex() const;

	/** 
	 * @brief	Get the zoom mode.
	 *
	 * @return	Zoom mode value. Please refer to {@link FS_ZOOMMODE::e_zoomXYZ FS_ZOOMMODE::e_zoomXXX} values and it would be one of them.
	 *			If there is any error, this function will return 0.
	 */
	virtual FS_ZOOMMODE getZoomMode() const;

	/** 
	 * @brief	Get left position value.
	 *
	 * @details	This is only useful when zoom mode is {@link FS_ZOOMMODE::e_zoomXYZ}, {@link FS_ZOOMMODE::e_zoomFitVert}, {@link FS_ZOOMMODE::e_zoomFitRect} or {@link FS_ZOOMMODE::e_zoomFitBVert}.
	 *			For other zoom mode, this function will return 0.0f.
	 *
	 * @return	The left position value.
	 */
	virtual FS_FLOAT getLeft() const;

	/** 
	 * @brief	Get top position value.
	 *
	 * @details	This is only useful when zoom mode is {@link FS_ZOOMMODE::e_zoomXYZ}, {@link FS_ZOOMMODE::e_zoomFitHorz}, {@link FS_ZOOMMODE::e_zoomFitBHorz} or {@link FS_ZOOMMODE::e_zoomFitRect}.
	 *			For other zoom mode, this function will be 0.0f.
	 *
	 * @return	The top position value.
	 */
	virtual FS_FLOAT getTop() const;

	/** 
	 * @brief	Get right position value.
	 *
	 * @details	This is only useful when zoom mode is {@link FS_ZOOMMODE::e_zoomFitRect}.
	 *			For other zoom mode, this function will be 0.0f.
	 *
	 * @return	The right position value.
	 */
	virtual FS_FLOAT getRight() const;

	/** 
	 * @brief	Get bottom position value.
	 *
	 * @details	This is only useful when zoom mode is {@link FS_ZOOMMODE::e_zoomFitRect}.
	 *			For other zoom mode, this function will be 0.0f.
	 *
	 * @return	The bottom position value.
	 */
	virtual FS_FLOAT getBottom() const;

	/** 
	 * @brief	Get zoom factor.
	 *
	 * @details	This is only useful when zoom mode is {@link FS_ZOOMMODE::e_zoomXYZ}.
	 *			For other zoom mode, this function will be 0.0f.
	 *
	 * @return	The zoom factor.
	 */
	virtual FS_FLOAT getZoomFactor() const;

protected:
	FR_OFDDestImp();
	~FR_OFDDestImp();

	void		init(FS_ZOOMMODE zoomMode, FSPDFPage* page, FS_FLOAT left, FS_FLOAT top, FS_FLOAT right, FS_FLOAT bottom, FS_FLOAT zoomFactor);

private:
	FSPDFPage*		m_Page;
	FS_INT32		m_PageIndex;
	FS_ZOOMMODE		m_ZoomMode;
	FS_FLOAT		m_Left;
	FS_FLOAT		m_Top;
	FS_FLOAT		m_Right;
	FS_FLOAT		m_Bottom;
	FS_FLOAT		m_ZoomFactor;
};



#endif