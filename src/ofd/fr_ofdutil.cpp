#include "fr_include.h"
#include "fr_ofdutil.h"

// class CoordinatorAssistant
CoordinatorAssistant::CoordinatorAssistant(IOFD_Page* page)
: m_Page(page)
{

}

void			CoordinatorAssistant::toPDF(const CFX_RectF& src, CFX_FloatRect& dst)
{
	dst.left 	= MM_2_PT(src.left);
	dst.top 	= GetHeight() - MM_2_PT(src.top);
	dst.right 	= MM_2_PT(src.right());
	dst.bottom 	= GetHeight() - MM_2_PT(src.bottom());
}

void			CoordinatorAssistant::fromPDF(const CFX_FloatRect& src, CFX_RectF& dst)
{
	dst.left 	= PT_2_MM(src.left);
	dst.top 	= PT_2_MM(GetHeight() - src.top);
	dst.width 	= PT_2_MM(src.right) - dst.left;
	dst.height 	= PT_2_MM(GetHeight() - src.bottom) - dst.top;
}

void			CoordinatorAssistant::toPDF(float& x, float& y)
{
	x = MM_2_PT(x);
	y = GetHeight() - MM_2_PT(y);
}

void			CoordinatorAssistant::fromPDF(float& x, float& y)
{
	x = PT_2_MM(x);
	y = PT_2_MM(GetHeight() - y);
}

void			CoordinatorAssistant::toPDF(float& len)
{
	len = GetHeight() - MM_2_PT(len);
}

void			CoordinatorAssistant::fromPDF(float& len)
{
	len = PT_2_MM(GetHeight() - len);
}

float			CoordinatorAssistant::GetHeight()
{
	COFD_PageArea area = m_Page->GetPageArea();
	CFX_RectF rect;
	area.GetPageArea(OFD_PAGEAREA_PhysicalBox, rect);
	return MM_2_PT(rect.height);
}


