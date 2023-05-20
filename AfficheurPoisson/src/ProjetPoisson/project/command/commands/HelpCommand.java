package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class HelpCommand implements ICommand<String> {
    @Override
    public ResultCommand<String> process(String[] args) {
        return null;
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                   "> help(help) : \n" +
                        "   help : affiche une aide générale");
    }
}
