#include <errno.h>
#include <stdio.h>

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
     @constant               MaxChildSwitches          "How many meta switches can be active at once?".
     @constant               NumChildSwitches          "How many meta switches are there in ChildSwitches?".
     @constant               ChildSwitches             "Pointer to an array that contains the numbers of the meta switches".
     */
    struct CommandLineSwitch {
        uint64_t             FlagSize;
        uint64_t             MaxChildSwitches;
        uint64_t             NumChildSwitches;
        bool                 SwitchHasResult;
        uint64_t            *ChildSwitches;
        const char          *Flag;
        const char          *SwitchDescription;
    };
    
    /*!
     @struct                 CommandLineArgument
     @abstract                                         "Contains the data to support a single argument".
     @constant               SwitchNum                 "Which switch is this argument?".
     @constant               ArgumentResult            "If there is a path or other result expected for this switch's argument, it'll be here".
     @constant               NumChildSwitches          "How many meta arguments were found in this argument?".
     @constant               ChildSwitches             "Array of meta argument numbers, to look up in CLI->Switches".
     */
    struct CommandLineArgument {
        uint64_t             SwitchNum;
        uint64_t             NumChildArgs;
        const char          *ArgumentResult;
        uint64_t            *ChildArguments;
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
        uint64_t             NumSwitches;
        uint64_t             MinSwitches;
        uint64_t             NumArguments;
        CommandLineSwitch   *Switches;
        CommandLineArgument *Arguments;
        bool                 IsProprietary;
        const char          *ProgramName;
        const char          *ProgramAuthor;
        const char          *ProgramDescription;
        const char          *ProgramVersion;
        const char          *ProgramCopyright;
        const char          *ProgramLicenseDescription;
        const char          *ProgramLicenseURL;
    };
    
    CommandLineInterface *InitCommandLineInterface(const uint64_t NumSwitches) {
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
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                free((char*)CLI->Switches[Switch].Flag);
                free((char*)CLI->Switches[Switch].SwitchDescription);
            }
            /* Free CommandLineArguments */
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                for (uint64_t MetaSwitch = 0; MetaSwitch < CLI->Switches[Argument].NumChildSwitches; MetaSwitch++) {
                    free((char*)CLI->Arguments[Argument].ArgumentResult);
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
    
    void SetCLILicenseURL(CommandLineInterface *CLI, const char *LicenseURL) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to CommandLineInterface is NULL\n");
        } else if (LicenseURL == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLILicenseURL", "Pointer to LicenseURL is NULL\n");
        } else {
            CLI->ProgramLicenseURL = LicenseURL;
        }
    }
    
    void SetCLIMinSwitches(CommandLineInterface *CLI, const uint64_t MinSwitches) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLIMinSwitches", "Pointer to CommandLineInterface is NULL\n");
        } else {
            CLI->MinSwitches = MinSwitches;
        }
    }
    
    void SetCLISwitchFlag(CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Flag, const uint64_t FlagSize) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to CommandLineInterface is NULL\n");
        } else if (Flag == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Pointer to switch Flag is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CLI->NumSwitches);
        } else if (FlagSize == 0) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchFlag", "Invalid FlagSize %d\n", FlagSize);
        } else {
            CLI->Switches[SwitchNum].Flag     = Flag;
            CLI->Switches[SwitchNum].FlagSize = FlagSize + 1; // add one for the trailing NULL
        }
    }
    
    void SetCLISwitchDescription(const CommandLineInterface *CLI, const uint64_t SwitchNum, const char *Description) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to CommandLineInterface is NULL\n");
        } else if (SwitchNum > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "SwitchNum %d is too high, there are only %d switches\n", SwitchNum, CLI->NumSwitches);
        } else if (Description == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchDescription", "Pointer to switch Description is NULL\n");
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
            CLI->Switches[SwitchNum].SwitchHasResult = IsThereAResult;
        }
    }
    
    bool GetCLISwitchPresence(const CommandLineInterface *CLI, const uint64_t SwitchNum) {
        bool SwitchFound = false;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLISwitchPresence", "Pointer to CommandLineInterface is NULL\n");
        } else {
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) {
                if (CLI->Arguments[Argument].SwitchNum == SwitchNum) {
                    SwitchFound = true;
                }
            }
        }
        return SwitchFound;
    }
    
    uint64_t GetCLIChildSwitchArgument(const CommandLineInterface *CLI, const uint64_t ParentSwitch, const uint64_t ChildSwitch) { // GetCLIMetaSwitchArgument
        uint64_t SwitchContainingMetaArg = 0xFFFFFFFFFFFFFFFF;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "Pointer to CommandLineInterface is NULL\n");
        } else if (ParentSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "ParentSwitch: %d, should be between 0 and %d\n", ParentSwitch, CLI->NumSwitches);
        } else if (ChildSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "GetCLIChildSwitchArgument", "ChildSwitch: %d, should be between 0 and %d\n", ChildSwitch, CLI->NumSwitches);
        } else {
            for (uint64_t Argument = 0; Argument < CLI->NumArguments; Argument++) { // Actual arguments
                for (uint64_t ChildArg = 0; ChildArg < CLI->Arguments[Argument].NumChildArgs; ChildArg++) {
                    if (CLI->Arguments[Argument].ChildArguments[ChildArg] == ChildSwitch && CLI->Arguments[Argument].SwitchNum == ParentSwitch) {
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
            Result = (char*)CLI->Arguments[ArgumentNum].ArgumentResult;
        }
        return Result;
    }
    
    static void DisplayCLIHelp(const CommandLineInterface *CLI) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "DisplayCLIHelp", "Pointer to CommandLineInterface is NULL\n");
        } else {
            bool *IsChildSwitch = calloc(CLI->NumSwitches, sizeof(bool));
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                // make an array of all the switches that are child switches
                for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[Switch].NumChildSwitches; ChildSwitch++) {
                    IsChildSwitch[CLI->Switches[Switch].ChildSwitches[ChildSwitch]] = true;
                }
            }
            // Now, loop through the switches and print the ones that aren't in IsChildSwitch
            printf("Options: (-|--|/)\n");
            for (uint64_t Switch = 0; Switch < CLI->NumSwitches; Switch++) {
                if (IsChildSwitch[Switch] == false) {
                    printf("%s: %s\n", CLI->Switches[Switch].Flag, CLI->Switches[Switch].SwitchDescription);
                    for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[Switch].NumChildSwitches; ChildSwitch++) {
                        
                        uint64_t CurrentChildSwitch = CLI->Switches[Switch].ChildSwitches[ChildSwitch];
                        
                        printf("\t %s: %s\n", CLI->Switches[CurrentChildSwitch].Flag, CLI->Switches[CurrentChildSwitch].SwitchDescription);
                        
                    }
                }
            }
            free(IsChildSwitch);
        }
    }
    
    static void DisplayProgramBanner(const CommandLineInterface *CLI) { // We should probably check that all the fields are valid and not NULL
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
    
    void SetCLISwitchMetaFlag(const CommandLineInterface *CLI, const uint64_t ParentSwitch, const uint64_t ChildSwitch) {
        // Switch is the switch to set as a meta flag of the other switch, but it should be specified the other way.
        // So that we first specify the switch that has the dependent flag, then which flag is dependent on current switch.
        /*
         So, SwitchNum's MetaFlags is incremented and it's Metaflag is set to MetaFlag.
         */
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchMetaFlag", "Pointer to CommandLineInterface is NULL\n");
        } else if (ParentSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchMetaFlag", "ParentSwitch: %d, should be between 0 and %d\n", ParentSwitch, CLI->NumSwitches);
        } else if (ChildSwitch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "SetCLISwitchMetaFlag", "ChildSwitch: %d, should be between 0 and %d\n", ChildSwitch, CLI->NumSwitches);
        } else {
            CLI->Switches[ParentSwitch].NumChildSwitches  += 1;
            if (CLI->Switches[ParentSwitch].ChildSwitches == NULL) {
                CLI->Switches[ParentSwitch].ChildSwitches = calloc(1, sizeof(uint64_t));
            } else {
                uint64_t ChildSwitchesSize = CLI->Switches[ParentSwitch].NumChildSwitches * sizeof(uint64_t);
                CLI->Switches[ParentSwitch].ChildSwitches = (uint64_t) realloc(CLI->Switches[ParentSwitch].ChildSwitches, ChildSwitchesSize);
            }
            CLI->Switches[ParentSwitch].ChildSwitches[CLI->Switches[ParentSwitch].NumChildSwitches] = ChildSwitch;
        }
    }
    
    void ParseCommandLineArguments(CommandLineInterface *CLI, const int argc, const char *argv[]) {
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "ParseCommandLineArguments", "Pointer to CommandLineInterface is NULL\n");
        } else if (argc <= 1 || (argc < CLI->MinSwitches && CLI->MinSwitches > 1) || argv == NULL) {
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
            for (int Argument = 1; Argument < argc; Argument++) { // No point in scanning the program's path
                printf("Argument: %d\n", Argument);
                for (uint64_t CurrentSwitch = 0; CurrentSwitch < CLI->NumSwitches; CurrentSwitch++) { // For each argument, it checks each switch for a match, makes sense.
                    printf("CurrentSwitch: %zu\n", CurrentSwitch);
                    
                    uint64_t FlagSize  = CLI->Switches[CurrentSwitch].FlagSize;
                    SingleDashFlag   = (char*) calloc(1, FlagSize + 1); // Wait, you have to free these after each and every argument...
                    DoubleDashFlag   = (char*) calloc(1, FlagSize + 2);
                    SingleSlashFlag  = (char*) calloc(1, FlagSize + 1);
                    
                    snprintf(SingleDashFlag,  FlagSize + 1,  "-%s", CLI->Switches[CurrentSwitch].Flag);
                    snprintf(DoubleDashFlag,  FlagSize + 2, "--%s", CLI->Switches[CurrentSwitch].Flag);
                    snprintf(SingleSlashFlag, FlagSize + 1,  "/%s", CLI->Switches[CurrentSwitch].Flag);
                    
                    if (strcasecmp(argv[Argument], SingleDashFlag) == 0 || strcasecmp(argv[Argument], DoubleDashFlag) == 0 || strcasecmp(argv[Argument], SingleSlashFlag) == 0) { // This argument = a switch flag; There has to be a better way than ArgvArgument == 1 tho; Yeah, maybe you want to know if it's even, but that still assumes that there's only up to 1 meta flag
                        
                        // So, we set CLI->Arguments[ArgvArgument] to this, this is the easy part, we haven't run into meta switches yet.
                        
                        CLI->Arguments = realloc(CLI->Arguments, sizeof(CommandLineArgument) * NumArguments);
                        
                        for (uint64_t ChildSwitch = 0; ChildSwitch < CLI->Switches[CurrentSwitch].NumChildSwitches; ChildSwitch++) {
                            // Here is where we check if the next argument matches any of the meta switches.
                            if (strcasecmp(argv[Argument + 1], CLI->Switches[CLI->Switches[CurrentSwitch].ChildSwitches[ChildSwitch]].Flag) == 0) {
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
    
    uint64_t FindCLIArgument(CommandLineInterface *CLI, const uint64_t Switch, uint64_t NumChildSwitches, const uint64_t *ChildSwitches) {
        // So the gist is we search CLI->Arguments looking for Switch with ChildSwitches, and return that argument.
        // What should the error code be tho? just return NULL?
        
        uint64_t FoundArgument = 0;
        if (CLI == NULL) {
            Log(LOG_ERR, "libBitIO", "FindCLIArgument", "Pointer to CommandLineInterface is NULL\n");
        } else if (Switch > CLI->NumSwitches) {
            Log(LOG_ERR, "libBitIO", "FindCLIArgument", "Switch: %d, should be between 0 and %d\n", Switch, CLI->NumSwitches);
        } else {
            for (uint64_t Argument = 1; Argument < CLI->NumArguments; Argument++) {
                for (uint64_t MetaSwitch = 0; MetaSwitch < NumChildSwitches; MetaSwitch++) {
                    for (uint64_t ArgChildSwitch = 0; ArgChildSwitch < CLI->Arguments[Argument].NumChildArgs; ArgChildSwitch++) {
                        if (CLI->Arguments[Argument].SwitchNum == Switch && ChildSwitches[MetaSwitch] == CLI->Arguments[Argument].ChildArguments[ArgChildSwitch]) {
                            FoundArgument = Argument;
                        }
                    }
                }
            }
        }
        
        return FoundArgument;
    }
    
    
#ifdef __cplusplus
}
#endif
