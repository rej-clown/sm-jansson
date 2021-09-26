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

char *Jsonus::print(int tabs)
{
	std::string x; x.assign(m_pJson->dump(tabs));

	char *cptr = new char[x.length() + 1];
	std::strcpy(cptr, x.c_str());

    return cptr;
}

char *Jsonus::GetString(const char *key) 
{
	std::string x; x.assign(m_pJson->at(key).get<string_t>());

	char *cptr = new char[x.length() + 1];
	std::strcpy(cptr, x.c_str());

	return cptr;
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
    return (IJsonus *) new Jsonus(const_cast<char *>(m_pJson->at(key).get<json>().dump().c_str()));
}

bool Jsonus::IsNull(const char *key)
{
    return m_pJson->at(key).is_null();
}

void Jsonus::update(json val)
{
	m_pJson->update(val);
}

bool Jsonus::Write(const char *key, const char *value)
{
	json obj;
	obj[key] = json::parse(value);

	update(obj);

	return hasKey(key);
}

cell_t Jsonus::size()
{
	return (cell_t) m_pJson->size();
}

void Jsonus::clear()
{
    m_pJson->clear();
}

