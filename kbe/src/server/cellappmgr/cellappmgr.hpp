/*
This source file is part of KBEngine
For the latest info, see http://www.kbengine.org/

Copyright (c) 2008-2012 KBEngine.

KBEngine is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

KBEngine is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.
 
You should have received a copy of the GNU Lesser General Public License
along with KBEngine.  If not, see <http://www.gnu.org/licenses/>.
*/


#ifndef KBE_CELLAPPMGR_HPP
#define KBE_CELLAPPMGR_HPP
	
#include "cellapp.hpp"
#include "server/kbemain.hpp"
#include "server/serverapp.hpp"
#include "server/idallocate.hpp"
#include "server/serverconfig.hpp"
#include "server/forward_messagebuffer.hpp"
#include "cstdkbe/timer.hpp"
#include "network/endpoint.hpp"

namespace KBEngine{

class Cellappmgr :	public ServerApp, 
					public Singleton<Cellappmgr>
{
public:
	enum TimeOutType
	{
		TIMEOUT_GAME_TICK = TIMEOUT_SERVERAPP_MAX + 1
	};
	
	Cellappmgr(Mercury::EventDispatcher& dispatcher, 
		Mercury::NetworkInterface& ninterface, 
		COMPONENT_TYPE componentType,
		COMPONENT_ID componentID);

	~Cellappmgr();
	
	bool run();
	
	virtual void onChannelDeregister(Mercury::Channel * pChannel);

	void handleTimeout(TimerHandle handle, void * arg);
	void handleGameTick();

	/* 初始化相关接口 */
	bool initializeBegin();
	bool inInitialize();
	bool initializeEnd();
	void finalise();

	/** 找出一个最空闲的cellapp */
	COMPONENT_ID findFreeCellapp(void);
	void updateBestCellapp();

	/** 网络接口
		baseEntity请求创建在一个新的space中
	*/
	void reqCreateInNewSpace(Mercury::Channel* pChannel, MemoryStream& s);

	/** 网络接口
		baseEntity请求创建在一个新的space中
	*/
	void reqRestoreSpaceInCell(Mercury::Channel* pChannel, MemoryStream& s);
	
	/** 网络接口
		消息转发， 由某个app想通过本app将消息转发给某个app。
	*/
	void forwardMessage(Mercury::Channel* pChannel, MemoryStream& s);

	/** 网络接口
		更新cellapp情况。
	*/
	void updateCellapp(Mercury::Channel* pChannel, COMPONENT_ID componentID, float load);
protected:
	TimerHandle							gameTimer_;
	ForwardAnywhere_MessageBuffer		forward_cellapp_messagebuffer_;

	COMPONENT_ID						bestCellappID_;

	std::map< COMPONENT_ID, Cellapp >	cellapps_;
};

}

#endif // KBE_CELLAPPMGR_HPP
