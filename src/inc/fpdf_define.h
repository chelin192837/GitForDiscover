#ifndef _FPDF_DEFINE_H_
#define _FPDF_DEFINE_H_

#include "fs_precompilers.h"
#include "../fxcore.h"
#include "../fs_util.h"

#include "../../include/common.h"
#include "../../include/fs_annot.h"
#include "../../include/fs_pdfdoc.h"
#include "../../include/fs_pdfpage.h"
#include "../../include/fs_render.h"
#include "../../include/fs_formfiller.h"

#include "../inc/fs_pdfdocImp.h"
#include "../inc/fs_pdfpageImp.h"
#include "../inc/fs_renderImp.h"
#include "../annotation/fs_annot_imp.h"
#include "../form/fpdf_stactionhandler.h"
#include "../form/fpdf_stinterform.h"
#include "../form/fs_formfillerImp.h"
#include "../form/fpdf_annothandler.h"
#include "../form/fpdf_ltwidgethandler.h"
#include "../form/javascript/js_interface.h"
#include "../form/fs_pdfformImp.h"

#define FSCRT_ERRCODE_ST_OOM			0x80000000
#define FSCRT_ERRCODE_UNSUPPORTED		-16

//macros for flags of annotation
#define FPDFANNOT_FLAG_Invisible		0x0001
#define FPDFANNOT_FLAG_Hidden			0x0002
#define FPDFANNOT_FLAG_Print			0x0004
#define FPDFANNOT_FLAG_NoZoom			0x0008
#define FPDFANNOT_FLAG_NoRotate			0x0010
#define FPDFANNOT_FLAG_NoView			0x0020
#define FPDFANNOT_FLAG_ReadOnly			0x0040
#define FPDFANNOT_FLAG_Locked			0x0080
#define FPDFANNOT_FLAG_ToggleNoView		0x0100
#define FPDFANNOT_FLAG_LockedContents	0x0200

//macros for border types of annotation
#define FPDFANNOT_BORDER_Solid			0
#define FPDFANNOT_BORDER_Dashed			1
#define FPDFANNOT_BORDER_Underline		2
#define FPDFANNOT_BORDER_Beveled		3
#define FPDFANNOT_BORDER_Inset			4

/** @brief	The type of application forms version information. */
#define FSPDF_APPINFOTYPE_FORMSVERSION		0x0

/** @brief	The type of application viewer type information. */
#define FSPDF_APPINFOTYPE_VIEWERTYPE		0x1

/** @brief	The type of application viewer variation information. */
#define FSPDF_APPINFOTYPE_VIEWERVARIATION	0x2

/** @brief	The type of application viewer version information. */
#define FSPDF_APPINFOTYPE_VIEWERVERSION		0x3

/** @brief	The type of application app version information. */
#define FSPDF_APPINFOTYPE_APPVERSION		0x4


#ifndef _FSCRT_DEF_MACRO_LANGUAGE_
#define _FSCRT_DEF_MACRO_LANGUAGE_
/**
 * @name	Macro Definitions for Language
 */
/**@{*/

/** @brief	Unknown language. */
#define	FSCRT_LANGUAGE_UNK		0
/** @brief	Chinese Simplified. */
#define	FSCRT_LANGUAGE_CHS		1
/** @brief	Chinese Traditional. */
#define	FSCRT_LANGUAGE_CHT		2
/** @brief	Danish. */
#define	FSCRT_LANGUAGE_DAN		3
/** @brief	German. */
#define	FSCRT_LANGUAGE_DEU		4
/** @brief	English. */
#define	FSCRT_LANGUAGE_ENU		5
/** @brief	Spanish. */
#define	FSCRT_LANGUAGE_ESP		6
/** @brief	French. */
#define	FSCRT_LANGUAGE_FRA		7
/** @brief	Italian. */
#define	FSCRT_LANGUAGE_ITA		8
/** @brief	Korean. */
#define	FSCRT_LANGUAGE_KOR		9
/** @brief	Japanese. */
#define	FSCRT_LANGUAGE_JPN		10
/** @brief	Dutch. */
#define	FSCRT_LANGUAGE_NLD		11
/** @brief	Norwegian. */
#define	FSCRT_LANGUAGE_NOR		12
/** @brief	Brazilian Portuguese. */
#define	FSCRT_LANGUAGE_PTB		13
/** @brief	Finnish. */
#define	FSCRT_LANGUAGE_SUO		14
/** @brief	Swedish. */
#define	FSCRT_LANGUAGE_SVE		15

/**@}*/
#endif /* _FSCRT_DEF_MACRO_LANGUAGE_ */

#ifndef _FSPDF_DEF_MACRO_PERMISSION_
#define _FSPDF_DEF_MACRO_PERMISSION_
/**
 * @name	Macro Definitions for PDF Document Permissions
 * @note 	According to PDF Reference, a 32-bit integer is used to represent permissions.<br>
 * 			Bits in permission value, 1-2 bits must be zero, 7-8, 13-32 bits must be 1, and leave other unused bits to 0.
 */
/**@{*/

/**
 * @brief	Bit 3. Print PDF document with normal mode.
 *
 * @details	If the user wants to print a higher quality level of PDF document, set it with bit 12 together.<br>
 * 			Please refer to macro ::FSPDF_PERMISSION_PRINT_HIGH.
 */
#define FSPDF_PERMISSION_PRINT			0x0004
/**
 * @brief	Bit 4. Modify PDF contents.
 *
 * @details	if this bit is set, the user can modify contents of PDF document by operations other than
 * 			those controlled by bits 6, 9, and 11.
 */
#define FSPDF_PERMISSION_MODIFY			0x0008
/**
 * @brief	Bit 5. Extract PDF contents.
 *
 * @details	if this bit is set, the user can copy or otherwise extract text and graphics
 * 			from the document by operations other than that controlled by bit 10.
 */
#define FSPDF_PERMISSION_EXTRACT		0x0010
/**
 * @brief	Bit 6. Operate text annotations and fill in interactive form fields.
 *
 * @details	If bit 4 is also set, user can create or modify interactive form fields (including signature fields).
 */
#define FSPDF_PERMISSION_ANNOTFORM		0x0020
/**
 * @brief	Bit 9. Fill PDF form.
 *
 * @details	if this bit is set, user can fill in interactive form fields (including signature fields),
 * 			even if bit 6 is clear. 
 */
#define FSPDF_PERMISSION_FILLFORM		0x0100
/**
 * @brief	Bit 10. Disabilities support.
 *
 * @details	If this bit set, user can extract text and graphics in support of accessibility
 *			to users with disabilities or for other purposes. 
 */
#define FSPDF_PERMISSION_EXTRACTACCESS	0x0200
/**
 * @brief	Bit 11. Assemble PDF document.
 *
 * @details	This bit enables to assemble the document (insert, rotate, or delete pages
 * 			and create bookmarks or thumbnail images), regardless if bit 4 is set or not. 
 */
#define FSPDF_PERMISSION_ASSEMBLE		0x0400
/**
 * @brief	Bit 12. Print PDF document with higher qualities.
 *
 * @details	If this bit is clear (and bit 3 is set), printing is limited to a low-level
 * 			representation of the appearance, possibly of degraded quality. 
 */
#define FSPDF_PERMISSION_PRINT_HIGH		0x0800

/**@}*/
#endif /* _FSPDF_DEF_MACRO_PERMISSION_ */

#ifndef _FSPDF_DEF_MACRO_FORMFIELDTYPE_
#define _FSPDF_DEF_MACRO_FORMFIELDTYPE_
/**
 * @name	Macro Definitions for Form Field Types.
 */
/**@{*/

#define FSPDF_FORMFIELDTYPE_UNKNOWN			0	/**< @brief	Unknown type. */
#define FSPDF_FORMFIELDTYPE_PUSHBUTTON		1	/**< @brief	Push button type. */
#define FSPDF_FORMFIELDTYPE_CHECKBOX		2	/**< @brief	Check box type. */
#define FSPDF_FORMFIELDTYPE_RADIOBUTTON		3	/**< @brief	Radio button type. */
#define FSPDF_FORMFIELDTYPE_COMBOBOX		4	/**< @brief	Combo box type. */
#define FSPDF_FORMFIELDTYPE_LISTBOX			5	/**< @brief	List box type. */
#define FSPDF_FORMFIELDTYPE_TEXTFIELD		6	/**< @brief	single line or multi-line texts. */
#define FSPDF_FORMFIELDTYPE_UNSPECIFIED		FSPDF_FORMFIELDTYPE_UNKNOWN	/**< @brief	Unspecified type. */

/**@}*/
#endif /* _FSPDF_DEF_MACRO_FORMFIELDTYPE_ */

#ifndef _FSCRT_DEF_CALLBACK_TIMER_
#define _FSCRT_DEF_CALLBACK_TIMER_

/**
 * @brief	Declares a pointer type for the callback function for the FSPDF_FORMFILL_INFO::SetTimer method.
 *
 * @param[in] timer			Identifier of a timer.
 *
 * @return None.
 */
typedef void		(*FSCRT_CALLBACK_TIMER)(FS_INT32 timer);

#endif /* _FSCRT_DEF_CALLBACK_TIMER_ */


/*******************************************************************************/
/* Annotation common properties                                                */
/*******************************************************************************/
#ifndef _FSPDF_DEF_MACRO_ANNOTFLAG_
#define _FSPDF_DEF_MACRO_ANNOTFLAG_
/**
 * @name	Macro Definitions for Annotation Flags
 */
/**@{*/

/** @brief	Annotation flag: invisible */
#define	FSPDF_ANNOTFLAG_INVISIBLE			0x0001
/** @brief	Annotation flag: hidden */
#define FSPDF_ANNOTFLAG_HIDDEN				0x0002
/** @brief	Annotation flag: print */
#define FSPDF_ANNOTFLAG_PRINT				0x0004
/** @brief	Annotation flag: no zoom */
#define FSPDF_ANNOTFLAG_NOZOOM				0x0008
/** @brief	Annotation flag: no rotate */
#define FSPDF_ANNOTFLAG_NOROTATE			0x0010
/** @brief	Annotation flag: no view */
#define FSPDF_ANNOTFLAG_NOVIEW				0x0020
/** @brief	Annotation flag: read only */
#define FSPDF_ANNOTFLAG_READONLY			0x0040
/** @brief	Annotation flag: locked */
#define FSPDF_ANNOTFLAG_LOCKED				0x0080
/** @brief	Annotation flag: toggle no view */
#define FSPDF_ANNOTFLAG_TOGGLENOVIEW		0x0100
/** @brief	Annotation flag: locked contents */
#define FSPDF_ANNOTFLAG_LOCKEDCONTENTS		0x0200

/**@}*/
#endif /* _FSPDF_DEF_MACRO_ANNOTFLAG_ */

#endif // _FPDF_DEFINE_H_