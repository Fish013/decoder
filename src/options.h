/*
 * SPDX-License-Identifier: GPL-2.0-or-later
 *
 * vim: set tabstop=4 syntax=c :
 *
 * Copyright (C) 2014-2018, Peter Haemmerlein (peterpawn@yourfritz.de)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program, please look for the file LICENSE.
 */

#ifndef OPTIONS_H

#define OPTIONS_H

// helper macros

#define __usage(help, version)			((*entry->usage)(help, version))

#ifdef DECODER_CONFIG_AUTO_USAGE
#define	__autoUsage()					__usage(false, false)
#else
#define	__autoUsage()
#endif

#define	isStrict()						(__getVerbosity() == VERBOSITY_STRICT)

#define	failOnStrict()					if (isStrict())\
											return EXIT_FAILURE

#define verbosity_options_long			{ "verbose", no_argument, NULL, 'v' },\
										{ "quiet", no_argument, NULL, 'q' },\
										{ "strict", no_argument, NULL, 's' },\
										{ "help", no_argument, NULL, 'h' },\
										{ "version", no_argument, NULL, 'V' }

#define	options_long_end				{ NULL, 0, NULL, 0 }

#define verbosity_options_short			"vqshV"

#define check_verbosity_options_short()	case 'v':\
											__setVerbosity(VERBOSITY_VERBOSE);\
											break;\
										case 'q':\
											__setVerbosity(VERBOSITY_SILENT);\
											break;\
										case 's':\
											__setVerbosity(VERBOSITY_STRICT);\
											break

#define help_option()					case 'h':\
											__usage(true, false);\
											return EXIT_SUCCESS;\
										case 'V':\
											__usage(false, true);\
											return EXIT_SUCCESS;\

#define getopt_invalid_option()			case '?':\
										case 0:\
											errorMessage((*((argv[optind]) + 1) == '-' ? errorInvalidOrAmbiguousOption : errorInvalidOption), argv[optind]);\
											__autoUsage();\
											return EXIT_FAILURE

#define getopt_argument_missing()		case ':':\
											errorMessage(errorMissingOptionValue, argv[optind]);\
											__autoUsage();\
											return EXIT_FAILURE

// options check default

#define invalid_option(opt)				default:\
											errorMessage(errorInvalidOption, argv[optind]);\
											__autoUsage();\
											return EXIT_FAILURE

// options for more than one applet

// alternate environment file

#define altenv_options_long				{ "alt-env", required_argument, NULL, 'a' }

#define altenv_options_short			"a:"

#define check_altenv_options_short()	case 'a':\
											if ((altEnv = setAlternativeEnvironment(optarg)) == false) {\
												__autoUsage();\
												return EXIT_FAILURE;\
											}\
											break

#define altenv_verbose_message()		if (altEnv)\
											verboseMessage(verboseAltEnv, getEnvironmentPath())

// wrap output lines

#define width_options_long				{ "wrap-lines", optional_argument, NULL, 'w' }

#define width_options_short				"w::"

#define check_width_options_short()		case 'w':\
											{\
												int	offset = setLineWidth(optarg, argv[optind], argv[optind + 1]);\
												if (offset == -1) {\
													__autoUsage();\
													return EXIT_FAILURE;\
												}\
												else optind += offset;\
											}\
											break

// function prototypes

bool									setAlternativeEnvironment(char * newEnvironment);
int										setLineWidth(char * value, char * option, char * next);
bool									setInputBufferSize(char * value, char * option);
bool									checkLastArgumentIsInputFile(char * name);
void									warnAboutExtraArguments(char ** argv, int i);

#endif
