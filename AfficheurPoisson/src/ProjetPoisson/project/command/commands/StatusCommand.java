package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;

public class StatusCommand implements ICommand {
    @Override
    public String process(String[] args) {
        return "-> OK : Non connecté";
    }

    @Override
    public String returnHelp() {
        return null;
    }
}
