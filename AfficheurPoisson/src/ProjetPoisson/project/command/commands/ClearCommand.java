package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class ClearCommand implements ICommand {
    @Override
    public ResultCommand process(String[] args) {
        return new ResultCommand(ResultCommand.EResultAction.Clear);
    }

    @Override
    public ResultCommand returnHelp() {
        return null;
    }
}
