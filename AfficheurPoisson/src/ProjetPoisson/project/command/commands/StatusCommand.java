package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;
import ProjetPoisson.project.scenes.MenuScene;

public class StatusCommand implements ICommand {
    private final MenuScene.ConnectionStateContainer state;

    public StatusCommand(MenuScene.ConnectionStateContainer state){
        this.state = state;
    }
    @Override
    public ResultCommand process(String[] args) {
        return (state.get() == MenuScene.EConnectionState.Connected) ?
                new ResultCommand("> OK : Connecté au contrôle", ResultCommand.EResultAction.SendServer) :
                new ResultCommand("> NOK : Controleur introuvable");
    }

    @Override
    public ResultCommand returnHelp() {
        return null;
    }
}
