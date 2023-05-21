package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;

public class StartFishAll implements ICommand<String> {
    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != 1)
            return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"startFishAll help\" pour plus d'aide");

        return new ResultCommand<String>()
                .addAction("resultPrompt", new Object[]{"> OK : demande au server pour démarrer tout les poissons"})
                .addAction("runCommand", null)
                .addAction("showResult", null);
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                "> help(startFishAll) : \n" +
                        "    startFishAll : démarre tout les poissons existant", "    ");
    }
}
