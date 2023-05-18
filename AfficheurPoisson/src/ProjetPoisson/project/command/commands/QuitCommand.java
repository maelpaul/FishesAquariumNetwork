package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class QuitCommand implements ICommand {
    @Override
    public ResultCommand process(String[] args) {
        return new ResultCommand(ResultCommand.EResultAction.Quit);
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("Quit the program");
    }
}
