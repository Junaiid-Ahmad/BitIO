/*!
 @header    CommandLineInterface.h
 @author    Marcus Johnson aka BumbleBritches57
 @copyright 2017, Marcus Johnson
 @version   2.0.0
 FIXME:     The Version number needs to be FeatureLevel.ABI.BugFix
 @brief     This header contains code related to parsing the command line arguments.
 */

#include <stdbool.h>
#include <stdint.h>

#pragma once

#ifndef LIBBITIO_COMMANDLINEINTERFACE_H
#define LIBBITIO_COMMANDLINEINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif
    
    typedef struct        CommandLineSwitch         CommandLineSwitch;
    
    /*!
     @typedef             CommandLineArgument
     @abstract                                      "Contains the data to support a single switch".
     @constant            SwitchNum                 "Which switch is this argument?".
     @constant            NumMetaSwitches           "How many meta switches are part of this argument?".
     @constant            MetaSwitches              "Pointer to an array that contains the numbers of the meta switches".
     @constant            ArgumentResult            "If there is a path or other result expected for this switch's argument, it'll be here".
     */
    typedef struct        CommandLineArgument       CommandLineArgument;
    
    typedef struct        CommandLineInterface      CommandLineInterface;
    
    /*!
     @abstract                                      "Initializes a CommandLineInterface instance".
     @return                                        "Returns a pointer to an initialized CommandLineInterface instance".
     @param               NumSwitches               "The number of CommandLineSwitch structures to initalize".
     */
    CommandLineInterface *InitCommandLineInterface(const size_t NumSwitches);
    
    /*!
     @abstract                                      "Deallocates the instance of CommandLineInterface pointed to by CLI".
     @param               CLI                       "Pointer to the instance of CommandLineInterface you want to delete".
     */
    void                  DeinitCommandLineInterface(CommandLineInterface *CLI);
    
    /*!
     @abstract                                      "Parses argv for switches matching the ones contained in CLI".
     @remark                                        "Argv[0] (the path for the original executable) is NEVER searched or used".
     @param               CLI                       "Pointer to CommandLineInterface".
     @param               argc                      "Main's argc, for the number of arguments entered".
     @param               argv                      "Main's argv, for the actual arguments the user has entered".
     */
    void                  ParseCommandLineArguments(const CommandLineInterface *CLI, int argc, const char *argv[]);
    
    /*!
     @abstract                                      "Sets the name of the program".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               Name                      "Pointer to a C string containing the name of the program you're building"
     */
    void                  SetCLIName(CommandLineInterface *CLI, const char *Name);
    
    /*!
     @abstract                                      "Sets the name of the program".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               VersionString             "Pointer to a C string contining the version of the program you're building"
     */
    void                  SetCLIVersion(CommandLineInterface *CLI, const char *VersionString);
    
    /*!
     @abstract                                      "Sets the description of the program".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               Description               "Description of what the program does".
     */
    void                  SetCLIDescription(CommandLineInterface *CLI, const char *Description);
    
    /*!
     @abstract                                      "Sets the author of the program".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               Author                    "Author of this program".
     */
    void                  SetCLIAuthor(CommandLineInterface *CLI, const char *Author);
    
    /*!
     @abstract                                      "Sets the copyright years of the program".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               Copyright                 "The starting year this program was written dash (CURRENTYEAR)".
     */
    void                  SetCLICopyright(CommandLineInterface *CLI, const char *Copyright);
    
    /*!
     @abstract                                      "Sets the license of the program".
     @remark                                        "If your program is closed source, do NOT use the License options, use the EULA functions".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               License                   "The license this program is licensed under".
     @param               IsProprietary             "Is this program released under an open source license, or a EULA?"
     */
    void                  SetCLILicense(CommandLineInterface *CLI, const char *License, const bool IsProprietary);
    
    /*!
     @abstract                                      "Sets the URL for the license, in the main program banner".
     @remark                                        "If your program is closed source, do NOT use the License options, use the EULA functions".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               LicenseURL                "the actual URL for the license".
     @param               IsEULA                    "Is this program released under an open source license, or a EULA?"
     */
    void                  SetCLILicenseURL(CommandLineInterface *CLI, const char *LicenseURL, const bool IsEULA);
    
    /*!
     @abstract                                      "What is the minimum number of switches your program needs to operate?".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               MinSwitches               "The minimum number of switches".
     */
    void                  SetCLIMinSwitches(CommandLineInterface *CLI, const uint64_t MinSwitches);
    
    /*!
     @abstract                                      "Sets SwitchNum's flag in the CommandLineInterface instance pointed by CLI".
     @remark                                        "Just enter the number of characters you typed into the string not counting the quotes".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               SwitchNum                 "The switch to set".
     @param               Flag                      "The flag to identify an option with".
     @param               FlagSize                  "Size of the flag string".
     */
    void                  SetCLISwitchFlag(CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Flag, const size_t FlagSize);
    
    /*!
     @abstract                                      "Sets MetaFlag switch as a meta flag for switch SwitchNum".
     @param               CLI                       "Pointer to CommandLineInterface".
     @param               SwitchNum                 "Which switch are you trying to say has a meta flag?".
     @param               MetaFlag                  "Which flag number is a meta flag for SwitchNum?".
     */
    void                  SetCLISwitchMetaFlag(const CommandLineInterface *CLI, const size_t SwitchNum, const size_t MetaFlag);
    
    /*!
     @abstract                                      "Sets SwitchDescription's flag in the CommandLineInterface instance pointed by CLI".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               SwitchNum                 "The switch to set".
     @param               Description               "Pointer to a C string containing the description of what this program does"
     */
    void                  SetCLISwitchDescription(const CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Description);
    
    /*!
     @abstract                                      "Sets SwitchResult's flag in the CommandLineInterface instance pointed by CLI".
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               SwitchNum                 "The switch to set".
     @param               IsThereAResult            "Are you expecting this switch to contain data, or are you just testing for it's presence?".
     */
    void                  SetCLISwitchResultStatus(const CommandLineInterface *CLI, const uint64_t SwitchNum, const bool IsThereAResult);
    
    /*!
     @abstract                                      "Gets the data contained in Switch->Result"
     @return                                        "Returns the data after the switch, if the switch is resultless it will return 0"
     @param               CLI                       "Pointer to the instance of CommandLineInterface".
     @param               SwitchNum                 "The switch to check".
     */
    const char           *GetCLISwitchResult(const CommandLineInterface *CLI, const uint64_t SwitchNum);
    
#ifdef __cplusplus
}
#endif

#endif  //LIBBITIO_COMMANDLINEINTERFACE_H
