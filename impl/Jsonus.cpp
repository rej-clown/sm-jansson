#include "../extension.h"

Jsonus::Jsonus()
{
    m_pJson = new json();
}

Jsonus::Jsonus(const char *input)
{
    m_pJson = new json(json::parse(input));
}

Jsonus::Jsonus(IJsonus *p)
{
    m_pJson = new json(json::parse(p->print(-1)));
}

Jsonus::Jsonus(std::ifstream *i)
{
	m_pJson = new json(json::parse(*i));
}

IJsonus *Jsonus::create()
{
    return (IJsonus *) new Jsonus();
}

Jsonus::~Jsonus()
{
    delete m_pJson;
}

void Jsonus::removeKey(const char *key)
{
	if (hasKey(key))
		m_pJson->erase(key);
}

bool Jsonus::hasKey(const char *key)
{
    return m_pJson->contains(key);
}

const char *Jsonus::print(int tabs)
{
    return m_pJson->dump(tabs).c_str();
}

const char *Jsonus::GetString(const char *key) 
{
    return m_pJson->at(key).get<string_t>().c_str();
}

cell_t Jsonus::GetType()
{
    return (cell_t) m_pJson->type();
}

float Jsonus::GetFloat(const char *key)
{
    return m_pJson->at(key).get<float>();
}

int Jsonus::GetInt(const char *key)
{
    return m_pJson->at(key).get<int>();
}

bool Jsonus::GetBool(const char *key)
{
    return m_pJson->at(key).get<bool>();
}

long Jsonus::GetInt64(const char *key)
{
    return m_pJson->at(key).get<long>();
}

IJsonus *Jsonus::Get(const char *key)
{
    return (IJsonus *) new Jsonus(m_pJson->at(key).get<json>().dump().c_str());
}

bool Jsonus::SetString(const char *key, const char *value)
{
    m_pJson->at(key) = value;

    return strcmp(GetString(key), value) == 0; 
}

bool Jsonus::SetInt(const char *key, const int value)
{
    m_pJson->at(key) = value;

    return GetInt(key) == value;
}

bool Jsonus::IsNull(const char *key)
{
    return m_pJson->at(key).is_null();
}

bool Jsonus::SetBool(const char *key, const bool value)
{
    m_pJson->at(key) = value;

    return GetBool(key) == value;
}

bool Jsonus::SetInt64(const char *key, const long value)
{
	m_pJson->at(key) = value;

	return GetInt64(key) == value;
}

// bool Jsonus::SetNull(const char *key)
// {

// }

bool Jsonus::Set(const char *key, IJsonus *value)
{
    m_pJson->emplace(key, json::parse(value->print(-1)));

    return strcmp(print(-1), value->print(-1)) == 0;
}

bool Jsonus::SetFloat(const char *key, const float value)
{
	m_pJson->at(key) = value;

	return m_pJson->at(key).type() == value_t::number_float;
}

cell_t Jsonus::size()
{
	return (cell_t) m_pJson->size();
}

void Jsonus::clear()
{
    m_pJson->clear();
}

