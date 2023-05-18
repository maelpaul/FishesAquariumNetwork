package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;

public class QuitCommand implements ICommand {
    @Override
    public String process(String[] args) {
        return "¤¤quit¤¤";
    }

    @Override
    public String returnHelp() {
        return null;
    }
}
