#include <errno.h>

#include "../include/BitIO.h"
#include "../include/CommandLineInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @struct                 CommandLineSwitch
     @abstract                                         "Contains the data to support a single switch".
     @remark                                           "Should non-Meta switches be assumed to have a result by default?"
     @constant               IsMetaSwitch              "Is this switch a meta switch?".
     @constant               SwitchHasResult           "Does this switch have a result?".
     @constant               Flag                      "What is this switch called, how do we identify it?".
     @constant               FlagSize                  "What is the strlen of this switch (assuming ASCII, codeunits if UTF)".
     @constant               SwitchDescription         "Describe to the user what this switch does".
     @constant               NumMetaSwitches           "How many meta switches are part of this argument?".
     @constant               MetaSwitches              "Pointer to an array that contains the numbers of the meta switches".
     */
    struct CommandLineSwitch {
        bool                 IsMetaSwitch;
        bool                 SwitchHasResult;
        const char          *Flag;
        size_t               FlagSize;
        const char          *SwitchDescription;
        uint64_t             NumMetaSwitches;
        uint64_t            *MetaSwitches;
    };
    
    /*! 
     @struct                 CommandLineArgument
     @abstract                                         "Contains the data to support a single argument".
     @constant               SwitchNum                 "Which switch is this argument?".
     @constant               ArgumentResult            "If there is a path or other result expected for this switch's argument, it'll be here".
     */
    struct CommandLineArgument {
        uint64_t             SwitchNum;
        const char          *ArgumentResult;
    };
    
    /*!
     @struct                 CommandLineInterface
     @abstract                                         "Contains all the information on the command line in an easy to understand format".
     @constant               NumSwitches               "How many switches are there in CLI->Switches?".
     @constant               MinSwitches               "The minimum number of switches to accept without dumping the help".
     @constant               IsProprietary             "Is this program proprietary?".
     @constant               ProgramName               "What is the name of this program?".
     @constant               ProgramAuthor             "Who wrote this program?".
     @constant               ProgramDescription        "What does this program do?".
     @constant               ProgramVersion            "What is the version of this program?".
     @constant               ProgramCopyright          "String containing the copyright years like "2015 - 2017"".
     @constant               ProgramLicenseDescription "Describe the license or EULA".
     @constant               ProgramLicenseURL         "URL for the EULA, ToS, or Open source license".
     @constant               Switches                  "Pointer to an array of switches".
     @constant               Arguments                 "Pointer to an array of arguments".
     */
    struct CommandLineInterface {
        size_t               NumSwitches;
        size_t               NumArguments;
        size_t               MinSwitches;
        bool                 IsProprietary:1;
        const char          *ProgramName;
        const char          *ProgramAuthor;
        const char          *ProgramDescription;
        const char          *ProgramVersion;
        const char          *ProgramCopyright;
        const char          *ProgramLicenseDescription;
        const char          *ProgramLicenseURL;
        CommandLineSwitch   *Switches;
        CommandLineArgument *Arguments;
    };
    
    CommandLineInterface *InitCommandLineInterface(const size_t NumSwitches) {
        errno = 0;
        CommandLineInterface *CLI = (CommandLineInterface*)calloc(1, sizeof(CommandLineInterface));
        if (errno != 0) {
            char *ErrnoError = (char*)calloc(1, 96);
            strerror_r(errno, ErrnoError, 96);
            Log(LOG_ERR, "libBitIO", "InitCommandLineInterface", "Errno Initing CommandLineInterface: %s\n", ErrnoError);
            free(ErrnoError);
            errno = 0;
        }
        CLI->NumSwitches        = NumSwitches;
        
        size_t ArgumentSize     = sizeof(CommandLineArgument);
        
        CLI->Arguments          = (CommandLineArgument*)calloc(NumSwitches, ArgumentSize);
        if (errno != 0) {
            char *ErrnoError    = (char*)calloc(1, 96);
            strerror_r(errno, ErrnoError, 96);
            Log(LOG_ERR, "libBitIO", "InitCommandLineInterface", "Errno Initing CommandLineSwitch: %s\n", ErrnoError);
            free(ErrnoError);
            errno               = 0;
        }
        
        return CLI;
    }
    
    void CloseCommandLineInterface(const CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "CloseCommandLineInterface", "Pointer to CommandLineInterface is NULL\n");
        } else {
            /* Free CommandLineSwitches */
            for (size_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                free(CLI->Switches[Switch].Flag);
                free(CLI->Switches[Switch].SwitchDescription);
            }
            free(CLI->Switches);
            /* Free CommandLineArguments */
            for (size_t Arg = 0; Arg < CLI->NumArguments; Arg++) {
                free(CLI->Arguments[Arg].ArgumentResult);
                free(CLI->Arguments[Arg].MetaSwitches);
            }
            free(CLI->Arguments);
            /* Free CommandLineInterface */
            free(CLI->ProgramName);
            free(CLI->ProgramAuthor);
            free(CLI->ProgramDescription);
            free(CLI->ProgramVersion);
            free(CLI->ProgramCopyright);
            free(CLI->ProgramLicenseDescription);
            free(CLI->ProgramLicenseURL);
            free(CLI);
        }
    }
    
    void SetCLIName(const CommandLineInterface *CLI, const char *Name) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to CommandLineInterface is NULL\n");
        } else if (Name == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to Name is NULL\n");
        } else {
            CLI->ProgramName = Name;
        }
    }
    
    void SetCLIVersion(const CommandLineInterface *CLI, const char *VersionString) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to CommandLineInterface is NULL\n");
        } else if (VersionString == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to VersionString is NULL\n");
        } else {
            CLI->ProgramVersion = VersionString;
        }
    }
    
    void SetCLIDescription(const CommandLineInterface *CLI, const char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to CommandLineInterface is NULL\n");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to Description is NULL\n");
        } else {
            CLI->ProgramDescription = Description;
        }
    }
    
    void SetCLIAuthor(const CommandLineInterface *CLI, const char *Author) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to CommandLineInterface is NULL\n");
        } else if (Author == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to Author is NULL\n");
        } else {
            CLI->ProgramAuthor = Author;
        }
    }
    
    void SetCLICopyright(const CommandLineInterface *CLI, const char *Copyright) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to CommandLineInterface is NULL\n");
        } else if (Copyright == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to Copyright is NULL\n");
        } else {
            CLI->ProgramCopyright = Copyright;
        }
    }
    
    void SetCLILicense(const CommandLineInterface *CLI, const char *License, const bool IsProprietary) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicense", "Pointer to CommandLineInterface is NULL\n");
        } else if (License == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicense", "Pointer to License is NULL\n");
        } else {
            CLI->ProgramLicenseDescription = License;
            if (IsProprietary == true) {
                CLI->IsProprietary         = false;
            } else {
                CLI->IsProprietary         = true;
            }
        }
    }
    
    void SetCLILicenseURL(const CommandLineInterface *CLI, const char *LicenseURL, const bool IsProprietary) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to CommandLineInterface is NULL\n");
        } else if (LicenseURL == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to LicenseURL is NULL\n");
        } else {
            CLI->ProgramLicenseURL = LicenseURL;
            if (IsProprietary == true) {
                CLI->IsProprietary = false;
            } else {
                CLI->IsProprietary = true;
            }
        }
    }
    
    void SetCLIMinSwitches(const CommandLineInterface *CLI, const uint64_t MinSwitches) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIMinSwitches", "Pointer to CommandLineInterface is NULL\n");
        } else {
            CLI->MinSwitches = MinSwitches;
        }
    }
    
    void SetCLISwitchFlag(const CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Flag, const size_t FlagSize) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to CommandLineInterface is NULL\n");
        } else if (Flag == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to switch Flag is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CLI->NumSwitches);
        } else {
            CLI->Switches[SwitchNum].Flag     = Flag;
            CLI->Switches[SwitchNum].FlagSize = FlagSize + 1; // add one for the trailing NULL
        }
    }
    
    void SetCLISwitchDescription(const CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to CommandLineInterface is NULL\n");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to switch Description is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CLI->NumSwitches);
        } else {
            CLI->Switches[SwitchNum].SwitchDescription = Description;
        }
    }
    
    void SetCLISwitchResultStatus(const CommandLineInterface *CLI, const uint64_t SwitchNum, const bool IsThereAResult) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchResultStatus", "Pointer to CommandLineInterface is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchResultStatus", "SwitchNum: %d, should be between 0 and %d\n", SwitchNum, CLI->NumSwitches);
        } else {
            CLI->Switches[SwitchNum].SwitchHasResult = IsThereAResult & 1;
        }
    }
    
    const char *GetCLISwitchResult(const CommandLineInterface *CLI, const uint64_t SwitchNum) {
        const char *Result = NULL;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLISwitchResult", "Pointer to CommandLineInterface is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLISwitchResult", "SwitchNum: %d, should be between 0 and %d\n", SwitchNum, CLI->NumSwitches);
        } else {
            Result = CLI->Switches[SwitchNum].SwitchResult;
        }
        return Result;
    }
    
    static void DisplayCLIHelp(const CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayCLIHelp", "Pointer to CommandLineInterface is NULL\n");
        } else {
            printf("Accepted prefixes: -, --, /\n");
            printf("Options: \n");
            /*
             I'm thinking something like:
             
             Options: (-|--|/)
             Input: Input file or stdin with -
             Output: Output file or stdout with -
             Encode: Encode input to PNG
             Resolution: Resolution in WidthxHeight format (if 3D specify the per eye resolution)
             Interlace: Resolution in WidthxHeight format (if 3D specify the per eye resolution)
             Optimize: Optimize the encoded PNG to be as small as possible (try all filter options)
             Stereo3D: Encode an image as a single stereoscopic, 3D image (the first input should be the left eye)
             Decode: Decode PNG to output
             Split3D: Decode stereo PNG to 2 output files
             
             */
            
            if (CLI->DependentSwitchesPresent == true) {
                // Well, logically we should iterate over each switch in numerical order, and if it has a dependent switch, we should print that first, then tab the dependency.
                // But for that to work seamlessly, we need to have a flag on each dependenency that says if it's been printed or not.
                // But why put that in the structs when it's only needed here?
                bool *DependentSwitchPrinted = (bool*)calloc(CLI->NumSwitches, sizeof(bool));
                // So, as we loop we need to check if a switch is dependent on anything, if it is, check if the dependent switch has been printed
                
                for (size_t DepSwitch = 0; DepSwitch < CLI->NumSwitches; DepSwitch++) {
                    /*
                     if (CLI->Arguments[DepSwitch].IsDependent == true && DependentSwitchPrinted[DepSwitch] == false) {
                     // Print the dependent switch stored in Switch->DependsOn
                     size_t DepSwitchDependsOn = CLI->Arguments[DepSwitch].DependsOn;
                     printf("\t%s: %s\n", CLI->Arguments[DepSwitchDependsOn].Flag, CLI->Arguments[DepSwitchDependsOn].SwitchDescription);
                     }
                     */
                }
                free(DependentSwitchPrinted);
            } else {
                for (uint8_t SwitchNum = 0; SwitchNum < CLI->NumSwitches - 1; SwitchNum++) {
                    printf("\t%s: %s\n", CLI->Arguments[SwitchNum].Flag, CLI->Arguments[SwitchNum].SwitchDescription);
                }
            }
        }
    }
    
    static void DisplayProgramBanner(const CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayProgramBanner", "Pointer to CommandLineInterface is NULL\n");
        } else {
            printf("%s version %s by %s © %s: %s, ", CLI->ProgramName, CLI->ProgramVersion, CLI->ProgramAuthor, CLI->ProgramCopyright, CLI->ProgramDescription); // Generic part of the string.
            if (CLI->IsProprietary == true) {
                printf("Released under the \"%s\" license: %s\n\n", CLI->ProgramLicenseDescription, CLI->ProgramLicenseURL);
            } else {
                printf("By using this software, you agree to the End User License Agreement %s, available at: %s\n\n", CLI->ProgramLicenseDescription, CLI->ProgramLicenseURL);
            }
        }
    }
    
    void SetCLISwitchMetaFlag(const CommandLineInterface *CLI, const size_t SwitchNum, const size_t MetaFlag) {
        /*
         So, SwitchNum's MetaFlags is incremented and it's Metaflag is set to MetaFlag.
         */
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchMetaFlag", "Pointer to CommandLineInterface is NULL\n");
        } else {
            CLI->Switches[SwitchNum].NumMetaSwitches += 1;
            CLI->Switches[SwitchNum].MetaSwitches     = realloc(CLI->Switches[SwitchNum].MetaSwitches, sizeof(CLI->Switches[SwitchNum].MetaSwitches) + sizeof(uintptr_t));
            CLI->Switches[SwitchNum].MetaSwitches[CLI->Switches[SwitchNum].NumMetaSwitches] = MetaFlag;
        }
    }
    
    void ParseCommandLineArguments2(const CommandLineInterface *CLI, const int argc, const char *argv[]) {
        /*
         Read Argv and CLI->Switches put everything into a nice easy way to understand it all.
         So, we need to set which
         */
    }
    
    void ParseCommandLineArguments(const CommandLineInterface *CLI, const int argc, const char *argv[]) {
        // TODO : Scan for equals signs as well, if found, after the equal sign is the result, everything before is the switch.
        // TODO : add support for generating the short versions of the flags.
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Pointer to CommandLineInterface is NULL\n");
        } else {
            if ((CLI->NumSwitches < CLI->MinSwitches && CLI->MinSwitches > 0) || argc == 1) {
                DisplayProgramBanner(CLI);
                DisplayCLIHelp(CLI);
            } else {
                DisplayProgramBanner(CLI);
                
                char *SingleDash = NULL;
                char *DoubleDash = NULL;
                char *Slash      = NULL;
                
                errno = 0;
                
                for (uint8_t SwitchNum = 0; SwitchNum < CLI->NumSwitches; SwitchNum++) {
                    for (uint8_t Argument = 1; Argument < argc - 1; Argument++) { // the executable path is skipped over
                                                                                  // Once the switch is found, we should skip over this argument.
                        
                        // Make sure the switch dependency was found before the depedent one.
                        // So, idk how to add this...
                        
                        size_t SingleDashSize                       = CLI->Arguments[SwitchNum].FlagSize + 1;
                        size_t DoubleDashSize                       = CLI->Arguments[SwitchNum].FlagSize + 2;
                        size_t SlashSize                            = CLI->Arguments[SwitchNum].FlagSize + 1;
                        
                        SingleDash                                  = (char*)calloc(1, SingleDashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*)calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno SingleDash = %s, Arg = %d, Switch = %d, errno = %s", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0;
                        } else {
                            snprintf(SingleDash, SingleDashSize, "-%s", CLI->Arguments[SwitchNum].Flag);
                        }
                        
                        DoubleDash                                  = (char*)calloc(1, DoubleDashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*)calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno DoubleDash = %s, Arg = %d, Switch = %d", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0;
                        } else {
                            snprintf(DoubleDash, DoubleDashSize, "--%s", CLI->Arguments[SwitchNum].Flag);
                        }
                        
                        Slash                                       = (char*)calloc(1, SlashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*)calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno Slash = %s, Arg = %d, Switch = %d", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0; // Here to reset to catch errors with the strcmp stuff below
                        } else {
                            snprintf(Slash, SlashSize, "/%s", CLI->Arguments[SwitchNum].Flag);
                        }
                        
                        if (strcasecmp(SingleDash, argv[Argument]) == 0 || strcasecmp(DoubleDash, argv[Argument]) == 0 || strcasecmp(Slash, argv[Argument]) == 0) {
                            
                            size_t ArgumentSize = strlen(argv[Argument + 1]) + 1;
                            
                            CLI->Arguments[SwitchNum].SwitchFound      = true;
                            if (CLI->Arguments[SwitchNum].IsThereAResult == true) {
                                char *SwitchResult                  = (char*)calloc(1, ArgumentSize);
                                if (errno != 0) {
                                    char *ErrnoError = (char*)calloc(1, 96);
                                    strerror_r(errno, ErrnoError, 96);
                                    Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno SwitchResult = %s, Arg = %d, Switch = %d", ErrnoError, Argument, SwitchNum);
                                    free(ErrnoError);
                                } else {
                                    snprintf(SwitchResult, ArgumentSize, "%s", argv[Argument + 1]);
                                    CLI->Arguments[SwitchNum].SwitchResult = SwitchResult;
                                }
                                free(SwitchResult);
                            }
                            SwitchNum += 1; // To break out of looking for this switch
                            Argument  += 1;
                        }
                    }
                }
                free(SingleDash);
                free(DoubleDash);
                free(Slash);
            }
        }
    }
    
#ifdef __cplusplus
}
#endif
