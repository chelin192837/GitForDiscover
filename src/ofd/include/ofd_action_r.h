#ifndef _OFD_ACTION_R_H_
#define _OFD_ACTION_R_H_

#include "ofd_base_r.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 获取动作对象个数。
 *
 * @param[in]	hActions	注释句柄。
 * @return 成功返回动作对象个数，失败返回-1。
 *				
 */
int				OFD_Action_Count(OFD_ACTIONS hActions);

/**
 * @brief 获取动作对象。
 *
 * @param[in]	hActions		注释句柄。
 * @param[in]	index			索引。
 * @return 成功返回取动作对象，失败返回NULL。
 *				
 */
OFD_ACTION		OFD_Action_Get(OFD_ACTIONS hActions, int index);

/**
 * @brief 获取动作对象事件。
 *
 * @param[in]	hAction			动作对象。
 * @param[out]	lpcEvent		事件。
 * @return 无。
 *				
 */
void			OFD_Action_GetEventTrigger(OFD_ACTION hAction, OFD_WSTR* wsEvent);

/**
 * @brief 获取动作对象类型。
 *
 * @param[in]	hAction			动作对象。
 * @return 成功获取动作对象类型，失败返回0。
 *				
 */
int				OFD_Action_GetType(OFD_ACTION hAction);

/**
 * @brief 获取动作跳转Dest位置。
 *
 * @param[in]	hAction			动作句柄。
 * @param[out]	type			动作类型。
 * @param[out]	destData		跳转位置数据。
 * @param[out]	pageID			页面ID。
 * @return 无。
 *				
 */
void			OFD_ActionGoTo_GetDest(OFD_ACTION hAction,int* type, OFD_DESTDATA* destData,OFD_DWORD* pageID);

/**
 * @brief 获取动作跳转URI地址。
 *
 * @param[in]	hAnnot		注释句柄。
 * @param[out]	lpcwUri		跳转URI地址。
 * @return 无。
 *				
 */
void			OFD_ActionURI_GetURI(OFD_ACTION hAction,OFD_WSTR* wsRri);

/**
 * @brief 获取音频资源ID。
 *
 * @param[in]	hAction			动作对象。
 * @return 成功返回资源ID，失败返回-1。
 *
 */
int		OFD_ActionSound_GetResID(OFD_ACTION hAction);

/**
 * @brief 获取音频音量。
 *
 * @param[in]	hAction			动作对象。
 * @return 成功返回声音音量，失败返回-1。
 *
 */
int		OFD_ActionSound_GetVolume(OFD_ACTION hAction);

/**
 * @brief 音频是否重复播放。
 *
 * @param[in]	hAction			动作对象。
 * @param[in]	bSet			声音是否重复。
 * @return 音频是否重复播放。
 *
 */
OFD_BOOL	OFD_ActionSound_GetRepeat(OFD_ACTION hAction);

/**
 * @brief 音频是否同步。
 *
 * @param[in]	hAction			动作对象。
 * @return 音频是否同步。
 *
 */
OFD_BOOL	OFD_ActionSound_GetSynchronous(OFD_ACTION hAction);

/**
 * @brief 获取视频资源ID。
 *
 * @param[in]	hAction			动作对象。
 * @return 成功返回资源ID，失败返回-1。
 *
 */
int		OFD_ActionMovie_GetResID(OFD_ACTION hAction);

/**
 * @brief 设置视频操作。
 *
 * @param[in]	hAction			动作对象。
 * @return 成功返回视频操作，失败返回-1。
 *
 */
int		OFD_ActionMovie_GetOperator(OFD_ACTION hAction);

#ifdef __cplusplus
}
#endif

#endif //_OFD_ACTION_R_H_
