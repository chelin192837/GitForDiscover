#include "js_fxlib.h"
#if defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)

#include "consts.h"

/* ------------------------------ border ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Border)
	JS_STATIC_CONST_ENTRY_STRING(s,	solid)
	JS_STATIC_CONST_ENTRY_STRING(b,	beveled)
	JS_STATIC_CONST_ENTRY_STRING(d,	dashed)
	JS_STATIC_CONST_ENTRY_STRING(i,	inset)
	JS_STATIC_CONST_ENTRY_STRING(u,	underline)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Border,border)

/* ------------------------------ display ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Display)
	JS_STATIC_CONST_ENTRY_NUMBER(visible,	0)
	JS_STATIC_CONST_ENTRY_NUMBER(hidden,	1)
	JS_STATIC_CONST_ENTRY_NUMBER(noPrint,	2)
	JS_STATIC_CONST_ENTRY_NUMBER(noView,	3)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Display,display)

/* ------------------------------ font ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Font)
	JS_STATIC_CONST_ENTRY_STRING(Times,		Times-Roman)
	JS_STATIC_CONST_ENTRY_STRING(TimesB,	Times-Bold)
	JS_STATIC_CONST_ENTRY_STRING(TimesI,	Times-Italic)
	JS_STATIC_CONST_ENTRY_STRING(TimesBI,	Times-BoldItalic)
	JS_STATIC_CONST_ENTRY_STRING(Helv,		Helvetica)
	JS_STATIC_CONST_ENTRY_STRING(HelvB,		Helvetica-Bold)
	JS_STATIC_CONST_ENTRY_STRING(HelvI,		Helvetica-Oblique)
	JS_STATIC_CONST_ENTRY_STRING(HelvBI,	Helvetica-BoldOblique)
	JS_STATIC_CONST_ENTRY_STRING(Cour,		Courier)
	JS_STATIC_CONST_ENTRY_STRING(CourB,		Courier-Bold)
	JS_STATIC_CONST_ENTRY_STRING(CourI,		Courier-Oblique)
	JS_STATIC_CONST_ENTRY_STRING(CourBI,	Courier-BoldOblique)
	JS_STATIC_CONST_ENTRY_STRING(Symbol,	Symbol)
	JS_STATIC_CONST_ENTRY_STRING(ZapfD,		ZapfDingbats)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Font,font)

/* ------------------------------ highlight ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Highlight)
	JS_STATIC_CONST_ENTRY_STRING(n,	none)
	JS_STATIC_CONST_ENTRY_STRING(i,	invert)
	JS_STATIC_CONST_ENTRY_STRING(p,	push)
	JS_STATIC_CONST_ENTRY_STRING(o,	outline)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Highlight,highlight)

/* ------------------------------ position ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Position)
	JS_STATIC_CONST_ENTRY_NUMBER(textOnly,		0)
	JS_STATIC_CONST_ENTRY_NUMBER(iconOnly,		1)
	JS_STATIC_CONST_ENTRY_NUMBER(iconTextV,		2)
	JS_STATIC_CONST_ENTRY_NUMBER(textIconV,		3)
	JS_STATIC_CONST_ENTRY_NUMBER(iconTextH,		4)
	JS_STATIC_CONST_ENTRY_NUMBER(textIconH,		5)
	JS_STATIC_CONST_ENTRY_NUMBER(overlay,		6)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Position,position)

/* ------------------------------ scaleHow ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_ScaleHow)
	JS_STATIC_CONST_ENTRY_NUMBER(proportional,	0)
	JS_STATIC_CONST_ENTRY_NUMBER(anamorphic,	1)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_ScaleHow,scaleHow)

/* ------------------------------ scaleWhen ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_ScaleWhen)
	JS_STATIC_CONST_ENTRY_NUMBER(always,	0)
	JS_STATIC_CONST_ENTRY_NUMBER(never,		1)
	JS_STATIC_CONST_ENTRY_NUMBER(tooBig,	2)
	JS_STATIC_CONST_ENTRY_NUMBER(tooSmall,	3)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_ScaleWhen,scaleWhen)

/* ------------------------------ style ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Style)
	JS_STATIC_CONST_ENTRY_STRING(ch,	check)
	JS_STATIC_CONST_ENTRY_STRING(cr,	cross)
	JS_STATIC_CONST_ENTRY_STRING(di,	diamond)
	JS_STATIC_CONST_ENTRY_STRING(ci,	circle)
	JS_STATIC_CONST_ENTRY_STRING(st,	star)
	JS_STATIC_CONST_ENTRY_STRING(sq,	square)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Style,style)


/* ------------------------------ zoomtype ------------------------------ */

BEGIN_JS_STATIC_CONST(CFXJS_Zoomtype)
	JS_STATIC_CONST_ENTRY_STRING(none,	NoVary)
	JS_STATIC_CONST_ENTRY_STRING(fitP,	FitPage)
	JS_STATIC_CONST_ENTRY_STRING(fitW,	FitWidth)
	JS_STATIC_CONST_ENTRY_STRING(fitH,	FitHeight)
	JS_STATIC_CONST_ENTRY_STRING(fitV,	FitVisibleWidth)
	JS_STATIC_CONST_ENTRY_STRING(pref,	Preferred)
	JS_STATIC_CONST_ENTRY_STRING(refW,	ReflowWidth)
END_JS_STATIC_CONST()

IMPLEMENT_JS_CLASS_CONST(CFXJS_Zoomtype,zoomtype)

/* ------------------------------ CJS_GlobalConsts ------------------------------ */

int	CFXJS_GlobalConsts::Init(IDS_Runtime* pRuntime)
{
	DEFINE_GLOBAL_CONST(pRuntime, IDS_GREATER_THAN , Invalid value: must be greater than or equal to %s.);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_GT_AND_LT,Invalid value: must be greater than or equal to %s and less than or equal to %s.);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_LESS_THAN,Invalid value: must be less than or equal to %s.);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_INVALID_MONTH,** Invalid **);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_INVALID_DATE,Invalid date/time: please ensure that the date/time exists. Field);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_INVALID_VALUE,The value entered does not match the format of the field);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_AM,am);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_PM,pm);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_MONTH_INFO,January[1]February[2]March[3]April[4]May[5]June[6]July[7]August[8]September[9]October[10]November[11]December[12]Sept[9]Jan[1]Feb[2]Mar[3]Apr[4]Jun[6]Jul[7]Aug[8]Sep[9]Oct[10]Nov[11]Dec[12]);
	DEFINE_GLOBAL_CONST(pRuntime, IDS_STARTUP_CONSOLE_MSG, ** ^_^ **);

	return 0;
}

/* ------------------------------ CJS_GlobalArrays ------------------------------ */

int	CFXJS_GlobalArrays::Init(IDS_Runtime* pRuntime)
{
	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_NUMBER_ENTRY_DOT_SEP";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"[+-]?\\d*\\.?\\d*"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_NUMBER_COMMIT_DOT_SEP";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"[+-]?\\d+(\\.\\d+)?",                /* -1.0 or -1 */
										(FX_LPCWSTR)L"[+-]?\\.\\d+",                            /* -.1 */
										(FX_LPCWSTR)L"[+-]?\\d+\\."                             /* -1. */
										};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_NUMBER_ENTRY_COMMA_SEP";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"[+-]?\\d*,?\\d*"};
		
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_NUMBER_COMMIT_COMMA_SEP";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"[+-]?\\d+([.,]\\d+)?",               /* -1,0 or -1 */
										(FX_LPCWSTR)L"[+-]?[.,]\\d+",                   /* -,1 */
                                        (FX_LPCWSTR)L"[+-]?\\d+[.,]"                            /* -1, */
                                        };
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_ZIP_ENTRY";
        FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{0,5}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_ZIP_COMMIT";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{5}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_ZIP4_ENTRY";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{0,5}(\\.|[- ])?\\d{0,4}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_ZIP4_COMMIT";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{5}(\\.|[- ])?\\d{4}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_PHONE_ENTRY";
		FX_LPCWSTR ArrayContent[] = {
				(FX_LPCWSTR)L"\\d{0,3}(\\.|[- ])?\\d{0,3}(\\.|[- ])?\\d{0,4}",		/* 555-1234 or 408 555-1234 */
				(FX_LPCWSTR)L"\\(\\d{0,3}",											/* (408 */
				(FX_LPCWSTR)L"\\(\\d{0,3}\\)(\\.|[- ])?\\d{0,3}(\\.|[- ])?\\d{0,4}",	/* (408) 555-1234 */
					/* (allow the addition of parens as an afterthought) */
				(FX_LPCWSTR)L"\\(\\d{0,3}(\\.|[- ])?\\d{0,3}(\\.|[- ])?\\d{0,4}",	/* (408 555-1234 */
				(FX_LPCWSTR)L"\\d{0,3}\\)(\\.|[- ])?\\d{0,3}(\\.|[- ])?\\d{0,4}",	/* 408) 555-1234 */
				(FX_LPCWSTR)L"011(\\.|[- \\d])*"										/* international */
			};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_PHONE_COMMIT";
		FX_LPCWSTR ArrayContent[] = {
					(FX_LPCWSTR)L"\\d{3}(\\.|[- ])?\\d{4}",							/* 555-1234 */
					(FX_LPCWSTR)L"\\d{3}(\\.|[- ])?\\d{3}(\\.|[- ])?\\d{4}",			/* 408 555-1234 */
					(FX_LPCWSTR)L"\\(\\d{3}\\)(\\.|[- ])?\\d{3}(\\.|[- ])?\\d{4}",	/* (408) 555-1234 */
					(FX_LPCWSTR)L"011(\\.|[- \\d])*"									/* international */
				};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_SSN_ENTRY";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{0,3}(\\.|[- ])?\\d{0,2}(\\.|[- ])?\\d{0,4}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	{
		FX_LPCWSTR ArrayName = (FX_LPCWSTR)L"RE_SSN_COMMIT";
		FX_LPCWSTR ArrayContent[] = {(FX_LPCWSTR)L"\\d{3}(\\.|[- ])?\\d{2}(\\.|[- ])?\\d{4}"};
		DEFINE_GLOBAL_ARRAY(pRuntime);
	}

	return 0;
}

#endif //defined(_FSDK_ALL_) || defined(_FSDK_JAVASCRIPT_)