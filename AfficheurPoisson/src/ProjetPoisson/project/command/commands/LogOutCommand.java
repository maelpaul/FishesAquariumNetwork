package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class LogOutCommand implements ICommand<String>  {

    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != 2 && !args[1].equals("out"))
            return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");

        return new ResultCommand<String>()
                .addAction("resultPrompt", new Object[]{"> OK : Log out"})
                .addAction("runCommand", null)
                .addAction("showResult", null);
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                "> help(log out) : \n" +
                     "    log out : se deconnecte du serveur", "    ");
    }
}
