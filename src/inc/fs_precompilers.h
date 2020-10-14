#ifndef _FS_PRECOMPILERS_H_
#define _FS_PRECOMPILERS_H_

#define _FSDK_ALL_

#ifdef _FSDK_ALL_
	#ifndef _FSDK_BASE_
		#define _FSDK_BASE_
	#endif
	#ifndef _FSDK_IMAGE_
		#define _FSDK_IMAGE_
	#endif
	#ifndef _FSDK_BARCODE_
		#define _FSDK_BARCODE_
	#endif
	#ifndef _FSDK_DRM_
		#define _FSDK_DRM_
	#endif
	#ifndef _FSDK_PDF_
		#define _FSDK_PDF_
	#endif
	#ifndef _FSDK_EMB_
		#define _FSDK_EMB_
	#endif
	#ifndef _FSDK_EPUB_
		#define _FSDK_EPUB_
	#endif
	#ifndef _FSDK_TXT_
		#define _FSDK_TXT_
	#endif
	#ifndef _FSDK_JAVASCRIPT_
		#define _FSDK_JAVASCRIPT_
	#endif
#endif

#ifdef _FSDK_IMAGE_
	#ifndef _FSDK_IMAGE_BMP_
		#define _FSDK_IMAGE_BMP_
	#endif
	#ifndef _FSDK_IMAGE_GIF_
		#define _FSDK_IMAGE_GIF_
	#endif
	#ifndef _FSDK_IMAGE_JPG_
		#define _FSDK_IMAGE_JPG_
	#endif
	#ifndef _FSDK_IMAGE_JPX_
		#define _FSDK_IMAGE_JPX_
	#endif
	#ifndef _FSDK_IMAGE_PNG_
		#define _FSDK_IMAGE_PNG_
	#endif
	#ifndef _FSDK_IMAGE_TIF_
		#define _FSDK_IMAGE_TIF_
	#endif
#endif

#ifdef _FSDK_PDF_
	#ifndef _FSDK_PDFBASE_
		#define _FSDK_PDFBASE_
	#endif
	#ifndef _FSDK_DOCUMENT_
		#define _FSDK_DOCUMENT_
	#endif
	#ifndef _FSDK_PDFPAGE_
		#define _FSDK_PDFPAGE_
	#endif
	#ifndef _FSDK_PDFVIEW_
		#define _FSDK_PDFVIEW_
	#endif
	#ifndef _FSDK_PDFEDIT_
		#define _FSDK_PDFEDIT_
	#endif
	#ifndef _FSDK_PDFANNOT_
		#define _FSDK_PDFANNOT_
	#endif
	#ifndef _FSDK_PDFFORM_
		#define _FSDK_PDFFORM_
	#endif
	#ifndef _FSDK_PDFSECURITY_
		#define _FSDK_PDFSECURITY_
	#endif
	#ifndef _FSDK_PDFSIGNATURE_
		#define _FSDK_PDFSIGNATURE_
	#endif
	#ifndef _FSDK_PDFWATERMARK_
		#define _FSDK_PDFWATERMARK_
	#endif
	#ifndef _FSDK_PDFPSI_
		#define _FSDK_PDFPSI_
	#endif
	#ifndef _FSDK_PDFOBJECT_
		#define _FSDK_PDFOBJECT_
	#endif
	#ifndef _FSDK_PDFREFLOW_
		#define _FSDK_PDFREFLOW_
	#endif
	#ifndef _FSDK_PDFTEXT_
		#define _FSDK_PDFTEXT_
	#endif
	#ifndef _FSDK_PDFLR_
		#define _FSDK_PDFLR_
	#endif
	#ifndef _FSDK_ASYNC_
		#define _FSDK_ASYNC_
	#endif
	#ifndef _FSDK_PDFPAINTENGINE_
		#define _FSDK_PDFPAINTENGINE_
	#endif
#endif

#ifdef _FSDK_PDFVIEW_
	#ifndef _FSDK_PDFVIEW_NORMAL_
		#define _FSDK_PDFVIEW_NORMAL_
	#endif
	#ifndef _FSDK_PDFVIEW_ASYNC_
		#define _FSDK_PDFVIEW_ASYNC_
	#endif
#endif

#ifdef _FSDK_PDFEDIT_
	#ifndef _FSDK_PDFEDIT_NORMAL_
		#define _FSDK_PDFEDIT_NORMAL_
	#endif
	#ifndef _FSDK_PDFEDIT_CREATION_
		#define _FSDK_PDFEDIT_CREATION_
	#endif
	#ifndef _FSDK_PDFEDIT_PAGEOBJECTS_
		#define _FSDK_PDFEDIT_PAGEOBJECTS_
	#endif
	#ifndef _FSDK_PDFEDIT_PAGEORGANIZATION_
		#define _FSDK_PDFEDIT_PAGEORGANIZATION_
	#endif
	#ifndef _FSDK_PDFEDIT_PAGEFLATTEN_
		#define _FSDK_PDFEDIT_PAGEFLATTEN_
	#endif
	#ifndef _FSDK_PDFEDIT_IMAGECONVERT_
		#define _FSDK_PDFEDIT_IMAGECONVERT_
	#endif
#endif

#ifdef _FSDK_PDFANNOT_
	#ifndef _FSDK_PDFANNOT_NORMAL_
		#define _FSDK_PDFANNOT_NORMAL_
	#endif
	#ifndef _FSDK_PDFANNOT_NOTE_
		#define _FSDK_PDFANNOT_NOTE_
	#endif
	#ifndef _FSDK_PDFANNOT_HIGHLIGHT_
		#define _FSDK_PDFANNOT_HIGHLIGHT_
	#endif
	#ifndef _FSDK_PDFANNOT_PENCIL_
		#define _FSDK_PDFANNOT_PENCIL_
	#endif
	#ifndef _FSDK_PDFANNOT_STAMP_
		#define _FSDK_PDFANNOT_STAMP_
	#endif
	#ifndef _FSDK_PDFANNOT_FILEATTACHMENT_
		#define _FSDK_PDFANNOT_FILEATTACHMENT_
	#endif
	#ifndef _FSDK_PDFANNOT_LINK_
		#define _FSDK_PDFANNOT_LINK_
	#endif
	#ifndef _FSDK_PDFANNOT_FREETEXT_
		#define _FSDK_PDFANNOT_FREETEXT_
	#endif
	#ifndef _FSDK_PDFANNOT_SQUARE_
		#define _FSDK_PDFANNOT_SQUARE_
	#endif
	#ifndef _FSDK_PDFANNOT_CIRCLE_
		#define _FSDK_PDFANNOT_CIRCLE_
	#endif
	#ifndef _FSDK_PDFANNOT_LINE_
		#define _FSDK_PDFANNOT_LINE_
	#endif
	#ifndef _FSDK_PDFANNOT_SCREEN_
		#define _FSDK_PDFANNOT_SCREEN_
	#endif
	#ifndef _FSDK_PDFANNOT_ANNOTDATA_
		#define _FSDK_PDFANNOT_ANNOTDATA_
	#endif
	#ifndef _FSDK_PDFANNOT_PSI_
		#define _FSDK_PDFANNOT_PSI_
	#endif
#endif

#ifdef _FSDK_PDFFORM_
	#ifndef _FSDK_PDFFORM_FORMFILL_
		#define _FSDK_PDFFORM_FORMFILL_
	#endif
	#ifndef _FSDK_PDFFORM_CREATION_
		#define _FSDK_PDFFORM_CREATION_
	#endif
	#ifndef _FSDK_PDFFORM_FORMDATA_
		#define _FSDK_PDFFORM_FORMDATA_
	#endif
#endif

#ifdef _FSDK_PDFSECURITY_
	#ifndef _FSDK_PDFSECURITY_STD_
		#define _FSDK_PDFSECURITY_STD_
	#endif
	#ifndef _FSDK_PDFSECURITY_PKI_
		#define _FSDK_PDFSECURITY_PKI_
	#endif
	#ifndef _FSDK_PDFSECURITY_FDRM_
		#define _FSDK_PDFSECURITY_FDRM_
	#endif
	#ifndef _FSDK_PDFSECURITY_CUSTOMIZED_
		#define _FSDK_PDFSECURITY_CUSTOMIZED_
	#endif
#endif

#ifdef _FSDK_PDFSIGNATURE_
	#ifndef _FSDK_PDFSIGNATURE_NORMAL_
		#define _FSDK_PDFSIGNATURE_NORMAL_
	#endif
	#ifndef _FSDK_PDFSIGNATURE_STRADDLE_
		#define _FSDK_PDFSIGNATURE_STRADDLE_
	#endif
#endif

#if defined(_FSDK_EPUB_) || defined(_FSDK_TXT_)
	#ifndef _FSDK_DP_
		#define _FSDK_DP_
	#endif
#endif

#if defined(_WIN32) || defined(_WIN64)

#ifndef _CRT_NON_CONFORMING_SWPRINTFS
#define _CRT_NON_CONFORMING_SWPRINTFS
#endif

#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif

#endif

#endif//_FS_PRECOMPILERS_H_
