#ifndef _FJS_RESOURCE_H_
#define _FJS_RESOURCE_H_

#define IDD_JS_MSGBOX                   25600
#define IDD_RESPONSE                    25601
#define IDC_JS_MSG_TEXT                 25602
#define ID_JS_MSG_OK                    25603
#define ID_JS_MSG_CANCEL                25604
#define IDC_JS_MSG_ICON                 25605
#define ID_JS_MSG_YES                   25606
#define ID_JS_MSG_NO                    25607
#define IDC_JS_QUESTION                 25608
#define ID_JS_OK                        25609
#define ID_JS_CANCEL                    25610
#define IDC_JS_ANSWER                   25611
#define IDC_JS_EDIT                     25612
#define IDS_STRING_JSALERT              25613
#define IDS_STRING_JSPARAMERROR         25614
#define IDS_STRING_JSAFNUMBER_KEYSTROKE 25615
#define IDS_STRING_JSINPUTERROR         25616
#define IDS_STRING_JSPARAM_TOOLONG      25617
#define IDS_STRING_JSPARSEDATE          25618
#define IDS_STRING_JSRANGE1             25619
#define IDS_STRING_JSRANGE2             25620
#define IDS_STRING_JSRANGE3             25621
#define IDS_STRING_JSRANGE4             25622
#define IDS_STRING_FILEOPENFAIL         25623
#define IDS_STRING_JSATTENTION          25624
#define IDS_STRING_JSSUBMITS            25625
#define IDS_STRING_JSSUBMITF            25626
#define IDS_STRING_NOTSUPPORT           25627
#define IDS_STRING_JSBUSY               25628
#define IDS_STRING_JSEVENT              25629
#define IDS_STRING_RUN                  25630
#define IDS_STRING_UNHANDLED            25631
#define IDS_STRING_JSPRINT1             25632
#define IDS_STRING_JSPRINT2             25633
#define IDS_STRING_LAUNCHURL            25634
#define IDS_JSPARAM_INCORRECT           25635
#define IDD_JS_CONSOLE                  25636
#define IDS_STRING_SAFEMODEL            25636
#define IDC_EDTSCRIPT                   25637
#define IDC_BTNCLEAR                    25638
#define IDC_EDTOUTPUT                   25639
#define IDC_CHECK_TIPS                  25640
#define IDC_BTNRUN                      25641


static CFX_WideString JSGetStringFromID(CFXJS_Context* pContext, FX_DWORD ID)
{
	switch(ID)
	{                  
	case IDS_STRING_JSALERT:
		return L"Alert";
	case IDS_STRING_JSPARAMERROR:
        return L"The amount of parameters is not correct !";	
	case IDS_STRING_JSAFNUMBER_KEYSTROKE:
		return L"The input value is invalid.";
	case	IDS_STRING_JSINPUTERROR:
        return L"Input error !";
	case	IDS_STRING_JSPARAM_TOOLONG:
		return L"The value you are going to input is too long.";
	case	IDS_STRING_JSPARSEDATE:
		return L"The input string can't be parsed to a valid date time (%S).";
	case	IDS_STRING_JSRANGE1:
		return L"Invalid value: must be greater or equal to %S and less than or equal to %S.";
	case	IDS_STRING_JSRANGE2:
		return L"Invalid value: must be greater or equal to %S.";
	case	IDS_STRING_JSRANGE3:
		return L"Invalid value: must be less than or equal to %S.";
	case	IDS_STRING_JSRANGE4:
		return L"Range Error";	
	case	IDS_STRING_FILEOPENFAIL:
        return L"Opening file failed.";
	case	IDS_STRING_JSATTENTION:
		return L"Attention";	
	case	IDS_STRING_JSSUBMITS:
		return L"Submit form successfully!";
	case	IDS_STRING_JSSUBMITF:
		return L"Submit form failed!";	
	case	IDS_STRING_NOTSUPPORT:
		return L"Not supported.";
	case	IDS_STRING_JSBUSY:
		return L"System is busy!";	
	case	IDS_STRING_JSEVENT:
		return L"The event of the formfield exists!";	
	case	IDS_STRING_RUN:
		return L"It runs successfully.";
	case	IDS_STRING_UNHANDLED:
		return L"An unhandled error!";
	case	IDS_STRING_JSPRINT1:
		return L"The second parameter can't convert to Date!";
	case	IDS_STRING_JSPRINT2:
		return L"The second parameter isn't a valid Date!";
	case	IDS_STRING_LAUNCHURL:
		return L"The Document is trying to connect to \r\n%S\r\nIf you trust the site, choose OK. If you don't trust the site, choose Cancel.";	
	case	IDS_JSPARAM_INCORRECT:
		return L"The parameter you inputted is incorrect!";
	case	IDS_STRING_SAFEMODEL:
		return L"Secure reading mode";
	default:
		return L"";

	}
}

#if (_FX_OS_ == _FX_ANDROID_) || (_FX_OS_ == _FX_IOS_) || (WINAPI_PARTITION_APP || WINAPI_PARTITION_PC_APP)
static CFX_ByteString JSGetByteStringFromID(CFXJS_Context* pContext, FX_DWORD ID)
{
	switch(ID)
	{                  
	case IDS_STRING_JSALERT:
		return "Alert";
	case IDS_STRING_JSPARAMERROR:
        return "The amount of parameters is not correct !";	
	case IDS_STRING_JSAFNUMBER_KEYSTROKE:
		return "The input value is invalid.";
	case	IDS_STRING_JSINPUTERROR:
        return "Input error !";
	case	IDS_STRING_JSPARAM_TOOLONG:
		return "The value you are going to input is too long.";
	case	IDS_STRING_JSPARSEDATE:
		return "The input string can't be parsed to a valid date time (%s).";
	case	IDS_STRING_JSRANGE1:
		return "Invalid value: must be greater or equal to %s and less than or equal to %s.";	
	case	IDS_STRING_JSRANGE2:
		return "Invalid value: must be greater or equal to %s.";
	case	IDS_STRING_JSRANGE3:
		return "Invalid value: must be less than or equal to %s.";
	case	IDS_STRING_JSRANGE4:
		return "Range Error";	
	case	IDS_STRING_FILEOPENFAIL:
        return "Opening file failed.";
	case	IDS_STRING_JSATTENTION:
		return "Attention";	
	case	IDS_STRING_JSSUBMITS:
		return "Submit form successfully!";
	case	IDS_STRING_JSSUBMITF:
		return "Submit form failed!";	
	case	IDS_STRING_NOTSUPPORT:
		return "Not supported.";
	case	IDS_STRING_JSBUSY:
		return "System is busy!";	
	case	IDS_STRING_JSEVENT:
		return "The event of the formfield exists!";	
	case	IDS_STRING_RUN:
		return "It runs successfully.";
	case	IDS_STRING_UNHANDLED:
		return "An unhandled error!";
	case	IDS_STRING_JSPRINT1:
		return "The second parameter can't convert to Date!";
	case	IDS_STRING_JSPRINT2:
		return "The second parameter isn't a valid Date!";
	case	IDS_STRING_LAUNCHURL:
		return "The Document is trying to connect to \r\n%s\r\nIf you trust the site, choose OK. If you don't trust the site, choose Cancel.";	
	case	IDS_JSPARAM_INCORRECT:
		return "The parameter you inputted is incorrect!";
	case	IDS_STRING_SAFEMODEL:
		return "Secure reading mode";
	default:
		return "";

	}
}
#endif

#endif //_FJS_RESOURCE_H_

