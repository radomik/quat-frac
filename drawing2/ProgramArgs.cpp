#include "stdafx.h"
#include "ProgramArgs.hpp"

#ifdef UNICODE
	/// Constructor
	ProgramArgs::ProgramArgs(int argc, wchar_t **argv) 
	{
		this->argv     = argv;
		this->argc     = argc;
	}
	
	int ProgramArgs::init() {
#ifdef PROGRAM_ARGS_HAS_ARGV_NON_UTF16
		if (! (argv_non_utf16 = _getArgvNonUtf16(argc, argv))) {
			fprintf(stderr, "%s: _getArgvNonUtf16() failed\n", __FUNCTION__);
			return -1;
		}
		return 0;
#else
		argv_non_utf16 = NULL;
		return 0;
#endif
	}
	
	/// Destructor ; used only if PROGRAM_ARGS_HAS_ARGV_NON_UTF16 is defined
	#ifdef PROGRAM_ARGS_HAS_ARGV_NON_UTF16
		ProgramArgs::~ProgramArgs() 
		{
			if (! argv_non_utf16) return;
			
			for (int i = 0; i < argc_non_utf16; i++) 
			{
				delete[] argv_non_utf16[i];
			}
			delete[] argv_non_utf16;
			
			//fprintf(stderr, "~ProgramArgs: Cleared %d items\n", argc_non_utf16);
			this->argc_non_utf16 = 0;
			argv_non_utf16 = NULL;
		}
	#endif /* PROGRAM_ARGS_HAS_ARGV_NON_UTF16 */
	
	/** Get argument from given index
	 * @param out_str  String to store output argument value
	 * @param argno    Index of argument inside argv
	 * 
	 * @return true  if arguemnt was retrieved
	 * @return false if argno index was invalid
	 */
	bool ProgramArgs::getArg(std::wstring &out_str, int argno) const 
	{
		if ((argno < argc) && (argno >= 0)) 
		{
			out_str.assign(argv[argno]);
			return true;
		}
		return false;
	}
	
	/** Get argument given by option
	 * e.g.: ./main -i value
	 * for op = "-i" -> out_str = "value", return: true
	 * for op = "xyz" -> out_str = <unchanged>, return: false
	 */
	bool ProgramArgs::getOption(std::wstring &out_str, const wchar_t *op)
	{
		const wchar_t* value = argline::getOption(argc, argv, op, NULL);
		if (value != NULL) {
			out_str.assign(value);
			return true;
		}
		return false;
	}
	
	/// convert argv from UNICODE to multi-byte
	char** ProgramArgs::_getArgvNonUtf16(int argc, wchar_t **argv) 
	{
		char **args;
			
		try {
			args = new char*[(size_t)argc];
			//fprintf(stderr, "[DBG] %s @ %d [NEW] %p (char*[] args)\n", __FILE__, __LINE__, args);
		}
		catch (const std::bad_alloc& e) {
			SysError ex;
			fprintf(stderr, "%s: Failed to allocate char*[%d] [what: %s, errno: %s]\n", __FUNCTION__, argc, e.what(), ex.msg);
			return NULL;
		}
		
		this->argc_non_utf16 = argc;
		
		for (int i = 0; i < argc; i++) 
		{
			if (! (args[i] = unicodeToMultibyte(argv[i]))) {
				fprintf(stderr, "%s: Failed to convert argument %d to multibyte\n", __FUNCTION__, i);
				delete[] args;
				return NULL;
			}

			//fprintf(stderr, "ProgramArgs: %d) '%s'\n", i, args[i]);
		}
		
		return args;
	}
#else /* UNICODE undefined */
	/// Constructor
	ProgramArgs::ProgramArgs(int argc, char **argv) {
		argv_non_utf16 = argv; // argv_non_utf16 the same as argv in non-unicode version
		this->argv     = argv;
		this->argc     = argc;
	}
	
	/** Get argument from given index
	 * @param out_str  String to store output argument value
	 * @param argno    Index of argument inside argv
	 * 
	 * @return true  if arguemnt was retrieved
	 * @return false if argno index was invalid
	 */
	bool ProgramArgs::getArg(std::string &out_str, int argno) const 
	{
		if ((argno < argc) && (argno >= 0)) 
		{
			out_str.assign(argv[argno]);
			return true;
		}
		return false;
	}
	
	/** Get argument given by option
	 * e.g.: ./main -i value
	 * for op = "-i" -> out_str = "value", return: true
	 * for op = "xyz" -> out_str = <unchanged>, return: false
	 */
	bool ProgramArgs::getOption(std::string &out_str, const char *op)
	{
		const char* value = argline::getOption(argc, argv, op, NULL);
		if (value != NULL) {
			out_str.assign(value);
			return true;
		}
		return false;
	}
#endif /* UNICODE */
