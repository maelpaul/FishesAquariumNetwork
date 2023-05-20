package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class ClearCommand implements ICommand {
    public static final int COMMAND_SIZE = 1;
    @Override
    public ResultCommand process(String[] args) {
        if (args.length != COMMAND_SIZE)
            return new ResultCommand("> NOK : Mauvais argument(s), faites \"clear help\" pour plus d'aide");

        return new ResultCommand(ResultCommand.EResultAction.Clear);
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("> help(clear) : \n" +
                "    clear : clear terminal");
    }
}
