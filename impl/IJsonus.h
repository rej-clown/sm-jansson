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
		virtual char *print(int tabs=-1) =0;
		// virtual IJsonus *FromFile(const char *path) =0;

		virtual bool IsNull(const char *key) =0;
		virtual bool hasKey(const char *) =0;
		virtual void load(const char *line) =0;
		virtual void clear(void) =0;
		virtual IJsonus *create(const char *input) =0;
    
		public:
		virtual char *GetString(const char *key) =0;
		virtual float GetFloat(const char *key) =0;
		virtual long GetInt64(const char *key) =0;
		virtual bool GetBool(const char *key) =0;
		virtual IJsonus *Get(const char *key) =0;
		virtual int GetInt(const char *key) =0;
		virtual cell_t GetType(void) =0;

		virtual bool Write(const char *key, const char *value) =0;

		virtual void removeKey(const char *key) =0;
		virtual cell_t size() =0;
};


#endif