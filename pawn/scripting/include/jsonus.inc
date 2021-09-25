#if defined _jsonus_included_
	#endinput
#endif
#define _jsonus_included_

enum JSONTypes
{
	JSONT_NULL,             ///< null value
    JSONT_OBJECT,           ///< object (unordered set of name/value pairs)
    JSONT_ARRAY,            ///< array (ordered collection of values)
    JSONT_STRING,           ///< string value
    JSONT_BOOLEAN,          ///< boolean value
    JSONT_NUMBER_INTEGER,   ///< number value (signed integer)
    JSONT_NUMBER_UNSIGNED,  ///< number value (unsigned integer) :/
    JSONT_NUMBER_FLOAT,     ///< number value (floating-point)
    JSONT_BINARY,           ///< binary array (ordered collection of bytes)
    JSONT_DISCARDED         ///< discarded by the parser callback function
}

// Generic type for encoding JSON.
methodmap JSON < Handle
{
	// JSON string representation
	//
	// @param json_line   JSON lexicographical string
	// @return           JSON pointer
	// @error			Invalid JSON line format
	public native JSON FromString(const char[] json_line);

	// Retrieves the JSON string representation.
	//
	// @param buffer     String buffer to write to.
	// @param maxlength  Maximum length of the string buffer.
	// @param flags      Encoding flags.
	// @return           True on success, false on failure.
	public native bool ToString(char[] buffer, int maxlength, int tabs = -1);

	// Creates a JSON pointer with NULL(JSONTypes) type
	//
	// The JSON must be freed via delete or CloseHandle().
	public native JSON();

	// Retrieves an array or object value from the JSON pointer (JSONT_OBJECT).
	//
	// The JSON must be freed via delete or CloseHandle().
	//
	// @param key        Key string.
	// @return           Value read.
	// @error            Invalid key.
	public native JSON Get(const char[] key);

	// Retrieves a boolean value from the object(JSONT_OBJECT).
	//
	// @param key        Key string.
	// @return           Value read.
	// @error            Invalid key.
	public native bool GetBool(const char[] key);

	// Retrieves a float value from the object.
	//
	// @param key        Key string.
	// @return           Value read.
	// @error            Invalid key.
	public native float GetFloat(const char[] key);

	// Retrieves an integer value from the object (JSONT_OBJECT).
	//
	// @param key        Key string.
	// @return           Value read.
	// @error            Invalid key.
	public native int GetInt(const char[] key);

	// Retrieves a string value from the object (JSONT_OBJECT).
	//
	// @param key        Key string.
	// @param buffer     String buffer to store value.
	// @param maxlength  Maximum length of the string buffer.
	// @return           True on success. False if the key was not found, or the value is not a string.
	public native bool GetString(const char[] key, char[] buffer, int maxlength);

	// Returns whether or not a value in the object is null.
	//
	// @param key        Key string.
	// @return           True if the value is null, false otherwise.
	// @error            Invalid key.
	public native bool IsNull(const char[] key);

	// Returns whether or not a key exists in the object (JSONT_OBJECT).
	//
	// @param key        Key string.
	// @return           True if the key exists, false otherwise.
	public native bool HasKey(const char[] key);

	// Sets an array or object value in the object (JSONT_OBJECT), either inserting a new entry or replacing an old one.
	//
	// @param key        Key string.
	// @param value      Value to store at this key.
	// @return           True on success, false on failure.
	public native bool Set(const char[] key, JSON value);

	// Sets a boolean value in the object (JSONT_OBJECT), either inserting a new entry or replacing an old one.
	//
	// @param key        Key string.
	// @param value      Value to store at this key.
	// @return           True on success, false on failure.
	public native bool SetBool(const char[] key, bool value);

	// Sets a float value in the object (JSONT_OBJECT), either inserting a new entry or replacing an old one.
	//
	// @param key        Key string.
	// @param value      Value to store at this key.
	// @return           True on success, false on failure.
	public native bool SetFloat(const char[] key, float value);

	// Sets an integer value in the object (JSONT_OBJECT), either inserting a new entry or replacing an old one.
	//
	// @param key        Key string.
	// @param value      Value to store at this key.
	// @return           True on success, false on failure.
	public native bool SetInt(const char[] key, int value);

	// Sets a string value in the object, either inserting a new entry or replacing an old one.
	//
	// @param key        Key string.
	// @param value      Value to store at this key.
	// @return           True on success, false on failure.
	public native bool SetString(const char[] key, const char[] value);

	// Removes an entry from the object.
	//
	// @param key        Key string.
	// @return           True on success, false if the key was not found.
	public native bool Remove(const char[] key);

	// Clears the object of all entries.
	// @return           True on success, false on failure.
	public native bool Clear();
};

/**
 * Do not edit below this line!
 */
public Extension __ext_jansson =
{
	name = "JSON in Pawn",
	file = "jsonus.ext",
#if defined AUTOLOAD_EXTENSIONS
	autoload = 1,
#else
	autoload = 0,
#endif
#if defined REQUIRE_EXTENSIONS
	required = 1,
#else
	required = 0,
#endif
};