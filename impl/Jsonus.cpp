#include "../extension.h"

Jsonus::Jsonus()
{
    json_p = new json();
}

Jsonus::Jsonus(const char *input)
{
    json_p = new json(json::parse(input));
}

Jsonus::Jsonus(IJsonus *p)
{
    json_p = new json(json::parse(p->print(-1)));
}

IJsonus *Jsonus::create()
{
    return (IJsonus *) new Jsonus();
}

Jsonus::~Jsonus()
{
    delete json_p;
}

bool Jsonus::hasKey(const char *key)
{
    return json_p->contains(key);
}

string_t Jsonus::print(int tabs)
{
    return json_p->dump(tabs);
}

const char *Jsonus::GetString(const char *key) 
{
    return json_p->at(key).get<string_t>().c_str();
}

value_t Jsonus::GetType()
{
    return json_p->type();
}

float Jsonus::GetFloat(const char *key)
{
    return json_p->at(key).get<float>();
}

int Jsonus::GetInt(const char *key)
{
    return json_p->at(key).get<int>();
}

bool Jsonus::GetBool(const char *key)
{
    return json_p->at(key).get<bool>();
}

const char *Jsonus::GetInt64(const char *key)
{
    const char *buffer = "hello world!";

    // sprintf(buffer, "%ld", json_p->at(key).get<long>());

    return buffer;
}

IJsonus *Jsonus::Get(const char *key)
{
    return (IJsonus *) new Jsonus(json_p->at(key).get<json>().dump().c_str());
}

bool Jsonus::SetString(const char *key, const char *value)
{
    json_p->at(key) = value;

    return strcmp(GetString(key), value) == 0; 
}

bool Jsonus::SetInt(const char *key, const int value)
{
    json_p->at(key) = value;

    return GetInt(key) == value;
}

bool Jsonus::IsNull(const char *key)
{
    return json_p->at(key).is_null();
}

bool Jsonus::SetBool(const char *key, const bool value)
{
    json_p->at(key) = value;

    return GetBool(key) == value;
}

// bool Jsonus::SetNull(const char *key)
// {

// }

bool Jsonus::Set(const char *key, IJsonus *value)
{
    json_p->emplace(key, json::parse(value->print(-1)));

    return strcmp(print(-1).c_str(), value->print(-1).c_str()) == 0;
}

bool Jsonus::SetFloat(const char *key, const float value)
{
	json_p->at(key) = value;

	return json_p->at(key).type() == value_t::number_float;
}

void Jsonus::RemoveKey(const char *key)
{
    if(hasKey(key))
        json_p->erase(key);
}

void Jsonus::Clear()
{
    json_p->clear();
}

