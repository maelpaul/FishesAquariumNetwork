package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.display.FishManager;
import ProjetPoisson.project.scenes.MenuScene;

public class DelFishCommand implements ICommand<String>  {
    public static final int COMMAND_SIZE = 2;

    public static final int ARG_NAME = 1;

    private final MenuScene.ConnectionStateContainer state;
    private final FishManager fishManager;
    public DelFishCommand(MenuScene.ConnectionStateContainer state, FishManager fishManager){
        this.state = state;
        this.fishManager = fishManager;
    }

    @Override
    public ResultCommand<String>  process(String[] args) {
        if (state.get() != MenuScene.EConnectionState.Connected)
            return new PromptResultCommand("> NOK : Controleur introuvable");

        if (args.length == COMMAND_SIZE) {
            for (String name : fishManager.getFishNames()){
                if (name.equals(args[ARG_NAME])) {
                    Runnable delFish = () -> fishManager.delFish(args[ARG_NAME]);

                    return new ResultCommand<String>()
                            .addAction("resultPrompt", new Object[] { "> OK : Demande au serveur pour supprimer Poisson" })
                            .addAction("showErrorResult", null)
                            .addAction("showSuccessResult",  null)
                            .addAction("successRun", new Object[] { delFish });
                }
            }

            return new PromptResultCommand("> NOK : Poisson inexistant");
        }

        return new PromptResultCommand("> NOK : Mauvais argument(s), faites \"delFish help\" pour plus d'aide");
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand("> help(delFish) : \n" +
                "    delFish NAME : supprime le poisson nommé NAME");
    }
}
