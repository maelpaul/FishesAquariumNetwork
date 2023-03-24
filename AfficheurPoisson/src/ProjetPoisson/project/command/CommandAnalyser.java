package ProjetPoisson.project.command;

import java.util.HashMap;

public class CommandAnalyser {
    private static final int EMPTY_COMMAND = 0;
    private static final int NO_OPTIONAL_ARGUMENT = 1;

    private static final int COMMAND_TYPE_ARGUMENT = 0;
    private static final int FIRST_OPTIONAL_ARGUMENT = 1;

    HashMap<String, ICommand> relations;

    public CommandAnalyser() {
        relations = new HashMap<>();
    }

    public String analyseCommand(String command){
        String[] args = command.split(" ");

        if (args.length == EMPTY_COMMAND)
            return "Empty command !";

        if ( ! relations.containsKey(args[COMMAND_TYPE_ARGUMENT]))
            return "Unknown command, type help !";

        if (args.length == NO_OPTIONAL_ARGUMENT)
            return relations.get(args[COMMAND_TYPE_ARGUMENT]).process(args);

        if (args[FIRST_OPTIONAL_ARGUMENT].equals("help"))
            return relations.get(args[COMMAND_TYPE_ARGUMENT]).returnHelp();

        return relations.get(args[COMMAND_TYPE_ARGUMENT]).process(args);
    }
}
