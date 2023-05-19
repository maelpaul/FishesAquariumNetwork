package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.display.FishManager;
import ProjetPoisson.project.scenes.MenuScene;

public class DelFishCommand implements ICommand {
    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final MenuScene.ConnectionStateContainer state;
    private final FishManager fishManager;
    public DelFishCommand(MenuScene.ConnectionStateContainer state, FishManager fishManager){
        this.state = state;
        this.fishManager = fishManager;
    }

    @Override
    public ResultCommand process(String[] args) {
        if (state.get() != MenuScene.EConnectionState.Connected)
            return new ResultCommand("-> NOK : Controleur introuvable");

        if (args.length == COMMAND_SIZE) {
            FishManager.EResult result = fishManager.delFish(args[ARG_NAME]);

            if (result == FishManager.EResult.DeleteErrorNameNotExisting)
                return new ResultCommand("  -> NOK : Poisson inexistant");

            return new ResultCommand("  -> OK : Poisson " + args[ARG_NAME] + " retiré", ResultCommand.EResultAction.SendServer);
        }

        return new ResultCommand("  -> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");
    }

    @Override
    public ResultCommand returnHelp() {
        return new ResultCommand("  -> help(delFish) : \n" +
                "    delFish NAME : delete a fish named NAME");
    }
}
