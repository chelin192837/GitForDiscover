#include "fr_include.h"
#include "../fs_util.h"
#include "fr_ofdactionImp.h"
#include "fr_ofdoutlineImp.h"

// class FR_OFDOutlineImp : public FSBookmark, public CFX_Object
FR_OFDOutlineImp::FR_OFDOutlineImp()
	: m_Parent(NULL)
	, m_Index(0)
	, m_FirstChild(NULL)
	, m_NextSibling(NULL)
	, m_pNode(NULL)
	, m_Destination(NULL)
{

}

FR_OFDOutlineImp::~FR_OFDOutlineImp()
{
	if (m_pNode) {
		m_pNode->Release();
		m_pNode = NULL;
	}
}

void	FR_OFDOutlineImp::init(FR_OFDDocImp* doc, FR_OFDOutlineImp* parent, int index, COFD_Outline* node)
{
	m_Doc = doc;
	m_Parent = parent;
	m_Index = index;
	m_pNode = node;
}

int		FR_OFDOutlineImp::countChild()
{
	initChild();
	return m_Child.GetSize();
}

FR_OFDOutlineImp*	FR_OFDOutlineImp::getChild(int index)
{
	initChild();
	if (index>=0 && index<m_Child.GetSize()) {
		return m_Child[index];
	}

	return NULL;
}

FSBookmark*			FR_OFDOutlineImp::getParent()
{
	return m_Parent;
}

FSBookmark*			FR_OFDOutlineImp::getFirstChild()
{
	if (m_FirstChild) {
		return m_FirstChild;
	}

	m_FirstChild = getChild(0);

	return m_FirstChild;
}

FSBookmark*			FR_OFDOutlineImp::getNextSibling()
{
	if (!m_Parent) {
		return NULL;
	}

	if (m_NextSibling) {
		return m_NextSibling;
	}

	m_NextSibling = m_Parent->getChild(m_Index+1);
	return m_NextSibling;
}

FSDestination*		FR_OFDOutlineImp::getDestination()
{
	if (m_Destination) {
		return m_Destination;
	}

	//* note by gm 20170414, need add ofd destination first
	const COFD_Actions* actions = m_pNode->GetActions();
	if (!actions) {
		return NULL;
	}

	int count = actions->CountActions();
	for (int i=0; i<count; ++i) {
		const COFD_Action* act = actions->GetAction(i);
		if (!act) {
			continue;
		}

		OFD_ACTIONTYPE type = act->GetActionType();
		switch (type) {
		case OFD_ACTIONTYPE_Goto:
			{
				COFD_ActionGoto* actGoto = (COFD_ActionGoto*)act;
				COFD_Dest dest = actGoto->GetDest();
				if (!dest.IsValid()) {
					// note by gm 20170414use dest in bookmark, need impl
				}

				if (dest.IsValid()) {
					OFD_DESTTYPE desttype = dest.GetDestType();
					m_Destination = FR_OFDDestImp::createFromOFDDest(m_Doc, &dest);
				}
				break;
			}
		default:
			{
				break;
			}
		}
	}

	return m_Destination;
	// */
}

void				FR_OFDOutlineImp::setDestination(const FSDestination* dest)
{
	return;
}

FSString	FR_OFDOutlineImp::getTitle() const
{
	FSString result;

	if (m_pNode && !m_pNode->IsNull()) {
		CFX_WideString ws = m_pNode->GetTitle();
		if (!ws.IsEmpty()) {
			FSString strTitle = FSString::createFromWideString(ws);
			strTitle.toUTF8();
			return strTitle;
		}
	}
	return FSString();
}

void		FR_OFDOutlineImp::setTitle(const char* title)
{
	return ;
}

FS_ARGB		FR_OFDOutlineImp::getColor() const
{
	return NULL;
}

void		FR_OFDOutlineImp::setColor(FS_ARGB color)
{
	return ;
}

FS_DWORD	FR_OFDOutlineImp::getStyle() const
{
	return 0;
}

void		FR_OFDOutlineImp::setStyle(FS_DWORD style)
{
	return ;
}

FSBookmark* FR_OFDOutlineImp::insert(const char* title, FS_BOOKMARKPOSITION pos)
{
	return NULL;
}

FS_BOOL FR_OFDOutlineImp::moveTo(FSBookmark* destBookmark, FS_BOOKMARKPOSITION pos)
{
	return FALSE;
}

void		FR_OFDOutlineImp::initChild()
{
	if (!m_pNode || m_pNode->IsNull() || m_pNode->CountSubOutlines()<=0 || m_Child.GetSize()>0) {
		return;
	}

	int count = m_pNode->CountSubOutlines();
	for (int i=0; i<count; ++i) {
		COFD_Outline node = m_pNode->GetSubOutline(i);
		FR_OFDOutlineImp* child = new FR_OFDOutlineImp;
		child->init(m_Doc, this, i, node.Clone());
		m_Child.Add(child);
	}
}
