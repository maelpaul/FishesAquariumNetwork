package ProjetPoisson.project.command.commands;

import ProjetPoisson.project.client.ClientTcp;
import ProjetPoisson.project.command.ICommand;

public class StatusCommand implements ICommand {
    private ClientTcp client;

    public StatusCommand(ClientTcp client){
        this.client = client;
    }
    @Override
    public String process(String[] args) {
        return (client.isConnected()) ?
                "-> OK : Connecté au contrôle, x poissons trouvés":
                "-> NOK : Controleur introuvable";
    }

    @Override
    public String returnHelp() {
        return null;
    }
}
