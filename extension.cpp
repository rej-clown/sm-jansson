/**
 * vim: set ts=4 :
 * =============================================================================
 * SourceMod REST in Pawn Extension
 * Copyright 2017-2020 Erik Minekus
 * =============================================================================
 *
 * This program is free software: you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "extension.h"

JSON 				g_JSON;		/**< Global singleton for extension's main interface */
IJsonus				*g_Jsonus;

SMEXT_LINK(&g_JSON);

JSONHandler			g_JSONHandler;
HandleType_t		htJSON;

bool JSON::SDK_OnLoad(char *error, size_t maxlength, bool late)
{
	sharesys->AddInterface(myself, g_Jsonus);
	sharesys->AddNatives(myself, json_natives);
	sharesys->RegisterLibrary(myself, "jsonus");

	/* Set up access rights for the 'JSON' handle type */
	HandleAccess haJSON;
	haJSON.access[HandleAccess_Clone] = 0;
	haJSON.access[HandleAccess_Delete] = 0;
	haJSON.access[HandleAccess_Read] = 0;

	htJSON = handlesys->CreateType("JSON", &g_JSONHandler, 0, NULL, &haJSON, myself->GetIdentity(), NULL);
	
	return true;
}

void JSON::SDK_OnUnload()
{
	handlesys->RemoveType(htJSON, myself->GetIdentity());
}

void JSONHandler::OnHandleDestroy(HandleType_t type, void *object)
{
	delete (IJsonus *)object;
}