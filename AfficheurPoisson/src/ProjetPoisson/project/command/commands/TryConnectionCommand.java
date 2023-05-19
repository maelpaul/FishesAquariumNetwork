package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.scenes.MenuScene;

public class TryConnectionCommand implements ICommand {
    private final MenuScene.TryConnectionContainer shouldTryConnection;
    public TryConnectionCommand(MenuScene.TryConnectionContainer shouldTryConnection){
        this.shouldTryConnection = shouldTryConnection;
    }

    @Override
    public ResultCommand process(String[] args) {
        if (args.length != 1)
            return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");

        shouldTryConnection.set(true);
        return new ResultCommand("  -> OK : Tentative de connexion");
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("  -> help(tryConnection) : \n" +
                "    tryConnection : make the program trying again to connect after a log out");
    }
}
