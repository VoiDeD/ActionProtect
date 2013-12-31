/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod Sample Extension
 * Copyright (C) 2004-2008 AlliedModders LLC.  All rights reserved.
 * =============================================================================
 *
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License, version 3.0, as published by the
 * Free Software Foundation.
 * 
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * As a special exception, AlliedModders LLC gives you permission to link the
 * code of this program (as well as its derivative works) to "Half-Life 2," the
 * "Source Engine," the "SourcePawn JIT," and any Game MODs that run on software
 * by the Valve Corporation.  You must obey the GNU General Public License in
 * all respects for all other code used.  Additionally, AlliedModders LLC grants
 * this exception to all derivative works.  AlliedModders LLC defines further
 * exceptions, found in LICENSE.txt (as of this writing, version JULY-31-2007),
 * or <http://www.sourcemod.net/license.php>.
 *
 * Version: $Id$
 */



#include "extension.h"

#include "cbase.h"

// memdbgon must be the last include file in a .cpp file!!!
#include "tier0/memdbgon.h"

/**
 * @file extension.cpp
 * @brief Implement extension code here.
 */

ActionProtect g_ActionProtect;		/**< Global singleton for extension's main interface */

SMEXT_LINK( &g_ActionProtect );


IServerGameClients *g_pServerGameClients = NULL;

SH_DECL_HOOK2_void( IServerGameClients, ClientCommandKeyValues, SH_NOATTRIB, 0, edict_t *, KeyValues * );


bool ActionProtect::SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlen, bool late)
{
	GET_V_IFACE_CURRENT( GetServerFactory, g_pServerGameClients, IServerGameClients, INTERFACEVERSION_SERVERGAMECLIENTS );

	SH_ADD_HOOK( IServerGameClients, ClientCommandKeyValues, g_pServerGameClients, SH_MEMBER( this, &ActionProtect::ClientCommandKeyValues ), false );

	return true;
}

bool ActionProtect::SDK_OnMetamodUnload(char *error, size_t maxlength)
{
	SH_REMOVE_HOOK( IServerGameClients, ClientCommandKeyValues, g_pServerGameClients, SH_MEMBER( this, &ActionProtect::ClientCommandKeyValues ), false );

	return true;
}


void ActionProtect::ClientCommandKeyValues( edict_t *pEntity, KeyValues *pKeyValues )
{
	if ( !pEntity || !pKeyValues || !pKeyValues->GetName() )
		return;

	int client = gamehelpers->IndexOfEdict( pEntity );

	if ( V_stristr( pKeyValues->GetName(), "use_action_slot_item" ) != NULL )
	{
		int targetEnt = pKeyValues->GetInt( "entindex" );

		if ( targetEnt != client )
		{
			// why valve...
			RETURN_META( MRES_SUPERCEDE );
		}
	}

	RETURN_META( MRES_IGNORED );
}
