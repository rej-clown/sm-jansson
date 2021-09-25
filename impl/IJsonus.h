#ifndef _INCLUDE_SOURCEMOD_JSON_INTERFACE_H_
#define _INCLUDE_SOURCEMOD_JSON_INTERFACE_H_

#include <nlohmann/json.hpp>
#include <IShareSys.h>

#define SMINTERFACE_MYINTERFACE_NAME	"IJsonus"
#define SMINTERFACE_MYINTERFACE_VERSION	1

using json = nlohmann::json;
using string_t = std::string;
using value_t = nlohmann::detail::value_t;

using namespace SourceMod;

class IJsonus : public SMInterface
{
    public:
    virtual string_t print(int tabs) =0;
    // virtual IJsonus *FromFile(const char *path) =0;

    virtual bool hasKey(const char *) =0;
    virtual IJsonus *create(void) =0;
    virtual void Clear(void) =0;
    
    public:
    virtual const char *GetString(const char *key) =0;
    virtual const char *GetInt64(const char *key) =0;
    virtual float GetFloat(const char *key) =0;
    virtual bool GetBool(const char *key) =0;
    virtual int GetInt(const char *key) =0;
    virtual IJsonus *Get(const char *key) =0;
    virtual value_t GetType(void) =0;
    virtual bool SetString(const char *key, const char *value) =0;
    virtual bool SetBool(const char *key, const bool value) =0;
    virtual bool SetInt(const char *key, const int value) =0;
    virtual bool Set(const char *key, IJsonus *value) =0;

	// virtual void RemoveKey(const char *key);
};


#endif