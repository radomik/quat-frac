/*
 * File:   ProgramArgs.hpp
 * Author: darek
 *
 * Created on 26 November 2014, 22:14
 */
#ifndef QFRACTAL_PROGRAMARGS_HPP
#define QFRACTAL_PROGRAMARGS_HPP

#include "utils.hpp"

// if this is defined then argv_non_utf16 is always created
// otherwise is only created if UNICODE is not defined
#define PROGRAM_ARGS_HAS_ARGV_NON_UTF16 1

#ifdef UNICODE
	class ProgramArgs {
	public:
		/// argument line array of c-strings (UNICODE, UTF16)
		wchar_t		**argv;
		
		/// argv converted to multi-byte strings 
		/// (or NULL if PROGRAM_ARGS_HAS_ARGV_NON_UTF16 is undefined)
		char		**argv_non_utf16;
		
		/// count of arguments
		int			argc;
		
		/// Constructor
		ProgramArgs(int argc, wchar_t **argv);
		
		/// call init after construction (return -1 on error for UNICODE version)
		int init();
		
		/// Destructor ; used only if PROGRAM_ARGS_HAS_ARGV_NON_UTF16 is defined
		#ifdef PROGRAM_ARGS_HAS_ARGV_NON_UTF16
			~ProgramArgs();
		#endif

		/** Get argument from given index
		 * @param out_str  String to store output argument value
		 * @param argno    Index of argument inside argv
		 * 
		 * @return true  if arguemnt was retrieved
		 * @return false if argno index was invalid
		 */
		bool getArg(std::wstring &out_str, int argno) const;
		
		/** Get argument given by option
		 * e.g.: ./main -i value
		 * for op = "-i" -> out_str = "value", return: true
		 * for op = "xyz" -> out_str = <unchanged>, return: false
		 */
		bool getOption(std::wstring &out_str, const wchar_t *op);
		
		/** Check if argv (starting from index: 1) contains string op */
		bool hasOption(const wchar_t *op) const { return argline::hasOption(argc, argv, op); }
		
		/** Check if argv contains help option (-h, -help, --h, --help) */
		bool hasHelpOption() const { return argline::hasHelpOption(argc, argv); }

	private:
		/// convert argv from UNICODE to multi-byte
		char** _getArgvNonUtf16(int argc, wchar_t **argv);
		
		/// size of argv_non_utf16 (note: argc may be changed by other code (public))
		int argc_non_utf16;
	};
#else /* UNICODE undefined */
	class ProgramArgs {
	public:
		/// argument line array of c-strings (multi-byte)
		char		**argv;
		
		/// the same as argv
		char		**argv_non_utf16;
		
		/// count of arguments
		int			argc;
		
		/// Constructor
		ProgramArgs(int argc, char **argv);
		
		/// call init after construction (return -1 on error for UNICODE version)
		int init() { return 0; }

		/** Get argument from given index
		 * @param out_str  String to store output argument value
		 * @param argno    Index of argument inside argv
		 * 
		 * @return true  if arguemnt was retrieved
		 * @return false if argno index was invalid
		 */
		bool getArg(std::string &out_str, int argno) const;
		
		/** Get argument given by option
		 * e.g.: ./main -i value
		 * for op = "-i" -> out_str = "value", return: true
		 * for op = "xyz" -> out_str = <unchanged>, return: false
		 */
		bool getOption(std::string &out_str, const char *op);
		
		/** Check if argv (starting from index: 1) contains string op */
		bool hasOption(const char *op) const { return argline::hasOption(argc, argv, op); }
		
		/** Check if argv contains help option (-h, -help, --h, --help) */
		bool hasHelpOption() const { return argline::hasHelpOption(argc, argv); }
	};
#endif /* UNICODE */

#endif

