#ifndef _INCLUDE_SOURCEMOD_JSON_INTERFACE_H_
#define _INCLUDE_SOURCEMOD_JSON_INTERFACE_H_

#include <IShareSys.h>

#define SMINTERFACE_JSONUS_NAME	"IJsonus"
#define SMINTERFACE_JSONUS_VERSION	1

using namespace SourceMod;

class IJsonus : public SMInterface
{
    public:
    virtual ~IJsonus() {};

    public:
    virtual const char *print(int tabs) =0;
    // virtual IJsonus *FromFile(const char *path) =0;

	virtual bool IsNull(const char *key) =0;
    virtual bool hasKey(const char *) =0;
    virtual IJsonus *create(void) =0;
    virtual void clear(void) =0;
    
    public:
    virtual const char *GetString(const char *key) =0;
    virtual float GetFloat(const char *key) =0;
    virtual long GetInt64(const char *key) =0;
    virtual bool GetBool(const char *key) =0;
    virtual IJsonus *Get(const char *key) =0;
    virtual int GetInt(const char *key) =0;
    virtual cell_t GetType(void) =0;

	virtual bool SetFloat(const char *key, float value) =0;
    virtual bool SetString(const char *key, const char *value) =0;
	virtual bool SetInt64(const char *key, const long value) = 0;
    virtual bool SetBool(const char *key, const bool value) =0;
    virtual bool SetInt(const char *key, const int value) =0;
    virtual bool Set(const char *key, IJsonus *value) =0;

	virtual void removeKey(const char *key) =0;
	virtual cell_t size() =0;
};


#endif