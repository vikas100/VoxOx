/*
 * WengoPhone, a voice over Internet phone
 * Copyright (C) 2004-2007  Wengo
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"		//VOXOX - JRT - 2009.04.05
#include "QtPresenceSIPMenuManager.h"
#include <model/profile/UserProfile.h>
#include <control/profile/CUserProfile.h>
#include "QtPresenceIMMenuManager.h"
#include <model/connect/ConnectHandler.h>
#include <presentation/qt/QtIMAccountHelper.h>
#include <QtGui/QMenu>
#include <util/SafeDelete.h>


QtPresenceSIPMenuManager::QtPresenceSIPMenuManager(QObject* parent, CUserProfile* cUserProfile, QtWengoPhone * qtWengoPhone)
	: QtPresenceMenuManager(parent, cUserProfile, qtWengoPhone)
{


}

QtPresenceSIPMenuManager::~QtPresenceSIPMenuManager(){
	OWSAFE_DELETE(_logOffAction);
	OWSAFE_DELETE(_actionsGroup);
}

void QtPresenceSIPMenuManager::addPresenceActions(QMenu* menu, EnumPresenceState::PresenceState currentPresenceState) {
	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateOnline, currentPresenceState,
		QPixmap(":/pics/status/online.png"), tr("Online"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateAway, currentPresenceState,
		QPixmap(":/pics/status/away.png"), tr("Away"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateDoNotDisturb, currentPresenceState,
		QPixmap(":/pics/status/donotdisturb.png"), tr("Do Not Disturb"));

	addGlobalPresenceAction(
		menu, EnumPresenceState::PresenceStateInvisible, currentPresenceState,
		QPixmap(":/pics/status/invisible.png"), tr("Invisible"));

	//menu->addSeparator();
	_logOffAction = menu->addAction(QPixmap(":/pics/status/offline.png"), tr("Offline"), this, SLOT(logOff()) );

	_logOffAction->setCheckable ( true );
	_actionsGroup->addAction(_logOffAction);
	
	bool connected = _cUserProfile->getUserProfile().isSIPConnected();
	_logOffAction->setEnabled(connected);
	if(!connected){
		_logOffAction->setChecked(true);
	}
}

void QtPresenceSIPMenuManager::slotGlobalPresenceActionTriggered() {
	IMAccountList imAccountList = _cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
	QtIMAccountHelper::QtIMAccountPtrVector imAccountPtrVector;
	QtIMAccountHelper::copyListToPtrVector(imAccountList, &imAccountPtrVector);
	std::sort(imAccountPtrVector.begin(), imAccountPtrVector.end(), QtIMAccountHelper::compareIMAccountPtrs);

	// Updates IMAccounts icons status
	QtIMAccountHelper::QtIMAccountPtrVector::const_iterator
		it = imAccountPtrVector.begin(),
		end = imAccountPtrVector.end();

	for (; it!=end; ++it) 
	{
//		if((*it)->getProtocol() == EnumIMProtocol::IMProtocolWengo || (*it)->getProtocol() == EnumIMProtocol::IMProtocolSIP)
		if( EnumIMProtocol::isSip( (*it)->getProtocol() ) )
		{
			EnumPresenceState::PresenceState state = _actionToState[sender()];
			_cUserProfile->getUserProfile().setPresenceState(state, (*it)->getKey(), true, String::null );	//VOXOX - JRT - 2009.09.14 
		}		
	}
	
}

void QtPresenceSIPMenuManager::logOff() {
	IMAccountList imAccountList = _cUserProfile->getUserProfile().getIMAccountManager().getIMAccountListCopy();
	QtIMAccountHelper::QtIMAccountPtrVector imAccountPtrVector;
	QtIMAccountHelper::copyListToPtrVector(imAccountList, &imAccountPtrVector);
	std::sort(imAccountPtrVector.begin(), imAccountPtrVector.end(), QtIMAccountHelper::compareIMAccountPtrs);

	// Updates IMAccounts icons status
	QtIMAccountHelper::QtIMAccountPtrVector::const_iterator
		it = imAccountPtrVector.begin(),
		end = imAccountPtrVector.end();

	for (; it!=end; ++it) 
	{
//		if((*it)->getProtocol() == EnumIMProtocol::IMProtocolWengo || (*it)->getProtocol() == EnumIMProtocol::IMProtocolSIP)
		if( EnumIMProtocol::isSip( (*it)->getProtocol() ) )
		{			
			_cUserProfile->getUserProfile().getConnectHandler().disconnect((*it)->getKey());
		}		
	}
}
