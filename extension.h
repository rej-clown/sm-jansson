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

#ifndef _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
#define _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_

#include <jansson.h>
#include "smsdk_ext.h"

enum JsonUpdateType
{
	JSON_UPDATE = 0, 
    JSON_UPDATE_EXISTING,
    JSON_UPDATE_MISSING,
    JSON_UPDATE_RECURSIVE
};

struct JSONObjectKeys {
	JSONObjectKeys(json_t *object) : object(object), iter(json_object_iter(object)) {}

	const char *GetKey()
	{
		return json_object_iter_key(iter);
	}

	void Next()
	{
		iter = json_object_iter_next(object, iter);
	}

private:
	json_t *object;
	void *iter;
};


/**
 * @brief Implementation of the REST in Pawn Extension.
 * Note: Uncomment one of the pre-defined virtual functions in order to use it.
 */
class Jansson : public SDKExtension
{
public:
	/**
	 * @brief This is called after the initial loading sequence has been processed.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @param late		Whether or not the module was loaded after map load.
	 * @return			True to succeed loading, false to fail.
	 */
	virtual bool SDK_OnLoad(char *error, size_t maxlength, bool late);

	/**
	 * @brief This is called right before the extension is unloaded.
	 */
	virtual void SDK_OnUnload();

	/**
	 * @brief This is called once all known extensions have been loaded.
	 * Note: It is is a good idea to add natives here, if any are provided.
	 */
	//virtual void SDK_OnAllLoaded();

	/**
	 * @brief Called when the pause state is changed.
	 */
	//virtual void SDK_OnPauseChange(bool paused);

	/**
	 * @brief this is called when Core wants to know if your extension is working.
	 *
	 * @param error		Error message buffer.
	 * @param maxlength	Size of error message buffer.
	 * @return			True if working, false otherwise.
	 */
	//virtual bool QueryRunning(char *error, size_t maxlength);
public:
#if defined SMEXT_CONF_METAMOD
	/**
	 * @brief Called when Metamod is attached, before the extension version is called.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @param late			Whether or not Metamod considers this a late load.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodLoad(ISmmAPI *ismm, char *error, size_t maxlength, bool late);

	/**
	 * @brief Called when Metamod is detaching, after the extension version is called.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodUnload(char *error, size_t maxlength);

	/**
	 * @brief Called when Metamod's pause state is changing.
	 * NOTE: By default this is blocked unless sent from SourceMod.
	 *
	 * @param paused		Pause state being set.
	 * @param error			Error buffer.
	 * @param maxlength		Maximum size of error buffer.
	 * @return				True to succeed, false to fail.
	 */
	//virtual bool SDK_OnMetamodPauseChange(bool paused, char *error, size_t maxlength);

#endif
};

class JSONHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

class JSONObjectKeysHandler : public IHandleTypeDispatch
{
public:
	void OnHandleDestroy(HandleType_t type, void *object);
};

extern Jansson g_Jansson;

extern JSONHandler	g_JSONHandler;
extern HandleType_t		htJSON;

extern JSONObjectKeysHandler	g_JSONObjectKeysHandler;
extern HandleType_t				htJSONObjectKeys;

extern const sp_nativeinfo_t json_natives[];

#endif // _INCLUDE_SOURCEMOD_EXTENSION_PROPER_H_
