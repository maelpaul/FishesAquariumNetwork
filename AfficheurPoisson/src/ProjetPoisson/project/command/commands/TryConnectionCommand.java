package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.scenes.MenuScene;

public class TryConnectionCommand implements ICommand<String> {
    private final MenuScene.TryConnectionContainer shouldTryConnection;
    public TryConnectionCommand(MenuScene.TryConnectionContainer shouldTryConnection){
        this.shouldTryConnection = shouldTryConnection;
    }

    @Override
    public ResultCommand<String> process(String[] args) {
        if (args.length != 1)
            return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");

        shouldTryConnection.set(true);
        return new PromptResultCommand("> OK : Tentative de connexion");
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand(
                "> help(tryConnection) : \n" +
                "    tryConnection : l'application va tenter de se connecter en boucle, a faire après un log out");
    }
}
