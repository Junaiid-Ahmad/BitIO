#include "../include/BitIO.h"
#include "../include/CommandLineIO.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @struct                 CommandLineSwitch
     @abstract                                         "Contains the data to support a single switch".
     
     @constant               IsMasterSwitch            "Does this switch depend on any others?"
     @constant               SwitchHasResult           "Does this switch take any arguments, or is it's mere presence all that matters?".
     @constant               FlagSize                  "What is the number of bytes (if ASCII)/ code points (if UTF) of this switch?".
     @constant               NumChildSwitches          "How many child switches are there in ChildSwitches?".
     @constant               ChildSwitches             "Pointer to an array that contains the numbers of the child switches".
     @constant               Flag                      "What is this switch called, how do we identify it?".
     @constant               SwitchDescription         "Describe to the user what this switch does".
     */
    struct CommandLineSwitch {
        bool                 IsMasterSwitch;
        bool                 SwitchHasResult;
        uint64_t             FlagSize;
        uint64_t             NumChildSwitches;
        uint64_t            *ChildSwitches;
        char                *Flag;
        char                *SwitchDescription;
    };
    
    /*!
     @struct                 CommandLineArgument
     @abstract                                         "Contains the data to support a single argument".
     @constant               SwitchNum                 "Which switch is this argument?".
     @constant               NumChildArguments         "How many child arguments were found in this argument?".
     @constant               ChildSwitches             "Array of child argument numbers, to look up in CLI->Switches".
     @constant               ArgumentResult            "If there is a path or other result expected for this switch's argument, it'll be here".
     */
    struct CommandLineArgument {
        uint64_t             SwitchNum;
        uint64_t             NumChildArguments;
        uint64_t            *ChildArguments;
        char                *ArgumentResult;
    };
    
    /*!
     @struct                 CommandLineIO
     @abstract                                         "Contains all the information on the command line in an easy to understand format".
     @constant               NumSwitches               "How many switches are there?".
     @constant               MinSwitches               "The minimum number of switches to accept without dumping the help".
     @constant               NumArguments              "The number of arguments present in argv, after extracting any child switches".
     @constant               Switches                  "Pointer to an array of switches".
     @constant               Arguments                 "Pointer to an array of arguments".
     @constant               IsProprietary             "Is this program proprietary?".
     @constant               ProgramName               "What is the name of this program?".
     @constant               ProgramAuthor             "Who wrote this program?".
     @constant               ProgramDescription        "What does this program do?".
     @constant               ProgramVersion            "What is the version of this program?".
     @constant               ProgramCopyright          "String containing the copyright years like "2015 - 2017"".
     @constant               ProgramLicenseDescription "Describe the license or EULA".
     @constant               ProgramLicenseURL         "URL for the EULA, ToS, or Open source license".
     */
    struct CommandLineIO {
        uint64_t             NumSwitches;
        uint64_t             MinSwitches;
        uint64_t             NumArguments;
        CommandLineSwitch   *Switches;
        CommandLineArgument *Arguments;
        bool                 IsProprietary;
        char                *ProgramName;
        char                *ProgramAuthor;
        char                *ProgramDescription;
        char                *ProgramVersion;
        char                *ProgramCopyright;
        char                *ProgramLicenseName;
        char                *ProgramLicenseDescription;
        char                *ProgramLicenseURL;
    };
    
    CommandLineIO *InitCommandLineIO(const uint64_t NumSwitches) {
        CommandLineIO *CLI = (CommandLineIO*) calloc(1, sizeof(CommandLineIO));
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "InitCommandLineIO", "Not enough memory to allocate CommandLineIO");
        } else {
            CLI->NumSwitches          = NumSwitches;
            CLI->Switches             = (CommandLineSwitch*) calloc(NumSwitches, sizeof(CommandLineSwitch));
            if (CLI->Switches == NULL) {
                Log(LOG_ERR, "libBitIO", "InitCommandLineIO", "Not enough memory to allocate CommandLineSwitch, NumSwitches = %d", NumSwitches);
            }
        }
        return CLI;
    }
    
    void SetCLIName(CommandLineIO *CLI, char *Name) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to CommandLineIO is NULL");
        } else if (Name == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to Name is NULL");
        } else {
            CLI->ProgramName = Name;
        }
    }
    
    void SetCLIVersion(CommandLineIO *CLI, char *VersionString) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to CommandLineIO is NULL");
        } else if (VersionString == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to VersionString is NULL");
        } else {
            CLI->ProgramVersion = VersionString;
        }
    }
    
    void SetCLIDescription(CommandLineIO *CLI, char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to CommandLineIO is NULL");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to Description is NULL");
        } else {
            CLI->ProgramDescription = Description;
        }
    }
    
    void SetCLIAuthor(CommandLineIO *CLI, char *Author) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to CommandLineIO is NULL");
        } else if (Author == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to Author is NULL");
        } else {
            CLI->ProgramAuthor = Author;
        }
    }
    
    void SetCLICopyright(CommandLineIO *CLI, char *Copyright) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to CommandLineIO is NULL");
        } else if (Copyright == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to Copyright is NULL");
        } else {
            CLI->ProgramCopyright = Copyright;
        }
    }
    
    void SetCLILicense(CommandLineIO *CLI, char *Name, char *LicenseDescription, const bool IsProprietary) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicense", "Pointer to CommandLineIO is NULL");
        } else if (Name == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicense", "Pointer to Name is NULL");
        } else if (LicenseDescription == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicense", "Pointer to LicenseDescription is NULL");
        } else {
            CLI->ProgramLicenseName        = Name;
            CLI->ProgramLicenseDescription = LicenseDescription;
            if (IsProprietary == true) {
                CLI->IsProprietary         = false;
            } else {
                CLI->IsProprietary         = true;
            }
        }
    }
    
    void SetCLILicenseURL(CommandLineIO *CLI, char *LicenseURL) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to CommandLineIO is NULL");
        } else if (LicenseURL == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to LicenseURL is NULL");
        } else {
            CLI->ProgramLicenseURL = LicenseURL;
        }
    }
    
    void SetCLIMinSwitches(CommandLineIO *CLI, const uint64_t MinSwitches) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIMinSwitches", "Pointer to CommandLineIO is NULL");
        } else {
            CLI->MinSwitches = MinSwitches;
        }
    }
    
    void SetCLISwitchAsMaster(CommandLineIO *CLI, const uint64_t Switch, const bool IsMaster) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to CommandLineIO is NULL");
        } else {
            CLI->Switches[Switch].IsMasterSwitch = IsMaster;
        }
    }
    
    void SetCLISwitchFlag(CommandLineIO *CLI, const uint64_t SwitchNum, char *Flag) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to CommandLineIO is NULL");
        } else if (Flag == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to switch Flag is NULL");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "SwitchNum %d is too high, there are only %d switches", SwitchNum, CLI->NumSwitches);
        } else {
            CLI->Switches[SwitchNum].Flag     = Flag;
            CLI->Switches[SwitchNum].FlagSize = strlen(Flag);
        }
    }
    
    void SetCLISwitchDescription(CommandLineIO *CLI, const uint64_t SwitchNum, char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to CommandLineIO is NULL");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "SwitchNum %d is too high, there are only %d switches", SwitchNum, CLI->NumSwitches);
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to switch Description is NULL");
        } else {
            CLI->Switches[SwitchNum].SwitchDescription = Description;
        }
    }
    
    void SetCLISwitchResultStatus(CommandLineIO *CLI, const uint64_t SwitchNum, const bool IsThereAResult) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchResultStatus", "Pointer to CommandLineIO is NULL");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchResultStatus", "SwitchNum: %d, should be between 0 and %d", SwitchNum, CLI->NumSwitches);
        } else {
            CLI->Switches[SwitchNum].SwitchHasResult = IsThereAResult;
        }
    }
    
    static void DisplayCLIHelp(CommandLineIO *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayCLIHelp", "Pointer to CommandLineIO is NULL");
        } else {
            bool *IsChildSwitch = calloc(CLI->NumSwitches, sizeof(bool));
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[Switch].NumChildSwitches; ChildSwitch++) {
                    IsChildSwitch[CLI->Switches[Switch].ChildSwitches[ChildSwitch]] = true;
                }
            }
            printf("Options: (-|--|/)");
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                if (IsChildSwitch[Switch] == false) {
                    printf("%s: %s", CLI->Switches[Switch].Flag, CLI->Switches[Switch].SwitchDescription);
                    for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[Switch].NumChildSwitches; ChildSwitch++) {
                        uint64_t CurrentChildSwitch = CLI->Switches[Switch].ChildSwitches[ChildSwitch];
                        printf("\t%s: %s", CLI->Switches[CurrentChildSwitch].Flag, CLI->Switches[CurrentChildSwitch].SwitchDescription);
                    }
                }
            }
            free(IsChildSwitch);
        }
    }
    
    static void DisplayProgramBanner(CommandLineIO *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayProgramBanner", "Pointer to CommandLineIO is NULL");
        } else {
            if (CLI->ProgramName != NULL) {
                printf("%s ", CLI->ProgramName);
            } else if (CLI->ProgramVersion != NULL) {
                printf("version %s", CLI->ProgramVersion);
            } else if (CLI->ProgramAuthor != NULL) {
                printf(" by %s", CLI->ProgramAuthor);
            } else if (CLI->ProgramCopyright != NULL) {
                printf(" © %s", CLI->ProgramCopyright);
            } else if (CLI->ProgramDescription != NULL) {
                printf(": %s", CLI->ProgramDescription);
            } else if (CLI->IsProprietary == true && CLI->ProgramLicenseDescription != NULL && CLI->ProgramLicenseURL != NULL) {
                printf(", Released under the \"%s\" license: %s, available at: %s", CLI->ProgramLicenseName, CLI->ProgramLicenseDescription, CLI->ProgramLicenseURL);
            } else if (CLI->IsProprietary == false && CLI->ProgramLicenseDescription != NULL && CLI->ProgramLicenseURL != NULL) {
                printf(", By using this software, you agree to the End User License Agreement %s, available at: %s", CLI->ProgramLicenseDescription, CLI->ProgramLicenseURL);
            }
        }
    }
    
    void SetCLISwitchAsChild(CommandLineIO *CLI, const uint64_t ParentSwitch, const uint64_t ChildSwitch) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchAsChild", "Pointer to CommandLineIO is NULL");
        } else if (ParentSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchAsChild", "ParentSwitch: %d, should be between 0 and %d", ParentSwitch, CLI->NumSwitches);
        } else if (ChildSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchAsChild", "ChildSwitch: %d, should be between 0 and %d", ChildSwitch, CLI->NumSwitches);
        } else {
            CLI->Switches[ParentSwitch].NumChildSwitches  += 1;
            if (CLI->Switches[ParentSwitch].ChildSwitches == NULL) {
                CLI->Switches[ParentSwitch].ChildSwitches = calloc(1, sizeof(uint64_t));
            } else {
                uint64_t ChildSwitchesSize = CLI->Switches[ParentSwitch].NumChildSwitches * sizeof(uint64_t);
                CLI->Switches[ParentSwitch].ChildSwitches = realloc(CLI->Switches[ParentSwitch].ChildSwitches, ChildSwitchesSize);
            }
            uint64_t CurrentChildSwitch = CLI->Switches[ParentSwitch].NumChildSwitches - 1;
            CLI->Switches[ParentSwitch].ChildSwitches[CurrentChildSwitch] = ChildSwitch;
        }
    }
    
    void ParseCommandLineArguments(CommandLineIO *CLI, const int argc, const char *argv[]) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Pointer to CommandLineIO is NULL");
        } else if (argc <= 1 || (argc < (int) CLI->MinSwitches && CLI->MinSwitches > 1) || argv == NULL) {
            DisplayProgramBanner(CLI);
            DisplayCLIHelp(CLI);
        } else {
            DisplayProgramBanner(CLI);
            char    *SingleDashFlag       = NULL;
            char    *DoubleDashFlag       = NULL;
            char    *SingleSlashFlag      = NULL;
            char    *ChildSingleDashFlag  = NULL;
            char    *ChildDoubleDashFlag  = NULL;
            char    *ChildSingleSlashFlag = NULL;
            
            for (int Argument = 1; Argument < argc; Argument++) { // No point in scanning the program's path
                for (uint64_t CurrentSwitch = 0; CurrentSwitch < CLI->NumSwitches; CurrentSwitch++) {
                    if (CLI->Switches[CurrentSwitch].IsMasterSwitch == true) {
                        uint64_t FlagSize  = CLI->Switches[CurrentSwitch].FlagSize;
                        SingleDashFlag   = (char*) calloc(1, FlagSize + 1);
                        DoubleDashFlag   = (char*) calloc(1, FlagSize + 2);
                        SingleSlashFlag  = (char*) calloc(1, FlagSize + 1);
                        snprintf(SingleDashFlag,  FlagSize + 1,  "-%s", CLI->Switches[CurrentSwitch].Flag);
                        snprintf(DoubleDashFlag,  FlagSize + 2, "--%s", CLI->Switches[CurrentSwitch].Flag);
                        snprintf(SingleSlashFlag, FlagSize + 1,  "/%s", CLI->Switches[CurrentSwitch].Flag);
                        if (strcasecmp(argv[Argument], SingleDashFlag) == 0 || strcasecmp(argv[Argument], DoubleDashFlag) == 0 || strcasecmp(argv[Argument], SingleSlashFlag) == 0) {
                            if (CLI->NumArguments == 0) {
                                CLI->Arguments = calloc(1, sizeof(CommandLineArgument));
                            } else {
                                uint64_t NumArguments  = (uint64_t) CLI->Arguments + 1;
                                uint64_t ArgumentsSize = sizeof(CommandLineArgument) * NumArguments;
                                CLI->Arguments = realloc(CLI->Arguments, ArgumentsSize);
                            }
                            
                            for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[CurrentSwitch].NumChildSwitches; ChildSwitch++) {
                                uint64_t ChildFlagSize  = CLI->Switches[CurrentSwitch].FlagSize;
                                ChildSingleDashFlag     = (char*) calloc(1, ChildFlagSize + 1);
                                ChildDoubleDashFlag     = (char*) calloc(1, ChildFlagSize + 2);
                                ChildSingleSlashFlag    = (char*) calloc(1, ChildFlagSize + 1);
                                
                                snprintf(ChildSingleDashFlag,  ChildFlagSize + 1,  "-%s", CLI->Switches[CurrentSwitch + 1].Flag);
                                snprintf(ChildDoubleDashFlag,  ChildFlagSize + 2, "--%s", CLI->Switches[CurrentSwitch + 1].Flag);
                                snprintf(ChildSingleSlashFlag, ChildFlagSize + 1,  "/%s", CLI->Switches[CurrentSwitch + 1].Flag);
                                if (strcasecmp(argv[Argument + (int) ChildSwitch], ChildSingleDashFlag) == 0 || strcasecmp(argv[Argument + (int) ChildSwitch], ChildDoubleDashFlag) == 0 || strcasecmp(argv[Argument + (int) ChildSwitch], ChildSingleSlashFlag) == 0) {
                                    CLI->Arguments[ChildSwitch].NumChildArguments += 1;
                                    CLI->Arguments[ChildSwitch].ChildArguments     = GetCLISwitchNumFromFlag(CLI, argv[Argument + 1]);
                                    /*
                                     TODO: What I really need to do tho, is loop over the remaining arguments to be sure I catch any and ALL child arguments, and just do it right, but this'll work for now.
                                     
                                     How do I structure that tho?
                                     
                                     Well, have 4 loops, the first loops over argc from 1, the second loops over the first to the end of argc, the 3rd loops over all the switches, and the 4th loops over all of it's child switches
                                     
                                     for each switch, load up
                                     
                                     */
                                }
                                free(ChildSingleDashFlag);
                                free(ChildDoubleDashFlag);
                                free(ChildSingleSlashFlag);
                            }
                        }
                        free(SingleDashFlag);
                        free(DoubleDashFlag);
                        free(SingleSlashFlag);
                    }
                }
            }
        }
    }
    
    uint64_t GetCLINumArgumentsMatchingSwitch(CommandLineIO *CLI, const uint64_t Switch) { // The gist, is that we want to find how many arguments there are for Switch X.
        uint64_t NumSwitchesFound = 0;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLINumArgumentsMatchingSwitch", "Pointer to CommandLineIO is NULL");
        } else if (Switch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLINumArgumentsMatchingSwitch", "Switch: %d, should be between 0 and %d", Switch, CLI->NumSwitches);
        } else {
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                if (CLI->Arguments[Argument].SwitchNum == Switch) {
                    NumSwitchesFound += 1;
                }
            }
        }
        return NumSwitchesFound;
    }
    
    uint64_t GetCLIArgumentNumFromFlag(CommandLineIO *CLI, const char *Flag) {
        uint64_t FoundSwitch = 0xFFFFFFFFFFFFFFFF;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentNumFromFlag", "Pointer to CommandLineIO is NULL");
        } else if (Flag == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentNumFromFlag", "Pointer to Flag is NULL");
        } else {
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                if (strcasecmp(CLI->Switches[Switch].Flag, Flag) == 0) {
                    FoundSwitch = Switch;
                }
            }
        }
        return FoundSwitch;
    }
    
    char *GetCLIArgumentResult(CommandLineIO *CLI, const uint64_t ArgumentNum) {
        char *Result = NULL;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentResult", "Pointer to CommandLineIO is NULL");
        } else if (ArgumentNum > CLI->NumArguments) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentResult", "ArgumentNum is greater than there are arguments");
        } else {
            Result = CLI->Arguments[ArgumentNum].ArgumentResult;
        }
        return Result;
    }
    
    uint64_t GetCLIChildSwitchArgument(CommandLineIO *CLI, const uint64_t ParentSwitch, const uint64_t ChildSwitch) {
        uint64_t SwitchContainingMetaArg = 0xFFFFFFFFFFFFFFFF;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "Pointer to CommandLineIO is NULL");
        } else if (ParentSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "ParentSwitch: %d, should be between 0 and %d", ParentSwitch, CLI->NumSwitches);
        } else if (ChildSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "ChildSwitch: %d, should be between 0 and %d", ChildSwitch, CLI->NumSwitches);
        } else {
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                for (uint64_t ChildArg = 0; ChildArg < CLI->Arguments[Argument].NumChildArguments; ChildArg++) {
                    if (CLI->Arguments[Argument].ChildArguments[ChildArg] == ChildSwitch && CLI->Arguments[Argument].SwitchNum == ParentSwitch) {
                        SwitchContainingMetaArg = Argument;
                    }
                }
            }
        }
        return SwitchContainingMetaArg;
    }
    
    void DeinitCommandLineIO(CommandLineIO *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DeinitCommandLineIO", "Pointer to CommandLineIO is NULL");
        } else {
            /* Free CommandLineSwitches */
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                free((char*) CLI->Switches[Switch].Flag);
                free((char*) CLI->Switches[Switch].SwitchDescription);
                free((char*) CLI->Switches[Switch].ChildSwitches);
            }
            /* Free CommandLineArguments */
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                free(CLI->Arguments[Argument].ChildArguments);
                free(CLI->Arguments[Argument].ArgumentResult);
            }
            free(CLI->Switches);
            free(CLI->Arguments);
            /* Free CommandLineIO */
            free((char*) CLI->ProgramName);
            free((char*) CLI->ProgramAuthor);
            free((char*) CLI->ProgramDescription);
            free((char*) CLI->ProgramVersion);
            free((char*) CLI->ProgramCopyright);
            free((char*) CLI->ProgramLicenseName);
            free((char*) CLI->ProgramLicenseDescription);
            free((char*) CLI->ProgramLicenseURL);
            free(CLI);
        }
    }
    
#ifdef __cplusplus
}
#endif
