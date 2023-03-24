package ProjetPoisson.project.command;

import java.util.HashMap;

public class CommandAnalyser {


    HashMap<String, ICommand> relations;

    public CommandAnalyser() {

    }

    /**
     * Analyse and return the display for the console
     * @param command command written by user
     * @return the display to display
     */
    public String sendCommand(String command){
        String[] args = command.split(" ");

        if (args.length == 0)
            return "Empty command !";

        if (!relations.containsKey(args[0]))
            return "Unknown command, type help !";

        if (args.length == 1)
            return relations.get(args[0]).process(args);

        if (args[1].equals("help"))
            return relations.get(args[0]).returnHelp();

        return relations.get(args[0]).process(args);
    }
}
