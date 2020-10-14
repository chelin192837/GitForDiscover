#ifndef _FR_OFDOUTLINEIMP_H_
#define _FR_OFDOUTLINEIMP_H_

#include "../../include/common.h"
#include "../../include/fs_bookmark.h"
#include "fr_ofdutil.h"

class FR_OFDDocImp;

class FR_OFDOutlineImp : public FSBookmark, public CFX_Object
{
public:
	FR_OFDOutlineImp();
	virtual ~FR_OFDOutlineImp();

	void	init(FR_OFDDocImp* doc, FR_OFDOutlineImp* parent, int index, COFD_Outline* node);
	int		countChild();
	FR_OFDOutlineImp*	getChild(int index);

public:
	/**
	 * @brief	Get the parent bookmark.
	 * 
	 * @return	A ::FSBookmark pointer that receives the parent bookmark.
	 */
	virtual FSBookmark*			getParent();

	/** 
	 * @brief	Get the first child bookmark.
	 * 
	 * @return	A ::FSBookmark pointer that receives the first child bookmark.
	 */
	virtual FSBookmark*			getFirstChild();

	/** 
	 * @brief	Get the next sibling bookmark.
	 * 
	 * @return	A ::FSBookmark pointer that receives the next sibling bookmark.
	 */
	virtual FSBookmark*			getNextSibling();

	/** 
	 * @brief	Get the destination of the bookmark.
	 *
	 * @details	If current bookmark dictionary has /Dest or a goto action (/A), this function can get the destination information directly.
	 *			If current bookmark dictionary does not have /Dest or its /A is not a goto action, 
	 *			no destination information can be retrieved and this function will return <b>NULL</b>.
	 *
	 * @return	A ::FSDestination pointer that receives the destionation of current bookmark.
	 */
	virtual FSDestination*		getDestination();

	/** 
	 * @brief	Set the destination.
	 * 
	 * @param[in]	dest	The new destination.
	 *
	 * @return	None.
	 */
	virtual void				setDestination(const FSDestination* dest);

	/** 
	 * @brief	Get the title.
	 * 
	 * @return	A ::FString object receives the title string value, in UTF-8 encoding.
	 */
	virtual FSString	getTitle() const;

	/** 
	 * @brief	Set the title.
	 *
	 * @param[in]	title	New title string. It should be UTF-8 encoding.
	 *
	 * @return	None.
	 */
	virtual void		setTitle(const char* title);

	/**
	 * @brief	Get the color of the bookmark.
	 *
	 * @return	A ::FS_ARGB vaule receives the color used for displaying title. 
	 *			Format: 0xAARRGGBB. Alpha value is ignored and will always be 0xFF by default.
	 */
	virtual FS_ARGB		getColor() const;

	/** 
	 * @brief	Set the color to the bookmark.
	 *
	 * @param[in]	color	Color used for displaying title. Format:0xAARRGGBB.
	 *						Alpha value is ignored and will always be treated as 0xFF internally.
	 *
	 * @return	None.
	 */
	virtual void		setColor(FS_ARGB color);

	/**
	 * @brief	Get the style of the bookmark.
	 *
	 * @return	The bookmark style values. This would be one or a combination of {@link FS_BOOKMARKSTYLE::e_bookmarkStyleNormal FS_BOOKMARKSTYLE::e_bookmarkStyleXXX} values.
	 */
	virtual FS_DWORD	getStyle() const;

	/** 
	 * @brief	Set the style to the bookmark.
	 *
	 * @param[in]	style	The style of the bookmark. This can be one or a combination of {@link FS_BOOKMARKSTYLE::e_bookmarkStyleNormal FS_BOOKMARKSTYLE::e_bookmarkStyleXXX} values.
	 *						If the <i>style</i> is invalid, {@link FS_BOOKMARKSTYLE::e_bookmarkStyleNormal} will be set by default.
	 *
	 * @return	None.
	 */
	virtual void		setStyle(FS_DWORD style);

	/** 
	 * @brief	Insert a new bookmark according to the position.
	 *
	 * @details	For the new bookmark, the color be 0xFF000000 and the style would be {@link FS_BOOKMARKSTYLE::e_bookmarkStyleNormal} by default.<br>
	 *
	 * @param[in]	title	Title string for the new bookmark. It should be UTF-8 encoding.
	 * @param[in]	pos		The position where the new bookmark item is to be inserted, based on current bookmark.
	 *						Please refer to {@link FS_BOOKMARKPOSITION::e_bookmarkPosFirstChild FS_BOOKMARKPOSITION::e_bookmarkPosXXX} values and it would be one of these values.<br>
	 *						If current bookmark is just the "root bookmark", parameter <i>pos</i> can only be {@link FS_BOOKMARKPOSITION::e_bookmarkPosFirstChild} or {@link FS_BOOKMARKPOSITION::e_bookmarkPosLastChild}.
	 *
	 * @return	A new bookmark object.
	 *			If there is any error, this function will return <b>NULL</b>.
	 */
	virtual FSBookmark* insert(const char* title, FS_BOOKMARKPOSITION pos);

	/**
	 * @brief	Move current bookmark to be child or sibling of another bookmark.
	 *
	 * @details	The root bookmark cannot be moved to be child or sibling of any bookmark in the bookmark tree.
	 *
	 * @param[in]	destBookmark	The destination bookmark. It should not be current bookmark itself or any descendant of current bookmark.<br>
	 *								Current bookmark will be moved to be child or sibling of the destination bookmark. 
	 * @param[in]	pos				The position where the current bookmark is to be moved to, based on parameter <i>destBookmark</i>.
	 *								Please refer to {@link FS_BOOKMARKPOSITION::e_bookmarkPosFirstChild FS_BOOKMARKPOSITION::e_bookmarkPosXXX} values and it would be one of these values.
	 *
	 * @return	<b>TRUE</b> means success, while <b>FALSE</b> means failure.
	 */
	virtual FS_BOOL moveTo(FSBookmark* destBookmark, FS_BOOKMARKPOSITION pos);

protected:
	void		initChild();

protected:
	FR_OFDDocImp*		m_Doc;
	FR_OFDOutlineImp*	m_Parent;
	FR_OFDOutlineImp*	m_FirstChild;
	FR_OFDOutlineImp*	m_NextSibling;

	FSDestination*		m_Destination;

protected:
	COFD_Outline*		m_pNode;
	int		m_Index;
	CFX_ArrayTemplate<FR_OFDOutlineImp*>	m_Child;
};


#endif