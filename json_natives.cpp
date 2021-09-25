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

	IJsonus *p = NULL;
	if ((err=handlesys->ReadHandle(hndl, htJSON, &sec, (void **)&p)) != HandleError_None)
		pContext->ThrowNativeError("Invalid JSON handle %x (error %d)", hndl, err);

	return p;
}

static cell_t CreateNullJson(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p = new Jsonus();

	Handle_t hndl = BAD_HANDLE;
	if ((hndl = handlesys->CreateHandle(htJSON, p, pContext->GetIdentity(), myself->GetIdentity(), NULL)) == BAD_HANDLE)
		pContext->ThrowNativeError("Could not create JSON handle.");

	return hndl;
}

static cell_t GetObjectSize(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
		pContext->ThrowNativeError("Invalid JSON handle");

	return !p ? -1 : p->size();
}

static cell_t GetJSONValue(IPluginContext *pContext, const cell_t *params)
{
	HandleSecurity sec(pContext->GetIdentity(), myself->GetIdentity());

	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;


	char *key;
	pContext->LocalToString(params[2], &key);

	IJsonus *j;

	try
	{
		j = p->Get(key);
	}
	catch (const std::exception &e)
	{
		pContext->ThrowNativeError(e.what());

		if (j) delete j;

		return BAD_HANDLE;
	}

	Handle_t hndlValue; 
	if ((hndlValue = handlesys->CreateHandleEx(htJSON, j, &sec, NULL, NULL)) == BAD_HANDLE)
	{
		delete j;
		pContext->ThrowNativeError("(JSON::Get) Could not create JSON handle.");
	}

	return hndlValue;
}

static cell_t GetObjectBoolValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return p->GetBool(key);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError(e.what());
	}
	
	return false;
}

static cell_t GetObjectFloatValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return sp_ftoc(p->GetFloat(key));
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError(e.what());
	}
	
	return -1.0;
}

static cell_t GetObjectIntValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return (cell_t) p->GetInt(key);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError(e.what());
	}
	
	return -1;
}

static cell_t GetObjectStringValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		pContext->StringToLocalUTF8(params[3], params[4], p->GetString(key), NULL);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError(e.what());
	}
	
	return (size_t) params[4];
}

static cell_t IsObjectNullValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	// Json is JSONT_NULL type;;;
	// This an empty JSON
	if (!p->GetType())
		return BAD_HANDLE;

	char *key;
	pContext->LocalToString(params[2], &key);

	try
	{		
		return p->IsNull(key);
	}
	catch(const std::exception& e)
	{
		pContext->ThrowNativeError(e.what());
	}
	
	return 0;
}

static cell_t IsObjectKeyValid(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->hasKey(key);
}

static cell_t SetObjectValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	IJsonus *j;
	if ((p = GetJSONFromHandle(pContext, params[2])) == NULL)
	{
		pContext->ThrowNativeError("(JSON::Set::arg) Invalid JSON handle");
		return BAD_HANDLE;
	}

	return p->Set(key, j);
}

static cell_t SetObjectBoolValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetBool(key, (bool) params[3]);
}

static cell_t SetObjectFloatValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetFloat(key, sp_ctof(params[3]));
}

static cell_t SetObjectIntValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	return p->SetInt(key, (int) params[3]);
}

static cell_t SetObjectInt64Value(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	char *value;
	pContext->LocalToString(params[3], &value);

	return p->SetInt64(key, strtol(value, NULL, 10));
}

static cell_t SetObjectNullValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	IJsonus *j = new Jsonus();

	if (p->Set(key, j))
	{
		delete j;
		return true;
	}

	delete j;

	return false;
}

static cell_t SetObjectStringValue(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	char *value;
	pContext->LocalToString(params[3], &value);

	return p->SetString(key, value);
}

static cell_t RemoveFromObject(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *key;
	pContext->LocalToString(params[2], &key);

	p->removeKey(key);

	return 0;
}

static cell_t ClearObject(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	p->clear();

	return 0;
}

static cell_t GetType(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	return p->GetType();
}

static cell_t FromString(IPluginContext *pContext, const cell_t *params)
{
	char *buffer;
	pContext->LocalToString(params[1], &buffer);
	
	IJsonus *p = NULL;

	try
	{
		p = new Jsonus(buffer);
	}
	catch(const std::exception& e)
	{
		if(p) delete p;

		pContext->ThrowNativeError(e.what());
	}
	
	if(!p) return 0;

	Handle_t hndlObject;
	if ((hndlObject = handlesys->CreateHandle(htJSON, p, pContext->GetIdentity(), myself->GetIdentity(), NULL)) == BAD_HANDLE)
	{
		delete p;
		pContext->ThrowNativeError("Could not create object handle.");
	}

	return hndlObject;
}

static cell_t Deserialize(IPluginContext *pContext, const cell_t *params)
{
	char *path;
	pContext->LocalToString(params[1], &path);

	char realpath[PLATFORM_MAX_PATH];
	smutils->BuildPath(Path_Game, realpath, sizeof(realpath), "%s", path);

	std::ifstream i(realpath);
	if (!i)
	{
		pContext->ThrowNativeError("(JSON::Deser)Cannot open file: %s", realpath);
		return BAD_HANDLE;
	}

	IJsonus *p;

	try
	{
		p = new Jsonus(&i);
	}
	catch (const std::exception& e)
	{
		if (p) delete p;

		i.close(); // :/

		pContext->ThrowNativeError(e.what());
	}

	i.close();

	if (!p)
		return BAD_HANDLE;

	Handle_t hndlObject;
	if ((hndlObject = handlesys->CreateHandle(htJSON, p, pContext->GetIdentity(), myself->GetIdentity(), NULL)) == BAD_HANDLE)
	{
		delete p;
		pContext->ThrowNativeError("Could not create object handle.");
	}

	return hndlObject;
}

static cell_t ToString(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	size_t tabs = (size_t)params[4];

	pContext->StringToLocalUTF8(params[2], params[3], p->print(tabs), NULL);

	return 1;
}

static cell_t Serialize(IPluginContext *pContext, const cell_t *params)
{
	IJsonus *p;
	if ((p = GetJSONFromHandle(pContext, params[1])) == NULL)
	{
		pContext->ThrowNativeError("Invalid JSON handle");
		return BAD_HANDLE;
	}

	char *path;
	pContext->LocalToString(params[2], &path);

	char realpath[PLATFORM_MAX_PATH];
	smutils->BuildPath(Path_Game, realpath, sizeof(realpath), "%s", path);

	std::ofstream o(realpath);
	if (!o)
	{
		pContext->ThrowNativeError("(JSON::Serialize) Cannot open file: %s", realpath);
		return false;
	}

	size_t tabs = (size_t) params[3];

	o << p->print(tabs) << std::endl;

	o.close();

	return true;
}

const sp_nativeinfo_t json_natives[] =
{
	// Objects
	{"JSON.JSON",					CreateNullJson},
	{"JSON.Size.get",				GetObjectSize},
	{"JSON.Get",					GetJSONValue},
	{"JSON.GetBool",				GetObjectBoolValue},
	{"JSON.GetFloat",				GetObjectFloatValue},
	{"JSON.GetInt",					GetObjectIntValue},
	// {"JSON.GetInt64",				GetObjec},
	{"JSON.GetString",				GetObjectStringValue},
	{"JSON.IsNull",					IsObjectNullValue},
	{"JSON.HasKey",					IsObjectKeyValid},
	{"JSON.Set",					SetObjectValue},
	{"JSON.SetBool",				SetObjectBoolValue},
	{"JSON.SetFloat",				SetObjectFloatValue},
	{"JSON.SetInt",					SetObjectIntValue},
	{"JSON.SetInt64",				SetObjectInt64Value},
	{"JSON.SetNull",				SetObjectNullValue},
	{"JSON.SetString",				SetObjectStringValue},
	{"JSON.Remove",					RemoveFromObject},
	{"JSON.Clear",					ClearObject},
	{"JSON.GetType",				GetType},
	
	// Decoding
	{"JSON.FromString",				FromString},
	{"JSON.FromFile",				Deserialize},

	// Encoding
	{"JSON.ToString",					ToString},
	{"JSON.ToFile",						Serialize},

	{NULL,								NULL}
};
