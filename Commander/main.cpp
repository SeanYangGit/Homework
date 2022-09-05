#include <iostream>             // for cout
#include <vector>

#include "Commander.cpp"        // the commander++ api

/**
 * test.cpp - exercise the commander++ api
 *
 * The `MyCommand` class derives from `Command` and specifies command option
 * callbacks which set member variables based on command line options.
 */

using namespace std;

/**
 * An external option callback
 *
 * Deal with a command line switch that may or may not have an associated
 * argument.
 *
 * For simple use cases you may not need a custom `Command` class.  If so you
 * can pass callbacks along the following line to `Command.option()`.
 */
static void
optional(Command_t* self) {
    if (self->Arg != NULL) {
        cout << "Optional: " << self->Arg << endl;
    }
    else {
        cout << "Optional: enabled" << endl;
    }
}

/**
 * A custom MyCommand class deriving from the base Command
 *
 * This contains private members which are set by static member functions.
 */
class MyCommand : public Command {
public:
    // A constructor setting appropriate defaults
    MyCommand(const char* name, const char* version, bool verboseEnabled = false) :
        Command(name, version),
        verboseEnabled(verboseEnabled),
        required(NULL)
    {}

    /**
     * An option callback setting the verbose flag
     */
    static void
        setVerbose(Command_t* command) {
        static_cast<MyCommand*>(Command::Self(command))->verboseEnabled = true;
    }

    /**
     * An option callback setting the required value
     */
    static void
        setRequired(Command_t* command) {
        static_cast<MyCommand*>(Command::Self(command))->required = command->Arg;
    }

    /**
     * Get the required value
     */
    const char*
        getRequired() {
        return required;
    }

    /**
     * Get the verbosity setting
     */
    bool
        isVerbose() {
        return verboseEnabled;
    }

private:
    // members set by the option callbacks
    bool verboseEnabled;
    const char* required;
};


int main(int argc, char* argv[]) {
    // Instantiate the command with program name and version strings
    MyCommand command = MyCommand(argv[0], "0.0.1");

    // optionally set a more descriptive usage string
    command.SetUsage("[options] [ARG1 [ARG2 [ARG3]]");

    // Set the various options
    command.Option("-v", "--verbose", "enable verbose stuff", MyCommand::setVerbose);
    command.Option("-r", "--required <arg>", "required arg", MyCommand::setRequired);
    command.Option("-o", "--optional [arg]", "optional arg", optional);

    // Parse the arguments
    command.Parse(argc, argv);

    // Get and print any additional arguments specified after the callbacks
    std::vector<const char*> args = command.AdditionalArgs();

    if (args.size() > 3) {
        cerr << "Too many command line arguments were specified" << endl;
        command.Help();                 // this exits the program

    }
    else if (args.size()) {
        cout << "Additional args:" << endl;
        for (vector<const char*>::iterator it = args.begin(); it != args.end(); ++it) {
            cout << "  - '" << *it << "'" << endl;
        }

    }
    else {
        cout << "No additional args" << endl;
    }

    // If a required option is set, output it
    if (command.getRequired() != NULL) {
        cout << "Required is: " << command.getRequired() << endl;
    }

    // Output the verbosity setting
    cout << "Verbose status is " << ((command.isVerbose()) ? "enabled" : "disabled") << endl;

    return 0;
}
