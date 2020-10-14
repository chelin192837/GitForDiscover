#ifndef _OFD_ACTION_W_H_
#define _OFD_ACTION_W_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 创建动作对象。
 *
 * @param[in]	type			类型。
 * @return 动作对象。
 *				
 */
OFD_ACTION  OFD_Action_Create(int type);

/**
 * @brief 将动作对象插入到动作对象数组中。
 *
 * @param[in]	hActions		动作对象数组。
 * @param[in]	hAction			动作对象。
 * @return 无。
 *				
 */
void		OFD_Action_Insert(OFD_ACTIONS hActions,OFD_ACTION hAction);

/**
 * @brief 设置动作事件。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	bsTrigger		动作事件（PO、DO、CLICK）。
 * @return 无。
 *
 */
void		OFD_Action_SetEventTrigger(OFD_ACTION hAction, OFD_LPCSTR bsTrigger);

/**
 * @brief 设置动作跳转Dest位置。
 *
 * @param[in]	hAction		动作对象。
 * @param[in]	destPoint	跳转位置。
 * @param[in]	iPage		跳转页ID。
 * @return 无。
 *
 */
void		OFD_ActionGoTo_SetDest(OFD_ACTION hAction, OFD_POINT destPoint, int iPage);

/**
 * @brief 设置动作跳转URI地址。
 *
 * @param[in]	hAction		动作对象。
 * @param[in]	lpcwUri		跳转URI地址。
 * @return 无。
 *
 */
void		OFD_ActionURI_SetDestURI(OFD_ACTION hAction, OFD_LPCWSTR lpcwUri);

/**
 * @brief 设置音频资源ID。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	dwSoundID		音频资源ID。
 * @return 无。
 *
 */
void		OFD_ActionSound_SetResID(OFD_ACTION hAction, OFD_DWORD dwSoundID);

/**
 * @brief 设置音频音量。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	volume			音频音量。
 * @return 无。
 *
 */
void		OFD_ActionSound_SetVolume(OFD_ACTION hAction, int volume);

/**
 * @brief 设置音频是否重复播放。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	bSet			音频是否重复播放。
 * @return 无。
 *
 */
void		OFD_ActionSound_SetRepeat(OFD_ACTION hAction, OFD_BOOL bSet);

/**
 * @brief 设置音频同步。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	bSet			音频是否同步。
 * @return 无。
 *
 */
void		OFD_ActionSound_SetSynchronous(OFD_ACTION hAction, OFD_BOOL bSet);

/**
 * @brief 设置视频资源ID。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	dwSoundID		视频资源ID。
 * @return 无。
 *
 */
void		OFD_ActionMovie_SetResID(OFD_ACTION hAction, OFD_DWORD dwSoundID);

/**
 * @brief 设置视频操作。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	Operator		视频操作。
 * @return 无。
 *
 */
void		OFD_ActionMovie_SetOperator(OFD_ACTION hAction, int Operator);


#ifdef __cplusplus
}
#endif

#endif //_OFD_ACTION_W_H_
