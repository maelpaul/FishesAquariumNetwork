package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class HelpCommand implements ICommand {
    @Override
    public ResultCommand process(String[] args) {
        return null;
    }

    @Override
    public ResultCommand returnHelp() {
        return null;
    }
}
