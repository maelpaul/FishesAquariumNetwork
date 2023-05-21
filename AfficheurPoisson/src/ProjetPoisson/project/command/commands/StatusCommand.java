package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.PromptResultCommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.scenes.MenuScene;

public class StatusCommand implements ICommand<String> {
    private final MenuScene.ConnectionStateContainer state;

    public StatusCommand(MenuScene.ConnectionStateContainer state){
        this.state = state;
    }
    @Override
    public ResultCommand<String> process(String[] args) {
        return (state.get() == MenuScene.EConnectionState.Connected) ?
                new ResultCommand<String>()
                    .addAction("resultPrompt", new Object[] { "> OK : Connecté au contrôle" })
                    .addAction("runCommand", null)
                    .addAction("showErrorResult", null)
                    .addAction("showSuccessResult",  null):
                new PromptResultCommand("> NOK : Controleur introuvable");
    }

    @Override
    public ResultCommand<String> returnHelp() {
        return new PromptResultCommand("> help(startFish) : \n" +
                "    startFish NAME : démarre le poisson nommé NAME,\n" +
                "    startFish all : démarre tout les poissons", "    ");
    }
}
