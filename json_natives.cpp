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

static IJsonus *GetJSONFromHandle(IPluginContext *pContext, Handle_t hndl)
{
	HandleError err;
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());

	Jsonus *p;
	if ((err=handlesys->ReadHandle(hndl, htJSON, &sec, (void **)&p)) != HandleError_None)
	{
		pContext->ThrowNativeError("Invalid JSON handle %x (error %d)", hndl, err);
	}

	return p;
}

static cell_t CreateNullObject(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = new Jsonus();

	Handle_t hndl = handlesys->CreateHandle(htJSON, p, pContext->GetIdentity(), myself->GetIdentity(), NULL);
	if (hndl == BAD_HANDLE)
		pContext->ThrowNativeError("Could not create object handle.");

	return hndl;
}

// static cell_t GetObjectSize(IPluginContext *pContext, const cell_t *params)
// {
// 	json_t *object = GetJSONFromHandle(pContext, params[1]);
// 	if (object == NULL)
// 	{
// 		return 0;
// 	}

// 	return json_object_size(object);
// }

static cell_t GetObjectValue(IPluginContext *pContext, const cell_t *params)
{
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());

	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);

	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Json is already empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	Jsonus *j = (Jsonus *) p->Get(key);

	if(!j || j->GetType() == value_t::null)
	{
		delete j;
		return BAD_HANDLE;
	}

	Handle_t hndlValue = handlesys->CreateHandleEx(htJSON, j, &sec, NULL, NULL);
	if (hndlValue == BAD_HANDLE)
	{
		delete j;

		pContext->ThrowNativeError("Could not create value handle.");
		return BAD_HANDLE;
	}

	return hndlValue;
}

static cell_t GetObjectBoolValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	
	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Invalid JSON Handle or JSON is empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return p->GetBool(key);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError("%s", e.what());
	}
	
	return false;
}

static cell_t GetObjectFloatValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	
	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Invalid JSON Handle or JSON is empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return sp_ftoc(p->GetFloat(key));
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError("%s", e.what());
	}
	
	return -1.0;
}

static cell_t GetObjectIntValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	
	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Invalid JSON Handle or JSON is empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return static_cast<cell_t>(p->GetInt(key));
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError("%s", e.what());
	}
	
	return 0;
}

static cell_t GetObjectStringValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	
	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Invalid JSON Handle or JSON is empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		const char *value = p->GetString(key);
		pContext->StringToLocalUTF8(params[3], params[4], value, NULL);
		delete value;
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError("%s", e.what());
	}
	
	return 0;
}

static cell_t IsObjectNullValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	
	if(!p || p->GetType() == value_t::null)
	{
		pContext->ThrowNativeError("Invalid JSON Handle or JSON is empty.");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return p->IsNull(key);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError("%s", e.what());
	}
	
	return 0;
}

static cell_t IsObjectKeyValid(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->hasKey(key);
}

static cell_t SetObjectValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	Jsonus *j = (Jsonus *) GetJSONFromHandle(pContext, params[3]);
	if (!j)
		return 0;

	return p->Set(key, j);
}

static cell_t SetObjectBoolValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetBool(key, (bool) params[3]);
}

static cell_t SetObjectFloatValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetFloat(key, sp_ctof(params[3]));
}

static cell_t SetObjectIntValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetInt(key, (int) params[3]);
}

// static cell_t SetObjectInt64Value(IPluginContext *pContext, const cell_t *params)
// {
// 	json_t *object = GetJSONFromHandle(pContext, params[1]);
// 	if (object == NULL)
// 	{
// 		return 0;
// 	}

// 	char *key;
// 	pContext->LocalToString(params[2], &key);

// 	char *val;
// 	pContext->LocalToString(params[3], &val);

// 	json_t *value = json_integer(strtoll(val, NULL, 10));

// 	return (json_object_set_new(object, key, value) == 0);
// }

// static cell_t SetObjectNullValue(IPluginContext *pContext, const cell_t *params)
// {
// 	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
// 	if (!p)
// 		return 0;

// 	char *key;
// 	pContext->LocalToString(params[2], &key);

// 	return p->SetInt(key, (int) params[3]);
// }

static cell_t SetObjectStringValue(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	char *value;
	pContext->LocalToString(params[3], &value);

	return p->SetString(key, value);
}

static cell_t RemoveFromObject(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	char *key;
	pContext->LocalToString(params[2], &key);

	p->RemoveKey(key);

	return 0;
}

static cell_t ClearObject(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	p->Clear();

	return 0;
}

// static cell_t CreateObjectKeys(IPluginContext *pContext, const cell_t *params)
// {
// 	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());

// 	json_t *object = GetJSONFromHandle(pContext, params[1]);
// 	if (object == NULL)
// 	{
// 		return BAD_HANDLE;
// 	}

// 	struct JSONObjectKeys *keys = new struct JSONObjectKeys(object);

// 	Handle_t hndlKeys = handlesys->CreateHandleEx(htJSONObjectKeys, keys, &sec, NULL, NULL);
// 	if (hndlKeys == BAD_HANDLE)
// 	{
// 		delete keys;

// 		pContext->ThrowNativeError("Could not create object keys handle.");
// 		return BAD_HANDLE;
// 	}

// 	return hndlKeys;
// }

static cell_t GetType(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	return (cell_t) p->GetType();
}

static cell_t FromString(IPluginContext *pContext, const cell_t *params)
{
	char *buffer;
	pContext->LocalToString(params[1], &buffer);
	
	IJsonus *p = nullptr;

	try
	{
		p = (IJsonus *) new Jsonus(buffer);
	}
	catch(const std::exception& e)
	{
		if(p)
			delete p;

		pContext->ThrowNativeError("%s", e.what());
	}
	
	if(!p) return 0;

	Handle_t hndlObject = handlesys->CreateHandle(htJSON, p, pContext->GetIdentity(), myself->GetIdentity(), NULL);
	if (hndlObject == BAD_HANDLE)
	{
		delete p;

		pContext->ThrowNativeError("Could not create object handle.");
		return BAD_HANDLE;
	}

	return hndlObject;
}

// static cell_t FromFile(IPluginContext *pContext, const cell_t *params)
// {
// 	char *path;
// 	pContext->LocalToString(params[1], &path);

// 	char realpath[PLATFORM_MAX_PATH];
// 	smutils->BuildPath(Path_Game, realpath, sizeof(realpath), "%s", path);

// 	size_t flags = (size_t)params[2];

// 	json_error_t error;
// 	json_t *object = json_load_file(realpath, flags, &error);
// 	if (object == NULL)
// 	{
// 		pContext->ThrowNativeError("Invalid JSON in line %d, column %d: %s", error.line, error.column, error.text);
// 		return BAD_HANDLE;
// 	}

// 	Handle_t hndlObject = handlesys->CreateHandle(htJSON, object, pContext->GetIdentity(), myself->GetIdentity(), NULL);
// 	if (hndlObject == BAD_HANDLE)
// 	{
// 		json_decref(object);

// 		pContext->ThrowNativeError("Could not create object handle.");
// 		return BAD_HANDLE;
// 	}

// 	return hndlObject;
// }

static cell_t ToString(IPluginContext *pContext, const cell_t *params)
{
	Jsonus *p = (Jsonus *) GetJSONFromHandle(pContext, params[1]);
	if (!p)
		return 0;

	size_t flags = (size_t)params[4];

	const char *result = p->print(flags).c_str();
	if (result == NULL)
		return 0;

	pContext->StringToLocalUTF8(params[2], params[3], result, NULL);
	delete p;

	return 1;
}

// static cell_t ToFile(IPluginContext *pContext, const cell_t *params)
// {
// 	json_t *object = GetJSONFromHandle(pContext, params[1]);
// 	if (object == NULL)
// 	{
// 		return 0;
// 	}

// 	char *path;
// 	pContext->LocalToString(params[2], &path);

// 	char realpath[PLATFORM_MAX_PATH];
// 	smutils->BuildPath(Path_Game, realpath, sizeof(realpath), "%s", path);

// 	size_t flags = (size_t)params[3];

// 	return (json_dump_file(object, realpath, flags) == 0);
// }


const sp_nativeinfo_t json_natives[] =
{
	// Objects
	{"JSON.JSON",					CreateNullObject},
	// {"JSON.Size.get",				},
	{"JSON.Get",					GetObjectValue},
	{"JSON.GetBool",				GetObjectBoolValue},
	{"JSON.GetFloat",				GetObjectFloatValue},
	{"JSON.GetInt",					GetObjectIntValue},
	// {"JSON.GetInt64",				GetObjectInt64Value},
	{"JSON.GetString",				GetObjectStringValue},
	{"JSON.IsNull",					IsObjectNullValue},
	{"JSON.HasKey",					IsObjectKeyValid},
	{"JSON.Set",					SetObjectValue},
	{"JSON.SetBool",				SetObjectBoolValue},
	{"JSON.SetFloat",				SetObjectFloatValue},
	{"JSON.SetInt",					SetObjectIntValue},
	// {"JSON.SetInt64",				SetObjectInt64Value},
	// {"JSON.SetNull",				SetObjectNullValue},
	{"JSON.SetString",				SetObjectStringValue},
	{"JSON.Remove",					RemoveFromObject},
	{"JSON.Clear",					ClearObject},

	// {"JSON.Keys",					CreateObjectKeys},
	// {"JSONObjectKeys.ReadKey",			ReadObjectKey},

	// Decoding
	{"JSON.FromString",				FromString},
	// {"JSON.FromFile",				FromFile},
	// {"JSONArray.FromString",		FromString},
	// {"JSONArray.FromFile",				FromFile},

	// Encoding
	{"JSON.ToString",					ToString},
	// {"JSON.ToFile",						ToFile},

	{NULL,								NULL}
};
