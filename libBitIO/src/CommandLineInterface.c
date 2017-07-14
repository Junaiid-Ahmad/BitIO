#include <errno.h>

#include "../include/BitIO.h"
#include "../include/CommandLineInterface.h"

#ifdef __cplusplus
extern "C" {
#endif
    
    /*!
     @struct                 CommandLineSwitch
     @abstract                                         "Contains the data to support a single switch".
     @remark                                           "Any switch can be a meta switch of any other, we're not creating 2 classes of switch".
     @constant               SwitchHasResult           "Does this switch have a result?".
     @constant               Flag                      "What is this switch called, how do we identify it?".
     @constant               FlagSize                  "What is the number of bytes (if ASCII)/ code points (if UTF) of this switch?".
     @constant               SwitchDescription         "Describe to the user what this switch does".
     @constant               MaxMetaSwitches           "How many meta switches can be active at once?".
     @constant               NumMetaSwitches           "How many meta switches are there in MetaSwitches?".
     @constant               MetaSwitches              "Pointer to an array that contains the numbers of the meta switches".
     */
    struct CommandLineSwitch {
        bool                 SwitchHasResult;
        const char          *Flag;
        size_t               FlagSize;
        const char          *SwitchDescription;
        uint64_t             MaxMetaSwitches;
        uint64_t             NumMetaSwitches;
        uint64_t            *MetaSwitches;
    };
    
    /*!
     @struct                 CommandLineArgument
     @abstract                                         "Contains the data to support a single argument".
     @constant               SwitchNum                 "Which switch is this argument?".
     @constant               ArgumentResult            "If there is a path or other result expected for this switch's argument, it'll be here".
     @constant               NumMetaArgs               "How many meta arguments were found in this argument?".
     @constant               MetaArgs                  "Array of meta argument numbers, to look up in CLI->Switches".
     */
    struct CommandLineArgument {
        uint64_t             SwitchNum;
        const char          *ArgumentResult;
        uint64_t             NumMetaArgs;
        uint64_t            *MetaArgs;
    };
    
    /*!
     @struct                 CommandLineInterface
     @abstract                                         "Contains all the information on the command line in an easy to understand format".
     @constant               NumSwitches               "How many switches are there?".
     @constant               MinSwitches               "The minimum number of switches to accept without dumping the help".
     @constant               NumArguments              "The number of arguments present in argv, after extracting any meta switches".
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
        size_t               MinSwitches;
        size_t               NumArguments;
        bool                 IsProprietary;
        const char          *ProgramName;
        const char          *ProgramAuthor;
        const char          *ProgramDescription;
        const char          *ProgramVersion;
        const char          *ProgramCopyright;
        const char          *ProgramLicenseDescription;
        const char          *ProgramLicenseURL;
        uint64_t            *NumSwitchArguments; // Array that says switch (the index) has X arguments
        CommandLineSwitch   *Switches;
        CommandLineArgument *Arguments;
    };
    
    CommandLineInterface *InitCommandLineInterface(const size_t NumSwitches) {
        errno = 0;
        CommandLineInterface *CLI = (CommandLineInterface*) calloc(1, sizeof(CommandLineInterface));
        if (errno != 0) {
            char *ErrnoError      = (char*) calloc(1, 96);
            strerror_r(errno, ErrnoError, 96);
            Log(LOG_ERR, "libBitIO", "InitCommandLineInterface", "Errno Initing CommandLineInterface: %s\n", ErrnoError);
            free(ErrnoError);
            errno = 0;
        }
        CLI->NumSwitches          = NumSwitches;
        CLI->Switches             = (CommandLineSwitch*) calloc(NumSwitches, sizeof(CommandLineSwitch));
        if (errno != 0) {
            char *ErrnoError      = (char*) calloc(1, 96);
            strerror_r(errno, ErrnoError, 96);
            Log(LOG_ERR, "libBitIO", "InitCommandLineInterface", "Errno Initing CommandLineSwitch: %s\n", ErrnoError);
            free(ErrnoError);
            errno                 = 0;
        }
        return CLI;
    }
    
    void DeinitCommandLineInterface(CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DeinitCommandLineInterface", "Pointer to CommandLineInterface is NULL\n");
        } else {
            /* Free CommandLineSwitches */
            for (size_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                free((char*)CLI->Switches[Switch].Flag);
                free((char*)CLI->Switches[Switch].SwitchDescription);
            }
            /* Free CommandLineArguments */
            for (size_t Arg = 0; Arg < CLI->NumArguments; Arg++) {
                for (size_t MetaSwitch = 0; MetaSwitch < CLI->Switches[Arg].NumMetaSwitches; MetaSwitch++) {
                    free((char*)CLI->Arguments[Arg].ArgumentResult);
                }
            }
            free(CLI->Switches);
            free(CLI->Arguments);
            /* Free CommandLineInterface */
            free((char*)CLI->ProgramName);
            free((char*)CLI->ProgramAuthor);
            free((char*)CLI->ProgramDescription);
            free((char*)CLI->ProgramVersion);
            free((char*)CLI->ProgramCopyright);
            free((char*)CLI->ProgramLicenseDescription);
            free((char*)CLI->ProgramLicenseURL);
            free(CLI);
        }
    }
    
    void SetCLIName(CommandLineInterface *CLI, const char *Name) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to CommandLineInterface is NULL\n");
        } else if (Name == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIName", "Pointer to Name is NULL\n");
        } else {
            CLI->ProgramName = Name;
        }
    }
    
    void SetCLIVersion(CommandLineInterface *CLI, const char *VersionString) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to CommandLineInterface is NULL\n");
        } else if (VersionString == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIVersion", "Pointer to VersionString is NULL\n");
        } else {
            CLI->ProgramVersion = VersionString;
        }
    }
    
    void SetCLIDescription(CommandLineInterface *CLI, const char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to CommandLineInterface is NULL\n");
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIDescription", "Pointer to Description is NULL\n");
        } else {
            CLI->ProgramDescription = Description;
        }
    }
    
    void SetCLIAuthor(CommandLineInterface *CLI, const char *Author) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to CommandLineInterface is NULL\n");
        } else if (Author == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIAuthor", "Pointer to Author is NULL\n");
        } else {
            CLI->ProgramAuthor = Author;
        }
    }
    
    void SetCLICopyright(CommandLineInterface *CLI, const char *Copyright) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to CommandLineInterface is NULL\n");
        } else if (Copyright == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLICopyright", "Pointer to Copyright is NULL\n");
        } else {
            CLI->ProgramCopyright = Copyright;
        }
    }
    
    void SetCLILicense(CommandLineInterface *CLI, const char *License, const bool IsProprietary) {
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
    
    void SetCLILicenseURL(CommandLineInterface *CLI, const char *LicenseURL, const bool IsProprietary) {
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
    
    void SetCLIMinSwitches(CommandLineInterface *CLI, const uint64_t MinSwitches) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIMinSwitches", "Pointer to CommandLineInterface is NULL\n");
        } else {
            CLI->MinSwitches = MinSwitches;
        }
    }
    
    void SetCLISwitchFlag(CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Flag, const size_t FlagSize) {
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
    
    bool GetCLISwitchPresence(const CommandLineInterface *CLI, const uint64_t SwitchNum) {
        bool SwitchFound = false;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLISwitchPresence", "Pointer to CommandLineInterface is NULL\n");
        } else {
            for (size_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                if (CLI->Arguments[Argument].SwitchNum == SwitchNum) {
                    SwitchFound = true;
                }
            }
        }
        return SwitchFound;
    }
    
    uint64_t GetCLIMetaSwitchArgument(const CommandLineInterface *CLI, const uint64_t SwitchNum, const uint64_t MetaSwitchNum) {
        uint64_t SwitchContainingMetaArg = 0xFFFFFFFFFFFFFFFF;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIMetaSwitchArgument", "Pointer to CommandLineInterface is NULL\n");
        } else {
            for (size_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                for (size_t MetaArg = 0; MetaArg < CLI->Arguments[Argument].NumMetaArgs; MetaArg++) {
                    if (CLI->Arguments[Argument].MetaArgs[MetaArg] == MetaSwitchNum && CLI->Arguments[Argument].SwitchNum == SwitchNum) {
                        SwitchContainingMetaArg = Argument;
                    }
                }
            }
        }
        return SwitchContainingMetaArg;
    }
    
    const char *GetCLIArgumentResult(const CommandLineInterface *CLI, const uint64_t ArgumentNum) {
        char *Result = NULL;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentResult", "Pointer to CommandLineInterface is NULL\n");
        } else if (ArgumentNum > CLI->NumArguments) {
            Log(LOG_ERR, "libBitIO", "GetCLIArgumentResult", "ArgumentNum is greater than there are arguments");
        } else {
            Result = CLI->Arguments[ArgumentNum].ArgumentResult;
        }
        return Result;
    }
    
    static void DisplayCLIHelp(const CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayCLIHelp", "Pointer to CommandLineInterface is NULL\n");
        } else {
            printf("Options: (-|--|/)\n");
            for (size_t CurrentSwitch = 0; CurrentSwitch < CLI->NumSwitches; CurrentSwitch++) {
                printf("%s: %s\n", CLI->Switches[CurrentSwitch].Flag, CLI->Switches[CurrentSwitch].SwitchDescription);
                if (CLI->Switches[CurrentSwitch].NumMetaSwitches > 0) {
                    for (size_t MetaSwitch = 0; MetaSwitch < CLI->Switches[CurrentSwitch].NumMetaSwitches; MetaSwitch++) {
                        size_t CurrentMetaSwitch = CLI->Switches[CurrentSwitch].MetaSwitches[MetaSwitch];
                        printf("\t %s: %s\n", CLI->Switches[CurrentMetaSwitch].Flag, CLI->Switches[CurrentMetaSwitch].SwitchDescription);
                    }
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
    
    void ParseCommandLineArguments(CommandLineInterface *CLI, const int argc, const char *argv[]) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Pointer to CommandLineInterface is NULL\n");
        } else if (argc == 1 || (argc < CLI->MinSwitches && CLI->MinSwitches > 1)) {
            DisplayProgramBanner(CLI);
            DisplayCLIHelp(CLI);
        } else {
            DisplayProgramBanner(CLI);
            errno                    = 0;
            char    *SingleDashFlag  = NULL;
            char    *DoubleDashFlag  = NULL;
            char    *SingleSlashFlag = NULL;
            uint64_t NumArguments    = 0;
            
            // loop over argv looking for arguments
            for (size_t ArgvArgument = 1; ArgvArgument < argc; ArgvArgument++) { // No point in scanning the program's path
                printf("ArgvArgument: %zu\n", ArgvArgument);
                for (size_t CurrentSwitch = 0; CurrentSwitch < CLI->NumSwitches; CurrentSwitch++) { // For each argument, it checks each switch for a match, makes sense.
                    printf("CurrentSwitch: %zu\n", CurrentSwitch);
                    
                    size_t FlagSize  = CLI->Switches[CurrentSwitch].FlagSize;
                    SingleDashFlag   = (char*) calloc(1, FlagSize + 1); // Wait, you have to free these after each and every argument...
                    DoubleDashFlag   = (char*) calloc(1, FlagSize + 2);
                    SingleSlashFlag  = (char*) calloc(1, FlagSize + 1);
                    
                    snprintf(SingleDashFlag,  FlagSize + 1,  "-%s", CLI->Switches[CurrentSwitch].Flag);
                    snprintf(DoubleDashFlag,  FlagSize + 2, "--%s", CLI->Switches[CurrentSwitch].Flag);
                    snprintf(SingleSlashFlag, FlagSize + 1,  "/%s", CLI->Switches[CurrentSwitch].Flag);
                    
                    if (strcasecmp(argv[ArgvArgument], SingleDashFlag) == 0 || strcasecmp(argv[ArgvArgument], DoubleDashFlag) == 0 || strcasecmp(argv[ArgvArgument], SingleSlashFlag) == 0) { // This argument = a switch flag; There has to be a better way than ArgvArgument == 1 tho; Yeah, maybe you want to know if it's even, but that still assumes that there's only up to 1 meta flag
                        
                        // So, we set CLI->Arguments[ArgvArgument] to this, this is the easy part, we haven't run into meta switches yet.
                        
                        realloc(CLI->Arguments, sizeof(CommandLineArgument) * NumArguments);
                        
                        for (size_t MetaSwitch = 0; MetaSwitch < CLI->Switches[CurrentSwitch].NumMetaSwitches; MetaSwitch++) {
                            // Here is where we check if the next argument matches any of the meta switches.
                            if (strcasecmp(argv[ArgvArgument + 1], CLI->Switches[CLI->Switches[CurrentSwitch].MetaSwitches[MetaSwitch]].Flag) == 0) {
                                // If we're here, the next argv argument has been found as a meta switch, now the only problem is dickering in multiple meta switches.
                            }
                        }
                    }
                    free(SingleDashFlag);
                    free(DoubleDashFlag);
                    free(SingleSlashFlag);
                }
            }
        }
    }
    
    void FindCLIArgument(CommandLineInterface *CLI, const uint64_t Switch, uint64_t NumMetaSwitches, const uint64_t *MetaSwitches) {
        // So the gist is we search CLI->Arguments looking for Switch with Metaswitches, and return that argument.
        // What should the error code be tho? just return NULL?
        for (size_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
            for (size_t MetaSwitch = 0; MetaSwitch < NumMetaSwitches; MetaSwitch++) {
                for (size_t ArgMetaSwitch = 0; ArgMetaSwitch < CLI->Arguments[Argument].NumMetaArgs; ArgMetaSwitch++) {
                    if (CLI->Arguments[Argument].SwitchNum == Switch && MetaSwitches[MetaSwitch] == CLI->Arguments[Argument].MetaArgs) {
                        
                    }
                }
            }
        }
    }
    
    /*
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
                        
                        size_t SingleDashSize                       = CLI->Switches[SwitchNum].FlagSize + 1;
                        size_t DoubleDashSize                       = CLI->Switches[SwitchNum].FlagSize + 2;
                        size_t SlashSize                            = CLI->Switches[SwitchNum].FlagSize + 1;
                        
                        SingleDash                                  = (char*) calloc(1, SingleDashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*) calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno SingleDash = %s, Arg = %d, Switch = %d, errno = %s", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0;
                        } else {
                            snprintf(SingleDash, SingleDashSize, "-%s", CLI->Switches[SwitchNum].Flag);
                        }
                        
                        DoubleDash                                  = (char*) calloc(1, DoubleDashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*) calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno DoubleDash = %s, Arg = %d, Switch = %d", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0;
                        } else {
                            snprintf(DoubleDash, DoubleDashSize, "--%s", CLI->Switches[SwitchNum].Flag);
                        }
                        
                        Slash                                       = (char*) calloc(1, SlashSize);
                        if (errno != 0) {
                            char *ErrnoError = (char*) calloc(1, 96);
                            strerror_r(errno, ErrnoError, 96);
                            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Errno Slash = %s, Arg = %d, Switch = %d", ErrnoError, Argument, SwitchNum);
                            free(ErrnoError);
                            errno = 0; // Here to reset to catch errors with the strcmp stuff below
                        } else {
                            snprintf(Slash, SlashSize, "/%s", CLI->Switches[SwitchNum].Flag);
                        }
                        
                        if (strcasecmp(SingleDash, argv[Argument]) == 0 || strcasecmp(DoubleDash, argv[Argument]) == 0 || strcasecmp(Slash, argv[Argument]) == 0) {
                            
                            size_t ArgumentSize = strlen(argv[Argument + 1]) + 1;
                            
                            CLI->Switches[SwitchNum].SwitchFound        = true;
                            if (CLI->Switches[SwitchNum].IsThereAResult == true) {
                                char *SwitchResult                      = (char*) calloc(1, ArgumentSize);
                                if (errno != 0) {
                                    char *ErrnoError = (char*) calloc(1, 96);
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
     */
    
#ifdef __cplusplus
}
#endif
