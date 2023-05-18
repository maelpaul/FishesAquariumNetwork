package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;
import ProjetPoisson.project.command.ResultCommand;

public class StatusCommand implements ICommand {
    private ClientTcp client;

    public StatusCommand(ClientTcp client){
        this.client = client;
    }
    @Override
    public ResultCommand process(String[] args) {
        return (client.isConnected()) ?
                new ResultCommand("-> OK : Connecté au contrôle, x poissons trouvés") :
                new ResultCommand("-> NOK : Controleur introuvable");
    }

    @Override
    public ResultCommand returnHelp() {
        return null;
    }
}
